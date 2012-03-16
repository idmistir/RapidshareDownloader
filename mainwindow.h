#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>
#include <QLabel>
#include <QStatusBar>
#include <QAction>
#include <QTextDocument>
#include <QStringList>
#include <QSettings>
#include <QTableWidgetItem>
#include <QModelIndexList>
#include <QClipboard>
#include <QDesktopServices>
#include <QSystemTrayIcon>
#include <QEvent>
#include <QMenu>
#include <QTimer>
#include <QThread>

#include "downloader.h"
#include "settingspanel.h"
#include "addlinkspanel.h"
#include "progressbardelegate.h"
#include "aboutbox.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    enum DOWNLOADSTATES { WAITING = 0, QUEUING, DOWNLOADING, COMPLETED, PAUSED, CANCELLED, ERROR };

    int  downloadState(const QString &link);
    int  downloadLast(const QString &link);
    int  downloadTotal(const QString &link);

public slots:
    void updateDownload(QString filename, QString size = "", QString progress = "", QString speed = "", QString eta = "", int state = -1, QString next = "", QString total = "", QString errorString = "ERROR: Generic Error");

signals:
    void startNewDownload( void );
    void pauseDownload( const QString &link);
    void stopDownload( const QString &link);


private slots:
    void settingsMenu( void );
    void addLinksMenu( void );
    void btStart_clicked( void );
    void btPause_clicked( void );
    void btStop_clicked( void );
    void btMoveUp_clicked( void );
    void btMoveDown_clicked( void );
    void btDel_clicked( void );
    void clipboard_dataChanged( void );
    void about_clicked( void );
    void openTargetDirectory( void );
    void aboutToQuit( void );
    void changeEvent(QEvent *);
    void trayActivated(QSystemTrayIcon::ActivationReason);

private:
    void resizeEvent(QResizeEvent *);
    void loadSettings( void );
    void saveLinks( void );
    void loadLinks( void );
    void addLink( const QString &link, const QString &saveAs, const int stateValue = -1, const int nextText = 0, const int totalText = 0);
    void sortList(QModelIndexList &list, bool ascending = true);


    QAction *settings, *about, *openInBrowser, *trayActionShow, *trayActionExit;
    QClipboard *clipboard;
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    Downloader *downloader;

    QTime time;

    int concd, active;
    bool fastmode, autostart, startminimized;

    enum COLUMNS { FileName = 0, FileSize, Progress, Speed, ETA, Status, Path, Next, Total, DownloadState };

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
