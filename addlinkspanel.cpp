#include "addlinkspanel.h"

AddLinksPanel::AddLinksPanel(QWidget *parent) :
    QDialog(parent)
{
    lbAddLinks = new QLabel(this);
    lbSaveTo = new QLabel(this);
    lbNewRule = new QLabel(this);

    teLinks = new QTextEdit(this);
    tbPath = new QLineEdit(this);
    tbNewRuleName = new QLineEdit(this);
    tbNewRulePath = new MLineEdit(this);

    btFind = new QPushButton(this);
    btAdd = new QPushButton(this);
    btCancel = new QPushButton(this);

    cmbNewRuleType = new QComboBox(this);

    layoutHSave = new QBoxLayout(QBoxLayout::LeftToRight);
    layoutHButtons = new QBoxLayout(QBoxLayout::LeftToRight);
    layoutVAddLinks = new QBoxLayout(QBoxLayout::Down);
    layoutHNewRuleLb = new QBoxLayout(QBoxLayout::LeftToRight);
    layoutHNewRule = new QBoxLayout(QBoxLayout::LeftToRight);

    lbAddLinks->setText(tr("Add Links"));
    lbSaveTo->setText(tr("Save To"));
    lbNewRule->setText(tr("New Rule:"));

    btFind->setText(tr("Find"));
    btAdd->setText(tr("Add"));
    btCancel->setText(tr("Cancel"));

    cmbNewRuleType->addItem(tr("Begins With"));
    cmbNewRuleType->addItem(tr("Contains"));

    teLinks->setWordWrapMode(QTextOption::NoWrap);

    tbNewRuleName->setPlaceholderText("Data");
    tbNewRulePath->setPlaceholderText("Destination Path");
    tbNewRulePath->setToolTip("Focus in for Find dialog");

    layoutHSave->addWidget(lbSaveTo);
    layoutHSave->addWidget(tbPath, 1);
    layoutHSave->addWidget(btFind);

    layoutHButtons->addWidget(btAdd);
    layoutHButtons->addWidget(btCancel);

    layoutHNewRuleLb->addWidget(lbNewRule, 1, Qt::AlignHCenter);
    layoutHNewRule->addWidget(cmbNewRuleType);
    layoutHNewRule->addWidget(tbNewRuleName);
    layoutHNewRule->addWidget(tbNewRulePath);

    layoutVAddLinks->addWidget(lbAddLinks, 0, Qt::AlignHCenter);
    layoutVAddLinks->addWidget(teLinks);
    layoutVAddLinks->addLayout(layoutHSave);
    layoutVAddLinks->addLayout(layoutHNewRuleLb);
    layoutVAddLinks->addLayout(layoutHNewRule);
    layoutVAddLinks->addLayout(layoutHButtons);

    this->setLayout(layoutVAddLinks);

    btAdd->setEnabled(false);

    connect(teLinks, SIGNAL(textChanged()), this, SLOT(resizeToContents()));
    connect(teLinks, SIGNAL(textChanged()), this, SLOT(suggestPath()));
    connect(btFind, SIGNAL(clicked()), this, SLOT(findPath()));
    connect(btAdd, SIGNAL(clicked()), this, SLOT(accepted()));
    connect(btCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(tbPath, SIGNAL(textChanged(QString)), this, SLOT(tbPath_textChanged(QString)));
    connect(tbNewRulePath, SIGNAL(focused()), this, SLOT(tbNewRulePath_focused()));

    this->setWindowTitle("Add Links");
    this->setTabOrder(teLinks, tbPath);
    this->setTabOrder(tbPath, btFind);
    this->setTabOrder(btFind, cmbNewRuleType);
    this->setTabOrder(cmbNewRuleType, tbNewRuleName);
    this->setTabOrder(tbNewRuleName, tbNewRulePath);
    this->setTabOrder(tbNewRulePath, btAdd);
    this->setTabOrder(btAdd, btCancel);
}

void AddLinksPanel::accepted( void ) {
    if (!tbNewRuleName->text().isEmpty() && !tbNewRulePath->text().isEmpty()) {
        QSettings settings("NoOrganization", "RapidshareDownloader");
        settings.beginGroup("Settings");
        for (int i = 0;; i++) {
            settings.beginGroup(QString("Preference#%1").arg(i));
            if (settings.value("1").toString().isEmpty()) {
                settings.remove("");
                settings.setValue("1", cmbNewRuleType->itemText(cmbNewRuleType->currentIndex()));
                settings.setValue("2", tbNewRuleName->text());
                settings.setValue("3", tbNewRulePath->text());
                break;
            }
            settings.endGroup();
        }
        settings.endGroup();
    }
    accept();
}

void AddLinksPanel::tbNewRulePath_focused( void ) {
    tbNewRulePath->setText(QFileDialog::getExistingDirectory(this, tr("Locate destination directory"), "/"));
    tbPath->setText(tbNewRulePath->text());
}

QTextEdit* AddLinksPanel::getTeLinks( void ) {
    return teLinks;
}

QLineEdit* AddLinksPanel::getTbPath( void ) {
    return tbPath;
}

void AddLinksPanel::resizeToContents( void ) {
    int resizeFactor = teLinks->document()->size().width() + teLinks->geometry().left() + 5;
    if (resizeFactor < this->width()) {
        if (resizeFactor < this->sizeHint().width())
            return;
        else
            resizeFactor = this->sizeHint().width();
    }
    this->resize(resizeFactor, this->height());
}

void AddLinksPanel::tbPath_textChanged( QString path ) {
    tbNewRulePath->setText(path);
    QDir dir(path);
    if (path.isEmpty()) {
        btAdd->setEnabled(false);
        return;
    }
    btAdd->setEnabled(dir.exists() && !teLinks->document()->isEmpty() ? true : false);
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
    tbPath_textChanged(tbPath->text());
    QSettings settings("NoOrganization", "RapidshareDownloader");
    settings.beginGroup("Settings");
    for (int currentPref = 0;; currentPref++) {
        settings.beginGroup(QString("Preference #%1").arg(currentPref));
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
    if (!tbPath->text().isEmpty()) {
        lbNewRule->setVisible(false);
        tbNewRuleName->setVisible(false);
        tbNewRulePath->setVisible(false);
        cmbNewRuleType->setVisible(false);
    }
}

