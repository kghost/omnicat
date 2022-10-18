#include "../common/header.h"

#include "Manager.h"

#include "Coroutine.h"
#include "Fiber.h"

namespace Omni {
namespace Fiber {

Manager &Manager::GetInstance() {
  static Manager manager;
  return manager;
}

void Manager::Schedule(Fiber &fiber) {
  fiber.SetReady();
  _ReadyQueue.push(fiber);
}

void Manager::RunForEver() {
  assert(!Manager::GetInstance().HasFiberRunning());

  while (!_ReadyQueue.empty()) {
    Fiber &fiber = _ReadyQueue.front();
    _ReadyQueue.pop();
    auto setter = CurrentFiberSetter(fiber);
    fiber.Resume();
  }
}

} // namespace Fiber
} // namespace Omni