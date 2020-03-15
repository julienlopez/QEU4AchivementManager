#include "filedownloader.hpp"

#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>

FileDownloader::FileDownloader(const QString& root_domain, QNetworkAccessManager* network_manager, QObject* parent)
    : QObject(parent)
    , m_root_domain(root_domain)
    , m_network_manager(network_manager)
{
}

void FileDownloader::downloadFile(const QString& url, Callback_t callback)
{
    auto* reply = m_network_manager->get(QNetworkRequest{QUrl(m_root_domain + url)});
    connect(reply, &QNetworkReply::finished, [ reply, c = std::move(callback) ]() {
        auto data = reply->readAll();
        const auto error = reply->error();
        reply->deleteLater();
        if(error == QNetworkReply::NoError)
            c(std::move(data));
        else
        {
            qDebug() << "error: " << error;
            c(tl::make_unexpected("Unable to download the wiki page"));
        }
    });
}

bool FileDownloader::writeFile(const QString& file_path, const QByteArray& data)
{
    bool res = false;
    auto* f = new QFile{file_path};
    if(f->open(QIODevice::WriteOnly))
        if(f->write(data) == data.size()) res = true;
    f->deleteLater();
    return res;
}
