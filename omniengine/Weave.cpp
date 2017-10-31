#include "../common/header.h"

#include "Weave.h"

#include <list>
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

		std::list<FiberContext> fibers;

		template<template <typename Result> class Continuation, typename Result, typename ... Args>
		using CodePiece = std::function<Fiber(Args && ..., Continuation<Result>&&)>;

		template<typename ... Result>
		void run(std::function<void()>&& body) {
			auto fc = fibers.emplace_back();
			fc.me = fibers.back();
			ScheduleOutFinal(
				ScheduleIn(fc, body)
			);
		}

		template<template <typename Result> class Continuation>
		Fiber ScheduleIn(FiberContext& context, Continuation<>&& continuation) {
		}

		Fiber ScheduleOut() {
		}

		void ScheduleOutFinal(Fiber fiber) {
		}
	}
}
