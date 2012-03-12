#include "downloader.h"
#include "mainwindow.h"

Downloader::Downloader(QObject *parent) :
    QObject(parent)
{
    window = qobject_cast<MainWindow*>(parent);
    manager = new QNetworkAccessManager(this);
    rsuser = "";
    rspass = "";
    connect(this, SIGNAL(updateMainWindow(QString,QString,QString,QString,QString,QString)), window, SLOT(updateDownload(QString,QString,QString,QString,QString,QString)));
    loadSettings();
}

void Downloader::loadSettings( void ) {
    QSettings settings("NoOrganization", "RapidshareDownloader");
    settings.beginGroup("Settings");
    rsuser = settings.value("rsuser").toString();
    rspass = settings.value("rspass").toString();
    settings.endGroup();
}

bool Downloader::checkAccount(const QString &user, const QString &pass) {
    QString purl = QString("http://api.rapidshare.com/cgi-bin/rsapi.cgi?sub=getaccountdetails&login=%1&password=%2&withcookie=1").arg(user).arg(pass);
    QUrl url(purl);
    QNetworkRequest request(url);
    QNetworkReply *reply = manager->get(request);

    //Cry me a river if you think this is an abomination
    //Asynchronous APIs are not everything.
    QEventLoop loop;
    connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    loop.exec();

    if (reply->readAll().contains("accountid"))
        return true;
    return false;
}

// sub=download

// Description:
//         Call this function to download a file from RapidShare as a free user or a RapidPro user. Free downloads are always using http://, while RapidPro
//         downloads must use https:// (SSL) for security reasons. Since this function is also called by browsers, we append a JavaScript to every error message,
//         which redirects the user to our homepage. Line 1 is the plain error string as usual. Line 2 is JavaScript code for browser redirection.
// Parameters:
//         fileid=File ID
//         filename=Filename
//         dlauth=The download authentication string (only used by free users)
//         try=If 1, it will always return only "DL:$hostname,$dlauth,$countdown"
//         login=Your login (if using premium downloads)
//         password=Your password (if using premium downloads)
//         [start|range]=<x|x-y> You can specifiy where to start downloading. Valid examples are: start=123000 start=0-10000 position=5000-10000 position=50000
// Reply:
//         The reply is dynamic. As always, errors start with "ERROR: <string>". The different scenarios are now explained.
//         We always assume that no error will be given. If there is an error, you should display it to your user (or parse it and display something else).

//         Downloading as a free user:
//             Step 1: Call this function with fileid and filename. You will get "DL:$hostname,$dlauth,$countdown,$md5hex".
//             Step 2: Wait $countdown seconds.
//             Step 3: Call the server $hostname with download authentication string $dlauth after $countdown seconds. You instantly get the file.
//                 WARNING: In our terms of service, we prohibit the automation of downloads for free users. It IS allowed to create tools,
//                 which download ONE file as a free user from RapidShare. It is NOT allowed to implement queuing mechanisms. Resuming aborted
//                 downloads is technically disabled.

//         Downloading as a RapidPro user:
//             Step 1: Call this function with fileid, filename, login and password.
//                 If you called the right server and "try" is not given, you will instantly receive the file. If you called the wrong server,
//                 or try=1 is given, you will get "DL:$hostname,$dlauth,$countdown,$md5hex". See side note #1.
//             Step 2: Call the same function on the server $hostname. You instantly get the file. Since you are a RapidPro user, $dlauth and
//                 $countdown are always 0. You MUST use https in this step.

// Side notes:
//         #1: The instant download request can only be fulfilled on the server hosting the file. To completely avoid this error, you have to know the server ID,
//         where the file is hosted. There are scenarios where the server ID is available. For example if you call sub=checkfiles before. If so, you will not get
//         this error message if you directly call the correct server. If you do not know the server ID, you just have to parse for this error and send your request
//         again to the correct server.


bool Downloader::download(const QString &link, const QString &saveAs) {
    DOWNLOADINFO *newDownload = new DOWNLOADINFO;
    newDownload->link = link;
    newDownload->path = saveAs;
    newDownload->paused = false;

    QStringList parts = link.split('/');
    newDownload->fileid = parts.at(parts.count() - 2);
    newDownload->filename = parts.last();

    QUrl url = QString("https://api.rapidshare.com/cgi-bin/rsapi.cgi?sub=download&fileid=%1&filename=%2&login=%3&password=%4").arg(newDownload->fileid).arg(newDownload->filename).arg(rsuser).arg(rspass);
    QNetworkRequest request(url);
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());

    QString localFileName = saveAs + '\\' + newDownload->filename;
    newDownload->file = new QFile(localFileName);

    for (int i = 0; newDownload->file->exists(); i++)
        newDownload->file->setFileName(localFileName + QString::number(i));

    if (newDownload->file->open(QIODevice::WriteOnly)) {
        newDownload->reply = manager->get(request);
        connect(newDownload->reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
        connect(newDownload->reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(requestError(QNetworkReply::NetworkError)));
        connect(newDownload->reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(requestSslErrors(QList<QSslError>)));
        newDownload->timer = new QTime();
        newDownload->timer->start();
        downloads.append(newDownload);
        return true;
    }
    delete newDownload;
    return false;
}

void Downloader::requestSslErrors(QList<QSslError> errors) {
    for (int i = 0; i < errors.length(); i++) {
        QSslError err = errors.at(i);
        QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
        reply->ignoreSslErrors();
    }
}

void Downloader::requestError(QNetworkReply::NetworkError nerror) {
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QString error = reply->errorString();
    //todo: handle + display error
}

void Downloader::requestFinished( void ) {
    //fallback
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(QObject::sender());
    DOWNLOADINFO *currDownload;
    for (int i = 0; i < downloads.count(); i++) {
        if (((DOWNLOADINFO*)downloads.at(i))->reply == reply) {
            currDownload = downloads.at(i);
            currDownload->file->write(reply->readAll());
            currDownload->file->close();
            currDownload->file->deleteLater();
            currDownload->reply->deleteLater();
            downloads.removeAt(i);
        }
    }
}

void Downloader::downloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
    //Find the structure from the list
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QString progress, size, unit;

    DOWNLOADINFO *currDownload = NULL;
    for (int i = 0; i < downloads.count(); i++) {
        if (((DOWNLOADINFO*)downloads.at(i))->reply == reply)
            currDownload = downloads.at(i);
    }

    if (currDownload == NULL)
        return;

    QByteArray got = reply->readAll();
    //todo: replace this with a regexp and be more extensive to count for
    //the latter content
    if (got.startsWith("DL:")) {
        //wrong server
        currDownload->reply->deleteLater();
        QString url = "https://" + QString(got.split(',').at(0).split(':').at(1)) + QString("/cgi-bin/rsapi.cgi?sub=download&fileid=%1&filename=%2&login=%3&password=%4").arg(currDownload->fileid).arg(currDownload->filename).arg(rsuser).arg(rspass);
        QNetworkRequest request(url);
        request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
        currDownload->reply = manager->get(request);
        connect(currDownload->reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
        connect(currDownload->reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(requestError(QNetworkReply::NetworkError)));
        connect(currDownload->reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(requestSslErrors(QList<QSslError>)));
        currDownload->timer->restart();
        return;
    }
    currDownload->file->write(got);
    double speed = bytesReceived * 1000.0 / currDownload->timer->elapsed();

    QTime time(0, 0, (bytesTotal / speed) - (currDownload->timer->elapsed() / 1000));
    QString eta = time.toString("hh:mm:ss");

    if (speed < 1024) {
        unit = QString::number((qint64)speed) + "B/s";
    } else if (speed < 1024*1024) {
        speed /= 1024;
        unit = QString::number((qint64)speed) + "kB/s";
    } else {
        speed /= 1024*1024;
        unit = QString::number((qint64)speed) + "MB/s";
    }

    if (bytesTotal)
        progress = QString::number((qint64)(((double)bytesReceived / (double)bytesTotal) * 100)) + "%";
    else
        progress = "0%";

    double bytes = bytesTotal;
    for (int i = 0;; i++) {
        if (bytes > 1)
            bytes = bytes / 1024;
        else {
            size = QString::number(bytes, 'f', 2);
            switch (i) {
                case 0:
                    size += "B";
                    break;
                case 1:
                    size += "KB";
                    break;
                case 2:
                    size += "MB";
                    break;
                case 3:
                    size += "GB";
                    break;
                case 4:
                    size += "TB";
                    break;
                default:
                    break;
            }
            break;
        }
    }

    QString status = "DOWNLOADING";
    if (bytesReceived == bytesTotal) {
        for (int i = 0; i < downloads.count(); i++) {
            if (((DOWNLOADINFO*)downloads.at(i))->reply == reply) {
                currDownload->file->close();
                currDownload->file->deleteLater();
                currDownload->reply->deleteLater();
                downloads.removeAt(i);
                status = "COMPLETED";
            }
        }
    }
    emit updateMainWindow(currDownload->link, size, progress, unit, eta, status);
}
