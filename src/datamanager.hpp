#pragma once

#include "achivement.hpp"
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

    void achievementsChanged(QList<Achivemevent>);

private:
    FileDownloader* m_file_downloader;
    bool m_is_working = false;
    QList<Achivemevent> m_achivements;

    void setIsWorking(bool b);

    void createDataFolder();

    bool isDataFolderComplete() const;

    QFileInfo dataFile() const;
    QFileInfo dataFolder() const;
    QFileInfo dataImagesFolder() const;

    void parseAchievementHtml(Result<QByteArray> html);

    void saveAchivements(const QList<Achivemevent>& res);

    void downloadImages();

    void writeJsonFile();

    void parseAchievements();

    void setAchivements(QList<Achivemevent> res);
};
