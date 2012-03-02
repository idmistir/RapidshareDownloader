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
#include <QSettings>
#include <QFontComboBox>
#include <QFileDialog>
#include <QAction>
#include <QCheckBox>
#include "downloader.h"
#include "fontcomboboxdelegate.h"

class SettingsPanel : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsPanel(QWidget *parent = 0);
    
signals:
    
public slots:

private slots:
    void saveSettings( void );
    void loadSettings( void );
    void findPathForCell( int, int );
    void insertRow( void );
    void removeRow( void );
    void checkPremium( void );

private:
    QLabel *lbSettings, *lbRsHeader, *lbRsUser, *lbRsPass, *lbRsChecked, *lbConcDownloads,
           *lbNameToPath, *lbLinkContains, *lbSuggestedPath, *lbFastMode, *lbAutoStart;
    QLineEdit *tbRsUser, *tbRsPass, *tbConcDownloads;
    QPushButton *btOk, *btCancel, *btRsCheckAccount;
    QGridLayout *layoutRs;
    QBoxLayout *layoutSettings, *layoutHButtons;
    QTableWidget *tblN2P;
    QAction *addRow, *delRow;
    QCheckBox *cbFastMode, *cbAutoStart;

    enum COLUMNS {Order = 0, Tag, Path};
};

#endif // SETTINGSPANEL_H
