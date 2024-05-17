namespace my
{

// 仅当T具有可接受Args...类型实参的构造函数时返回真。
template <typename T, typename... Args>
inline constexpr bool is_constructible_v = __is_constructible(T, Args...);


/// 你需要修改本文件的以下内容。

// 仅当T为void时返回真。
template <typename> inline constexpr bool is_void_v                      = false;
template <>         inline constexpr bool is_void_v<void>                = true;
template <>         inline constexpr bool is_void_v<const void>          = true;
template <>         inline constexpr bool is_void_v<volatile void>       = true;
template <>         inline constexpr bool is_void_v<const volatile void> = true;

// 返回对应的带有const修饰的类型。
template <typename T> using add_const_t = const T;

// 返回对应的左值引用类型。适用引用折叠规则。
template <typename T> using add_lvalue_reference_t = T&;

// 返回对应的右值引用类型。适用引用折叠规则。
template <typename T> using add_rvalue_reference_t = T&&;

// 返回对应的非引用类型。
template <typename T> struct remove_reference      { using type = T; };
template <typename T> struct remove_reference<T&>  { using type = T; };
template <typename T> struct remove_reference<T&&> { using type = T; };
template <typename T> using  remove_reference_t = typename remove_reference<T>::type;

// 返回对应的非引用类型的指针。
template <typename T> using add_pointer_t = remove_reference_t<T>*;

// 仅当T1, T2是相同类型时返回真。考虑CV修饰。
template <typename, typename> inline constexpr bool is_same_v = false;
template <typename T>         inline constexpr bool is_same_v<T, T> = true;

// 仅当T为Types...中众类型之一时返回真。这不是标准规定的算子。
template <typename T, typename... Types> inline constexpr bool is_any_of_v = (is_same_v<T, Types> || ...);

// 仅当T不为Types...中众类型之一时返回真。这不是标准规定的算子。
template <typename T, typename... Types> inline constexpr bool is_none_of_v = !is_any_of_v<T, Types...>;

// 返回对应的不带有CV修饰的类型。
template <typename T> struct remove_cv                   { using type = T; };
template <typename T> struct remove_cv<const T>          { using type = T; };
template <typename T> struct remove_cv<volatile T>       { using type = T; };
template <typename T> struct remove_cv<const volatile T> { using type = T; };
template <typename T> using  remove_cv_t = typename remove_cv<T>::type;

// 仅当T为布尔类型、字符类型、整数类型时返回真。
template <typename T> inline constexpr bool is_integral_v = is_any_of_v<remove_cv_t<T>, bool, char, signed char, unsigned char, char8_t, char16_t, char32_t, wchar_t, short, unsigned short, int, unsigned, long, unsigned long, long long, unsigned long long>;

// 仅当T具有顶层const修饰时返回真。
template <typename>   inline constexpr bool is_const_v          = false;
template <typename T> inline constexpr bool is_const_v<const T> = true;

// 仅当T具有顶层volatile修饰时返回真。
template <typename>   inline constexpr bool is_volatile_v             = false;
template <typename T> inline constexpr bool is_volatile_v<volatile T> = true;

// 仅当T为指针时返回真。
template <typename>   inline constexpr bool is_pointer_v                    = false;
template <typename T> inline constexpr bool is_pointer_v<T*>                = true;
template <typename T> inline constexpr bool is_pointer_v<T* const>          = true;
template <typename T> inline constexpr bool is_pointer_v<T* volatile>       = true;
template <typename T> inline constexpr bool is_pointer_v<T* const volatile> = true;

// 仅当T为引用时返回真。
template <typename>   inline constexpr bool is_reference_v      = false;
template <typename T> inline constexpr bool is_reference_v<T&>  = true;
template <typename T> inline constexpr bool is_reference_v<T&&> = true;

// 仅当T为函数时返回真。提示：只有U为函数或引用时，is_const_v<const U>返回假。
template <typename T> inline constexpr bool is_function_v = !is_const_v<const T> && !is_reference_v<T>;

// 仅当T为除了void、函数、引用以外的任何类型时返回真。
template <typename T> inline constexpr bool is_object_v = !is_void_v<T> && !is_function_v<T> && !is_reference_v<T>;

// 仅当T为原生数组时返回真。
template <typename>             inline constexpr bool is_array_v       = false;
template <typename T, size_t N> inline constexpr bool is_array_v<T[N]> = true;
template <typename T>           inline constexpr bool is_array_v<T[]>  = true;

// 仅当T能完成默认构造时返回真。
template <typename T> inline constexpr bool is_default_constructible_v = is_constructible_v<T>;

// 仅当T能完成拷贝构造时返回真。
template <typename T> inline constexpr bool is_copy_constructible_v = is_constructible_v<T, add_lvalue_reference_t<add_const_t<T>>>;

// 仅当T能完成移动构造时返回真。
template <typename T> inline constexpr bool is_move_constructible_v = is_constructible_v<T, add_rvalue_reference_t<T>>;

// 返回对应的不带有const修饰的类型。
template <typename T> struct remove_const          { using type = T; };
template <typename T> struct remove_const<const T> { using type = T; };
template <typename T> using  remove_const_t = typename remove_const<T>::type;

// 返回数组T的维度，若非数组返回0。
template <typename T>           inline constexpr size_t rank_v       = 0;
template <typename T, size_t N> inline constexpr size_t rank_v<T[N]> = rank_v<T> + 1;
template <typename T>           inline constexpr size_t rank_v<T[]>  = rank_v<T> + 1;

// 返回数组T在第I个维度上的大小。
template <typename T, size_t I>           inline constexpr size_t extent_v          = 0;
template <typename T, size_t N>           inline constexpr size_t extent_v<T[N], 0> = N;
template <typename T, size_t I, size_t N> inline constexpr size_t extent_v<T[N], I> = extent_v<T, I - 1>;
template <typename T, size_t I>           inline constexpr size_t extent_v<T[], I>  = extent_v<T, I - 1>;
template <typename T>                     inline constexpr size_t extent_v<T[], 0>  = 0;

// 获取数组T的元素的类型。若非数组返回其本身。
template <typename T>           struct remove_extent       { using type = T; };
template <typename T>           struct remove_extent<T[]>  { using type = T; };
template <typename T, size_t N> struct remove_extent<T[N]> { using type = T; };
template <typename T> using remove_extent_t = typename remove_extent<T>::type;

std::decay<int>;
// 返回T的退化类型。
template <typename T>                           struct decay       { using type = remove_cv_t<T>; };
template <typename T>                           struct decay<T&>   { using type = typename decay<T>::type; };
template <typename T>                           struct decay<T&&>  { using type = typename decay<T>::type; };
template <typename T>                           struct decay<T[]>  { using type = typename decay<add_pointer_t<T>>::type; };
template <typename T, size_t N>                 struct decay<T[N]> { using type = typename decay<add_pointer_t<T>>::type; };
template <typename T> requires is_function_v<T> struct decay<T>    { using type = typename decay<add_pointer_t<T>>::type; };
template <typename T> using  decay_t = typename decay<T>::type;

}