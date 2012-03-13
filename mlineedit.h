#ifndef MLINEEDIT_H
#define MLINEEDIT_H

#include <QLineEdit>
#include <QFocusEvent>

class MLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit MLineEdit(QWidget *parent = 0);
    
signals:
    void focused( void );

private:
    virtual void focusInEvent(QFocusEvent *);
};

#endif // MLINEEDIT_H
