#pragma once

#include <QObject>

class QNetworkAccessManager;

class FileDownloader : public QObject
{
    Q_OBJECT

public:
    explicit FileDownloader(QNetworkAccessManager* network_manager, QObject* parent = nullptr);

    virtual ~FileDownloader() = default;

    static void downloadFile(QNetworkAccessManager* const network_manager, const QUrl& url,
                             std::function<void(QByteArray)> callback, QObject* parent = nullptr);

private:
    QNetworkAccessManager* m_network_manager;

    void grabFile(const QUrl& url, std::function<void(QByteArray)> callback);
};