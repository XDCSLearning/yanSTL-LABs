#pragma once

namespace my
{

// ����T���пɽ���Args...����ʵ�εĹ��캯��ʱ�����档
template <typename T, typename... Args>
inline constexpr bool is_constructible_v = __is_constructible(T, Args...);


/// ����Ҫ�޸ı��ļ����������ݡ�

// ����TΪvoidʱ�����档
//template <typename T>
//inline constexpr bool is_void_v = false;
template <typename T>
inline constexpr bool is_void_v = std::is_void<T>::value;

// ����TΪ�������͡��ַ����͡���������ʱ�����档
//template <typename>
//inline constexpr bool is_integral_v = false;
template <typename T>
inline constexpr bool is_integral_v = std::is_integral<T>::value;

// ����T���ж���volatile����ʱ�����档
//template <typename>
//inline constexpr bool is_volatile_v = false;
// ����ģ�壬���Ƿ���false  
template <typename T>
struct is_volatile_impl : std::false_type {};

// �ػ�ģ�壬����T��volatile�汾������true  
template <typename T>
struct is_volatile_impl<volatile T> : std::true_type {};

// ����ģ�壬ʹ��ʹ�ø����  
template <typename T>
inline constexpr bool is_volatile_v = is_volatile_impl<std::remove_reference_t<T>>::value;


// ����TΪָ��ʱ�����档
//template <typename>
//inline constexpr bool is_pointer_v = false;
template <typename T>
inline constexpr bool is_pointer_v = std::is_pointer<T>::value;

// ����TΪ����ʱ�����档
//template <typename>
//inline constexpr bool is_reference_v = false;
template <typename T>
inline constexpr bool is_reference_v = std::is_reference<T>::value;

// ����TΪ����ʱ�����档��ʾ��ֻ��UΪ����������ʱ��is_const_v<const U>���ؼ١�
//template <typename>
//inline constexpr bool is_function_v = false;
template <typename T>
inline constexpr bool is_function_v = std::is_function<T>::value;


// ����TΪ����void������������������κ�����ʱ�����档
//template <typename>
//inline constexpr bool is_object_v = false;

template <typename T>
inline constexpr bool is_object_v =
!std::is_void<T>::value &&
!std::is_function<T>::value &&
!std::is_reference<T>::value;


// ����TΪԭ������ʱ�����档
//template <typename>
//inline constexpr bool is_array_v = false;
template <typename T>
inline constexpr bool is_array_v = std::is_array<T>::value;

// ����T�����Ĭ�Ϲ���ʱ�����档
//template <typename>
//inline constexpr bool is_default_constructible_v = false;
template <typename T>
inline constexpr bool is_default_constructible_v = std::is_default_constructible<T>::value;

// ����T����ɿ�������ʱ�����档
//template <typename>
//inline constexpr bool is_copy_constructible_v = false;
template <typename T>
inline constexpr bool is_copy_constructible_v = std::is_copy_constructible<T>::value;


// ����T������ƶ�����ʱ�����档
//template <typename>
//inline constexpr bool is_move_constructible_v = false;
template <typename T>
inline constexpr bool is_move_constructible_v = std::is_move_constructible<T>::value;

// ����T1, T2����ͬ����ʱ�����档����CV���Ρ�
//template <typename, typename>
//inline constexpr bool is_same_v = false;
template <typename T1, typename T2>
inline constexpr bool is_same_v = std::is_same<T1, T2>::value;

// ����TΪTypes...��������֮һʱ�����档�ⲻ�Ǳ�׼�涨�����ӡ�
//template <typename, typename...>
//inline constexpr bool is_any_of_v = false;
template <typename T, typename... Types>
struct is_any_of_impl : std::false_type {};

// �ݹ���������T�Ƿ��Types�еĵ�һ��������ͬ  
template <typename T, typename First, typename... Rest>
struct is_any_of_impl<T, First, Rest...> :
    std::conditional<
    std::is_same<T, First>::value,
    std::true_type,
    is_any_of_impl<T, Rest...>
    >::type {};

// ģ�������ʹ���÷������  
template <typename T, typename... Types>
inline constexpr bool is_any_of_v = is_any_of_impl<T, Types...>::value;


// ����T��ΪTypes...��������֮һʱ�����档�ⲻ�Ǳ�׼�涨�����ӡ�
//template <typename, typename...>
//inline constexpr bool is_none_of_v = false;

// �ݹ���ֹ������û�и����������Ҫ���  
template <typename T, typename... Types>
struct is_none_of_impl : std::true_type {};

// �ݹ���������T�Ƿ��Types�еĵ�һ��������ͬ  
template <typename T, typename First, typename... Rest>
struct is_none_of_impl<T, First, Rest...> :
    std::conditional<
    std::is_same<T, First>::value,
    std::false_type,
    is_none_of_impl<T, Rest...>
    >::type {};

// ģ�������ʹ���÷������  
template <typename T, typename... Types>
inline constexpr bool is_none_of_v = is_none_of_impl<T, Types...>::value;


// ���ض�Ӧ�Ĵ���const���ε����͡�
//template <typename>
//using add_const_t = void;
template <typename T>
using add_const_t = typename std::add_const<T>::type;

// ���ض�Ӧ�Ĳ�����const���ε����͡�
//template <typename>
//using remove_const_t = void;
template <typename T>
using remove_const_t = typename std::remove_const<T>::type;

// ���ض�Ӧ�Ĳ�����CV���ε����͡�
//template <typename>
//using remove_cv_t = void;

template <typename T>
using remove_cv_t = typename std::remove_cv<T>::type;

// ���ض�Ӧ�ķ��������͡�
//template <typename>
//using remove_reference_t = void;
template <typename T>
using remove_reference_t = typename std::remove_reference<T>::type;

// ���ض�Ӧ����ֵ�������͡����������۵�����
//template <typename>
//using add_lvalue_reference_t = void;
template <typename T>
using add_lvalue_reference_t = typename std::add_lvalue_reference<T>::type;

// ���ض�Ӧ����ֵ�������͡����������۵�����
//template <typename>
//using add_rvalue_reference_t = void;
template <typename T>
using add_rvalue_reference_t = typename std::add_rvalue_reference<T>::type;

// ���ض�Ӧ�ķ��������͵�ָ�롣
//template <typename>
// using add_pointer_t = void;
template <typename T>
using add_pointer_t = typename std::add_pointer<T>::type;


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


// �����������д���  
template <bool B, typename T, typename F>
// ��ȷ������ѡ��ʵ��  
using conditional_t = typename std::conditional<B, T, F>::type;





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