#pragma once

#include <cassert>
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


#ifndef NDEBUG
		typedef int Fiber;
#else
		typedef void Fiber;
#endif

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
		extern std::list<FiberContext> fibers;
		class FiberContext {
			public:
				decltype(fibers)::const_iterator me;
				std::stack<FiberExceptionHandler*> ehs;
		};

		extern thread_local FiberContext* current;

		template<template <typename Result> class Continuation, typename Result, typename ... Args>
		using CodePiece = std::function<Fiber(Args && ..., Continuation<Result>&&)>;

		void run(std::function<void()>&& body);

		template<template <typename Result> class Continuation, typename ... Args>
		Fiber ScheduleIn(FiberContext& context, Continuation<Args ...>&& continuation) {
			current = &context;
		}

		Fiber ScheduleOut() {
		}

		void ScheduleOutFinal(Fiber fiber) {
		}
	}
}
