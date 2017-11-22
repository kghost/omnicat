#include "../common/header.h"

#include "Weave.h"

#include <list>
#include <stack>
#include <optional>
#include <functional>
#include <boost/log/trivial.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include "Exception.h"

namespace Omni {
	namespace Fiber {
		//  Transform:
		//    pre-pause;
		//    v = pausable();
		//    post-pause;
		//  To
		//    pre-pause;
		//    return Continue{
		//      pausable_cps_form() -> future;
		//      [](v) { post-pause; }
		//    };
		//
		//  Transform:
		//    V v = try { body } catch(...) { handle-fail }
		//  To
		//    Fiber WithTry([] -> Fiber { body }, handle-fail, Fiber (post-body)(V, ContinuationRef))
		//
		//  post-pause is a continuation object. Many language can auto generate it by using CPS transform, but in c++ world, we need to write it manually.
		//  CPS form: instead returns a value , pausable returns a future

		class FiberSwitch {
		public:
			virtual ~FiberSwitch() {}
			virtual Fiber unwind(std::exception_ptr && eptr) noexcept = 0;
			virtual void set(std::shared_ptr<FiberSwitch>) noexcept = 0;
			virtual std::shared_ptr<FiberSwitch> next() noexcept = 0;
		};

		class FiberSwitchOutMost : public FiberSwitch, public std::enable_shared_from_this<FiberSwitchOutMost> {
		public:
			virtual void set(std::shared_ptr<FiberSwitch>) noexcept { assert(false); }
			virtual std::shared_ptr<FiberSwitch> next() noexcept { return shared_from_this(); }
		};

		class FiberSwitchMiddle : public FiberSwitch {
		public:
			virtual Fiber unwind(std::exception_ptr && eptr) noexcept {
				return next()->unwind(std::move(eptr));
			}

			// After I have been switched out, all exceptions should pass to outer.
			virtual void set(std::shared_ptr<FiberSwitch> next) noexcept { outer = next; }

			virtual std::shared_ptr<FiberSwitch> next() noexcept { return outer; }
		private:
			std::shared_ptr<FiberSwitch> outer;
		};

		class FiberSwitchEnd : public FiberSwitch {
			virtual Fiber unwind(std::exception_ptr && eptr) noexcept { assert(false); return nullptr; }
			virtual void set(std::shared_ptr<FiberSwitch>) noexcept {}
			virtual std::shared_ptr<FiberSwitch> next() noexcept { assert(false); return nullptr; }
		};

		class FiberSwitchYield : public FiberSwitchMiddle {
		public:
			std::shared_ptr<FiberSwitch> continued;
			bool switched = false;
		};

		class FiberSwitchHandle : public FiberSwitchMiddle, public std::enable_shared_from_this<FiberSwitchHandle> {
		public:
			FiberSwitchHandle(CodePiece<ContinuationRef, std::exception_ptr>&& handler, ContinuationRef continuation)
				: handler(std::move(handler)), continuation(std::move(continuation)) {}

			virtual Fiber unwind(std::exception_ptr && eptr) noexcept {
				if (installed) {
					try {
						return handler(std::move(eptr), std::move(continuation));
					} catch (...) {
						return next()->unwind(std::current_exception());
					}
				} else {
					return next()->unwind(std::move(eptr));
				}
			}

			virtual std::shared_ptr<FiberSwitch> next() noexcept {
				if (installed) return shared_from_this();
				else return FiberSwitchMiddle::next();
			}

			Fiber cont() {
				installed = false;
				auto c = std::move(continuation); // consume continuation
				return c();
			}
		private:
			CodePiece<ContinuationRef, std::exception_ptr> handler;
			Continuation continuation;
			bool installed = true;
		};

		class FiberSwitchMain : public FiberSwitchOutMost {
		public:
			virtual Fiber unwind(std::exception_ptr && eptr) noexcept {
				BOOST_LOG_TRIVIAL(trace) << "Fiber run-exc: " << this;
				try {
					std::rethrow_exception(eptr);
				} catch (const std::exception & e) {
					BOOST_LOG_TRIVIAL(fatal) << e.what();
					throw;
				}
			}
		};

		class FiberSwitchThread : public FiberSwitchOutMost {
		public:
			FiberSwitchThread(std::string&& name) : name(name) {}
			virtual ~FiberSwitchThread() { assert(bool(join)); }

			virtual Fiber unwind(std::exception_ptr && eptr) noexcept {
				// capture eptr, rethrow at join
				BOOST_LOG_TRIVIAL(info) << "Fiber(" << name << ") exception: " << boost::diagnostic_information(eptr);
				done = true;
				exc = eptr;
				if (bool(join)) join.value()(*this);
				return std::make_shared<FiberSwitchEnd>();
			}

			std::string name;
			std::optional<std::function<void(FiberSwitchThread&)>> join;
			std::optional<std::exception_ptr> exc;
			bool done = false;
		};

		SHARED void run(CodePiece<ContinuationRef>&& body) {
			BOOST_LOG_NAMED_SCOPE("R");
			auto fiber = std::make_shared<FiberSwitchMain>();
			BOOST_LOG_TRIVIAL(info) << "Fiber run-started: " << fiber.get();
			try {
				auto inner = body([fiber] {
					BOOST_LOG_TRIVIAL(info) << "Fiber run-ended: " << fiber.get();
					return std::make_shared<FiberSwitchEnd>();
				});;
				inner->set(fiber);
				BOOST_LOG_TRIVIAL(trace) << "Fiber run-switch-out: " << fiber.get();
			} catch (...) {
				fiber->unwind(std::current_exception());
			}
		}

		SHARED Fiber handle(CodePiece<ContinuationRef>&& body, CodePiece<ContinuationRef, std::exception_ptr>&& handler, ContinuationRef continuation) {
			auto fiber = std::make_shared<FiberSwitchHandle>(std::move(handler), std::move(continuation));
			BOOST_LOG_TRIVIAL(trace) << "Fiber handle: " << fiber.get();
			try {
				auto inner = body([fiber] {
					BOOST_LOG_TRIVIAL(trace) << "Fiber handle-exit: " << fiber.get();
					return fiber->cont();
				});
				inner->set(fiber);
				return fiber;
			} catch (...) {
				BOOST_LOG_TRIVIAL(trace) << "Fiber handle-exc: " << fiber.get();
				return fiber->unwind(std::current_exception());
			}
		}

		SHARED Fiber yield(std::function<void(Restart&&)>&& finalize) {
			BOOST_LOG_NAMED_SCOPE("YO");
			auto fiber = std::make_shared<FiberSwitchYield>();
			BOOST_LOG_TRIVIAL(trace) << "Fiber yield: " << fiber.get();
			finalize([outer = fiber](ContinuationRef continuation) mutable -> void {
				BOOST_LOG_NAMED_SCOPE("YI");
				if (!outer->switched) {
					outer->continued = continuation();
				} else {
					BOOST_LOG_TRIVIAL(trace) << "Fiber yield-switch-in: " << outer.get();
					auto next = outer->next();
					try {
						auto inner = continuation();
						inner->set(next);
						BOOST_LOG_TRIVIAL(trace) << "Fiber yield-switch-out: " << outer.get() << " -> " << inner.get();
					} catch (...) {
						BOOST_LOG_TRIVIAL(trace) << "Fiber yield-exc: " << outer.get();
						// XXX: unwind return fiber ?
						next->unwind(std::current_exception());
					}
				}
			});
			if (fiber->continued) {
				BOOST_LOG_TRIVIAL(trace) << "Fiber yield-noswitch-back: " << fiber.get();
				return fiber->continued;
			} else {
				BOOST_LOG_TRIVIAL(trace) << "Fiber yield-switched: " << fiber.get();
				fiber->switched = true;
				return fiber;
			}
		}

		SHARED Fiber fork(std::string && name, CodePiece<ContinuationRef>&& body) {
			BOOST_LOG_NAMED_SCOPE("F");
			auto fiber = std::make_shared<FiberSwitchThread>(std::move(name));
			BOOST_LOG_TRIVIAL(info) << "Fiber(" << fiber->name << ") started: " << fiber.get();
			try {
				auto inner = body([fiber] {
					fiber->done = true;
					BOOST_LOG_TRIVIAL(info) << "Fiber(" << fiber->name << ") ended: " << fiber.get();
					if (bool(fiber->join)) fiber->join.value()(*fiber);
					return std::make_shared<FiberSwitchEnd>();
				});
				inner->set(fiber);
				BOOST_LOG_TRIVIAL(trace) << "Fiber fork-switch-out: " << fiber.get() << " -> " << inner.get();
			} catch (...) {
				BOOST_LOG_TRIVIAL(trace) << "Fiber fork-exc: " << fiber.get();
				return fiber->unwind(std::current_exception());
			}
			return fiber;
		}

		SHARED Fiber join(Fiber fiber, ContinuationRef continuation) {
			BOOST_LOG_NAMED_SCOPE("JO");
			auto f = std::dynamic_pointer_cast<FiberSwitchThread>(fiber);
			assert(!bool(f->join));
			BOOST_LOG_TRIVIAL(trace) << "Fiber fork-join: " << f.get();
			if (f->done) {
				if (bool(f->exc)) std::rethrow_exception(f->exc.value());
				return continuation();
			}
			return yield([&](::Omni::Fiber::Restart&& restart) -> void {
				f->join = [restart = std::move(restart), continuation = std::move(continuation)](FiberSwitchThread& f) mutable {
					restart([&] {
						BOOST_LOG_NAMED_SCOPE("JI");
						BOOST_LOG_TRIVIAL(trace) << "Fiber fork-joined: " << &f;
						if (bool(f.exc)) std::rethrow_exception(f.exc.value());
						return continuation();
					});
				};
			});
		}
	}
}
