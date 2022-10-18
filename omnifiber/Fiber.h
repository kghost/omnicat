#pragma once

#include <cassert>
#include <coroutine>
#include <exception>
#include <memory>
#include <optional>
#include <queue>
#include <set>
#include <type_traits>

#include "Coroutine.h"

namespace Omni {
namespace Fiber {

class Fiber {
private:
  class FiberFrame {
  public:
    class Promise {
    public:
      Promise(Fiber &fiber, auto &) : _Fiber(fiber) {}

      Promise(Promise &&) = delete;
      Promise(const Promise &) = delete;

      auto initial_suspend() const noexcept {
        class Awaitor {
        public:
          Awaitor(Fiber &owner) : _Fiber(owner) {}
          constexpr bool await_ready() const noexcept { return false; }
          void await_suspend(std::coroutine_handle<> caller) const noexcept { _Fiber.StartingYield(caller); }
          constexpr void await_resume() const noexcept {}

        private:
          Fiber &_Fiber;
        };
        return Awaitor(_Fiber);
      }

      auto final_suspend() const noexcept {
        class Awaitor {
        public:
          Awaitor(Fiber &owner) : _Fiber(owner) {}
          constexpr bool await_ready() const noexcept { return false; }
          void await_suspend(std::coroutine_handle<> caller) const noexcept { _Fiber._State = State::Finishing; }
          constexpr void await_resume() const noexcept {}

        private:
          Fiber &_Fiber;
        };
        return Awaitor(_Fiber);
      }

      void unhandled_exception() { _Fiber.SetException(std::current_exception()); } // TODO
      FiberFrame get_return_object() { return FiberFrame(std::coroutine_handle<promise_type>::from_promise(*this)); }
      void return_void() {}

    private:
      Fiber &_Fiber;
    };

    using promise_type = Promise;

    void operator co_await() = delete;

    FiberFrame(const FiberFrame &) = delete;
    FiberFrame &operator=(const FiberFrame &) = delete;
    FiberFrame(FiberFrame &&that) = delete;
    FiberFrame &operator=(FiberFrame &&) = delete;

    ~FiberFrame() { _CoroutineState.destroy(); }

  private:
    FiberFrame(std::coroutine_handle<> state) : _CoroutineState(state) {}
    std::coroutine_handle<> _CoroutineState;
  };

public:
  template <typename CoroutineFunction> static FiberFrame Spawn(Fiber &owner, CoroutineFunction function) { co_await function(); }

  enum class State { NotStart, Running, Suspending, Suspended, Ready, Finishing, Finished };
  bool IsFinished() { return _State == State::Finished; }
  void Suspend(std::coroutine_handle<> caller);
  void Join();

private:
  template <typename CoroutineFunction>
  requires std::is_invocable_r_v<Coroutine<void>, CoroutineFunction> Fiber(CoroutineFunction &&function)
      : _OutMostFrame(Spawn(*this, std::forward<CoroutineFunction>(function))) {}

  friend class Manager;
  void SetReady(); // Called by Manager when Manager.Schedule(Fiber&) is called.
  void Resume();   // Called by Manager to schedule this fiber.

  void StartingYield(std::coroutine_handle<> caller);
  void Finishing();
  void SetException(std::exception_ptr eptr);

  State _State = State::NotStart;
  std::optional<std::coroutine_handle<>> _Continuation;
  FiberFrame _OutMostFrame; // This field must initialized later than _Continuation, becasue _Continuation is wroten when initializing Frame
  std::optional<std::exception_ptr> _Exception;
  std::set<std::shared_ptr<Fiber>> _Children;
};

} // namespace Fiber
} // namespace Omni
