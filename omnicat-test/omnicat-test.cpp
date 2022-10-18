// omnicat-test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../omnifiber/Event.h"
#include "../omnifiber/Fiber.h"
#include "../omnifiber/Manager.h"

using namespace Omni::Fiber;

class A {};
class B {
public:
  B(A) {}
};

Event e;

Coroutine<void> TestVoid() {
  co_await e;
  co_return;
}

Coroutine<void> TestVoid2() {
  co_await TestVoid();
  co_return;
}

Coroutine<A> Test() { co_return A(); }

Coroutine<B> Test2() { co_return B(co_await Test()); }

Coroutine<void> Test3() {
  co_await Test2();
  co_return;
}

int main() {
  auto &manager = Manager::GetInstance();
  Fiber &fiber = manager.SpawnRoot(Test3);
  manager.RunForEver();
  e.Set();
  manager.RunForEver();
  assert(fiber.IsFinished());
}
