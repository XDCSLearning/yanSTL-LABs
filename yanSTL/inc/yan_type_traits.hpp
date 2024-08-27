#pragma once

namespace my
{

// ����T���пɽ���Args...����ʵ�εĹ��캯��ʱ�����档
template <typename T, typename... Args>
inline constexpr bool is_constructible_v = __is_constructible(T, Args...);


/// ����Ҫ�޸ı��ļ����������ݡ�

// ����TΪvoidʱ�����档
template <typename T>
inline constexpr bool is_void_v = false;


// �ػ�ģ����� void ����  
template <>
inline constexpr bool is_void_v<void> = true; // T �� void 
template <>
inline constexpr bool is_void_v<void const> = true;
template <>
inline constexpr bool is_void_v<volatile void> = true;
template <>
inline constexpr bool is_void_v<volatile const void> = true;
template<>
inline constexpr bool is_void_v<volatile signed char> = true;



//inline constexpr bool is_void_v = std::is_void<T>::value;

// ����TΪ�������͡��ַ����͡���������ʱ�����档
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


// ����T���ж���volatile����ʱ�����档
template <typename>
inline constexpr bool is_volatile_v = false;
template <typename T>
inline constexpr bool is_volatile_v<volatile T> = true;




// ����TΪָ��ʱ�����档
template <typename>
inline constexpr bool is_pointer_v = false;
template <typename T>
inline constexpr bool is_pointer_v<T*> = true;
template <typename T>
inline constexpr bool is_pointer_v<T* const> = true;

// ����TΪ����ʱ�����档
template <typename>
inline constexpr bool is_reference_v = false;
template <typename T>
inline constexpr bool is_reference_v<T&> =true;
template <typename T>
inline constexpr bool is_reference_v<T&&> = true;

// ����TΪ����ʱ�����档��ʾ��ֻ��UΪ����������ʱ��is_const_v<const U>���ؼ١�
template <typename>
inline constexpr bool is_function_v = false;
// �ػ���ƥ����ͨ��������  
template <typename R, typename... Args>
inline constexpr bool is_function_v<R(Args...)> = true;

// �ػ���ƥ��� noexcept �ĺ�������  
template <typename R, typename... Args>
inline constexpr bool is_function_v<R(Args...) noexcept> = true;

// �ػ���ƥ�亯��ָ������  
template <typename R, typename... Args>
inline constexpr bool is_function_v<R(*)(Args...)> = true;

// �ػ���ƥ��� noexcept �ĺ���ָ������  
template <typename R, typename... Args>
inline constexpr bool is_function_v<R(*)(Args...) noexcept> = true;



// ����TΪ����void������������������κ�����ʱ�����档
//template <typename>
//inline constexpr bool is_object_v = false;

template <typename T>
inline constexpr bool is_object_v =!is_void_v<T> &&!is_function_v<T> &&!is_reference_v<T>;


// ����TΪԭ������ʱ�����档
template <typename>
inline constexpr bool is_array_v = false;
template <typename T>
inline constexpr bool is_array_v<T[]> = true;
template <typename T,size_t N>
inline constexpr bool is_array_v <T[N]> = true;


// Ĭ������£���֧��Ĭ�Ϲ���  
//template <typename T>
//inline constexpr bool is_default_constructible_v = false;
template <typename T>
inline constexpr bool is_default_constructible_helper = requires() {
    T(); // ����Ĭ�Ϲ��� T  
};
template <typename T>
inline constexpr bool is_default_constructible_v = is_default_constructible_helper<T>;
// �ػ����������Ϳ���Ĭ�Ϲ���  
template <>
inline constexpr bool is_default_constructible_v<int> = true;

template <>
inline constexpr bool is_default_constructible_v<float> = true;

// �ػ���ָ�����Ϳ���Ĭ�Ϲ���  
template <typename T>
inline constexpr bool is_default_constructible_v<T*> = true;

// �ػ����������Ͳ���Ĭ�Ϲ���  
template <typename T>
inline constexpr bool is_default_constructible_v<T&> = false;

// �ػ�����ֵ�������Ͳ���Ĭ�Ϲ���  
template <typename T>
inline constexpr bool is_default_constructible_v<T&&> = false;


// ��� void ���͵��ػ�  
template <>
inline constexpr bool is_default_constructible_v<void> = false;


// ����T����ɿ�������ʱ�����档
template <typename T, typename = void>
inline constexpr bool is_copy_constructible_v = false;

// �ػ����� T ���Կ�������ʱ���� true  
template <typename T>
inline constexpr bool is_copy_constructible_v<T, std::void_t<decltype(T(std::declval<const T&>()))>> = true;

// ����T������ƶ�����ʱ�����档
template <typename T, typename = void>
inline constexpr bool is_move_constructible_v = false;

// �ػ����� T �����ƶ�����ʱ���� true  
template <typename T>
inline constexpr bool is_move_constructible_v<T, std::void_t<decltype(T(std::declval<T&&>()))>> = true;


// ����T1, T2����ͬ����ʱ�����档����CV���Ρ�
template <typename T1, typename T2>
inline constexpr bool is_same_v = false;

// �ػ��汾���� T1 �� T2 ��ȫ��ͬ������£����� true  
template <typename T>
inline constexpr bool is_same_v<T, T> = true;

// ����TΪTypes...��������֮һʱ�����档�ⲻ�Ǳ�׼�涨�����ӡ�
template <typename T, typename... Types>
inline constexpr bool is_any_of_v = false;

// �ػ��汾���� T �� Types... �е�ĳһ������ʱ���� true  
template <typename T, typename First, typename... Rest>
inline constexpr bool is_any_of_v<T, First, Rest...> =
std::is_same_v<T, First> || is_any_of_v<T, Rest...>;





// ����T��ΪTypes...��������֮һʱ�����档�ⲻ�Ǳ�׼�涨�����ӡ�
template <typename, typename...>
inline constexpr bool is_none_of_v = false;


// �ػ��汾���� T �� Types... �е�ĳһ������ʱ���� false  
template <typename T, typename First, typename... Rest>
inline constexpr bool is_none_of_v<T, First, Rest...> =
!std::is_same_v<T, First>&& is_none_of_v<T, Rest...>;


// ���ض�Ӧ�Ĵ���const���ε����͡�
//template <typename>
//using add_const_t = void;
// Ĭ��ģ�壬�� T ��Ϊ const T  

template <typename T>
struct add_const_impl {
    using type = const T;
};

// �����������ͣ��������ò���  
template <typename T>
struct add_const_impl<T&> {
    using type = T&; // �������ò���  
};

// ����ָ�����ͣ���� const ��ָ����ָ�������  
template <typename T>
struct add_const_impl<T*> {
    using type = T* const; // �� const ��ӵ�ָ����ָ�������  
};

// ���� const ���ͣ�����Ϊ const  
template <typename T>
struct add_const_impl<const T> {
    using type = const T; // ����ԭ const ����  
};

// ��������ͱ���ģ��  
template <typename T>
using add_const_t = typename add_const_impl<T>::type;




// ���ض�Ӧ�Ĳ�����const���ε����͡�
// Ĭ��ģ�壺������ T ֱ�ӷ���  
template <typename T>
struct remove_const_impl {
    using type = T; // Ĭ������·���ԭ����  
};

// �ػ��汾����� T �� const ���ͣ����� T �ķ� const �汾  
template <typename T>
struct remove_const_impl<const T> {
    using type = T; // ȥ�� const ���Σ�ֻ���� T  
};

// ���ͱ���ģ�壬���ڷ����ʹ��  
template <typename T>
using remove_const_t = typename remove_const_impl<T>::type;

// ���ض�Ӧ�Ĳ�����CV���ε����͡�
template <typename T>
struct remove_cv_impl {
    using type = T; // Ĭ������·���ԭ����  
};

// �ػ��汾����� T �� const ���ͣ�����ȥ�� const ������  
template <typename T>
struct remove_cv_impl<const T> {
    using type = T; // ȥ�� const ����  
};

// �ػ��汾����� T �� volatile ���ͣ�����ȥ�� volatile ������  
template <typename T>
struct remove_cv_impl<volatile T> {
    using type = T; // ȥ�� volatile ����  
};

// �ػ��汾����� T �� const volatile ���ͣ����� T �ķ� const volatile �汾  
template <typename T>
struct remove_cv_impl<const volatile T> {
    using type = T; // ȥ�� const �� volatile ����  
};

// ���ͱ���ģ�壬���ڷ���ʹ��  
template <typename T>
using remove_cv_t = typename remove_cv_impl<T>::type;






// ���ض�Ӧ�ķ��������͡�
//template <typename>
//using remove_reference_t = void;
// Ĭ��ģ�壺������ T ֱ�ӷ���  
template <typename T>
struct remove_reference_impl {
    using type = T; // Ĭ������·���ԭ����  
};

// �ػ��汾����� T ���������ͣ��������õķ����ð汾  
template <typename T>
struct remove_reference_impl<T&> {
    using type = T; // ȥ����ֵ����  
};

// �ػ��汾����� T ����ֵ���ã�����������ð汾  
template <typename T>
struct remove_reference_impl<T&&> {
    using type = T; // ȥ����ֵ����  
};

// ���ͱ���ģ�壬���ڷ���ʹ��  
template <typename T>
using remove_reference_t = typename remove_reference_impl<T>::type;


// ���ض�Ӧ����ֵ�������͡����������۵�����
//template <typename>
//using add_lvalue_reference_t = void;
template <typename T>
struct add_lvalue_reference_impl {
    using type = T&; // ������ֵ��������  
};

// �ػ��汾����� T ���������ͣ��򱣳����ò��䣬���� T  
template <typename T>
struct add_lvalue_reference_impl<T&> {
    using type = T&; // ������ֵ����  
};

// �ػ��汾����� T ����ֵ���ã��򷵻�������͵���ֵ����  
template <typename T>
struct add_lvalue_reference_impl<T&&> {
    using type = T&; // ����ֵ����תΪ��ֵ����  
};

// ���ͱ���ģ�壬���ڷ����ʹ��  
template <typename T>
using add_lvalue_reference_t = typename add_lvalue_reference_impl<T>::type;




// ���ض�Ӧ����ֵ�������͡����������۵�����
//template <typename>
//using add_rvalue_reference_t = void;
// Ĭ������£����� T ����ֵ��������  
template <typename T>
struct add_rvalue_reference_impl {
    using type = T&&; // Ĭ�Ϸ�����ֵ��������  
};

// �ػ��汾����� T ���������ͣ��򷵻� T  
template <typename T>
struct add_rvalue_reference_impl<T&> {
    using type = T&; // ��Ϊ��ֵ���ã��򱣳ֲ���  
};

// �ػ��汾����� T ����ֵ���ã��򷵻�����ֵ���ã����ֲ��䣩  
template <typename T>
struct add_rvalue_reference_impl<T&&> {
    using type = T&&; // ������ֵ����  
};

// ���ͱ���ģ�壬���ڷ���ʹ��  
template <typename T>
using add_rvalue_reference_t = typename add_rvalue_reference_impl<T>::type;


// ���ض�Ӧ�ķ��������͵�ָ�롣
//template <typename>
// using add_pointer_t = void;
template <typename T>
struct add_pointer_impl {
    using type = T*; // ���� T ��ָ������  
};

// �ػ��汾����� T ���������ͣ��򷵻�����������͵�ָ��  
template <typename T>
struct add_pointer_impl<T&> {
    using type = typename add_pointer_impl<T>::type; // ȥ�����ú���ȡ��ָ������  
};

// �ػ��汾����� T ����ֵ���ã��򷵻�����������͵�ָ��  
template <typename T>
struct add_pointer_impl<T&&> {
    using type = typename add_pointer_impl<T>::type; // ȥ�����ú���ȡ��ָ������  
};

// ���ͱ���ģ�壬���ڷ����ʹ��  
template <typename T>
using add_pointer_t = typename add_pointer_impl<T>::type;






// ��������T��ά�ȣ��������鷵��0��

template <typename>
inline constexpr size_t rank_v = 0;

 //ģ���ػ���������������  
template <typename T, size_t N>
inline constexpr size_t rank_v<T[N]> = rank_v<T> +1;

 //���ﴦ��ȥ������ά�Ⱥ������  
template <typename T>
inline constexpr size_t rank_v<T[]> = rank_v<T> +1;










// ��������T�ڵ�I��ά���ϵĴ�С��
//template <typename T, size_t I>
//inline constexpr size_t extent_v = 0;
// Ĭ������£�extent_v Ϊ 0  
// Ĭ������£�extent_v Ϊ 0  
template <typename T, std::size_t I>
inline constexpr std::size_t extent_v = 0;

// ����̶���С��һά����  
template <typename T, std::size_t N>
inline constexpr std::size_t extent_v<T[N], 0> = N;

// �����ά����  
template <typename T, std::size_t N, std::size_t I>
inline constexpr std::size_t extent_v<T[N], I> = extent_v<T, I - 1>;

// ����̬���飨δ֪��С��  
template <typename T>
inline constexpr std::size_t extent_v<T[], 0> = 0; // ��һά�Ķ�̬�����СΪ 0  

// �����ά��̬����  
template <typename T, std::size_t N>
inline constexpr std::size_t extent_v<T[][N], 1> = N; // ��ȷ��ȡ�ڶ�ά�Ĵ�С  

// �������ά��  
template <typename T, std::size_t N, std::size_t I>
inline constexpr std::size_t extent_v<T[][N], I> = extent_v<T, I - 1>;



// ��ȡ����T��Ԫ�ص����͡��������鷵���䱾��
//template <typename T>
//using remove_extent_t = void;

// ����ģ�嶨��  
template <typename T>
struct remove_extent {
    using type = T; // Ĭ�Ϸ���ԭ����  
};

// ����̬����  
template <typename T>
struct remove_extent<T[]> {
    using type = T; // ���ض�̬�����Ԫ������  
};

// ����̬����  
template <typename T, std::size_t N>
struct remove_extent<T[N]> {
    using type = T; // ���ؾ�̬�����Ԫ������  
};

// �������ͱ���  
template <typename T>
using remove_extent_t = typename remove_extent<T>::type;









// �� B == true ʱ���� T�����򷵻� F��
//template <bool B, typename T, typename F>
//using conditional_t = void;


template <bool B, typename T, typename F>
struct conditional_impl;

// �ػ����� B == true  
template <typename T, typename F>
struct conditional_impl<true, T, F> {
    using type = T; // ���� T  
};

// �ػ����� B == false  
template <typename T, typename F>
struct conditional_impl<false, T, F> {
    using type = F; // ���� F  
};

// ���ͱ���ģ�壬���ڷ����ʹ��  
template <bool B, typename T, typename F>
using conditional_t = typename conditional_impl<B, T, F>::type;





// ����T���˻����͡�
//template <typename>
//struct decay { using type = void; };

/// ����� requires �Ӿ���Ա�֤�� T Ϊ����ʱ��ѡ���ģ�����ʵ������
/// ��Ҳ�����Լ���һ����ʹ�� requires �İ취��
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