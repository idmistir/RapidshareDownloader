#ifndef SETTINGSPANEL_H
#define SETTINGSPANEL_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QBoxLayout>
#include <QGridLayout>
#include <QTableWidget>
#include <QTableWidgetItem>

class SettingsPanel : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsPanel(QWidget *parent = 0);
    
signals:
    
public slots:

private:
    QLabel *lbSettings, *lbRsHeader, *lbRsUser, *lbRsPass, *lbRsChecked, *lbConcDownloads,
           *lbNameToPath, *lbLinkContains, *lbSuggestedPath;
    QLineEdit *tbRsUser, *tbRsPass, *tbConcDownloads;
    QPushButton *btOk, *btCancel, *btRsCheckAccount;
    QSpacerItem *hSpacerMid, *hSpacerRight, *hSpacerBot;
    QGridLayout *layoutRs;
    QBoxLayout *layoutSettings, *layoutVTable, *layoutHButtons;

    QTableWidget *tblN2P;
};

#endif // SETTINGSPANEL_H
