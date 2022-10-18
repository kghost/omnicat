#pragma once

#include <set>

#include "Fiber.h"

namespace Omni {
namespace Fiber {

class Event {
public:
  bool IsSet() { return _IsSet; }
  void Reset() { _IsSet = false; }

  void Set();

private:
  class Awaitable {
  public:
    Awaitable(Event &event) : _Event(event) {}

    bool await_ready() { return _Event.IsSet(); }
    void await_suspend(std::coroutine_handle<> caller);
    void await_resume() {}

  private:
    Event &_Event;
  };

public:
  Awaitable operator co_await();

private:
  bool _IsSet = false;
  std::set<std::reference_wrapper<Fiber>,
           typename decltype([](const auto &lhs, const auto &rhs) -> bool { return std::addressof(lhs.get()) < std::addressof(rhs.get()); })>
      _PendingSet;
};

} // namespace Fiber
} // namespace Omni
