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
		//    Fiber WithTry([] -> Fiber { body }, handle-fail, Fiber (post-body)(V, ContinuationRef))
		//
		//  post-pause is a continuation object. Many language can auto generate it by using CPS transform, but in c++ world, we need to write it manually.
		//  CPS form: instead returns a value , pausable returns a future

		class FiberSwitch;
		typedef std::shared_ptr<FiberSwitch> Fiber;

		template<typename ContinuationRef, typename ... Args> using CodePiece = std::function<Fiber(Args && ..., ContinuationRef)>;
		typedef std::function<Fiber()> Continuation;
		typedef Continuation&& ContinuationRef;

		typedef std::function<void(ContinuationRef)> Restart;
		SHARED Fiber yield(std::function<void(Restart&&)>&& finalize);

		SHARED void run(CodePiece<ContinuationRef>&& body);

		SHARED Fiber handle(CodePiece<ContinuationRef>&& body, CodePiece<ContinuationRef, std::exception_ptr>&& handler, ContinuationRef continuation);

		SHARED Fiber fork(std::string && name, CodePiece<ContinuationRef>&& body);
		SHARED Fiber join(Fiber fiber, ContinuationRef continuation);

		template<typename Iterator>
		static Fiber join(Iterator begin, Iterator end, ContinuationRef continuation) {
			if (begin == end)
				return continuation();
			else {
				auto& next = *begin;
				return join(next, [begin = ++begin, end, continuation = std::move(continuation)]() mutable {
					return join(begin, end, std::move(continuation));
				});
			}
		}
	}

	template<typename ... Result> using Completion = std::function<Fiber::Fiber(Result && ...)>&&;
}
