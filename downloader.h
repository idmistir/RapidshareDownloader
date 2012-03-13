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
#include <QSslConfiguration>
#include <QSslError>

class MainWindow;

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = 0);
    
    bool checkAccount(const QString &user, const QString &pass);
    bool download(const QString &link, const QString &saveAs);
    bool downloadDirect(const QString &link);

public slots:
    void pauseDownload( const QString &link );
    void stopDownload( const QString &link );

private slots:
    void requestFinished( void );
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void requestError(QNetworkReply::NetworkError nerror);
    void requestSslErrors(QList<QSslError> errors);

signals:
    void updateMainWindow(QString filename, QString size, QString progress, QString speed, QString eta, QString status, QString next, QString total);

private:
    void loadSettings( void );
    QUrl redirectUrl(const QUrl& possibleRedirectUrl, const QUrl& oldRedirectUrl) const;

    QNetworkAccessManager *manager;
    
    struct DOWNLOADINFO {
        QString link;
        QString redirectedFrom;
        QString path;
        QString fileid;
        QString filename;
        qint64 downloaded;
        qint64 total;
        QNetworkReply *reply;
        QFile *file;
        QTime *timer;
    };

    QList<DOWNLOADINFO*> downloads;

    QString rsuser, rspass;
    MainWindow *window;
};

#endif // DOWNLOADER_H
