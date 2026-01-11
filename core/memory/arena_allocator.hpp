#pragma once
#include <cstddef>
#include <cassert>
#include <cstdint>


/**
 * @class Arena
 * @brief Implements an arena based memory allocator
 * 
 * Supports memory allocation and deallocation in O(1) time
 */
class Arena{
private:
  std::byte* base;
  std::size_t capacity;
  std::size_t offset;

public:
  /**
   * @brief Constructor for the arena allocator
   */
  Arena() : base(nullptr), capacity(0), offset(0) {}

  Arena(std::size_t size) : capacity(size), offset(0) {
    base = new std::byte[size];
  }

  Arena(const Arena&) = delete;
  Arena& operator=(const Arena&) = delete;

  Arena(Arena&& other) noexcept : base(other.base), capacity(other.capacity), offset(other.offset) {
    other.base = nullptr;
    other.capacity = 0;
    other.offset = 0;
  }

  ~Arena() {
    delete[] base;
  }

  /**
   * @brief Allocates contiguous memory as requested
   * 
   * Changes the current offset of the arena to hand out memory
   * when requested by external functions
   * 
   * @param size The amount of memory requested
   * @param align The alignment needed by the datatype, use alignof()
   * 
   * @exception
   * 
   * @return std::byte A pointer in the arena to the requested memory
   */
  std::byte* alloc(std::size_t size, std::size_t align = alignof(std::max_align_t)) {
    std::uintptr_t currPtr = reinterpret_cast<std::uintptr_t>(base + offset);

    std::size_t padding{};
    std::size_t modulo = currPtr % align;
    if (modulo != 0) {
      padding = align - modulo;
    }

    assert(offset + padding + size <= capacity && "Arena Out Of Memory!");

    offset += padding;
    std::byte* ptr = base + offset;
    offset += size;

    return ptr;
  }

  /**
   * @brief Allocates memory for a single instance of an object T
   * 
   * @tparam T Type of the variable
   * @return Address in the Arena interpreted as the data type T
   */
  template <typename T>
  T* alloc() {
    return reinterpret_cast<T*>(alloc(sizeof(T), alignof(T)));
  }

  /**
   * @brief Allocates memory for specified instances of an object T
   * 
   * @tparam T Type of the variable
   * @param count The number of objects of the type T that need memory
   * @return Address of the array of type T in the arena 
   */
  template <typename T>
  T* allocArray(std::size_t count) {
    return reinterpret_cast<T*>(alloc(sizeof(T) * count, alignof(T)));
  }

  /**
   * @brief Immediately resets all memory in arena
   * 
   * All memory handed out by arena is 'freed' by resetting 
   * the value of the offset pointer to zero
   */
  void clear() {
    offset = 0;
  }
};