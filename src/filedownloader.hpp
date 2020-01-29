#pragma once

#include "result.hpp"

#include <QObject>

#include <functional>

class QNetworkAccessManager;

class FileDownloader : public QObject
{
    Q_OBJECT

    using Callback_t = std::function<void(Result<QByteArray>)>;

public:
    explicit FileDownloader(QNetworkAccessManager* network_manager, QObject* parent = nullptr);

    virtual ~FileDownloader() = default;

    static void downloadFile(QNetworkAccessManager* const network_manager, const QUrl& url, Callback_t callback,
                             QObject* parent = nullptr);

private:
    QNetworkAccessManager* m_network_manager;

    void grabFile(const QUrl& url, Callback_t callback);
};
