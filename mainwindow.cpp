#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    settings = new QAction(this);
    settings->setText(tr("Settings"));

    this->menuBar()->addAction(settings);
    connect(settings, SIGNAL(triggered()), this, SLOT(settingsMenu()));
    connect(ui->btAdd, SIGNAL(clicked()), this, SLOT(addLinksMenu()));
    return;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addLinksMenu( void ) {
    AddLinksPanel *panel = new AddLinksPanel(this);
    panel->exec();
}

void MainWindow::settingsMenu( void ) {
    SettingsPanel *panel = new SettingsPanel(this);
    panel->exec();
}

void MainWindow::resizeEvent(QResizeEvent *e) {
    QMainWindow::resizeEvent(e);
    if (!(e->size().height() < 0)) {
        ui->layoutActions->setGeometry(QRect(0, 0, ui->layoutActions->geometry().width(), ui->layoutActions->geometry().height()));
        ui->tblDownloads->setGeometry(QRect(0, ui->layoutActions->geometry().height() + 10, width(), height() - ui->layoutActions->geometry().height() - 10));
    }
    return;
}
