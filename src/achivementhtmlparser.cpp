#include "achivementhtmlparser.hpp"

#include <QFile>

#include <QDebug>

AchivementHtmlParser::AchivementHtmlParser(const QDir& data_folder)
    : m_data_folder(data_folder)
{
}

QList<Achivemevent> AchivementHtmlParser::parse(const QByteArray& full_html)
{
    qDebug() << "DataManager::parseAchievementHtml done!";
    qDebug() << full_html.size();

    const auto start_pos = full_html.indexOf("<table");
    const auto end_pos = full_html.indexOf("</table>", start_pos);
    auto html = full_html.mid(start_pos, end_pos - start_pos);
    html.replace("\n\n", "\n");

    auto* f = new QFile{m_data_folder.absoluteFilePath("res.txt")};
    f->open(QIODevice::WriteOnly);
    f->write(html);
    qDebug() << "write done!";

    return {};
}
