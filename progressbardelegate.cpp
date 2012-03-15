#include "progressbardelegate.h"

ProgressBarDelegate::ProgressBarDelegate(int choiceColumn, QObject *parent) :
    QItemDelegate(parent)
{
    this->choiceColumn = choiceColumn;
}

void ProgressBarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (index.column() == choiceColumn) {
        int progress = index.model()->data(index, Qt::DisplayRole).toInt();
        QStyleOptionProgressBarV2 progressBarOption;

        progressBarOption.state = QStyle::State_Enabled;
        progressBarOption.direction = QApplication::layoutDirection();
        progressBarOption.rect = option.rect;
        progressBarOption.fontMetrics = QApplication::fontMetrics();
        progressBarOption.type = QStyleOption::SO_ProgressBar;
        progressBarOption.version = 2;
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.textVisible = false;
        progressBarOption.progress = progress < 0 ? 0 : progress;
        progressBarOption.text = "";

        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter, 0);

        /*drawDisplay(painter, myOption, myOption.rect, text);
        drawFocus(painter, myOption, myOption.rect);*/
    } else {
        QItemDelegate::paint(painter, option, index);
    }
}

QWidget *ProgressBarDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (index.column() == choiceColumn) {
        QProgressBar *progressBar = new QProgressBar(parent);
        progressBar->setValue(0);
        progressBar->setTextVisible(false);
        return progressBar;
    } else {
        return QItemDelegate::createEditor(parent, option, index);
    }
}

void ProgressBarDelegate::commitAndCloseEditor( void ) {
    QProgressBar *editor = qobject_cast<QProgressBar *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

void ProgressBarDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    if (index.column() == choiceColumn) {
        int value = index.model()->data(index, Qt::DisplayRole).toInt();
        QProgressBar *progressBar = qobject_cast<QProgressBar *>(editor);
        progressBar->setValue(value);
    } else {
        QItemDelegate::setEditorData(editor, index);
    }
}

void ProgressBarDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    if (index.column() == choiceColumn) {
        QProgressBar *progressBar = qobject_cast<QProgressBar *>(editor);
        model->setData(index, progressBar->value());
    } else {
        QItemDelegate::setModelData(editor, model, index);
    }
}

