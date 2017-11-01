#pragma once

#include <cassert>
#include <memory>
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

		class FiberA;
		typedef std::shared_ptr<FiberA> Fiber;

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

		class FiberContext ;
		class SHARED FiberA {
		public:
			FiberA(FiberContext* ctxt) : ctxt(ctxt) {}
			void restart(std::function<Fiber()>&& continuation);
		private:
			FiberContext* ctxt;
		};

		template<typename Continuation, typename ... Args>
		using CodePiece = std::function<Fiber(Args && ..., Continuation&&)>;

		SHARED void run(CodePiece<std::function<Fiber()>>&& body);
		SHARED Fiber ScheduleOut();
	}

	template<typename ... Result> using Completion = std::function<Fiber::Fiber(Result && ...)>&&;
}
