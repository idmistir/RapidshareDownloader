#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    concd = 0;
    active = 0;
    fastmode = false;
    autostart = false;

    settings = new QAction(this);
    settings->setText(tr("Settings"));

    downloader = new Downloader(this);

    this->menuBar()->addAction(settings);
    connect(settings, SIGNAL(triggered()), this, SLOT(settingsMenu()));
    connect(ui->btAdd, SIGNAL(clicked()), this, SLOT(addLinksMenu()));
    connect(ui->btStart, SIGNAL(clicked()), this, SLOT(btStart_clicked()));
    connect(ui->btPause, SIGNAL(clicked()), this, SLOT(btPause_clicked()));
    connect(ui->btStop, SIGNAL(clicked()), this, SLOT(btStop_clicked()));
    connect(ui->btMoveUp, SIGNAL(clicked()), this, SLOT(btMoveUp_clicked()));
    connect(ui->btMoveDown, SIGNAL(clicked()), this, SLOT(btMoveDown_clicked()));

    loadSettings();
    return;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateDownload(QString filename, QString size, QString progress, QString speed, QString eta, QString status) {
    for (int i = 0; i < ui->tblDownloads->rowCount(); i++) {
        if (ui->tblDownloads->item(i, FileName)->text() == filename) {
            ui->tblDownloads->item(i, FileSize)->setText(size);
            ui->tblDownloads->item(i, Progress)->setText(progress);
            ui->tblDownloads->item(i, Speed)->setText(speed);
            ui->tblDownloads->item(i, ETA)->setText(eta);
            ui->tblDownloads->item(i, Status)->setText(status);
        }
    }
}

void MainWindow::btStart_clicked( void ) {
    if (active >= concd)
        return;
    if (ui->tblDownloads->selectedItems().count()) {
        if (ui->tblDownloads->selectedItems().at(0)->text().isEmpty())
            return;
        //todo

    } else {
        for (int i = 0; active < concd && i < ui->tblDownloads->rowCount(); i++) {
            if (ui->tblDownloads->item(i, Status)->text() == tr("WAITING")) {
                if (downloader->download(ui->tblDownloads->item(i, FileName)->text(), ui->tblDownloads->item(i, Path)->text()))
                    active++;
            }
        }
    }
}

void MainWindow::btPause_clicked( void ) {

}

void MainWindow::btStop_clicked( void ) {

}

void MainWindow::btMoveUp_clicked( void ) {

}

void MainWindow::btMoveDown_clicked( void ) {

}

void MainWindow::addLinksMenu( void ) {
    AddLinksPanel *panel = new AddLinksPanel(this);
    if (panel->exec()) {
        QStringList links = panel->getLinks()->toPlainText().split('\n');
        QString fpath = panel->getPath();
        for (int i = 0, nextrow = ui->tblDownloads->rowCount(); i < links.length(); i++, nextrow++) {
            ui->tblDownloads->insertRow(nextrow);
            QTableWidgetItem *name = new QTableWidgetItem();
            QTableWidgetItem *path = new QTableWidgetItem();
            QTableWidgetItem *status = new QTableWidgetItem();
            QTableWidgetItem *size = new QTableWidgetItem();
            QTableWidgetItem *progress = new QTableWidgetItem();
            QTableWidgetItem *speed = new QTableWidgetItem();
            QTableWidgetItem *eta = new QTableWidgetItem();

            name->setText(links.at(i));
            path->setText(fpath);
            status->setText(tr("WAITING"));
            size->setText("");
            progress->setText("");
            speed->setText("");
            eta->setText("");

            name->setTextAlignment(Qt::AlignCenter);
            path->setTextAlignment(Qt::AlignCenter);
            status->setTextAlignment(Qt::AlignCenter);
            size->setTextAlignment(Qt::AlignCenter);
            progress->setTextAlignment(Qt::AlignCenter);
            speed->setTextAlignment(Qt::AlignCenter);
            eta->setTextAlignment(Qt::AlignCenter);

            ui->tblDownloads->setItem(nextrow, FileName, name);
            ui->tblDownloads->setItem(nextrow, Path, path);
            ui->tblDownloads->setItem(nextrow, Progress, progress);
            ui->tblDownloads->setItem(nextrow, FileSize, size);
            ui->tblDownloads->setItem(nextrow, Speed, speed);
            ui->tblDownloads->setItem(nextrow, ETA, eta);
            ui->tblDownloads->setItem(nextrow, Status, status);
        }
    }
}

void MainWindow::settingsMenu( void ) {
    SettingsPanel *panel = new SettingsPanel(this);
    if (panel->exec())
        loadSettings();
}

void MainWindow::loadSettings( void ) {
    QSettings settings("NoOrganization", "RapidshareDownloader");
    settings.beginGroup("Settings");
    concd = settings.value("concd").toInt();
    fastmode = settings.value("fastmode").toBool();
    autostart = settings.value("autostart").toBool();
    settings.endGroup();
    return;
}

void MainWindow::resizeEvent(QResizeEvent *e) {
    QMainWindow::resizeEvent(e);
    if (!(e->size().height() < 0)) {
        ui->layoutActions->setGeometry(QRect(0, 0, ui->layoutActions->geometry().width(), ui->layoutActions->geometry().height()));
        ui->tblDownloads->setGeometry(QRect(0, ui->layoutActions->geometry().height() + 10, width(), height() - ui->layoutActions->geometry().height() - 10));
    }
    return;
}
