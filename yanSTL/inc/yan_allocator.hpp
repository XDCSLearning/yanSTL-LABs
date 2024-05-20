#pragma once
#include "yan_type_traits.hpp"
#include <utility>
#include <exception>
#include <format>

namespace my
{

using size_t = std::size_t;

class __alloc_proxy
{
public:
    class memory_leak : public std::exception
    {
    public:
        memory_leak() :
            std::exception("memory leak") {}
        memory_leak(size_t bytes, size_t allocations) :
            std::exception(std::format("memory leaked with {} bytes in {} allocations", bytes, allocations).c_str()) {}
    };

    static __alloc_proxy& get_instance()
    {
        static __alloc_proxy instance;
        return instance;
    }

    // 分配 size 字节的内存
    void* allocate(size_t size)
    {
        /// 在此处添加你的实现。
        return nullptr;
    }

    // 回收ptr指向的内存。为了记录，提供应当被回收的字节数。
    void deallocate(void* ptr, size_t size)
    {
        /// 在此处添加你的实现。
    }

    void reset()
    {
        if (current_allocated_bytes != 0 || current_allocations != 0)
        {
            throw memory_leak(current_allocated_bytes, current_allocations);
        }
        current_allocated_bytes = 0;
        total_allocated_bytes = 0;
        current_allocations = 0;
        total_allocations = 0;
    }

    size_t current_allocated_bytes; // 当前已分配未回收的字节数
    size_t total_allocated_bytes;   // 总共已分配的字节数
    size_t current_allocations;     // 尚未回收的分配数（:= allocate与deallocate的调用次数之差）
    size_t total_allocations;       // 总共已分配的分配数 （:= allocate的调用次数）

private:
    __alloc_proxy()
        : current_allocated_bytes(0), total_allocated_bytes(0),
          current_allocations(0), total_allocations(0) {}
    ~__alloc_proxy()
    {
         reset();
    }

    __alloc_proxy(const __alloc_proxy&) = delete;
    __alloc_proxy& operator=(const __alloc_proxy&) = delete;
};

template <class Ptr, class Size = size_t>
struct allocation_result {
    Ptr ptr;
    Size count;
};

template <typename T>
class allocator
{
public:
    // Member types
     /// 你需要修改在此处的代码。
    using value_type = bool;
    using size_type = bool;
    using difference_type = bool;
    using propagate_on_container_move_assignment = std::true_type;

    // Member functions
    // 分配可容纳n个元素的未初始化连续存储空间。
    [[nodiscard]] constexpr T* allocate(size_type n)
    {
        /// 在此处添加你的实现。
        return nullptr;
    }
    // 分配至少可容纳n个元素，实际上可容纳不小于n的最小的2的幂个元素的未初始化连续存储空间。
    [[nodiscard]] constexpr allocation_result<T*, size_type>
        allocate_at_least(size_type n)
    {
        /// 在此处添加你的实现。
        return { nullptr, 0 };
    }
    // 回收p所指示的、可容纳n个元素的存储空间。
    constexpr void deallocate(T* p, size_type n)
    {
        /// 在此处添加你的实现。
    }

    // 判断同一类模板定义的各分配器实例类型的两个对象是否相等。
    template<typename U>
    constexpr bool operator==(const allocator<U>&) const noexcept
    {
        return true;
    }

private:
    static __alloc_proxy& _proxy()
    {
        return __alloc_proxy::get_instance();
    }
};

template <typename Alloc>
struct allocator_traits
{
    // Member types
    /// 你需要修改在此处的代码。
    using allocator_type = bool;
    using value_type = bool;
    using pointer = bool;
    using const_pointer = bool;
    using void_pointer = bool;
    using const_void_pointer = bool;
    using difference_type = bool;
    using size_type = bool;
    using propagate_on_container_copy_assignment = bool;
    using propagate_on_container_move_assignment = bool;
    using propagate_on_container_swap = bool;
    using is_always_equal = bool;

    // Member alias templates
    /// 你需要修改在此处的代码。
    template <typename T>
    using rebind_alloc = void;
    template <typename T>
    using rebind_traits = void;

    // Member functions
    // 使用 a 申请 n 个 value_type 类型的元素所需的存储空间
    [[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n)
    {
        /// 在此处添加你的实现。
        return nullptr;
    }

    // 申请带提示的内存（如果 allocator 没有该方法，则调用无提示的 allocate）
    [[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n, const_void_pointer hint)
    {
        /// 在此处添加你的实现。
        return nullptr;
    }

    // 分配至少可容纳n个元素的未初始化连续存储空间。默认返回{a.allocate(n), n}。
    [[nodiscard]] static constexpr allocation_result<pointer, size_type>
        allocate_at_least(Alloc& a, size_type n)
    {
        /// 在此处添加你的实现。
        return { nullptr, 0 };
    }

    // 释放内存
    static constexpr void deallocate(Alloc& a, pointer p, size_type n)
    {
        /// 在此处添加你的实现。
    }

    // 在内存上构造对象
    template <typename T, typename... Args>
    static constexpr void construct(Alloc& a, T* p, Args&&... args)
    {
        /// 在此处添加你的实现。
    }

    // 销毁对象
    template <typename T>
    static constexpr void destroy(Alloc& a, T* p)
    {
        /// 在此处添加你的实现。
    }

    // 获取最大可分配的元素数量
    static constexpr size_type max_size(const Alloc& a) noexcept
    {
        /// 在此处添加你的实现。
    }

    // 调用a的select_on_container_copy_construction函数。
    // 若Alloc未实现该函数，则返回a。具体含义将在后续实验深究。
    static constexpr Alloc select_on_container_copy_construction(const Alloc& a)
    {
        /// 在此处添加你的实现。
        return {};
    }
};
}
