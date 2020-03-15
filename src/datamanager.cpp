#include "datamanager.hpp"

#include "achivementhtmlparser.hpp"
#include "filedownloader.hpp"

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
    if(isDataFolderComplete()) return;
    setIsWorking(true);
    createDataFolder();
    m_file_downloader->downloadFile("Achievements",
                                    std::bind(std::mem_fn(&DataManager::parseAchievementHtml), this, _1));
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
    m_achivements = res;
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
