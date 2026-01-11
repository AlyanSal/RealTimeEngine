#pragma once
#include <algorithm>
#include <cassert>
#include "entities.hpp"
#include "../memory/arena_allocator.hpp"

#define INVALID_ENTRY UINT32_MAX

template <typename T>
class ComponentStore {
  void init(Arena& arena, uint32_t maxEntities, uint32_t maxComponents) {
    m_data = arena.allocArray<T>(maxComponents);
    m_entities = arena.allocArray<Entity>(maxComponents);
    m_sparse = arena.allocArray<uint32_t>(maxEntities);

    std::fill_n(m_sparse, maxEntities, INVALID_ENTRY);

    m_size = 0;
    m_capacity = maxComponents;
  }

  bool has(Entity e) const {
    return m_sparse[e.id] != INVALID_ENTRY;
  }

  T& get(Entity e) {
    assert(has(e));
    return m_data[m_sparse[e.id]];
  }

  void add(Entity e, const T& data) {
    assert(!has(e));
    assert(m_size < m_capacity);

    uint32_t index = m_size++;

    m_data[index] = value;
    m_sparse[e.id] = index;
    m_entities[index] = e.id;
  }

  void remove(Entity e) {
    assert(has(e));

    uint32_t index = m_sparse[e.id];
    uint32_t last = --m_size;

    m_data[index] = m_data[last];
    Entity moved = m_entities[last];

    m_entities[index] = moved;
    m_sparse[moved.id] = index;

    m_sparse[e.id] = INVALID_ENTRY;
  }

  T* data() {return m_data;}
  Entity* entities() {return entities;}

private:
  T*        m_data;
  Entity*   m_entities;
  uint32_t* m_sparse;

  uint32_t  m_size;
  uint32_t  m_capacity;
};