#ifndef ABOUTBOX_H
#define ABOUTBOX_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QBoxLayout>
#include <QPushButton>
#include <QApplication>

class AboutBox : public QDialog
{
    Q_OBJECT
public:
    explicit AboutBox(QWidget *parent = 0);

private slots:
    void aboutQt( void );

private:
    QLabel *lbText;
    QBoxLayout *layout;
    QPushButton *btAboutQt, *btOk;

};

#endif // ABOUTBOX_H
