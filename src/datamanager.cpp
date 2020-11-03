#include "datamanager.hpp"

#include "achivementhtmlparser.hpp"
#include "filedownloader.hpp"
#include "result_utilities.hpp"

#include <QDir>
#include <QFileInfo>
#include <QGuiApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>

#include <QDebug>

using std::placeholders::_1;

namespace
{
QJsonObject toJson(const Achivemevent& achivement)
{
    QJsonObject res;
    res["description"] = achivement.description;
    res["title"] = achivement.title;
    res["image"] = achivement.image_url;
    return res;
}

void printError(const QString& error)
{
    qDebug() << "error : " << error;
}

Result<QString> readFileAsString(const QString& file_path)
{
    if(QFileInfo{file_path}.isReadable() == false) return tl::make_unexpected(file_path + " is not readable");
    qDebug() << file_path << "ok!";
    auto* file = new QFile(file_path);
    if(!file->open(QIODevice::ReadOnly)) return tl::make_unexpected(QString{"Unable to open file."});
    QTextStream stream{file};
    const auto res = stream.readAll();
    file->deleteLater();
    return res;
}

Result<QJsonDocument> parseJson(QString str)
{
    QJsonParseError err;
    const auto res = QJsonDocument::fromJson(str.toUtf8(), &err);
    if(err.error == QJsonParseError::NoError)
        return res;
    else
        return tl::make_unexpected(err.errorString());
}

Result<QJsonArray> documentToArray(QJsonDocument json)
{
    if(json.isArray())
        return json.array();
    else
        return tl::unexpected(QString("not a json array"));
}

std::function<Result<QString>()> findString(const QJsonObject& json, const QString& key)
{
    return [&]() -> Result<QString> {
        const auto res = json.value(key);
        if(res.isString())
            return res.toString();
        else
            return tl::unexpected("Unable to find " + key);
    };
}

Result<Achivemevent> parseAchievement(const QJsonObject& json)
{
    return stackIndependantResults(findString(json, "title"), findString(json, "description"),
                                   findString(json, "image"))
        .map([](const std::tuple<QString, QString, QString> tuple) {
            return Achivemevent{std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple)};
        });
}

Result<QList<Achivemevent>> parseAchievements(QJsonArray json)
{
    QList<Achivemevent> res;
    for(const auto& obj : json)
    {
        if(!obj.isObject()) return tl::make_unexpected(QString{"not a json object"});
        const auto achievement = parseAchievement(obj.toObject());
        if(achievement)
            res << *achievement;
        else
            return tl::make_unexpected(achievement.error());
    }
    return res;
}

} // namespace

DataManager::DataManager(QNetworkAccessManager* network_manager, QObject* parent)
    : QObject(parent)
{
    m_file_downloader = new FileDownloader("https://eu4.paradoxwikis.com/", network_manager);
}

bool DataManager::isWorking() const
{
    return m_is_working;
}

void DataManager::checkOrDownloadData()
{
    if(isDataFolderComplete())
    {
        parseAchievements();
        return;
    }
    else
    {
    setIsWorking(true);
    createDataFolder();
    m_file_downloader->downloadFile("Achievements",
                                    std::bind(std::mem_fn(&DataManager::parseAchievementHtml), this, _1));
    }
}

void DataManager::setIsWorking(bool b)
{
    m_is_working = b;
    emit isWorkingChanged(m_is_working);
}

void DataManager::createDataFolder()
{
    if(!dataFolder().isDir()) QDir().mkdir(dataFolder().filePath());
    if(!dataImagesFolder().isDir()) QDir().mkdir(dataImagesFolder().filePath());
}

bool DataManager::isDataFolderComplete() const
{
    return dataFolder().isDir() && dataFile().isReadable() && dataImagesFolder().isDir();
}

QFileInfo DataManager::dataFile() const
{
    return QFileInfo{QDir{dataFolder().filePath()}.absoluteFilePath("achivements.json")};
}

QFileInfo DataManager::dataFolder() const
{
    return QFileInfo{qGuiApp->applicationDirPath() + QDir::separator() + "data"};
}

QFileInfo DataManager::dataImagesFolder() const
{
    return QFileInfo{QDir{dataFolder().filePath()}.absoluteFilePath("images")};
}

void DataManager::parseAchievementHtml(Result<QByteArray> html)
{
    AchivementHtmlParser parser{dataFolder().absolutePath()};
    html.and_then([&parser](QByteArray html) { return parser.parse(html); })
        .map(std::bind(&DataManager::saveAchivements, this, _1))
        .or_else(&printError);
    setIsWorking(false);
}

void DataManager::saveAchivements(const QList<Achivemevent>& res)
{
    setAchivements(res);
    writeJsonFile();
    downloadImages();
}

void DataManager::downloadImages()
{
    for(const auto& a : m_achivements)
    {
        const auto url = a.image_url;
        const auto name = url.right(url.size() - url.lastIndexOf('/') - 1);
        m_file_downloader->downloadFile(
            url, [n = QDir(dataImagesFolder().filePath()).filePath(name)](const Result<QByteArray> data) {
                data.map(std::bind(&FileDownloader::writeFile, n, _1)).or_else(&printError);
            });
    }
}

void DataManager::writeJsonFile()
{
    QJsonArray arr;
    for(const auto& achivemevent : m_achivements)
        arr << toJson(achivemevent);
    FileDownloader::writeFile(dataFile().absoluteFilePath(), QJsonDocument{arr}.toJson());
}

void DataManager::parseAchievements()
{
    const auto json = readFileAsString(dataFile().absoluteFilePath())
                          .and_then(&parseJson)
                          .and_then(&documentToArray)
                          .and_then(&::parseAchievements)
                          .map(std::bind(&DataManager::setAchivements, this, _1))
                          .or_else(&printError);
}

void DataManager::setAchivements(QList<Achivemevent> res)
{
    m_achivements = std::move(res);
    emit achievementsChanged(m_achivements);
}
