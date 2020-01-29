#pragma once

#include "achivement.hpp"

#include <QByteArray>
#include <QDir>
#include <QList>

class AchivementHtmlParser
{
public:
    explicit AchivementHtmlParser(const QDir& data_folder);

    ~AchivementHtmlParser() = default;

    QList<Achivemevent> parse(const QByteArray& full_html);

private:
    const QDir m_data_folder;
};
