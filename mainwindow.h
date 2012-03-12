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

#include "downloader.h"
#include "settingspanel.h"
#include "addlinkspanel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
    void updateDownload(QString filename, QString size = "", QString progress = "", QString speed = "", QString eta = "", QString status = "", QString next = "");

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

private:
    void resizeEvent(QResizeEvent *);
    void loadSettings( void );
    void saveLinks( void );
    void loadLinks( void );
    void addLink( const QString &link, const QString &saveAs, const QString &status = "", const int next = 0 );
    void sortList(QModelIndexList &list, bool ascending = true);

    Downloader *downloader;

    QAction *settings;

    int concd, active;
    bool fastmode, autostart;

    enum COLUMNS { FileName = 0, FileSize, Progress, Speed, ETA, Status, Path, Next };

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
