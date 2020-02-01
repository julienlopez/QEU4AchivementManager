#pragma once

#include "achivement.hpp"
#include "result.hpp"

#include <QObject>

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

private:
    QNetworkAccessManager* const m_network_manager;
    bool m_is_working = false;

    void setIsWorking(bool b);

    bool isDataFolderComplete() const;

    QFileInfo dataFile() const;
    QFileInfo dataFolder() const;
    QFileInfo dataImagesFolder() const;

    void parseAchievementHtml(Result<QByteArray> html);

    void saveAchivements(const QList<Achivemevent>& res);

    QList<Achivemevent> downloadImages(const QList<Achivemevent>& res);

    QList<Achivemevent> writeJsonFile(const QList<Achivemevent>& res);
};
