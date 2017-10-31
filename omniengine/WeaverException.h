#pragma once

#include <functional>
#include <boost/core/noncopyable.hpp>

#include "Weave.h"

namespace Omni {
	template<template <typename Result> class Continuation, typename Result>
	using ExceptionHandler = CodePiece<Continuation, Result, std::exception_ptr>;

	template<template <typename Result> class Continuation, typename Result>
	class FiberExceptionHandlerImpl {
		public:
			FiberExceptionHandlerImpl(
				Continuation<Result>&& continuation),
				ExceptionHandler<Continuation, Result>&& handler
			: continuation(continuation), handler(handler) {
				install();
			}
			virtual Fiber done(Result && result) { uninstall(); continuation(result) }
			virtual Fiber handle(std::exception_ptr && eptr) { uninstall(); handler(eptr, std::move(continuation)); }
		private:
			ExceptionHandler<Continuation, Result> handler;
			Continuation<Result> continuation;
	};

	template<template <typename Result> class Continuation, typename Result>
	class TryScope : private boost::noncopyable {
		public:
			TryScope(
				ExceptionHandler<Continuation, Result>&& handler,
				Continuation<Result>&& continuation)
			: eh(new FiberExceptionHandlerImpl(continuation, handler)) {}

			Fiber operator()(Result && result) { eh->done(result); }
		private:
			std::unique_ptr<FiberExceptionHandlerImpl> eh;
	};

	template<typename Result>
	class TryScopeContinuation {
		// Curry TryScope to a Continuation
		template<template <typename Result2> class Continuation> using type = TryScope<Continuation, Result>;
	};

	template<template <typename Result> class Continuation, typename Result>
	Fiber WithTry(
		CodePiece<TryScopeContinuation<Result>::template type, Result>&& body,
		ExceptionHandler<Continuation, Result>&& handler,
		Continuation<Result>&& continuation
	) {
		return body(TryScope<Continuation, Result>(handler, continuation));
	}
}
