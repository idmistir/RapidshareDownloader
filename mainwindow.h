#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>
#include <QLabel>
#include <QStatusBar>
#include <QAction>

#include "settingspanel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void settingsMenu( void );

private:
    void resizeEvent(QResizeEvent *);

    QAction *settings;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
