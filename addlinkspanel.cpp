#include "addlinkspanel.h"

AddLinksPanel::AddLinksPanel(QWidget *parent) :
    QDialog(parent)
{
    lbAddLinks = new QLabel(this);
    lbSaveTo = new QLabel(this);

    teLinks = new QTextEdit(this);
    tbPath = new QLineEdit(this);

    btFind = new QPushButton(this);
    btAdd = new QPushButton(this);
    btCancel = new QPushButton(this);

    layoutHSave = new QBoxLayout(QBoxLayout::LeftToRight);
    layoutHButtons = new QBoxLayout(QBoxLayout::LeftToRight);
    layoutVAddLinks = new QBoxLayout(QBoxLayout::Down);

    lbAddLinks->setText(tr("Add Links"));
    lbSaveTo->setText(tr("Save To"));

    btFind->setText(tr("Find"));
    btAdd->setText(tr("Add"));
    btCancel->setText(tr("Cancel"));

    teLinks->setWordWrapMode(QTextOption::NoWrap);

    layoutHSave->addWidget(lbSaveTo);
    layoutHSave->addWidget(tbPath, 1);
    layoutHSave->addWidget(btFind);

    layoutHButtons->addWidget(btAdd);
    layoutHButtons->addWidget(btCancel);

    layoutVAddLinks->addWidget(lbAddLinks, 0, Qt::AlignHCenter);
    layoutVAddLinks->addWidget(teLinks);
    layoutVAddLinks->addLayout(layoutHSave);
    layoutVAddLinks->addLayout(layoutHButtons);

    this->setLayout(layoutVAddLinks);

    btAdd->setEnabled(false);

    connect(teLinks, SIGNAL(textChanged()), this, SLOT(resizeToContents()));
    connect(teLinks, SIGNAL(textChanged()), this, SLOT(suggestPath()));
    connect(btFind, SIGNAL(clicked()), this, SLOT(findPath()));
    connect(btAdd, SIGNAL(clicked()), this, SLOT(accept()));
    connect(btCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(tbPath, SIGNAL(textChanged(QString)), this, SLOT(tbPath_textChanged(QString)));
}

void AddLinksPanel::resizeToContents( void ) {
    int resizeFactor = teLinks->document()->size().width() + teLinks->geometry().left() + 5;
    this->resize(resizeFactor, this->height());
}

void AddLinksPanel::tbPath_textChanged( QString path ) {
    QDir dir(path);
    if (path.isEmpty())
        return;
    btAdd->setEnabled(dir.exists() ? true : false);
}

void AddLinksPanel::findPath( void ) {
    tbPath->setText(QFileDialog::getExistingDirectory(this, tr("Locate destination directory"), "/"));
}

QString AddLinksPanel::getPath( void ) {
    return tbPath->text();
}

QTextDocument *AddLinksPanel::getLinks( void ) {
    return teLinks->document();
}

void AddLinksPanel::suggestPath( void ) {
    QSettings settings("NoOrganization", "RapidshareDownloader");
    settings.beginGroup("Settings");
    for (int currentPref = 0;; currentPref++) {
        settings.beginGroup(QString("Preference#%1").arg(currentPref));
        if (settings.value("1").isNull()) {
            settings.endGroup();
            break;
        }

        QStringList list;
        list << teLinks->document()->toPlainText().split('\n').at(0).split('/');

        if (settings.value("1").toString() == "Begins With") {
            QString temp = list.last();
            temp.truncate(settings.value("2").toString().length());
            if (temp == settings.value("2").toString())
                tbPath->setText(settings.value("3").toString());
        } else if (settings.value("1").toString() == "Contains") {
            if (list.last().contains(settings.value("2").toString()))
                tbPath->setText(settings.value("3").toString());
        }
        settings.endGroup();
    }
}

