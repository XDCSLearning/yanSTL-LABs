#include "co_yantest.hpp"
#include "int_wrapper.hpp"
#include "yan_allocator.hpp"
#include <memory>

#define USE_STD

#ifdef USE_STD
#define NAMESPACE_MY ::std::
#else
#define NAMESPACE_MY ::my::
#endif

namespace my
{
    namespace test
    {

auto& ap = _alloc_proxy::get_instance();

case_t unique_ptr()
{
    co_yield "Create a unique_ptr `p1` with default construction.";
    NAMESPACE_MY unique_ptr<int> p1;
    co_yield{ p1.get() == nullptr, "`p1` should return a nullptr when get() called."};
    co_yield{ p1 == nullptr, "`p1` should be a nullptr, and should be eqeal to a nullptr_t value." };
    co_yield{ !p1, "`p1` should be a nullptr, and should return a false when being converted to bool." };

    co_yield "auto p = new int(43);";
    co_yield "p1.reset(p);";
    auto p = new int(43);
    p1.reset(p);
    co_yield{ !!p1 , "`p1` should hold a non-nil pointer, and should return a true when being converted to bool." };
    co_yield{ p1 != nullptr , "`p1` should hold a non-nil pointer, and should be not eqeal to a nullptr_t value." };
    co_yield{ p1.get() == p, "`p1.get()` should return pointer `p`."};
    co_yield nullptr;

    co_yield "Create a unique_ptr `p2` with pointer construction, Argument = `new int(42)`.";
    NAMESPACE_MY unique_ptr<int> p2(new int(42));
    co_yield{ !!p2, "`p2` should hold a non-nil pointer, and should return a true when being converted to bool." };
    co_yield{ p2 != nullptr , "`p2` should hold a non-nil pointer, and should be not eqeal to a nullptr_t value." };
    co_yield{ *p2 == 42, "Value that `p2` stored should be `42`." };

    co_yield "Create a unique_ptr `p3` by moving `p2`.";
    NAMESPACE_MY unique_ptr<int> p3(move(p2));
    co_yield{ !p2, "Moved-from unique_ptr `p2` should be empty now." };
    co_yield{ p3 && *p3 == 42, "Moved-to unique_ptr `p3` should a non-nil pointer and hold the value `42`." };
    co_yield "divide `*p3` by 2.";
    *p3 /= 2;
    co_yield{*p3 == 21, "`p3` should hold a value `21` now." };

    co_yield "Create a unique_ptr `p4`, and do assignment with `p3`";
    NAMESPACE_MY unique_ptr<int> p4;
    p4 = std::move(p3);
    co_yield{ !p3, "After move assignment, source `p3` should be empty." };
    co_yield{ p4 && *p4 == 21, "Move-assigned unique_ptr `p4` should hold the value `21`." };

    co_yield "Release `p4`.";
    int* raw_ptr = p4.release();
    co_yield{ !p4, "After release, unique_ptr `p4` should be empty." };
    co_yield{ raw_ptr && *raw_ptr == 42, "`p4.release()` should return a pointer that holds the value `21`." };
    delete raw_ptr;
    co_yield nullptr;

    co_yield "struct V2 { int value; };\np5(new V2(77));";
    struct V2
    {
        V2(int value) : value(value) {}
        int value;
        bool isHeapAllocated = false;
        static void* operator new(size_t size) {
            void* ptr = ::operator new(size);
            static_cast<V2*>(ptr)->isHeapAllocated = true;
            return ptr;
        }
        static void operator delete(void* ptr) {
            ::operator delete(ptr);
        }
    };
    NAMESPACE_MY unique_ptr<V2> p5(new V2(77));
    co_yield{ p5 && p5->isHeapAllocated, "`p5`'s value should be allocated by operator new but it appears not." };
    co_yield{ p5 && p5->value == 77, "`p5->value` should be `77`." };
    co_yield "let `p5->value` be `42`.";
    p5->value = 42;
    co_yield{ p5 && (*p5).value == 42, "`(*p5).value` should be `42` now." };
    co_yield nullptr;

    co_yield "Let `p6` handle a int array.";
    NAMESPACE_MY unique_ptr<int[]> p6(new int[3] {1, 2, 3});
    co_yield{ p6[0] == 1 && p6[1] == 2 && p6[2] == 3, "operator[] should access array elements correctly." };
    co_yield nullptr;

    co_yield "Create a unique_ptr `p7` holds `V2(88)` by make_unique.";
    auto p7 = NAMESPACE_MY make_unique<V2>(88);
    co_yield{ p7 && p7->isHeapAllocated, "`p7`'s value should be allocated by operator new but it appears not." };
    co_yield{ p7 && p7->value == 88, "make_unique `p7` should hold a value `88`." };

    co_yield "Testing custom deleter...";
    bool deleter_called = false;
    auto custom_deleter = [&](int* ptr) { delete ptr; deleter_called = true; };
    {
        NAMESPACE_MY unique_ptr<int, decltype(custom_deleter)> p10(new int(99), custom_deleter);
    }
    co_yield{ deleter_called, "Custom deleter should be called on destruction" };

    co_yield nullptr;
    co_return;
}

    } // namespace my::test
} // namespace my
int main()
{
    my::test::test t;
    t.new_case(my::test::unique_ptr(), "unique_ptr");
}