#include "filedownloader.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>

FileDownloader::FileDownloader(QNetworkAccessManager* network_manager, QObject* parent)
    : QObject(parent)
    , m_network_manager(network_manager)
{
}

void FileDownloader::downloadFile(QNetworkAccessManager* const network_manager, const QUrl& url, Callback_t callback,
                                  QObject* parent)
{
    auto* downloader = new FileDownloader(network_manager, parent);
    downloader->grabFile(url, callback);
}

void FileDownloader::grabFile(const QUrl& url, Callback_t callback)
{
    connect(m_network_manager, &QNetworkAccessManager::finished,
            [ c = std::move(callback), self = this ](QNetworkReply * reply) {
                auto data = reply->readAll();
                const auto error = reply->error();
                reply->deleteLater();
                self->deleteLater();
                if(error == QNetworkReply::NoError)
                    c(std::move(data));
                else
                    c(tl::make_unexpected("Unable to download the wiki page"));
            });
    m_network_manager->get(QNetworkRequest{url});
}
