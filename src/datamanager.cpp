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
void DataManager::parseAchievementHtml(QByteArray html)
{
    const auto res = AchivementHtmlParser{dataFolder().absolutePath()}.parse(html);

    qDebug() << "DataManager::parseAchievementHtml done!";
    qDebug() << res.size();

    setIsWorking(false);
}
