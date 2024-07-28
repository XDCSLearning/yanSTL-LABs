#pragma once

namespace my
{

// 仅当T具有可接受Args...类型实参的构造函数时返回真。
template <typename T, typename... Args>
inline constexpr bool is_constructible_v = __is_constructible(T, Args...);


/// 你需要修改本文件的以下内容。

// 仅当T为void时返回真。
//template <typename T>
//inline constexpr bool is_void_v = false;
template <typename T>
inline constexpr bool is_void_v = std::is_void<T>::value;

// 仅当T为布尔类型、字符类型、整数类型时返回真。
//template <typename>
//inline constexpr bool is_integral_v = false;
template <typename T>
inline constexpr bool is_integral_v = std::is_integral<T>::value;

// 仅当T具有顶层volatile修饰时返回真。
//template <typename>
//inline constexpr bool is_volatile_v = false;
// 基础模板，总是返回false  
template <typename T>
struct is_volatile_impl : std::false_type {};

// 特化模板，对于T的volatile版本，返回true  
template <typename T>
struct is_volatile_impl<volatile T> : std::true_type {};

// 别名模板，使得使用更简洁  
template <typename T>
inline constexpr bool is_volatile_v = is_volatile_impl<std::remove_reference_t<T>>::value;


// 仅当T为指针时返回真。
//template <typename>
//inline constexpr bool is_pointer_v = false;
template <typename T>
inline constexpr bool is_pointer_v = std::is_pointer<T>::value;

// 仅当T为引用时返回真。
//template <typename>
//inline constexpr bool is_reference_v = false;
template <typename T>
inline constexpr bool is_reference_v = std::is_reference<T>::value;

// 仅当T为函数时返回真。提示：只有U为函数或引用时，is_const_v<const U>返回假。
//template <typename>
//inline constexpr bool is_function_v = false;
template <typename T>
inline constexpr bool is_function_v = std::is_function<T>::value;


// 仅当T为除了void、函数、引用以外的任何类型时返回真。
//template <typename>
//inline constexpr bool is_object_v = false;

template <typename T>
inline constexpr bool is_object_v =
!std::is_void<T>::value &&
!std::is_function<T>::value &&
!std::is_reference<T>::value;


// 仅当T为原生数组时返回真。
//template <typename>
//inline constexpr bool is_array_v = false;
template <typename T>
inline constexpr bool is_array_v = std::is_array<T>::value;

// 仅当T能完成默认构造时返回真。
//template <typename>
//inline constexpr bool is_default_constructible_v = false;
template <typename T>
inline constexpr bool is_default_constructible_v = std::is_default_constructible<T>::value;

// 仅当T能完成拷贝构造时返回真。
//template <typename>
//inline constexpr bool is_copy_constructible_v = false;
template <typename T>
inline constexpr bool is_copy_constructible_v = std::is_copy_constructible<T>::value;


// 仅当T能完成移动构造时返回真。
//template <typename>
//inline constexpr bool is_move_constructible_v = false;
template <typename T>
inline constexpr bool is_move_constructible_v = std::is_move_constructible<T>::value;

// 仅当T1, T2是相同类型时返回真。考虑CV修饰。
//template <typename, typename>
//inline constexpr bool is_same_v = false;
template <typename T1, typename T2>
inline constexpr bool is_same_v = std::is_same<T1, T2>::value;

// 仅当T为Types...中众类型之一时返回真。这不是标准规定的算子。
//template <typename, typename...>
//inline constexpr bool is_any_of_v = false;
template <typename T, typename... Types>
struct is_any_of_impl : std::false_type {};

// 递归情况：检查T是否和Types中的第一个类型相同  
template <typename T, typename First, typename... Rest>
struct is_any_of_impl<T, First, Rest...> :
    std::conditional<
    std::is_same<T, First>::value,
    std::true_type,
    is_any_of_impl<T, Rest...>
    >::type {};

// 模板别名，使得用法更简洁  
template <typename T, typename... Types>
inline constexpr bool is_any_of_v = is_any_of_impl<T, Types...>::value;


// 仅当T不为Types...中众类型之一时返回真。这不是标准规定的算子。
//template <typename, typename...>
//inline constexpr bool is_none_of_v = false;

// 递归终止条件：没有更多的类型需要检查  
template <typename T, typename... Types>
struct is_none_of_impl : std::true_type {};

// 递归情况：检查T是否和Types中的第一个类型相同  
template <typename T, typename First, typename... Rest>
struct is_none_of_impl<T, First, Rest...> :
    std::conditional<
    std::is_same<T, First>::value,
    std::false_type,
    is_none_of_impl<T, Rest...>
    >::type {};

// 模板别名，使得用法更简洁  
template <typename T, typename... Types>
inline constexpr bool is_none_of_v = is_none_of_impl<T, Types...>::value;


// 返回对应的带有const修饰的类型。
//template <typename>
//using add_const_t = void;
template <typename T>
using add_const_t = typename std::add_const<T>::type;

// 返回对应的不带有const修饰的类型。
//template <typename>
//using remove_const_t = void;
template <typename T>
using remove_const_t = typename std::remove_const<T>::type;

// 返回对应的不带有CV修饰的类型。
//template <typename>
//using remove_cv_t = void;

template <typename T>
using remove_cv_t = typename std::remove_cv<T>::type;

// 返回对应的非引用类型。
//template <typename>
//using remove_reference_t = void;
template <typename T>
using remove_reference_t = typename std::remove_reference<T>::type;

// 返回对应的左值引用类型。适用引用折叠规则。
//template <typename>
//using add_lvalue_reference_t = void;
template <typename T>
using add_lvalue_reference_t = typename std::add_lvalue_reference<T>::type;

// 返回对应的右值引用类型。适用引用折叠规则。
//template <typename>
//using add_rvalue_reference_t = void;
template <typename T>
using add_rvalue_reference_t = typename std::add_rvalue_reference<T>::type;

// 返回对应的非引用类型的指针。
//template <typename>
// using add_pointer_t = void;
template <typename T>
using add_pointer_t = typename std::add_pointer<T>::type;


// 返回数组T的维度，若非数组返回0。

template <typename>
inline constexpr size_t rank_v = 0;

 //模板特化来处理数组类型  
template <typename T, size_t N>
inline constexpr size_t rank_v<T[N]> = rank_v<T> +1;

 //这里处理去掉数组维度后的类型  
template <typename T>
inline constexpr size_t rank_v<T[]> = rank_v<T> +1;










// 返回数组T在第I个维度上的大小。
//template <typename T, size_t I>
//inline constexpr size_t extent_v = 0;
// 默认情况下，extent_v 为 0  
// 默认情况下，extent_v 为 0  
template <typename T, std::size_t I>
inline constexpr std::size_t extent_v = 0;

// 处理固定大小的一维数组  
template <typename T, std::size_t N>
inline constexpr std::size_t extent_v<T[N], 0> = N;

// 处理多维数组  
template <typename T, std::size_t N, std::size_t I>
inline constexpr std::size_t extent_v<T[N], I> = extent_v<T, I - 1>;

// 处理动态数组（未知大小）  
template <typename T>
inline constexpr std::size_t extent_v<T[], 0> = 0; // 第一维的动态数组大小为 0  

// 处理多维动态数组  
template <typename T, std::size_t N>
inline constexpr std::size_t extent_v<T[][N], 1> = N; // 正确获取第二维的大小  

// 处理更高维度  
template <typename T, std::size_t N, std::size_t I>
inline constexpr std::size_t extent_v<T[][N], I> = extent_v<T, I - 1>;



// 获取数组T的元素的类型。若非数组返回其本身。
//template <typename T>
//using remove_extent_t = void;

// 基础模板定义  
template <typename T>
struct remove_extent {
    using type = T; // 默认返回原类型  
};

// 处理动态数组  
template <typename T>
struct remove_extent<T[]> {
    using type = T; // 返回动态数组的元素类型  
};

// 处理静态数组  
template <typename T, std::size_t N>
struct remove_extent<T[N]> {
    using type = T; // 返回静态数组的元素类型  
};

// 辅助类型别名  
template <typename T>
using remove_extent_t = typename remove_extent<T>::type;









// 当 B == true 时返回 T，否则返回 F。
//template <bool B, typename T, typename F>
//using conditional_t = void;


// 保留下面这行代码  
template <bool B, typename T, typename F>
// 正确的条件选择实现  
using conditional_t = typename std::conditional<B, T, F>::type;





// 返回T的退化类型。
//template <typename>
//struct decay { using type = void; };

/// 这里的 requires 子句可以保证当 T 为函数时，选择此模板进行实例化。
/// 你也可以自己想一个不使用 requires 的办法。
//template <typename T> requires is_function_v<T>
//struct decay<T> { using type = void; };

//template <typename T>
//using decay_t = typename decay<T>::type;


template <typename T>
struct decay {
   
    using type = typename std::remove_cv<
        typename std::remove_reference<T>::type  
    >::type;
};

 
template <typename T>
struct decay<T*> {
    using type = T*; 
};


template <typename T>
struct decay<T&> {
    using type = typename decay<T>::type; 
};


template <typename T>
struct decay<T&&> {
    using type = typename decay<T>::type;  
};


template <typename T, std::size_t N>
struct decay<T[N]> {
    using type = T*;
};

 
template <typename T>
struct decay<T[]> {
    using type = T*;  
};

 
template <typename T, std::size_t N>
struct decay<T(&)[N]> {
    using type = T*; 
};

template <typename T, std::size_t N>
struct decay<T(&&)[N]> {
    using type = T*;   
};


template <typename R, typename... Args>
struct decay<R(Args...)> {
    using type = R(*)(Args...);  
};


template <typename R, typename... Args>
struct decay<R(*)(Args...)> {
    using type = R(*)(Args...); 
};


template <typename T>
using decay_t = typename decay<T>::type;
}