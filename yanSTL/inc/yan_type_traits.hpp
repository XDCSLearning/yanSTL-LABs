namespace my
{

// 仅当T具有可接受Args...类型实参的构造函数时返回真。
template <typename T, typename... Args>
inline constexpr bool is_constructible_v = __is_constructible(T, Args...);


/// 你需要修改本文件的以下内容。

// 仅当T为void时返回真。
template <typename>
inline constexpr bool is_void_v = false;

// 仅当T为布尔类型、字符类型、整数类型时返回真。
template <typename>
inline constexpr bool is_integral_v = false;

// 仅当T具有顶层volatile修饰时返回真。
template <typename>
inline constexpr bool is_volatile_v = false;

// 仅当T为指针时返回真。
template <typename>
inline constexpr bool is_pointer_v = false;

// 仅当T为引用时返回真。
template <typename>
inline constexpr bool is_reference_v = false;

// 仅当T为函数时返回真。提示：只有U为函数或引用时，is_const_v<const U>返回假。
template <typename>
inline constexpr bool is_function_v = false;

// 仅当T为除了void、函数、引用以外的任何类型时返回真。
template <typename>
inline constexpr bool is_object_v = false;

// 仅当T为原生数组时返回真。
template <typename>
inline constexpr bool is_array_v = false;

// 仅当T能完成默认构造时返回真。
template <typename>
inline constexpr bool is_default_constructible_v = false;

// 仅当T能完成拷贝构造时返回真。
template <typename>
inline constexpr bool is_copy_constructible_v = false;

// 仅当T能完成移动构造时返回真。
template <typename>
inline constexpr bool is_move_constructible_v = false;

// 仅当T1, T2是相同类型时返回真。考虑CV修饰。
template <typename, typename>
inline constexpr bool is_same_v = false;

// 仅当T为Types...中众类型之一时返回真。这不是标准规定的算子。
template <typename, typename...>
inline constexpr bool is_any_of_v = false;

// 仅当T不为Types...中众类型之一时返回真。这不是标准规定的算子。
template <typename, typename...>
inline constexpr bool is_none_of_v = false;

// 返回对应的带有const修饰的类型。
template <typename>
using add_const_t = void;

// 返回对应的不带有const修饰的类型。
template <typename>
using remove_const_t = void;

// 返回对应的不带有CV修饰的类型。
template <typename>
using remove_cv_t = void;

// 返回对应的非引用类型。
template <typename>
using remove_reference_t = void;

// 返回对应的左值引用类型。适用引用折叠规则。
template <typename>
using add_lvalue_reference_t = void;

// 返回对应的右值引用类型。适用引用折叠规则。
template <typename>
using add_rvalue_reference_t = void;

// 返回对应的非引用类型的指针。
template <typename>
using add_pointer_t = void;

// 返回数组T的维度，若非数组返回0。
template <typename>
inline constexpr size_t rank_v = 0;

// 返回数组T在第I个维度上的大小。
template <typename, size_t>
inline constexpr size_t extent_v = 0;

// 获取数组T的元素的类型。若非数组返回其本身。
template <typename>
using remove_extent_t = void;

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