#pragma once

#include "achievementstatus.hpp"

#include <QList>
#include <QString>

struct Profile
{
    QString name;
    QList<AchievementStatus> achivements;
};
