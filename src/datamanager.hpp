#pragma once

#include "achievement.hpp"
#include "result.hpp"

#include <QObject>

class FileDownloader;

class QFileInfo;
class QNetworkAccessManager;

class DataManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool is_working READ isWorking NOTIFY isWorkingChanged)

public:
    explicit DataManager(QNetworkAccessManager* network_manager, QObject* parent = nullptr);

    virtual ~DataManager() = default;

    bool isWorking() const;

    void checkOrDownloadData();

signals:
    void isWorkingChanged(bool);

    void achievementsChanged(QList<Achievement>);

private:
    FileDownloader* m_file_downloader;
    bool m_is_working = false;
    QList<Achievement> m_achivements;

    void setIsWorking(bool b);

    void createDataFolder();

    bool isDataFolderComplete() const;

    QFileInfo dataFile() const;
    QFileInfo dataFolder() const;
    QFileInfo dataImagesFolder() const;

    void parseAchievementHtml(Result<QByteArray> html);

    void saveAchivements(const QList<Achievement>& res);

    void downloadImages();

    void writeJsonFile();

    void parseAchievements();

    void setAchivements(QList<Achievement> res);
};
