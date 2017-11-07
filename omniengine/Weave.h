#pragma once

#include <cassert>
#include <memory>
#include <stack>
#include <functional>

#include "shared.h"

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

		class FiberSwitch;
		typedef std::shared_ptr<FiberSwitch> Fiber;

		class FiberExceptionHandler {
			public:
				virtual ~FiberExceptionHandler() {
#ifndef NDEBUG
					assert(!installed);
#endif
				}

				virtual Fiber handle(std::exception_ptr && eptr);
			protected:
				void install();
				void uninstall();
			private:
#ifndef NDEBUG
				bool installed = false;
#endif
		};


		template<typename Continuation, typename ... Args>
		using CodePiece = std::function<Fiber(Args && ..., Continuation&&)>;

		typedef std::function<Fiber()> ContinuationType;
		typedef ContinuationType&& Continuation;

		typedef std::function<void(Continuation&&)> Restart;
		SHARED Fiber yield(std::function<void(Restart&&)>&& finalize);

		SHARED void run(CodePiece<Continuation>&& body);
		SHARED Fiber fork(CodePiece<Continuation>&& body);
		SHARED Fiber join(Fiber fiber, Continuation continuation);

		SHARED Fiber join(std::stack<Fiber> &&fs,  Continuation continuation);
	}

	template<typename ... Result> using Completion = std::function<Fiber::Fiber(Result && ...)>&&;
}
