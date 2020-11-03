#pragma once

#include "result.hpp"

template <class T> Result<std::tuple<T>> stackIndependantResults(std::function<Result<T>()> callback)
{
    return callback().map([](auto res) { return std::make_tuple(std::move(res)); });
}

/**
 * @brief calls several callbacks returning results, and if each one produces a result, return all the results as a
 * tuple (inside a Result<>)
 */
template <class TYPE1, class... TYPES>
Result<std::tuple<TYPE1, TYPES...>> stackIndependantResults(std::function<Result<TYPE1>()> cb1,
                                                            std::function<Result<TYPES>()>... cb2)
{
    auto res1 = stackIndependantResults(std::move(cb1));
    if(!res1) return tl::make_unexpected(res1.error());
    auto res2 = stackIndependantResults(cb2...);
    if(!res2) return tl::make_unexpected(res2.error());
    return std::tuple_cat(std::move(*res1), std::move(*res2));
}
