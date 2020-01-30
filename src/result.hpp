#pragma once

#include <tl/expected.hpp>

#include <QString>

template<class T>
using Result = tl::expected<T, QString>;
