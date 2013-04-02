#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    timer->start(1000);

    QIcon appIcon;
    appIcon.addFile(QString::fromUtf8(":/icons/images/icon.png"), QSize(), QIcon::Normal, QIcon::Off);

    concd = 0;
    active = 0;
    fastmode = false;
    autostart = false;
    startminimized = false;

    trayIcon = new QSystemTrayIcon(this);
    trayMenu = new QMenu(this);

    trayActionShow = new QAction(this);
    trayActionExit = new QAction(this);

    trayActionShow->setText("Restore");
    trayActionExit->setText("Exit");

    trayActionShow->setCheckable(false);
    trayActionExit->setCheckable(false);

    trayMenu->addAction(trayActionShow);
    trayMenu->addAction(trayActionExit);

    trayIcon->setContextMenu(trayMenu);
    trayIcon->setIcon(appIcon);

    settings = new QAction(this);
    settings->setText(tr("Settings"));
    about = new QAction(this);
    about->setText("About");
    openInBrowser = new QAction(this);
    openInBrowser->setText("Open target directory");
    exit = new QAction(this);
    exit->setText("Exit");

    clipboard = QApplication::clipboard();

    downloader = new Downloader(this);

    ui->tblDownloads->setColumnHidden(Next, true);
    ui->tblDownloads->setColumnHidden(Total, true);
    ui->tblDownloads->setColumnHidden(DownloadState, true);
    ui->tblDownloads->setSortingEnabled(false);

    ui->tblDownloads->setItemDelegate(new ProgressBarDelegate(Progress));
    ui->tblDownloads->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->tblDownloads->addAction(openInBrowser);

    this->menuBar()->addAction(settings);
    this->menuBar()->addAction(about);
    this->menuBar()->addAction(exit);

    connect(settings, SIGNAL(triggered()), this, SLOT(settingsMenu()));
    connect(about, SIGNAL(triggered()), this, SLOT(about_clicked()));
    connect(openInBrowser, SIGNAL(triggered()), this, SLOT(openTargetDirectory()));
    connect(exit, SIGNAL(triggered()), this, SLOT(closeApp()));
    connect(trayActionExit, SIGNAL(triggered()), this, SLOT(closeApp()));
    connect(trayActionShow, SIGNAL(triggered()), this, SLOT(showNormal()));

    connect(ui->btAdd, SIGNAL(clicked()), this, SLOT(addLinksMenu()));
    connect(ui->btDel, SIGNAL(clicked()), this, SLOT(btDel_clicked()));
    connect(ui->btStart, SIGNAL(clicked()), this, SLOT(btStart_clicked()));
    connect(ui->btPause, SIGNAL(clicked()), this, SLOT(btPause_clicked()));
    connect(ui->btStop, SIGNAL(clicked()), this, SLOT(btStop_clicked()));
    connect(ui->btMoveUp, SIGNAL(clicked()), this, SLOT(btMoveUp_clicked()));
    connect(ui->btMoveDown, SIGNAL(clicked()), this, SLOT(btMoveDown_clicked()));

    connect(this, SIGNAL(startNewDownload()), this, SLOT(btStart_clicked()), Qt::QueuedConnection);
    connect(this, SIGNAL(pauseDownload(QString)), downloader, SLOT(pauseDownload(QString)));
    connect(this, SIGNAL(stopDownload(QString)), downloader, SLOT(stopDownload(QString)));

    connect(clipboard, SIGNAL(dataChanged()), this, SLOT(clipboard_dataChanged()));
    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(showNormal()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
    connect(timer, SIGNAL(timeout()), this, SLOT(saveLinks()));

    loadSettings();
    loadLinks();

    this->setWindowTitle("RapidshareDownloader");
    this->setWindowIcon(appIcon);

    if (autostart)
        emit startNewDownload();

    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        if (startminimized)
            this->showMinimized();
        trayIcon->show();
    }

    return;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::trayActivated(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::DoubleClick) {
        this->showNormal();
        this->activateWindow();
        this->raise();
    }
}

void MainWindow::changeEvent(QEvent *event) {
    QWidget::changeEvent(event);
    if (event->type() == QEvent::WindowStateChange) {
        QWindowStateChangeEvent *e = (QWindowStateChangeEvent*)event;
        if ((e->oldState() != Qt::WindowMinimized) && isMinimized()) {
            QTimer::singleShot(0, this, SLOT(hide()));
            trayIcon->show();
            event->ignore();
        }
    }
}

void MainWindow::clipboard_dataChanged( void ) {
    //todo: improve
    //if (clipboard->text().contains(QRegExp("/^https?\\:\\/\\/www\\.rapidshare\\.com\\/files\\//i", Qt::CaseInsensitive)) || clipboard->text().contains(QRegExp("/^https?\\:\\/\\/rapidshare\\.com\\/files\\//i", Qt::CaseInsensitive))) {
    if (clipboard->text().split('/n').at(0).contains("rapidshare.com") || clipboard->text().split('/n').at(0).contains("safelinking.net")) {
        AddLinksPanel *panel = new AddLinksPanel(this);
        panel->getTeLinks()->setText(clipboard->text());
        if (panel->getTbPath()->text().isEmpty() || !fastmode) {
            Qt::WindowFlags flags = this->windowFlags();
            flags |= Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint;
            panel->setWindowFlags(flags);
            if (!panel->exec()) {
                panel->deleteLater();
                return;
            }
        }
        QStringList links = panel->getLinks()->toPlainText().split('\n');
        QString fpath = panel->getPath();
        for (int i = 0; i < links.length(); i++) {
            addLink(links.at(i).trimmed(), fpath);
        }
        //panel->accepted();
        panel->deleteLater();
        if (fastmode)
            emit startNewDownload();
        panel->deleteLater();
    }

    if (this->isMinimized()) {
        this->showNormal();
        this->showMinimized();
    }
}

void MainWindow::updateDownload(QString filename, QString size, QString progress, QString speed, QString eta, int state, QString next, QString total, QString errorString) {
    for (int i = 0; i < ui->tblDownloads->rowCount(); i++) {
        if (ui->tblDownloads->item(i, FileName)->text() == filename) {
            if (!size.isEmpty())
                ui->tblDownloads->item(i, FileSize)->setText(size);
            if (!progress.isEmpty())
                ui->tblDownloads->item(i, Progress)->setData(Qt::DisplayRole, progress);
            if (!speed.isEmpty())
                ui->tblDownloads->item(i, Speed)->setText(speed);
            if (!eta.isEmpty())
                ui->tblDownloads->item(i, ETA)->setText(eta);
            if (!next.isEmpty())
                ui->tblDownloads->item(i, Next)->setText(next);
            if (!total.isEmpty())
                ui->tblDownloads->item(i, Total)->setText(total);

            switch (state) {
                case WAITING:
                    ui->tblDownloads->item(i, DownloadState)->setText(QString::number(WAITING));
                    ui->tblDownloads->item(i, Status)->setText(tr("WAITING"));
                    break;
                case QUEUING:
                    ui->tblDownloads->item(i, DownloadState)->setText(QString::number(QUEUING));
                    ui->tblDownloads->item(i, Status)->setText(tr("QUEUING"));
                    break;
                case DOWNLOADING:
                    ui->tblDownloads->item(i, DownloadState)->setText(QString::number(DOWNLOADING));
                    ui->tblDownloads->item(i, Status)->setText(tr("DOWNLOADING"));
                    break;
                case COMPLETED:
                    ui->tblDownloads->item(i, DownloadState)->setText(QString::number(COMPLETED));
                    ui->tblDownloads->item(i, Status)->setText(tr("COMPLETED"));
                    decrementActive();
                    if (trayIcon->isVisible()) {
                        if (active && i != ui->tblDownloads->rowCount() - 1 )
                            trayIcon->showMessage("A download has completed!", ui->tblDownloads->item(i, FileName)->text() + QString(" has finished downloading."), QSystemTrayIcon::Information, 1000);
                        else
                            trayIcon->showMessage("All downloads have finished!", ui->tblDownloads->item(i, FileName)->text() + QString(" has finished downloading."), QSystemTrayIcon::Information, 1000);
                    }
                    emit startNewDownload();
                    break;
                case PAUSED:
                    ui->tblDownloads->item(i, DownloadState)->setText(QString::number(PAUSED));
                    ui->tblDownloads->item(i, Status)->setText(tr("PAUSED"));
                    decrementActive();
                    break;
                case CANCELLED:
                    ui->tblDownloads->item(i, DownloadState)->setText(QString::number(CANCELLED));
                    ui->tblDownloads->item(i, Status)->setText(tr("CANCELLED"));
                    decrementActive();
                    break;
                case ERROR:
                    ui->tblDownloads->item(i, DownloadState)->setText(QString::number(ERROR));
                    ui->tblDownloads->item(i, Status)->setText(errorString);
                    if (trayIcon->isVisible())
                        trayIcon->showMessage("A download has encountered an error!", ui->tblDownloads->item(i, Status)->text(), QSystemTrayIcon::Information, 1000);
                    decrementActive();
                    emit startNewDownload();
                    break;
                default:
                    break;
            }
            return;
        }
    }
}

void MainWindow::btDel_clicked( void ) {
    QModelIndexList list = ui->tblDownloads->selectionModel()->selectedRows();
    sortList(list);
    int selected = list.size();
    if (selected) {
        for (int i = 0; i < selected; i++) {
            int state = downloadState(ui->tblDownloads->item(list.at(i).row(), FileName)->text());
            if (state == DOWNLOADING || state == QUEUING) {
                emit stopDownload(ui->tblDownloads->item(list.at(i).row(), FileName)->text());
                ui->tblDownloads->removeRow(list.at(i).row());
            } else
                ui->tblDownloads->removeRow(list.at(i).row());
        }
    }
}

void MainWindow::btStart_clicked( void ) {
    if (active >= concd)
        return;

    if (sender() != this) {
        QModelIndexList list = ui->tblDownloads->selectionModel()->selectedRows();
        sortList(list);
        if (list.size()) {
            for (int i = 0; active < concd && i < list.size(); i++) {
                int state = downloadState(ui->tblDownloads->item(list.at(i).row(), FileName)->text());
                if (state == WAITING || state == PAUSED || state == CANCELLED) {
                    if (downloader->download(ui->tblDownloads->item(list.at(i).row(), FileName)->text(), ui->tblDownloads->item(list.at(i).row(), Path)->text())) {
                        updateDownload(ui->tblDownloads->item(list.at(i).row(), FileName)->text(), "", "", "", "", QUEUING, "", "", "");
                        active++;
                    }
                }
            }
            return;
        }
    }

    for (int i = 0; active < concd && i < ui->tblDownloads->rowCount(); i++) {
        int state = downloadState(ui->tblDownloads->item(i, FileName)->text());
        if (state == WAITING || state == PAUSED || state == CANCELLED) {
            if (downloader->download(ui->tblDownloads->item(i, FileName)->text(), ui->tblDownloads->item(i, Path)->text())) {
                updateDownload(ui->tblDownloads->item(i, FileName)->text(), "", "", "", "", QUEUING, "", "", "");
                active++;
            }
        }
    }
}

void MainWindow::btPause_clicked( void ) {
    QModelIndexList list = ui->tblDownloads->selectionModel()->selectedRows();
    sortList(list);
    int selected = list.size();
    if (selected) {
        for (int i = 0; i < selected; i++) {
            int state = downloadState(ui->tblDownloads->item(list.at(i).row(), FileName)->text());
            if (state == DOWNLOADING)
                emit pauseDownload(ui->tblDownloads->item(list.at(i).row(), FileName)->text());
        }
    }
}

void MainWindow::btStop_clicked( void ) {
    QModelIndexList list = ui->tblDownloads->selectionModel()->selectedRows();
    sortList(list);
    int selected = list.size();
    if (selected) {
        for (int i = 0; i < selected; i++) {
            int state = downloadState(ui->tblDownloads->item(list.at(i).row(), FileName)->text());
            if (state == DOWNLOADING)
                emit stopDownload(ui->tblDownloads->item(list.at(i).row(), FileName)->text());
            else
                updateDownload(ui->tblDownloads->item(list.at(i).row(), FileName)->text(), "", "", "", "", CANCELLED, "", "", "");
        }
    }
}

void MainWindow::btMoveUp_clicked( void ) {
    QModelIndexList list = ui->tblDownloads->selectionModel()->selectedRows();
    sortList(list, false);
    int selected = list.size();
    if (selected) {
        QTableWidgetItem *temporary = NULL;
        int rowFrom = 0;
        int rowTo   = 0;
        for (int i = 0; i < selected; i++) {
            rowFrom = list.at(i).row();
            rowTo = rowFrom - 1;
            if (rowTo >= 0) {
                for (int i = 0; i < ui->tblDownloads->columnCount(); i++) {
                    temporary = ui->tblDownloads->takeItem(rowFrom, i);
                    ui->tblDownloads->setItem(rowFrom, i, ui->tblDownloads->takeItem(rowTo, i));
                    ui->tblDownloads->setItem(rowTo, i, temporary);
                }
            }
        }

        ui->tblDownloads->selectRow(rowTo);
    }
}

void MainWindow::btMoveDown_clicked( void ) {
    QModelIndexList list = ui->tblDownloads->selectionModel()->selectedRows();
    sortList(list);
    int selected = list.size();
    if (selected) {
        QTableWidgetItem *temporary = NULL;
        int rowFrom = 0;
        int rowTo   = 0;
        for (int i = 0; i < selected; i++) {
            rowFrom = list.at(i).row();
            rowTo = rowFrom + 1;
            if (rowTo <= ui->tblDownloads->rowCount() - 1) {
                for (int i = 0; i < ui->tblDownloads->columnCount(); i++) {
                    temporary = ui->tblDownloads->takeItem(rowFrom, i);
                    ui->tblDownloads->setItem(rowFrom, i, ui->tblDownloads->takeItem(rowTo, i));
                    ui->tblDownloads->setItem(rowTo, i, temporary);
                }
            }
        }
        ui->tblDownloads->selectRow(rowTo);
    }
}

void MainWindow::addLinksMenu( void ) {
    AddLinksPanel *panel = new AddLinksPanel(this);
    if (panel->exec()) {
        QStringList links = panel->getLinks()->toPlainText().split('\n');
        QString fpath = panel->getPath();
        for (int i = 0; i < links.length(); i++) {
            if (links.at(i).contains("rapidshare.com") || links.at(i).contains("safelinking.net"))
                addLink(links.at(i).trimmed(), fpath);
        }
    }
    panel->deleteLater();
}

void MainWindow::addLink( const QString &link, const QString &saveAs, const int stateValue, const int nextText, const int totalText ) {
    if (downloadState(link) != -1)
        return;

    int nextrow = ui->tblDownloads->rowCount();
    ui->tblDownloads->insertRow(nextrow);
    QTableWidgetItem *name = new QTableWidgetItem();
    QTableWidgetItem *path = new QTableWidgetItem();
    QTableWidgetItem *status = new QTableWidgetItem();
    QTableWidgetItem *size = new QTableWidgetItem();
    QTableWidgetItem *progress = new QTableWidgetItem();
    QTableWidgetItem *speed = new QTableWidgetItem();
    QTableWidgetItem *eta = new QTableWidgetItem();
    QTableWidgetItem *next = new QTableWidgetItem();
    QTableWidgetItem *total = new QTableWidgetItem();
    QTableWidgetItem *state = new QTableWidgetItem();

    name->setText(link);
    path->setText(saveAs);
    size->setText("");
    speed->setText("");
    eta->setText("");

    switch (stateValue) {
        case WAITING:
            state->setText(QString::number(WAITING));
            status->setText(tr("WAITING"));
            break;
        case QUEUING:
            state->setText(QString::number(QUEUING));
            status->setText(tr("QUEUING"));
            break;
        case DOWNLOADING:
            state->setText(QString::number(DOWNLOADING));
            status->setText(tr("DOWNLOADING"));
            break;
        case COMPLETED:
            state->setText(QString::number(COMPLETED));
            status->setText(tr("COMPLETED"));
            progress->setData(Qt::DisplayRole, "100");
            break;
        case PAUSED:
            state->setText(QString::number(PAUSED));
            status->setText(tr("PAUSED"));
            break;
        case CANCELLED:
            state->setText(QString::number(CANCELLED));
            status->setText(tr("CANCELLED"));
            break;
        default:
            state->setText(QString::number(WAITING));
            status->setText(tr("WAITING"));
            break;
    }

    if (nextText)
        next->setText(QString::number(nextText));
    else
        next->setText("");

    if (totalText)
        total->setText(QString::number(totalText));
    else
        total->setText("");

    name->setTextAlignment(Qt::AlignCenter);
    path->setTextAlignment(Qt::AlignCenter);
    status->setTextAlignment(Qt::AlignCenter);
    size->setTextAlignment(Qt::AlignCenter);
    progress->setTextAlignment(Qt::AlignCenter);
    speed->setTextAlignment(Qt::AlignCenter);
    eta->setTextAlignment(Qt::AlignCenter);
    next->setTextAlignment(Qt::AlignCenter);
    total->setTextAlignment(Qt::AlignCenter);
    state->setTextAlignment(Qt::AlignCenter);

    ui->tblDownloads->setItem(nextrow, FileName, name);
    ui->tblDownloads->setItem(nextrow, Path, path);
    ui->tblDownloads->setItem(nextrow, Progress, progress);
    ui->tblDownloads->setItem(nextrow, FileSize, size);
    ui->tblDownloads->setItem(nextrow, Speed, speed);
    ui->tblDownloads->setItem(nextrow, ETA, eta);
    ui->tblDownloads->setItem(nextrow, Status, status);
    ui->tblDownloads->setItem(nextrow, Next, next);
    ui->tblDownloads->setItem(nextrow, Total, total);
    ui->tblDownloads->setItem(nextrow, DownloadState, state);
}

void MainWindow::settingsMenu( void ) {
    SettingsPanel *panel = new SettingsPanel(this);
    if (panel->exec())
        loadSettings();
    panel->deleteLater();
}

void MainWindow::saveLinks( void ) {
    QSettings settings("NoOrganization", "RapidshareDownloader");
    settings.beginGroup("Links");
    settings.remove("");
    for (int i = 0; i < ui->tblDownloads->rowCount(); i++) {
        settings.beginGroup(QString("Link #%1").arg(i));
        settings.setValue("link", ui->tblDownloads->item(i, FileName)->text());
        settings.setValue("state", ui->tblDownloads->item(i, DownloadState)->text());
        settings.setValue("path", ui->tblDownloads->item(i, Path)->text());
        settings.setValue("next", ui->tblDownloads->item(i, Next)->text());
        settings.setValue("total", ui->tblDownloads->item(i, Total)->text());
        settings.endGroup();
    }
    settings.endGroup();
}

void MainWindow::loadLinks( void ) {
    ui->tblDownloads->clearContents();
    ui->tblDownloads->setRowCount(0);
    QSettings settings("NoOrganization", "RapidshareDownloader");
    settings.beginGroup("Links");
    for (int i = 0;; i++) {
        settings.beginGroup(QString("Link #%1").arg(i));
        if (settings.value("link").toString() == "") {
            settings.endGroup();
            break;
        }
        addLink(settings.value("link").toString(), settings.value("path").toString(), settings.value("state").toInt(), settings.value("next").toInt(), settings.value("total").toInt());
        settings.endGroup();
    }
    settings.endGroup();
}

void MainWindow::loadSettings( void ) {
    QSettings settings("NoOrganization", "RapidshareDownloader");
    settings.beginGroup("Settings");
    concd = settings.value("concd").toInt();
    fastmode = settings.value("fastmode").toBool();
    autostart = settings.value("autostart").toBool();
    startminimized = settings.value("startminimized").toBool();
    settings.endGroup();

    if (active > 0 && active < concd)
        emit startNewDownload();
    return;
}

void MainWindow::resizeEvent(QResizeEvent *e) {
    QMainWindow::resizeEvent(e);
    if (!(e->size().height() < 0)) {
        ui->layoutActions->setGeometry(QRect(0, 0, ui->layoutActions->geometry().width(), ui->layoutActions->geometry().height()));
        ui->tblDownloads->setGeometry(QRect(0, ui->layoutActions->sizeHint().height() + 10, width(), (height() - ui->layoutActions->sizeHint().height()) - 62));
    }
    return;
}

void MainWindow::sortList(QModelIndexList &list, bool ascending) {
    for (int i = 0; i < list.size(); i++) {
        for (int j = 0; j < list.size() - 1; j++) {
            if (ascending) {
                if (list.at(j).row() < list.at(j + 1).row())
                    list.move(j, j + 1);
            } else {
                if (list.at(j).row() > list.at(j + 1).row())
                    list.move(j, j + 1);
            }
        }
    }
}

int MainWindow::downloadState(const QString &link) {
    for (int i = 0; i < ui->tblDownloads->rowCount(); i++) {
        if (ui->tblDownloads->item(i, FileName)->text() == link)
            return ui->tblDownloads->item(i, DownloadState)->text().toInt();
    }
    return -1;
}

int MainWindow::downloadLast(const QString &link) {
    for (int i = 0; i < ui->tblDownloads->rowCount(); i++) {
        if (ui->tblDownloads->item(i, FileName)->text() == link) {
            return ui->tblDownloads->item(i, Next)->text().toInt();
        }
    }
    return 0;
}

int MainWindow::downloadTotal(const QString &link) {
    for (int i = 0; i < ui->tblDownloads->rowCount(); i++) {
        if (ui->tblDownloads->item(i, FileName)->text() == link) {
            return ui->tblDownloads->item(i, Total)->text().toInt();
        }
    }
    return 0;
}

void MainWindow::about_clicked( void ) {
    AboutBox *box = new AboutBox(this);
    box->exec();
    box->deleteLater();
}

void MainWindow::openTargetDirectory( void ) {
    QModelIndexList list = ui->tblDownloads->selectionModel()->selectedRows();
    sortList(list);
    if (list.size()) {
        for (int i = 0; i < list.size(); i++) {
            QString path = QDir::toNativeSeparators(ui->tblDownloads->item(list.at(i).row(), Path)->text());
            QDesktopServices::openUrl(QUrl("file:///" + path));
        }
        return;
    }
}

void MainWindow::aboutToQuit( void ) {
    for (int i = 0; i < ui->tblDownloads->rowCount(); i++) {
        if (downloadState(ui->tblDownloads->item(i, FileName)->text()) == DOWNLOADING)
            pauseDownload(ui->tblDownloads->item(i, FileName)->text());
    }
    saveLinks();
}

void MainWindow::closeApp( void ) {
    qApp->quit();
}

void MainWindow::decrementActive( void ) {
    if (active - 1 >= 0)
        active--;
}
