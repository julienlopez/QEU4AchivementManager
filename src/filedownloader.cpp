#include "filedownloader.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>

FileDownloader::FileDownloader(QNetworkAccessManager* network_manager, QObject* parent)
    : QObject(parent)
    , m_network_manager(network_manager)
{
}

void FileDownloader::downloadFile(QNetworkAccessManager* const network_manager, const QUrl& url,
                                  std::function<void(QByteArray)> callback, QObject* parent)
{
    auto* downloader = new FileDownloader(network_manager, parent);
    downloader->grabFile(url, callback);
}

void FileDownloader::grabFile(const QUrl& url, std::function<void(QByteArray)> callback)
{
    connect(m_network_manager, &QNetworkAccessManager::finished,
            [ c = std::move(callback), self = this ](QNetworkReply * reply) {
                auto data = reply->readAll();
                reply->deleteLater();
                self->deleteLater();
                c(std::move(data));
            });
    m_network_manager->get(QNetworkRequest{url});
}
