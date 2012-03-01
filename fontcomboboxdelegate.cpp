#include "fontcomboboxdelegate.h"

ComboBoxDelegate::ComboBoxDelegate(int choiceColumn, QObject *parent) :
    QItemDelegate(parent)
{
    this->choiceColumn = choiceColumn;
}

void ComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (index.column() == choiceColumn) {
        QString text = index.model()->data(index, Qt::DisplayRole).toString();
        QStyleOptionViewItem myOption = option;
        myOption.displayAlignment = Qt::AlignHCenter | Qt::AlignVCenter;

        drawDisplay(painter, myOption, myOption.rect, text);
        drawFocus(painter, myOption, myOption.rect);
    } else {
        QItemDelegate::paint(painter, option, index);
    }
}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (index.column() == choiceColumn) {
        QComboBox *cmbBox = new QComboBox(parent);
        cmbBox->addItem(tr("Begins With"));
        cmbBox->addItem(tr("Contains"));
        return cmbBox;
    } else {
        return QItemDelegate::createEditor(parent, option, index);
    }
}

void ComboBoxDelegate::commitAndCloseEditor( void ) {
    QComboBox *editor = qobject_cast<QComboBox *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    if (index.column() == choiceColumn) {
        int item = index.model()->data(index, Qt::DisplayRole).toInt();
        QComboBox *cmbBox = qobject_cast<QComboBox *>(editor);
        cmbBox->setCurrentIndex(item);
    } else {
        QItemDelegate::setEditorData(editor, index);
    }
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    if (index.column() == choiceColumn) {
        QComboBox *cmbBox = qobject_cast<QComboBox *>(editor);
        model->setData(index, cmbBox->currentText());
    } else {
        QItemDelegate::setModelData(editor, model, index);
    }
}
