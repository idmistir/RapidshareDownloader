#ifndef FONTCOMBOBOXDELEGATE_H
#define FONTCOMBOBOXDELEGATE_H

#include <QItemDelegate>
#include <QComboBox>

class ComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ComboBoxDelegate(int choiceColumn, QObject *parent = 0);
    
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private slots:
    void commitAndCloseEditor( void );

private:
    int choiceColumn;
    
};

#endif // FONTCOMBOBOXDELEGATE_H
