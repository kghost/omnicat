#include "../common/header.h"

#include "Fiber.h"

#include <cassert>

#include "Manager.h"

namespace Omni {
namespace Fiber {

void Fiber::Suspend(std::coroutine_handle<> caller) {
  assert(_State == State::Running);
  assert(!_Continuation.has_value());
  _Continuation.emplace(caller);
  _State = State::Suspending;
}

void Fiber::StartingYield(std::coroutine_handle<> caller) {
  assert(_State == State::NotStart);
  assert(!_Continuation.has_value());
  _Continuation.emplace(caller);
  _State = State::Ready;
}

void Fiber::SetReady() {
  assert(_State == State::Suspended);
  _State = State::Ready;
}

void Fiber::Resume() {
  assert(_State == State::Ready);
  _State = State::Running;
  std::coroutine_handle<> cont = _Continuation.value();
  _Continuation.reset();
  cont.resume();
  switch (_State) {
  case State::Suspending:
    assert(_Continuation.has_value());
    _State = State::Suspended;
    return;
  case State::Finishing:
    assert(!_Continuation.has_value());
    assert(_Children.empty());
    _State = State::Finished;
    return;
  default:
    assert(false);
  }
}

void Fiber::Finishing() {
  assert(_State == State::Running);
  _State = State::Finishing;
}

void Fiber::SetException(std::exception_ptr eptr) {
  assert(_State == State::Running);
  _Exception.emplace(eptr);
  _State = State::Finishing;
}

} // namespace Fiber
} // namespace Omni