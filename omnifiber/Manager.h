#pragma once

#include <cassert>
#include <coroutine>
#include <exception>
#include <memory>
#include <optional>
#include <queue>
#include <set>
#include <type_traits>

namespace Omni {
namespace Fiber {

class Fiber;

class Manager {
public:
  static Manager &GetInstance();

  bool HasFiberRunning() const { return _CurrentFiber.has_value(); }

  Fiber &GetCurrentFiber() const {
    assert(_CurrentFiber.has_value());
    return _CurrentFiber.value();
  }

  void Schedule(Fiber &fiber); // Mark the fiber ready to schedule.
  void RunForEver();           // Run until all fibers are finished.

  class CurrentFiberSetter {
  public:
    CurrentFiberSetter(Fiber &current, Manager &manager = GetInstance())
#ifndef NDEBUG
        : _Manager(manager), _Current(current)
#endif
    {
      assert(!_Manager.HasFiberRunning());
      _Manager._CurrentFiber.emplace(current);
    }

    ~CurrentFiberSetter() {
#ifndef NDEBUG
      assert(&_Manager.GetCurrentFiber() == &_Current);
#endif
      _Manager._CurrentFiber.reset();
    }

  private:
    Manager &_Manager;
#ifndef NDEBUG
    Fiber &_Current;
#endif
  };

  template <typename CoroutineFunction>
  requires std::is_invocable_r_v<Coroutine<void>, CoroutineFunction> Fiber &SpawnRoot(CoroutineFunction &&function) {
    _RootFiber.reset(new Fiber(std::forward<CoroutineFunction>(function)));
    auto &fiber = *_RootFiber;
    _ReadyQueue.push(fiber);
    return fiber;
  }

private:
  Manager() {}
  Manager(Manager &&) = delete;
  Manager(const Manager &) = delete;

  std::optional<std::reference_wrapper<Fiber>> _CurrentFiber;
  std::queue<std::reference_wrapper<Fiber>> _ReadyQueue;
  std::shared_ptr<Fiber> _RootFiber;
};

} // namespace Fiber
} // namespace Omni
