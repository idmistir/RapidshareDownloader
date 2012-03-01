#include "settingspanel.h"

SettingsPanel::SettingsPanel(QWidget *parent) :
    QDialog(parent)
{
    lbSettings = new QLabel(this);
    lbRsHeader = new QLabel(this);
    lbRsUser = new QLabel(this);
    lbRsPass = new QLabel(this);
    lbRsChecked = new QLabel(this);
    lbConcDownloads = new QLabel(this);

    tbRsUser = new QLineEdit(this);
    tbRsPass = new QLineEdit(this);
    tbConcDownloads = new QLineEdit(this);

    btOk = new QPushButton(this);
    btCancel = new QPushButton(this);
    btRsCheckAccount = new QPushButton(this);

    hSpacerMid = new QSpacerItem(40, 20, QSizePolicy::Expanding);
    hSpacerRight = new QSpacerItem(40, 20, QSizePolicy::Expanding);
    hSpacerBot = new QSpacerItem(40, 20, QSizePolicy::Expanding);

    layoutRs = new QGridLayout();
    layoutSettings = new QBoxLayout(QBoxLayout::Down);
    layoutVTable = new QBoxLayout(QBoxLayout::Down);
    layoutHButtons = new QBoxLayout(QBoxLayout::LeftToRight);

    tblN2P = new QTableWidget(this);

    lbSettings->setText(tr("Settings"));
    lbRsHeader->setText(tr("Rapidshare Premium"));
    lbRsUser->setText(tr("Username"));
    lbRsPass->setText(tr("Password"));
    lbRsChecked->setText(tr("Not Checked"));
    lbConcDownloads->setText(tr("Concurrent Downloads"));

    lbRsHeader->setAlignment(Qt::AlignHCenter);
    lbRsUser->setAlignment(Qt::AlignHCenter);
    lbRsPass->setAlignment(Qt::AlignHCenter);
    lbRsChecked->setAlignment(Qt::AlignHCenter);

    btOk->setText(tr("OK"));
    btCancel->setText(tr("Cancel"));
    btRsCheckAccount->setText(tr("Check Account"));

    layoutRs->addWidget(lbRsHeader, 0, 0);
    layoutRs->addWidget(lbRsUser, 1, 0);
    layoutRs->addWidget(lbRsPass, 2, 0);
    layoutRs->addWidget(lbRsChecked, 3, 0);
    layoutRs->addWidget(tbRsUser, 1, 1);
    layoutRs->addWidget(tbRsPass, 2, 1);
    layoutRs->addWidget(btRsCheckAccount, 3, 1);
    layoutRs->addWidget(hSpacerMid->widget(), 1, 3);
    layoutRs->addWidget(lbConcDownloads, 1, 4);
    layoutRs->addWidget(tbConcDownloads, 1, 5);
    layoutRs->addWidget(hSpacerRight->widget(), 1, 6);

    layoutHButtons->addWidget(hSpacerBot->widget(), 1);
    layoutHButtons->addWidget(btOk);
    layoutHButtons->addWidget(btCancel);

    layoutSettings->addWidget(lbSettings, 1, Qt::AlignHCenter);
    layoutSettings->addLayout(layoutRs);
    layoutSettings->addWidget(tblN2P);
    layoutSettings->addLayout(layoutHButtons);

    this->setLayout(layoutSettings);
    this->layout()->setSizeConstraint(QLayout::SetFixedSize);
    return;
}
