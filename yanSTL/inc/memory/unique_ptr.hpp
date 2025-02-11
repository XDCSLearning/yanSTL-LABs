#pragma once
#include "common.hpp"

namespace my {

template <typename T, typename Deleter>
class unique_ptr_base {
public:

    /// ���ڴ˴����嵥����汾������汾 unique_ptr �Ĺ��г�Ա��

protected:
    T* ptr_;
    Deleter deleter_;
};

template <typename T, typename Deleter>
class unique_ptr : public unique_ptr_base<T, Deleter> {
    using Base = unique_ptr_base<T, Deleter>;
public:
    using Base::Base; // ʹ�û��๹�캯��

    /// ���ڴ˴����嵥����汾 unique_ptr �����г�Ա��
};

template <typename T, typename Deleter>
class unique_ptr<T[], Deleter> : public unique_ptr_base<T, Deleter> {
    using Base = unique_ptr_base<T, Deleter>;
public:
    using Base::Base; // ʹ�û��๹�캯��

    /// ���ڴ˴���������汾 unique_ptr �����г�Ա��
};

/// �����ڴ˴����� make_unique ������

} // namespace my