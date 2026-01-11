#pragma once
#include <cstdint>
#include <vector>
#include <cassert>
#include "entities.hpp"

class EntityManager {
public:
  EntityManager(std::size_t MAX_ENTITIES) {
    assert(MAX_ENTITIES>0);
    generations = std::vector<uint32_t>(MAX_ENTITIES, 0);
    entities = new EntityRecord[MAX_ENTITIES]{};
    freeEntities = std::vector<uint32_t>(MAX_ENTITIES);
    for (uint32_t i{1}; i<=MAX_ENTITIES; i++) {
      freeEntities.push_back(i);
    }
  }

  Entity& create() {
    uint32_t id = freeEntities.back(); freeEntities.pop_back();
    Entity e(id, generations[id]);
    entities[id].generation = generations[id];
    entities[id].componentsMask = 0b0;

    return e;
  }

  void destroy(Entity e) {
    generations[e.id]++;
    freeEntities.push_back(e.id);

    /* TODO: Also remove the component data from the component stores */
  }

  bool is_alive(Entity e) {
    return e.generation == generations[e.id];
  }

  EntityRecord& getEntityRecord(Entity e) {
    return entities[e.id];
  }

private:
  std::vector<uint32_t> freeEntities;
  std::vector<uint32_t> generations;
  EntityRecord* entities;
};