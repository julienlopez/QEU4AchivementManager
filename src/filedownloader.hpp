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
    explicit FileDownloader(const QString& root_domain, QNetworkAccessManager* network_manager,
                            QObject* parent = nullptr);

    virtual ~FileDownloader() = default;

    void downloadFile(const QString& url, Callback_t callback);

private:
    const QString m_root_domain;
    QNetworkAccessManager* m_network_manager;
};
