#pragma once
#include "yan_type_traits.hpp"
#include <utility>
#include <exception>
#include <format>

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

    // ���� size �ֽڵ��ڴ�
    void* allocate(size_t size)
    {
        /// �ڴ˴�������ʵ�֡�
        return nullptr;
    }

    // ����ptrָ����ڴ档Ϊ�˼�¼���ṩӦ�������յ��ֽ�����
    void deallocate(void* ptr, size_t size)
    {
        /// �ڴ˴�������ʵ�֡�
    }

    void reset()
    {
        if (current_allocated_bytes != 0 || current_allocations != 0)
        {
            throw memory_leak(current_allocated_bytes, current_allocations);
        }
        reset_uncheck();
    }

    size_t current_allocated_bytes; // ��ǰ�ѷ���δ���յ��ֽ���
    size_t total_allocated_bytes;   // �ܹ��ѷ�����ֽ���
    size_t current_allocations;     // ��δ���յķ�������:= allocate��deallocate�ĵ��ô���֮�
    size_t total_allocations;       // �ܹ��ѷ���ķ����� ��:= allocate�ĵ��ô�����

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
     /// ����Ҫ�޸��ڴ˴��Ĵ��롣
    using value_type = bool;
    using size_type = bool;
    using difference_type = bool;
    using propagate_on_container_move_assignment = std::true_type;

    // Member functions
    // ���������n��Ԫ�ص�δ��ʼ�������洢�ռ䡣
    [[nodiscard]] constexpr T* allocate(size_type n)
    {
        /// �ڴ˴�������ʵ�֡�
        return nullptr;
    }
    // �������ٿ�����n��Ԫ�أ�ʵ���Ͽ����ɲ�С��n����С��2���ݸ�Ԫ�ص�δ��ʼ�������洢�ռ䡣
    [[nodiscard]] constexpr allocation_result<T*, size_type>
        allocate_at_least(size_type n)
    {
        /// �ڴ˴�������ʵ�֡�
        return { nullptr, 0 };
    }
    // ����p��ָʾ�ġ�������n��Ԫ�صĴ洢�ռ䡣
    constexpr void deallocate(T* p, size_type n)
    {
        /// �ڴ˴�������ʵ�֡�
    }

    // �ж�ͬһ��ģ�嶨��ĸ�������ʵ�����͵����������Ƿ���ȡ�
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

template <typename Alloc>
struct allocator_traits
{
    // Member types
    /// ����Ҫ�޸��ڴ˴��Ĵ��롣
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
    /// ����Ҫ�޸��ڴ˴��Ĵ��롣
    template <typename T>
    using rebind_alloc = void;
    template <typename T>
    using rebind_traits = void;

    // Member functions
    // ʹ�� a ���� n �� value_type ���͵�Ԫ������Ĵ洢�ռ�
    [[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n)
    {
        /// �ڴ˴�������ʵ�֡�
        return nullptr;
    }

    // �������ʾ���ڴ棨��� allocator û�и÷��������������ʾ�� allocate��
    [[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n, const_void_pointer hint)
    {
        /// �ڴ˴�������ʵ�֡�
        return nullptr;
    }

    // �������ٿ�����n��Ԫ�ص�δ��ʼ�������洢�ռ䡣Ĭ�Ϸ���{a.allocate(n), n}��
    [[nodiscard]] static constexpr allocation_result<pointer, size_type>
        allocate_at_least(Alloc& a, size_type n)
    {
        /// �ڴ˴�������ʵ�֡�
        return { nullptr, 0 };
    }

    // �ͷ��ڴ�
    static constexpr void deallocate(Alloc& a, pointer p, size_type n)
    {
        /// �ڴ˴�������ʵ�֡�
    }

    // ���ڴ��Ϲ������
    template <typename T, typename... Args>
    static constexpr void construct(Alloc& a, T* p, Args&&... args)
    {
        /// �ڴ˴�������ʵ�֡�
    }

    // ���ٶ���
    template <typename T>
    static constexpr void destroy(Alloc& a, T* p)
    {
        /// �ڴ˴�������ʵ�֡�
    }

    // ��ȡ���ɷ����Ԫ������
    static constexpr size_type max_size(const Alloc& a) noexcept
    {
        /// �ڴ˴�������ʵ�֡�
    }

    // ����a��select_on_container_copy_construction������
    // ��Allocδʵ�ָú������򷵻�a�����庬�彫�ں���ʵ�����
    static constexpr Alloc select_on_container_copy_construction(const Alloc& a)
    {
        /// �ڴ˴�������ʵ�֡�
        return {};
    }
};
}
