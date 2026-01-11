#pragma once
#include <cstdint>
#include "entity_manager.hpp"

class Entity {
protected:
  Entity(uint32_t id, uint32_t gen) : id{id}, generation{gen} {};
  uint32_t id;
  uint32_t generation;

  friend EntityManager;
};

class EntityRecord {
protected:
  EntityRecord(uint32_t gen = 0, uint32_t mask = 0) : generation{gen}, componentsMask{mask} {};
  uint32_t generation;
  uint32_t componentsMask;

  friend EntityManager;
};