#pragma once

#include "achivement.hpp"
#include "result.hpp"

#include <QByteArray>
#include <QDir>
#include <QList>

class AchivementHtmlParser
{
public:
    explicit AchivementHtmlParser(const QDir& data_folder);

    ~AchivementHtmlParser() = default;

    Result<QList<Achivemevent>> parse(const QByteArray& full_html);

private:
    const QDir m_data_folder;
};
