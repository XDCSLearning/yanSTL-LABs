#pragma once
#include <memory>
#include <utility>
#include <atomic>
#include <stdexcept>

namespace my {

template <typename T, typename Deleter = std::default_delete<T>>
class unique_ptr;

template <typename T>
class shared_ptr;

template <typename T>
class weak_ptr;

template <typename T>
class enable_shared_from_this;

} // namespace my