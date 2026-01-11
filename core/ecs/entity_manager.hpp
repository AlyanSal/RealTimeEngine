#pragma once
#include <cstdint>
#include <vector>
#include <cassert>
#include "entities.hpp"
#include "memory/arena_allocator.hpp"

class EntityManager {
public:
  EntityManager(std::size_t MAX_ENTITIES, Arena& a): MAX{MAX_ENTITIES}, count(0), headNextFreeID(0) {
    assert(MAX_ENTITIES>0);
    entities = a.allocArray<EntityRecord>(MAX_ENTITIES);
    for (uint32_t i{0}; i<MAX_ENTITIES - 1; i++) {
      entities[i].generation = 0;
      entities[i].nextFree = i+1;
    }
    entities[MAX-1].generation = 0;
    entities[MAX-1].nextFree = UINT32_MAX;
  }

  ~EntityManager() = default;

  Entity create() {
    assert(headNextFreeID != UINT32_MAX && "Maximum entity limit reached");
    uint32_t index = headNextFreeID;

    headNextFreeID = entities[headNextFreeID].nextFree;
    count++;

    entities[index].componentsMask = 0b0;

    return Entity(index, entities[index].generation);
  }

  void destroy(Entity e) {
    assert(is_alive(e));

    uint32_t index = e.id;

    entities[index].generation++;

    entities[index].nextFree = headNextFreeID;
    headNextFreeID = index;

    count--;

    /* TODO: Also remove the component data from the component stores */
  }

  bool is_alive(Entity e) {
    return (e.id < MAX) && (entities[e.id].generation == e.generation);
  }

private:
  EntityRecord* entities;
  uint32_t count;
  uint32_t headNextFreeID;
  std::size_t MAX;
};