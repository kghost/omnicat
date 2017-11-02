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

		class FiberContext;
		std::list<FiberContext> fibers;
		class FiberContext {
		public:
			decltype(fibers)::const_iterator me;
			std::stack<FiberExceptionHandler*> ehs;
		};
		thread_local FiberContext* current;

		// Short living object for user to operate the fiber.
		class FiberSwitchImpl : public FiberSwitch {
		public:
			FiberSwitchImpl(FiberContext* ctxt) : ctxt(ctxt) {}
			virtual ~FiberSwitchImpl() {}
		private:
			FiberContext* ctxt;
		};

		Fiber unwind(std::stack<FiberExceptionHandler*>& ehs, std::exception_ptr && eptr) {
			FiberExceptionHandler* n = ehs.top();
			ehs.pop();
			try {
				return n->handle(std::move(eptr));
			} catch (...) {
				return unwind(ehs, std::current_exception());
			}
		}

		Fiber unwind(std::exception_ptr && eptr) {
			return unwind(current->ehs, std::move(eptr));
		}

		Fiber create() {
			auto fc = fibers.emplace_front();
			fc.me = fibers.begin();
			return std::unique_ptr<FiberSwitch>(new FiberSwitchImpl(&fc));
		}

		Fiber in(std::function<Fiber()>&& continuation) {
			try {
				return continuation();
			} catch (...) {
				return unwind(std::current_exception());
			}
		}

		SHARED void run(CodePiece<std::function<Fiber()>&&>&& body) {
			auto fiber = create();
			in([body = std::move(body)]{return body([] { return nullptr; });});
		}

		Fiber fork() {
			create();
		}

		SHARED Fiber yield(std::function<void(std::function<void(Restart&&)>)>&& finalize) {
			// out
			finalize([](Restart&& continuation) -> void {
				in(std::move(continuation));
			});
			return Fiber();
		}
	}
}
