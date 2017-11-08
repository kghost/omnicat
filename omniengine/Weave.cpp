#include "../common/header.h"

#include "Weave.h"

#include <list>
#include <stack>
#include <functional>

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
		//    Fiber WithTry([] -> Fiber { body }, handle-fail, Fiber (post-body)(V, Continuation))
		//
		//  post-pause is a continuation object. Many language can auto generate it by using CPS transform, but in c++ world, we need to write it manually.
		//  CPS form: instead returns a value , pausable returns a future

		class FiberSwitch {
		public:
			virtual ~FiberSwitch() {}
			virtual Fiber unwind(std::exception_ptr && eptr) = 0;
		};

		class FiberSwitchEnd : public FiberSwitch {
		public:
			virtual ~FiberSwitchEnd() {}
			virtual Fiber unwind(std::exception_ptr && eptr) {
				return unwind(std::move(eptr));
			}
		};

		class FiberSwitchYield : public FiberSwitch {
		public:
			virtual ~FiberSwitchYield() {}
			virtual Fiber unwind(std::exception_ptr && eptr) {
				return unwind(std::move(eptr));
			}
			std::shared_ptr<FiberSwitch> continued ;
			bool switched = false;
		};

		class FiberSwitchMain : public FiberSwitch {
		public:
			virtual ~FiberSwitchMain() {}
			virtual Fiber unwind(std::exception_ptr && eptr) {
				std::rethrow_exception(eptr);
			}
		};

		class FiberSwitchThread : public FiberSwitchMain {
		public:
			FiberSwitchThread() {}
			virtual ~FiberSwitchThread() {}
			virtual Fiber unwind(std::exception_ptr && eptr) {
				std::rethrow_exception(eptr);
			}

			std::function<void()> join;
			bool done = false;
		};

		SHARED void run(CodePiece<Continuation>&& body) {
			auto fiber = std::make_shared<FiberSwitchMain>();
			try {
				auto inner = body([] { return std::make_shared<FiberSwitchEnd>(); });;
			} catch (...) {
				fiber->unwind(std::current_exception());
			}
		}

		SHARED Fiber yield(std::function<void(Restart&&)>&& finalize) {
			auto fiber = std::make_shared<FiberSwitchYield>();
			finalize([outer = fiber](Continuation continuation) mutable -> void {
				if (!outer->switched) {
					outer->continued = continuation();
				} else {
					outer.reset();
					try {
						continuation();
					} catch (...) {
						outer->unwind(std::current_exception());
					}
				}
			});
			fiber->switched = true;
			return fiber->continued ? fiber->continued : fiber;
		}

		SHARED Fiber fork(CodePiece<Continuation>&& body) {
			auto fiber = std::make_shared<FiberSwitchThread>();
			try {
				auto inner = body([fiber] {
					fiber->done = true;
					if (bool(fiber->join)) fiber->join();
					return std::make_shared<FiberSwitchEnd>();
				});
			} catch (...) {
				return fiber->unwind(std::current_exception());
			}
			return fiber;
		}

		SHARED Fiber join(Fiber fiber, Continuation continuation) {
			auto f = std::dynamic_pointer_cast<FiberSwitchThread>(fiber);
			if (f->done) return continuation();
			else return yield([&](::Omni::Fiber::Restart&& restart) -> void {
				f->join = [restart = std::move(restart), continuation = std::move(continuation)]() mutable {
					restart(std::move(continuation));
				};
			});
		}

		SHARED Fiber join(std::stack<Fiber> &&fs,  Continuation continuation) {
			if (fs.empty())
				return continuation();
			else {
				auto head = fs.top();
				fs.pop();
				return join(head, [fs = std::move(fs), continuation = std::move(continuation)]() mutable {
					return join(std::move(fs), std::move(continuation));
				});
			}
		}
	}
}
