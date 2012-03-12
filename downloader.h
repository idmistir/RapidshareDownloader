#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QSettings>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QEventLoop>
#include <QList>
#include <QFile>
#include <QTime>

class MainWindow;

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = 0);
    
    bool checkAccount(const QString &user, const QString &pass);
    bool download(const QString &link, const QString &saveAs);

private slots:
    void requestFinished( void );
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void requestError(QNetworkReply::NetworkError nerror);

signals:
    void updateMainWindow(QString filename, QString size, QString progress, QString speed, QString eta, QString status);

private:
    void loadSettings( void );
    QNetworkAccessManager *manager;
    
    struct DOWNLOADINFO {
        QString link;
        QString path;
        bool paused;
        QNetworkAccessManager *manager;
        QNetworkReply *reply;
        QFile *file;
        QTime *timer;
    };

    QList<DOWNLOADINFO*> downloads;

    QString rsuser, rspass;
    MainWindow *window;
};

#endif // DOWNLOADER_H
