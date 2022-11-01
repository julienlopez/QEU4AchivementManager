#pragma once

namespace Utils
{

/**
 * @brief creates a lambda out of a pointer to a member variable
 */
template <class T, class RESULT> auto mem_var(const RESULT T::*member_ptr)
{
    return [member_ptr](const T& value) { return value.*member_ptr; };
}

}
