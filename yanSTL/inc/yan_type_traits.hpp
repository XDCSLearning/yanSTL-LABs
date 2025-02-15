#pragma once

namespace my
{

// 仅当T具有可接受Args...类型实参的构造函数时返回真。
template <typename T, typename... Args>
inline constexpr bool is_constructible_v = __is_constructible(T, Args...);


template <typename T>
inline constexpr bool is_const_v = false;
template <typename T>
inline constexpr bool is_const_v<T const> = false;
/// 你需要修改本文件的以下内容。

// 仅当T为void时返回真。
template <typename T>
inline constexpr bool is_void_v = false;

template <>
inline constexpr bool is_void_v<void> = true;

// 仅当T为布尔类型、字符类型、整数类型时返回真。
template <typename T>
inline constexpr bool is_integral_v = is_any_of_v<remove_cv_t<T>,bool,
                                                                char, unsigned char, signed char, 
                                                                short, unsigned short,
                                                                int, unsigned int,
                                                                long, unsigned long,
                                                                long long, unsigned long long>;

// 仅当T具有顶层volatile修饰时返回真。
template <typename T>
inline constexpr bool is_volatile_v = false;

template <typename T>
inline constexpr bool is_volatile_v<volatile T> = true;

// 仅当T为指针时返回真。
template <typename T>
inline constexpr bool is_pointer_v = false;

template <typename T>
inline constexpr bool is_pointer_v<T*> = true;

template <typename T>
inline constexpr bool is_pointer_v<T* const> = true;

template <typename T>
inline constexpr bool is_pointer_v<T* volatile> = true;

template <typename T>
inline constexpr bool is_pointer_v<T* const volatile> = true;

// 仅当T为引用时返回真。
template <typename T>
inline constexpr bool is_reference_v = is_any_of_v(remove_cv_t<T>, );

template <typename T>
inline constexpr bool is_reference_v = is_lvalue_reference<T> || is_rvalue_reference<T>;

template <typename T>
inline constexpr bool is_lvalue_reference = false;

template <typename T>
inline constexpr bool is_lvalue_reference<T&> = false;

template <typename T>
inline constexpr bool is_lvalue_reference<T& volatile> = true;

template <typename T>
inline constexpr bool is_rvalue_reference = false;

template <typename T>
inline constexpr bool is_rvalue_reference<T&&> = false;

template <typename T>
inline constexpr bool is_rvalue_reference<T&& volatile> = true;



// 仅当T为函数时返回真。提示：只有U为函数或引用时，is_const_v<const U>返回假。
template <typename T>
inline constexpr bool is_function_v = !is_reference_v<T> && !is_const_v<T>;

// 仅当T为除了void、函数、引用以外的任何类型时返回真。
template <typename T>
inline constexpr bool is_object_v = !(is_function_v<T> ||is_reference_v<T>||is_void_v<T>);

// 仅当T为原生数组时返回真。
template <typename T>
inline constexpr bool is_array_v = false;

template <typename T>
inline constexpr bool is_array_v<T[]> = true;
template <typename T>
inline constexpr bool is_array_v<T[] const> = true;
template <typename T>
inline constexpr bool is_array_v<T[] volatile> = true;

// 仅当T能完成默认构造时返回真。
template <typename T>
inline constexpr bool is_default_constructible_v = false;

std::is_default_constructible<myClass>
// 仅当T能完成拷贝构造时返回真。
template <typename T>
inline constexpr bool is_copy_constructible_v = __is_constructible(T);

// 仅当T能完成移动构造时返回真。
template <typename T>
inline constexpr bool is_move_constructible_v = __is_constructible(_T, _T);;

// 仅当T1, T2是相同类型时返回真。考虑CV修饰。
template <typename, typename>
inline constexpr bool is_same_v = false;

template<typename Ty>
inline constexpr bool _is_same_v<Ty, Ty> = true;

// 仅当T为Types...中众类型之一时返回真。这不是标准规定的算子。
template <typename T, typename... Types>
inline constexpr bool is_any_of_v = (is_same_v<T, Types> || ...);

// 返回对应的带有const修饰的类型。
template<typename T>
struct add_const
{ using type = T const; };
template <typename T>
using add_const_t = add_const<T>::type;


// 返回对应的不带有const修饰的类型。
template <class T>
struct remove_const {using type = T;};
template <class T>
struct remove_const<const T> {using type = T;};
template <typename T>
using remove_const_t = remove_const<T>::type;

template <class T>
struct remove_volatile {using type = T;};
template <class T>
struct remove_volatile<volatile T> {using type = T;};

template <class T>
using remove_volatile_t = remove_volatile<T>::type;

// 返回对应的不带有CV修饰的类型。
template <class T>
struct remove_cv {using type = T;};
template <class T>
struct remove_cv<volatile T> {using type = T;};

template <class T>
struct remove_cv<const T> {using type = T;};
template <class T>
struct remove_cv<const volatile T> {using type = T;};
template <typename T>
using remove_cv_t = remove_cv<T>::type;

// 返回对应的非引用类型。
template <class T>
struct remove_reference {using type = T;};
template <class T>
struct remove_reference<T&> {using type = T;};
template <class T>
struct remove_reference<T&&> {using type = T;};
template <typename>
using remove_reference_t = remove_reference<T>::type;


//C++11 __void_t
template <typename...>
using __void_t = void;

template <typename T, typename = void>
struct __is_referenceable{
    using value = false;
};
template <typename T>
struct __is_referenceable<T, __void_t<T&>>{
    using value = true;
};


// 返回对应的左值引用类型。适用引用折叠规则。
template <typename>
struct add_lvalue_reference{
    using type = T&;
};
template <typename T, bool = __is_referenceable<T>::value>>
struct __add_lvalue_reference_helper{
    using type = T;
}

template<typename T>
struct __add_lvalue_reference_helper<T, true>
{ using type = T&; };
template <typename T>
struct add_lvalue_reference:public __add_lvalue_reference_helper<T>{
    using type = __add_lvalue_reference_helper<T>::type;
};
template <typename>
using add_lvalue_reference_t = add_lvalue_reference::type;

// 返回对应的右值引用类型。适用引用折叠规则。
template <typename T, bool = __is_referenceable<T>>
struct __add_rvalue_reference_helper{
    using type = T;
};

template <typename T>
struct __add_rvalue_reference_helper<T, true>{
    using type = T&&;
}

template <typename T>
struct add_lvalue_reference: public __add_lvalue_reference_helper<T>{
    using type = __add_rvalue_reference_helper<T>::type;
};
template <typename T>
using add_rvalue_reference_t = add_lvalue_reference<T>::type;

// 返回对应的非引用类型的指针。

template <typename T, bool = __is_referenceable<T> || is_void_v<T>>
struct add_pointer{
    using type = T;
};

template <typename T, true>
struct add_pointer{
    using type = remove_reference<T>*;
};
template <typename T>
using add_pointer_t = add_pointer<T>::type;


// 返回数组T的维度，若非数组返回0。
template <typename T>
inline constexpr size_t rank_v = 0;

template <typename T>
inline constexpr size_t rank_v<T[]> = 1 + rank_v<T>();


// 返回数组T在第I个维度上的大小。
template <typename T, size_t I>
inline constexpr size_t extent_v = 0;

// 获取数组T的元素的类型。若非数组返回其本身。
template <typename T>
using remove_extent_t = void;

// 当 B == true 时返回 T，否则返回 F。
template <bool B, typename T, typename F>
using conditional_t = void;

// 返回T的退化类型。
template <typename>
struct decay { using type = void; };

/// 这里的 requires 子句可以保证当 T 为函数时，选择此模板进行实例化。
/// 你也可以自己想一个不使用 requires 的办法。
template <typename T> requires is_function_v<T>
struct decay<T> { using type = void; };

template <typename T>
using decay_t = typename decay<T>::type;
}