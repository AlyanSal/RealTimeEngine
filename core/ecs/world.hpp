#pragma once
#include <functional>
#include <vector>
#include "core/memory/arena_allocator.hpp"
#include "component_store.hpp"

class World {
private:
  Arena arena;
  EntityManager entityManager;
  std::vector<InterfaceComponentStore*> components;

public:
  using SysFunc = void(*)(World&);
  std::vector<std::vector<std::pair<SysFunc, uint32_t>>> systems;
};