#include "mlineedit.h"

MLineEdit::MLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
}

void MLineEdit::focusInEvent(QFocusEvent *event) {
    QLineEdit::focusInEvent(event);
    emit focused();
}
