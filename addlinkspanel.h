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

class AddLinksPanel : public QDialog
{
    Q_OBJECT
public:
    explicit AddLinksPanel(QWidget *parent = 0);
    
private slots:
    void resizeToContents( void );
    void suggestPath( void );
    void findPath( void );

private:
    QLabel *lbAddLinks, *lbSaveTo;
    QTextEdit *teLinks;
    QLineEdit *tbPath;
    QPushButton *btFind, *btAdd, *btCancel;
    QBoxLayout *layoutHSave, *layoutHButtons, *layoutVAddLinks;
};

#endif // ADDLINKSPANEL_H
