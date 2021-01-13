#ifndef MYDELEGATEFORLISTVIEW_H
#define MYDELEGATEFORLISTVIEW_H

#include <QStyledItemDelegate>
#include <QSortFilterProxyModel>


class MyDelegateForListView: public QStyledItemDelegate
{
public:
    MyDelegateForListView(QSortFilterProxyModel* proxy, const QVector<int>& roles, QObject* parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QSortFilterProxyModel* myProxy;
    QVector<int> myRoles;
};

#endif // MYDELEGATEFORLISTVIEW_H
