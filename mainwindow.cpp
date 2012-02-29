#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(this->size());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *e) {
    QMainWindow::resizeEvent(e);
    if (!(e->size().height() < 0)) {
        ui->layoutActions->setGeometry(QRect(0, 0, ui->layoutActions->geometry().width(), ui->layoutActions->geometry().height()));
        ui->tblDownloads->setGeometry(QRect(0, ui->layoutActions->geometry().height() + 10, width(), height() - ui->layoutActions->geometry().height() - 10));
    }
    return;
}
