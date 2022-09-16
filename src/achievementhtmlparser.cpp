#include "achievementhtmlparser.hpp"
#include "filedownloader.hpp"

#include <QFile>

#include <QRegularExpression>

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
    return [file_path](QStringList html) -> QStringList {
        FileDownloader::writeFile(file_path, html.join("\n\n").toUtf8());
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

QStringList splitByTag(QString html, const QString& open_tag, const QString& close_tag)
{
    if(html.startsWith(open_tag)) html = html.right(html.size() - 4);
    if(html.endsWith(close_tag)) html.chop(5);
    return html.trimmed().split(QRegularExpression(close_tag + "\\n*" + open_tag));
}

QStringList splitByLines(QString html)
{
    return splitByTag(html, "<tr>", "</tr>");
}

QStringList removeHeaderLine(QStringList lines)
{
    const auto& first_line = lines.front();
    if(first_line.startsWith("<th") && first_line.endsWith("</th>")) lines.pop_front();
    return lines;
}

QString removeUpTo(QString str, const QString& token)
{
    const auto pos = str.indexOf(token);
    return str.right(str.size() - pos - token.size());
}

QString parseTitleWithLink(QString html)
{
    auto res = html.right(html.size() - html.indexOf(">") - 1);
    return res.left(res.indexOf("<"));
}

auto parseTitleColumn(QString html)
{
    html = removeUpTo(html, "<img ");
    html = removeUpTo(html, "src=\"");
    const auto image_url = html.left(html.indexOf("\""));
    html = removeUpTo(html, "<div ");
    html = removeUpTo(html, "<div ");
    html = removeUpTo(html, ">");
    auto title = html.left(html.indexOf("<"));
    if(title.isEmpty()) title = parseTitleWithLink(html);
    html = removeUpTo(html, "<div ");
    html = removeUpTo(html, ">");
    const auto description = html.left(html.indexOf("<"));
    return Achievement{title, description, image_url};
}

Result<Achievement> parseAchivement(QString html)
{
    auto parts = splitByTag(html, "<td", "</td>");
    if(parts.size() != 7) return tl::make_unexpected("not enough columns");
    const auto res = parseTitleColumn(parts.front());
    return res;
}

QList<Achievement> parseAchivements(QStringList html)
{
    QList<Achievement> achivements;
    for(const auto& line : html)
    {
        parseAchivement(line).map([&achivements](Achievement a) { achivements << a; });
    }
    return achivements;
}
} // namespace

AchievementHtmlParser::AchievementHtmlParser(const QDir& data_folder)
    : m_data_folder(data_folder)
{
}

Result<QList<Achievement>> AchievementHtmlParser::parse(const QByteArray& full_html)
{
    return extractTheFirstTable(full_html)
        .map(&removeEmptyLines)
        .map(&removeTBodyTags)
        .map(&splitByLines)
        .map(&removeHeaderLine)
        // .map(writeCurrentHtmlContent(m_data_folder.absoluteFilePath("res.txt")))
        .map(&parseAchivements);
}
