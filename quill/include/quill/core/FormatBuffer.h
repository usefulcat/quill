/**
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/bundled/fmt/core.h"

#include <cstddef>
#include <memory>
#include <string>

namespace quill::detail
{
/**
 * This is a replication of fmt::basic_memory_buffer with only dynamic storage
 */
class FormatBuffer final : public fmtquill::detail::buffer<char>
{
public:
  using value_type = char;
  using const_reference = char const&;

  FormatBuffer() { this->set(nullptr, 0); }

  ~FormatBuffer() noexcept { _deallocate(); }

  FormatBuffer(FormatBuffer&& other) noexcept { _move(other); }

  FormatBuffer& operator=(FormatBuffer&& other) noexcept
  {
    FMTQUILL_ASSERT(this != &other, "");
    _deallocate();
    _move(other);
    return *this;
  }

  void resize(size_t count) { this->try_resize(count); }

  void reserve(size_t new_capacity) { this->try_reserve(new_capacity); }

  void append(std::string const& str) { append(str.data(), str.data() + str.size()); }

  void append(char const* begin, char const* end)
  {
    while (begin != end)
    {
      auto count = fmtquill::detail::to_unsigned(end - begin);
      this->try_reserve(size_ + count);

      auto free_cap = capacity_ - size_;
      if (free_cap < count)
      {
        count = free_cap;
      }

      std::uninitialized_copy_n(begin, count, ptr_ + size_);
      size_ += count;
      begin += count;
    }
  }

private:
  // Move data from other to this buffer.
  void _move(FormatBuffer& other)
  {
    this->set(other.data(), other.capacity());
    this->resize(other.size());

    // Set pointer to the inline array so that delete is not called
    // when deallocating.
    other.set(nullptr, 0);
    other.clear();
  }

  // Deallocate memory allocated by the buffer.
  void _deallocate()
  {
    if (this->data())
    {
      delete[] this->data();
    }
  }

protected:
  void grow(size_t size) override
  {
    size_t const old_capacity = this->capacity();
    size_t new_capacity = old_capacity * 2;

    if (size > new_capacity)
    {
      new_capacity = size;
    }

    char* old_data = this->data();
    char* new_data = new char[new_capacity];

    // The following code doesn't throw, so the raw pointer above doesn't leak.
    std::uninitialized_copy_n(old_data, this->size(), new_data);
    this->set(new_data, new_capacity);

    if (old_data)
    {
      delete[] old_data;
    }
  }
};
} // namespace quill::detail