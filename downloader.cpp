#include "downloader.h"

Downloader::Downloader(QObject *parent) :
    QObject(parent)
{
    manager = new QNetworkAccessManager(this);
}

bool Downloader::checkAccount(QString user, QString pass) {
    QString purl = QString("http://api.rapidshare.com/cgi-bin/rsapi.cgi?sub=getaccountdetails&login=%1&password=%2&withcookie=1").arg(user).arg(pass);
    QUrl url(purl);
    QNetworkRequest request(url);
    QNetworkReply *reply = manager->get(request);

    //Cry me a river if you think this is an abomination
    //Asynchronous APIs are not everything.
    QEventLoop loop;
    connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    loop.exec();

    if (reply->bytesAvailable()) {
        QStringList data = QString(reply->readAll()).split('\n');

        for (int i = 0; i < data.length(); i++) {
            if (data.at(i).contains("cookie")) {
                QSettings settings("NoOrganization", "RapidshareDownloader");
                settings.beginGroup("Settings");
                settings.setValue("cookie", data.at(i).split('=').at(1));
                settings.endGroup();
                return true;
            }
        }

    }
    return false;
}
