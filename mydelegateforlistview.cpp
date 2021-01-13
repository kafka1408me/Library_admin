#include "mydelegateforlistview.h"
#include <QPainter>
#include <QDebug>

#define MYCOLOR 50,159,255

static QColor SelectedColor = QRgba64::fromRgba(MYCOLOR, 210);
static QColor OverColor = QRgba64::fromRgba(MYCOLOR, 130);

MyDelegateForListView::MyDelegateForListView(QSortFilterProxyModel* proxy, const QVector<int>& roles, QObject* parent):
    QStyledItemDelegate(parent),
    myProxy(proxy),
    myRoles(roles)
{
}

void MyDelegateForListView::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect rect = option.rect;

    bool isColored = false;

    if(option.state & QStyle::State_Selected || option.state & QStyle::QStyle::State_MouseOver)
    {
        isColored = true;
        QColor* myColor;
        if(option.state & QStyle::State_Selected)
        {
            myColor = &SelectedColor;
        }
        else// if(option.state & QStyle::QStyle::State_MouseOver)
        {
            myColor = &OverColor;
        }
        painter->setBrush(QBrush(*myColor));
        painter->setRenderHint(QPainter::Antialiasing);
        painter->drawRoundedRect(rect, 5, 5);
    }

    if(isColored)
    {
        painter->setPen(Qt::white);
    }
    else
    {
        painter->setPen(Qt::black);
    }

    QString text;
    rect.setX(rect.x() + 5);
    rect.setSize(QSize(option.rect.width(), 40));

    for(const auto& role: myRoles)
    {
        text = myProxy->data(index, role).toString();
        painter->drawText(rect, text);
        rect.setY(rect.y()+17);
    }
}

QSize MyDelegateForListView::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(option.rect.width(), 30*myRoles.size());
}
