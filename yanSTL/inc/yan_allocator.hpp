#pragma once
#include "yan_type_traits.hpp"
#include <utility>
#include <exception>
#include <format>
#include <cmath>

namespace my
{

using size_t = std::size_t;

class _alloc_proxy
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

    static _alloc_proxy& get_instance()
    {
        static _alloc_proxy instance;
        return instance;
    }

    // 分配 size 字节的内存
    void* allocate(size_t size)
    {
        
        auto t = ::operator new(size);
        if(t != nullptr){
            total_allocated_bytes += size;
            current_allocated_bytes += size;
            current_allocations += 1;
            total_allocations += 1;
        }else{
	      std::__throw_bad_array_new_length();
        }
        return t;
    }

    // 回收ptr指向的内存。为了记录，提供应当被回收的字节数。
    void deallocate(void* ptr, size_t size)
    {
        ::operator delete(ptr 
#if __cpp_sized_deallocation
			  , size
#endif
        );
        current_allocated_bytes -= size;
        current_allocations -= 1;
        /// 在此处添加你的实现。
    }

    void reset()
    {
        if (current_allocated_bytes != 0 || current_allocations != 0)
        {
            throw memory_leak(current_allocated_bytes, current_allocations);
        }
        reset_uncheck();
    }

    size_t current_allocated_bytes; // 当前已分配未回收的字节数
    size_t total_allocated_bytes;   // 总共已分配的字节数
    size_t current_allocations;     // 尚未回收的分配数（:= allocate与deallocate的调用次数之差）
    size_t total_allocations;       // 总共已分配的分配数 （:= allocate的调用次数）

    void reset_uncheck()
    {
        current_allocated_bytes = 0;
        total_allocated_bytes = 0;
        current_allocations = 0;
        total_allocations = 0;
    }
private:
    _alloc_proxy()
        : current_allocated_bytes(0), total_allocated_bytes(0),
          current_allocations(0), total_allocations(0) {}
    ~_alloc_proxy()
    {
    }
    _alloc_proxy(const _alloc_proxy&) = delete;
    _alloc_proxy& operator=(const _alloc_proxy&) = delete;
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
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type;

    // Member functions
    // 分配可容纳n个元素的未初始化连续存储空间。
    [[nodiscard]] constexpr T* allocate(size_type n)
    {
        /// 在此处添加你的实现。
        size_type __n = n;
	    if (__builtin_mul_overflow(__n, sizeof(T), &__n))
	      std::__throw_bad_array_new_length();
        return static_cast<T*>(_proxy().allocate(n));
    }
    // 分配至少可容纳n个元素，实际上可容纳不小于n的最小的2的幂个元素的未初始化连续存储空间。
    [[nodiscard]] constexpr allocation_result<T*, size_type>
        allocate_at_least(size_type n)
    {
        /// 在此处添加你的实现。
        size_type __n = std::pow(2, n);
        return {allocate(__n), __n};
    }
    // 回收p所指示的、可容纳n个元素的存储空间。
    constexpr void deallocate(T* p, size_type n)
    {
        /// 在此处添加你的实现。
        _proxy().deallocate(p, n);
    }

    // 判断同一类模板定义的各分配器实例类型的两个对象是否相等。
    template<typename U>
    constexpr bool operator==(const allocator<U>&) const noexcept
    {
        return true;
    }

private:
    static _alloc_proxy& _proxy()
    {
        return _alloc_proxy::get_instance();
    }
};

template <typename _Def, template<typename...> class _Op, typename ..._Args>
    struct __detected_or
    {
        using type = _Def;
        using __is_detected = std::false_type;
    };

template <typename _Def, template<typename...> class _Op, typename ..._Args>
    requires requires{typename _Op<_Args...>;}
    struct __detected_or<_Def, _Op, _Args...>
    {
        using type = _Op<_Args...>;
        using __is_detected = std::true_type;
    };

template <typename Default, template<typename...> class _Op, typename ..._Args>
    using __detected_or_t = __detected_or<Default, _Op, _Args...>::type;


struct __allocator_traits_base
{
protected:
    template<typename _Tp>
      using __pointer = typename _Tp::pointer;
    template<typename _Tp>
      using __c_pointer = typename _Tp::const_pointer;
    template<typename _Tp>
      using __v_pointer = typename _Tp::void_pointer;
    template<typename _Tp>
      using __cv_pointer = typename _Tp::const_void_pointer;
    template<typename _Tp>
      using __pocca = typename _Tp::propagate_on_container_copy_assignment;
    template<typename _Tp>
      using __pocma = typename _Tp::propagate_on_container_move_assignment;
    template<typename _Tp>
      using __pocs = typename _Tp::propagate_on_container_swap;
    template<typename _Tp>
      using __equal = std::__type_identity<typename _Tp::is_always_equal>;
    template<typename _Tp>
      using __diff = typename _Tp::difference_type;
};

template <typename Alloc> 
struct allocator_traits : __allocator_traits_base
{
public:
    // Member types
    /// 你需要修改在此处的代码。
    using allocator_type = Alloc;
    using value_type = Alloc::value_type;
    using pointer = __detected_or_t<value_type*, __pointer, Alloc>;

private:
    template<template<typename...> class _Func, typename _Tp>
    using _Ptr = __detected_or_t<typename std::pointer_traits<pointer>::template rebind<_Tp>, _Func, allocator_type>;

    // Select _A2::difference_type or pointer_traits<_Ptr>::difference_type
    template<typename _A2, typename _PtrT>
    struct _Diff{
        using type = std::pointer_traits<_PtrT>::difference_type;
    };
    template<typename _A2, typename _PtrT> requires requires{typename _A2::difference_type;}  
    struct _Diff<_A2, _PtrT>{
        using type  = _A2::difference_type;
    };
      // Select _A2::size_type or make_unsigned<_DiffT>::type
    template<typename _A2, typename _DiffT>
	struct _Size : std::make_unsigned<_DiffT> { };

      template<typename _A2, typename _DiffT> requires requires{typename _A2::size_type;}
	struct _Size<_A2, _DiffT>
	{ using type = typename _A2::size_type; };

public:
    // Member types
    /// 你需要修改在此处的代码。
    using const_pointer = _Ptr<__c_pointer, const value_type>;
    using void_pointer = _Ptr<__v_pointer, void>;
    using const_void_pointer =_Ptr<__cv_pointer, const void>;
    using difference_type = _Diff<Alloc, pointer>;
    using size_type = typename _Size<Alloc, difference_type>::type;
    using propagate_on_container_copy_assignment = __detected_or_t<std::false_type, __pocca, Alloc>;
    using propagate_on_container_move_assignment = __detected_or_t<std::false_type, __pocma, Alloc>;
    using propagate_on_container_swap = __detected_or_t<std::false_type, __pocs, Alloc>;
    using is_always_equal = true;

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
        return a.allocate(n);
    }

    // 申请带提示的内存（如果 allocator 没有该方法，则调用无提示的 allocate）
    [[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n, const_void_pointer hint)
    {
        /// 在此处添加你的实现。
        if(requires{
            a.allocate(n, hint);
        }){
            return a.allocate(n, hint);
        }else{
            return a.allocate(n);
        }
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
