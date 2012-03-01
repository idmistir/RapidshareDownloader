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

class AddLinksPanel : public QDialog
{
    Q_OBJECT
public:
    explicit AddLinksPanel(QWidget *parent = 0);
    
    QTextDocument *getLinks( void );
    QString       getPath( void );
private slots:
    void resizeToContents( void );
    void suggestPath( void );
    void findPath( void );
    void tbPath_textChanged( QString path );

private:
    QLabel *lbAddLinks, *lbSaveTo;
    QTextEdit *teLinks;
    QLineEdit *tbPath;
    QPushButton *btFind, *btAdd, *btCancel;
    QBoxLayout *layoutHSave, *layoutHButtons, *layoutVAddLinks;
};

#endif // ADDLINKSPANEL_H
