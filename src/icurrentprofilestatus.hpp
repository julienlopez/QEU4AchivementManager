#pragma once

#include <boost/core/noncopyable.hpp>

class ICurrentProfileStatus : private boost::noncopyable
{
protected:
    ICurrentProfileStatus() = default;

public:
    virtual bool hasACurrentProfile() const = 0;

    virtual bool isTodo(const QString& achievement_title) const = 0;

    virtual bool isDone(const QString& achievement_title) const = 0;
};
