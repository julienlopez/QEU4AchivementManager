#pragma once

#include "achievement.hpp"
#include "result.hpp"

#include <QByteArray>
#include <QDir>
#include <QList>

class AchievementHtmlParser
{
public:
    explicit AchievementHtmlParser(const QDir& data_folder);

    ~AchievementHtmlParser() = default;

    Result<QList<Achievement>> parse(const QByteArray& full_html);

private:
    const QDir m_data_folder;
};
