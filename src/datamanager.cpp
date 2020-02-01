#include "datamanager.hpp"

#include "achivementhtmlparser.hpp"
#include "filedownloader.hpp"

#include <QDir>
#include <QFileInfo>
#include <QGuiApplication>
#include <QUrl>

DataManager::DataManager(QNetworkAccessManager* network_manager, QObject* parent)
    : QObject(parent)
    , m_network_manager(network_manager)
{
}

bool DataManager::isWorking() const
{
    return m_is_working;
}

void DataManager::checkOrDownloadData()
{
    if(isDataFolderComplete()) return;
    setIsWorking(true);
    FileDownloader::downloadFile(
        m_network_manager, QUrl("https://eu4.paradoxwikis.com/Achievements"),
        std::bind(std::mem_fn(&DataManager::parseAchievementHtml), this, std::placeholders::_1));
}

void DataManager::setIsWorking(bool b)
{
    m_is_working = b;
    emit isWorkingChanged(m_is_working);
}

bool DataManager::isDataFolderComplete() const
{
    return dataFolder().isDir() && dataFile().isReadable() && dataImagesFolder().isDir();
}

QFileInfo DataManager::dataFile() const
{
    return QFileInfo{QDir{dataFolder().absolutePath()}.absoluteFilePath("achivements.json")};
}

QFileInfo DataManager::dataFolder() const
{
    return QFileInfo{QDir{qGuiApp->applicationDirPath()}.absoluteFilePath("data")};
}

QFileInfo DataManager::dataImagesFolder() const
{
    return QFileInfo{QDir{dataFolder().absolutePath()}.absoluteFilePath("images")};
}
#include <QDebug>
void DataManager::parseAchievementHtml(Result<QByteArray> html)
{
    using std::placeholders::_1;
    AchivementHtmlParser parser{dataFolder().absolutePath()};

    html.and_then([&parser](QByteArray html) { return parser.parse(html); })
        .map([](const QList<Achivemevent>& res) {
            qDebug() << "DataManager::parseAchievementHtml done!";
            qDebug() << res.size();
            return res;
        })
        .map(std::bind(&DataManager::saveAchivements, this, _1))
        .or_else([](const QString& error) { qDebug() << "error: " << error; });
    setIsWorking(false);
}

void DataManager::saveAchivements(const QList<Achivemevent>& res)
{
}

QList<Achivemevent> DataManager::downloadImages(const QList<Achivemevent>& res)
{
}

QList<Achivemevent> DataManager::writeJsonFile(const QList<Achivemevent>& res)
{
}
