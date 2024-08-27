#pragma once

namespace my
{

// 仅当T具有可接受Args...类型实参的构造函数时返回真。
template <typename T, typename... Args>
inline constexpr bool is_constructible_v = __is_constructible(T, Args...);


/// 你需要修改本文件的以下内容。

// 仅当T为void时返回真。
template <typename T>
inline constexpr bool is_void_v = false;


// 特化模板针对 void 类型  
template <>
inline constexpr bool is_void_v<void> = true; // T 是 void 
template <>
inline constexpr bool is_void_v<void const> = true;
template <>
inline constexpr bool is_void_v<volatile void> = true;
template <>
inline constexpr bool is_void_v<volatile const void> = true;
template<>
inline constexpr bool is_void_v<volatile signed char> = true;



//inline constexpr bool is_void_v = std::is_void<T>::value;

// 仅当T为布尔类型、字符类型、整数类型时返回真。
template <typename T>
inline constexpr bool is_integral_v = false;
//template <typename T>
//inline constexpr bool is_integral_v = std::is_integral<T>::value;
template <>
inline constexpr bool is_integral_v<bool> = true;
template <>
inline constexpr bool is_integral_v<int> = true;
template <>
inline constexpr bool is_integral_v<const char> = true;
template<>
inline constexpr bool is_integral_v<volatile signed char> = true;
template<>
inline constexpr bool is_integral_v<const volatile unsigned char> = true;
template<>
inline constexpr bool is_integral_v<wchar_t> = true;
template<>
inline constexpr bool is_integral_v<const char8_t> = true;
template<>
inline constexpr bool is_integral_v<volatile char16_t> = true;
template<>
inline constexpr bool is_integral_v<const volatile char32_t> = true;
template<>
inline constexpr bool is_integral_v<short> = true;
template<>
inline constexpr bool is_integral_v<const unsigned short> = true;
template<>
inline constexpr bool is_integral_v<volatile unsigned> = true;
template<>
inline constexpr bool is_integral_v<const volatile long> = true;
template<>
inline constexpr bool is_integral_v<long long> = true;
template<>
inline constexpr bool is_integral_v<const unsigned long long> = true;


// 仅当T具有顶层volatile修饰时返回真。
template <typename>
inline constexpr bool is_volatile_v = false;
template <typename T>
inline constexpr bool is_volatile_v<volatile T> = true;




// 仅当T为指针时返回真。
template <typename>
inline constexpr bool is_pointer_v = false;
template <typename T>
inline constexpr bool is_pointer_v<T*> = true;
template <typename T>
inline constexpr bool is_pointer_v<T* const> = true;

// 仅当T为引用时返回真。
template <typename>
inline constexpr bool is_reference_v = false;
template <typename T>
inline constexpr bool is_reference_v<T&> =true;
template <typename T>
inline constexpr bool is_reference_v<T&&> = true;

// 仅当T为函数时返回真。提示：只有U为函数或引用时，is_const_v<const U>返回假。
template <typename>
inline constexpr bool is_function_v = false;
// 特化，匹配普通函数类型  
template <typename R, typename... Args>
inline constexpr bool is_function_v<R(Args...)> = true;

// 特化，匹配带 noexcept 的函数类型  
template <typename R, typename... Args>
inline constexpr bool is_function_v<R(Args...) noexcept> = true;

// 特化，匹配函数指针类型  
template <typename R, typename... Args>
inline constexpr bool is_function_v<R(*)(Args...)> = true;

// 特化，匹配带 noexcept 的函数指针类型  
template <typename R, typename... Args>
inline constexpr bool is_function_v<R(*)(Args...) noexcept> = true;



// 仅当T为除了void、函数、引用以外的任何类型时返回真。
//template <typename>
//inline constexpr bool is_object_v = false;

template <typename T>
inline constexpr bool is_object_v =!is_void_v<T> &&!is_function_v<T> &&!is_reference_v<T>;


// 仅当T为原生数组时返回真。
template <typename>
inline constexpr bool is_array_v = false;
template <typename T>
inline constexpr bool is_array_v<T[]> = true;
template <typename T,size_t N>
inline constexpr bool is_array_v <T[N]> = true;


// 默认情况下，不支持默认构造  
//template <typename T>
//inline constexpr bool is_default_constructible_v = false;
template <typename T>
inline constexpr bool is_default_constructible_helper = requires() {
    T(); // 尝试默认构造 T  
};
template <typename T>
inline constexpr bool is_default_constructible_v = is_default_constructible_helper<T>;
// 特化：基础类型可以默认构造  
template <>
inline constexpr bool is_default_constructible_v<int> = true;

template <>
inline constexpr bool is_default_constructible_v<float> = true;

// 特化：指针类型可以默认构造  
template <typename T>
inline constexpr bool is_default_constructible_v<T*> = true;

// 特化：引用类型不能默认构造  
template <typename T>
inline constexpr bool is_default_constructible_v<T&> = false;

// 特化：右值引用类型不能默认构造  
template <typename T>
inline constexpr bool is_default_constructible_v<T&&> = false;


// 针对 void 类型的特化  
template <>
inline constexpr bool is_default_constructible_v<void> = false;


// 仅当T能完成拷贝构造时返回真。
template <typename T, typename = void>
inline constexpr bool is_copy_constructible_v = false;

// 特化：当 T 可以拷贝构造时返回 true  
template <typename T>
inline constexpr bool is_copy_constructible_v<T, std::void_t<decltype(T(std::declval<const T&>()))>> = true;

// 仅当T能完成移动构造时返回真。
template <typename T, typename = void>
inline constexpr bool is_move_constructible_v = false;

// 特化：当 T 可以移动构造时返回 true  
template <typename T>
inline constexpr bool is_move_constructible_v<T, std::void_t<decltype(T(std::declval<T&&>()))>> = true;


// 仅当T1, T2是相同类型时返回真。考虑CV修饰。
template <typename T1, typename T2>
inline constexpr bool is_same_v = false;

// 特化版本，在 T1 和 T2 完全相同的情况下，返回 true  
template <typename T>
inline constexpr bool is_same_v<T, T> = true;

// 仅当T为Types...中众类型之一时返回真。这不是标准规定的算子。
template <typename T, typename... Types>
inline constexpr bool is_any_of_v = false;

// 特化版本，当 T 是 Types... 中的某一个类型时返回 true  
template <typename T, typename First, typename... Rest>
inline constexpr bool is_any_of_v<T, First, Rest...> =
std::is_same_v<T, First> || is_any_of_v<T, Rest...>;





// 仅当T不为Types...中众类型之一时返回真。这不是标准规定的算子。
template <typename, typename...>
inline constexpr bool is_none_of_v = false;


// 特化版本，当 T 是 Types... 中的某一个类型时返回 false  
template <typename T, typename First, typename... Rest>
inline constexpr bool is_none_of_v<T, First, Rest...> =
!std::is_same_v<T, First>&& is_none_of_v<T, Rest...>;


// 返回对应的带有const修饰的类型。
//template <typename>
//using add_const_t = void;
// 默认模板，把 T 变为 const T  

template <typename T>
struct add_const_impl {
    using type = const T;
};

// 对于引用类型，保持引用不变  
template <typename T>
struct add_const_impl<T&> {
    using type = T&; // 保持引用不变  
};

// 对于指针类型，添加 const 到指针所指向的类型  
template <typename T>
struct add_const_impl<T*> {
    using type = T* const; // 将 const 添加到指针所指向的类型  
};

// 对于 const 类型，保持为 const  
template <typename T>
struct add_const_impl<const T> {
    using type = const T; // 保持原 const 类型  
};

// 方便的类型别名模板  
template <typename T>
using add_const_t = typename add_const_impl<T>::type;




// 返回对应的不带有const修饰的类型。
// 默认模板：将类型 T 直接返回  
template <typename T>
struct remove_const_impl {
    using type = T; // 默认情况下返回原类型  
};

// 特化版本：如果 T 是 const 类型，返回 T 的非 const 版本  
template <typename T>
struct remove_const_impl<const T> {
    using type = T; // 去掉 const 修饰，只返回 T  
};

// 类型别名模板，用于方便的使用  
template <typename T>
using remove_const_t = typename remove_const_impl<T>::type;

// 返回对应的不带有CV修饰的类型。
template <typename T>
struct remove_cv_impl {
    using type = T; // 默认情况下返回原类型  
};

// 特化版本：如果 T 是 const 类型，返回去掉 const 的类型  
template <typename T>
struct remove_cv_impl<const T> {
    using type = T; // 去掉 const 修饰  
};

// 特化版本：如果 T 是 volatile 类型，返回去掉 volatile 的类型  
template <typename T>
struct remove_cv_impl<volatile T> {
    using type = T; // 去掉 volatile 修饰  
};

// 特化版本：如果 T 是 const volatile 类型，返回 T 的非 const volatile 版本  
template <typename T>
struct remove_cv_impl<const volatile T> {
    using type = T; // 去掉 const 和 volatile 修饰  
};

// 类型别名模板，用于方便使用  
template <typename T>
using remove_cv_t = typename remove_cv_impl<T>::type;






// 返回对应的非引用类型。
//template <typename>
//using remove_reference_t = void;
// 默认模板：将类型 T 直接返回  
template <typename T>
struct remove_reference_impl {
    using type = T; // 默认情况下返回原类型  
};

// 特化版本：如果 T 是引用类型，返回引用的非引用版本  
template <typename T>
struct remove_reference_impl<T&> {
    using type = T; // 去掉左值引用  
};

// 特化版本：如果 T 是右值引用，返回其非引用版本  
template <typename T>
struct remove_reference_impl<T&&> {
    using type = T; // 去掉右值引用  
};

// 类型别名模板，用于方便使用  
template <typename T>
using remove_reference_t = typename remove_reference_impl<T>::type;


// 返回对应的左值引用类型。适用引用折叠规则。
//template <typename>
//using add_lvalue_reference_t = void;
template <typename T>
struct add_lvalue_reference_impl {
    using type = T&; // 返回左值引用类型  
};

// 特化版本：如果 T 是引用类型，则保持引用不变，返回 T  
template <typename T>
struct add_lvalue_reference_impl<T&> {
    using type = T&; // 保持左值引用  
};

// 特化版本：如果 T 是右值引用，则返回其基类型的左值引用  
template <typename T>
struct add_lvalue_reference_impl<T&&> {
    using type = T&; // 将右值引用转为左值引用  
};

// 类型别名模板，用于方便的使用  
template <typename T>
using add_lvalue_reference_t = typename add_lvalue_reference_impl<T>::type;




// 返回对应的右值引用类型。适用引用折叠规则。
//template <typename>
//using add_rvalue_reference_t = void;
// 默认情况下，返回 T 的右值引用类型  
template <typename T>
struct add_rvalue_reference_impl {
    using type = T&&; // 默认返回右值引用类型  
};

// 特化版本：如果 T 是引用类型，则返回 T  
template <typename T>
struct add_rvalue_reference_impl<T&> {
    using type = T&; // 若为左值引用，则保持不变  
};

// 特化版本：如果 T 是右值引用，则返回其右值引用（保持不变）  
template <typename T>
struct add_rvalue_reference_impl<T&&> {
    using type = T&&; // 保持右值引用  
};

// 类型别名模板，用于方便使用  
template <typename T>
using add_rvalue_reference_t = typename add_rvalue_reference_impl<T>::type;


// 返回对应的非引用类型的指针。
//template <typename>
// using add_pointer_t = void;
template <typename T>
struct add_pointer_impl {
    using type = T*; // 返回 T 的指针类型  
};

// 特化版本：如果 T 是引用类型，则返回其非引用类型的指针  
template <typename T>
struct add_pointer_impl<T&> {
    using type = typename add_pointer_impl<T>::type; // 去掉引用后，再取得指针类型  
};

// 特化版本：如果 T 是右值引用，则返回其非引用类型的指针  
template <typename T>
struct add_pointer_impl<T&&> {
    using type = typename add_pointer_impl<T>::type; // 去掉引用后，再取得指针类型  
};

// 类型别名模板，用于方便的使用  
template <typename T>
using add_pointer_t = typename add_pointer_impl<T>::type;






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


template <bool B, typename T, typename F>
struct conditional_impl;

// 特化：当 B == true  
template <typename T, typename F>
struct conditional_impl<true, T, F> {
    using type = T; // 返回 T  
};

// 特化：当 B == false  
template <typename T, typename F>
struct conditional_impl<false, T, F> {
    using type = F; // 返回 F  
};

// 类型别名模板，用于方便的使用  
template <bool B, typename T, typename F>
using conditional_t = typename conditional_impl<B, T, F>::type;





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