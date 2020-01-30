#include "achivementhtmlparser.hpp"

#include <QFile>

#include <QDebug>

namespace
{

Result<QByteArray> extractTheFirstTable(const QByteArray& html)
{
    auto start_pos = html.indexOf("<table");
    start_pos = html.indexOf('<', start_pos + 1); // to remove the <table part
    const auto end_pos = html.indexOf("</table>", start_pos);
    return html.mid(start_pos, end_pos - start_pos);
}

auto writeCurrentHtmlContent(const QString& file_path)
{
    return [file_path](QByteArray html) -> QByteArray {
        auto* f = new QFile{file_path};
        f->open(QIODevice::WriteOnly);
        f->write(html);
        f->deleteLater();
        qDebug() << "write done!";
        return html;
    };
}

QByteArray removeEmptyLines(QByteArray html)
{
    return html.replace("\n\n", "\n");
}

QByteArray removeTBodyTags(QByteArray html)
{
    const auto start = html.startsWith("<tbody>") ? 7 : 0;
    auto end = html.endsWith("</tbody>") ? 8 : 0;
    if(html.at(html.size() - end - 1) == '\n') end++;
    return html.mid(start, html.size() - end - start);
}

Result<QList<Achivemevent>> parseAchivements(QByteArray html)
{
    return {};
}
}

AchivementHtmlParser::AchivementHtmlParser(const QDir& data_folder)
    : m_data_folder(data_folder)
{
}

Result<QList<Achivemevent>> AchivementHtmlParser::parse(const QByteArray& full_html)
{
    return extractTheFirstTable(full_html)
        .map(&removeEmptyLines)
        .map(&removeTBodyTags)
        .map(writeCurrentHtmlContent(m_data_folder.absoluteFilePath("res.txt")))
        .and_then(&parseAchivements);
}
