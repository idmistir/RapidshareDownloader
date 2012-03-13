#ifndef ADDLINKSPANEL_H
#define ADDLINKSPANEL_H

#include <QDialog>
#include <QBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QFileDialog>
#include <QSettings>
#include <QDir>
#include <QComboBox>

#include "mlineedit.h"

class AddLinksPanel : public QDialog
{
    Q_OBJECT
public:
    explicit AddLinksPanel(QWidget *parent = 0);
    
    QTextDocument *getLinks( void );
    QString       getPath( void );

    QTextEdit *getTeLinks( void );
    QLineEdit *getTbPath( void );

private slots:
    void resizeToContents( void );
    void suggestPath( void );
    void findPath( void );
    void tbPath_textChanged( QString path );
    void tbNewRulePath_focused( void );
    void accepted();

private:
    QLabel *lbAddLinks, *lbSaveTo, *lbNewRule, *lb;
    QTextEdit *teLinks;
    QLineEdit *tbPath, *tbNewRuleName;
    MLineEdit *tbNewRulePath;
    QPushButton *btFind, *btAdd, *btCancel;
    QBoxLayout *layoutHSave, *layoutHButtons, *layoutVAddLinks, *layoutHNewRuleLb, *layoutHNewRule;
    QComboBox *cmbNewRuleType;
};

#endif // ADDLINKSPANEL_H
