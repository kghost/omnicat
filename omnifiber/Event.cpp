#include "Event.h"

#include "Manager.h"

namespace Omni {
namespace Fiber {

Event::Awaitable Event::operator co_await() { return Event::Awaitable(*this); }

void Event::Set() {
  _IsSet = true;
  Manager &manager = Manager::GetInstance();
  for (auto &fiber : _PendingSet) {
    manager.Schedule(fiber);
  }
  _PendingSet.clear();
}

void Event::Awaitable::await_suspend(std::coroutine_handle<> caller) {
  Fiber &fiber = Manager::GetInstance().GetCurrentFiber();
  fiber.Suspend(caller);
  _Event._PendingSet.insert(fiber);
}

} // namespace Fiber
} // namespace Omni
