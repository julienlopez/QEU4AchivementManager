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
        .map(writeCurrentHtmlContent(m_data_folder.absoluteFilePath("res.txt")))
        .and_then(&parseAchivements);
}
