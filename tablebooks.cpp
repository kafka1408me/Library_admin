#include "tablebooks.h"
#include <QImage>
#include <QPixmap>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

TableBooks::TableBooks(QObject* parent):
    QAbstractTableModel(parent)
{

}

void TableBooks::createFromJsonArray(const QJsonArray &jA)
{
    qDebug() << "TableBooks::createFromJsonArray";
    int oldSize = books.size();

    bool removeRows = !books.isEmpty();

    if(removeRows)
    {
        beginRemoveRows(QModelIndex(),0,oldSize-1);
        books.clear();
        endRemoveRows();
    }

    QJsonObject jObject;

    books.reserve(jA.size());
    beginInsertRows(QModelIndex(), 0, jA.size()-1);

    for(auto j: jA)
    {
        Book book;
        jObject = j.toObject();
        book.book_id = jObject.value("book_id").toInt();
        book.title = jObject.value("title").toString();
        book.genre = jObject.value("genre").toString();
        book.reservation_user_id = jObject.value("reservation_user_id").toInt();
        book.year_publication = jObject.value("year_publication").toInt();
        book.photo = jObject.value("photo").toString();
        book.description = jObject.value("description").toString();
        book.author = jObject.value("author").toString();
        book.time_unblocking = jObject.value("time_unblocking").toString();

        books.push_back(std::move(book));
    }
    endInsertRows();
}

QVariant TableBooks::data(const QModelIndex &index, int nRole) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    if(nRole > Qt::UserRole)
    {
        const Book& book = books[index.row()];
        switch (nRole) {
        case Title:
            return book.title;
        case Author:
            return book.author;
        case Genre:
            return book.genre;
        case BookId:
            return book.book_id;
        case ReservationUserId:
            return book.reservation_user_id;
        case YearPublication:
            return book.year_publication;
        case Photo:
            return book.photo;
        case Description:
            return book.description;
        case TimeUnblocking:
            return book.time_unblocking;
        default:
            return QVariant();
        }
    }
    else if(nRole == Qt::DisplayRole)
    {
        const Book& book = books[index.row()];
        switch (index.column())
        {
        case 0:
            return book.book_id;
        case 1:
            return book.title;
        case 2:
            return book.author;
        case 3:
            return book.genre;
        case 4:
            return book.year_publication;
        case 5:
            return book.description;
        case 6:
            return book.reservation_user_id ? book.reservation_user_id: QVariant();
        case 7:
            return book.time_unblocking;
        default:
            break;
        }
    }
    else if(index.column() == 8)
    {
        if(nRole == Qt::DecorationRole)
        {
            const Book& book = books[index.row()];
            const QString& photoStr = book.photo;
            if(!photoStr.isEmpty())
            {
                QByteArray by = QByteArray::fromBase64(photoStr.toLatin1());

                QImage image = QImage::fromData(by, "PNG");
                QPixmap pix = QPixmap::fromImage(image).scaled(200,250, Qt::KeepAspectRatio);
                return pix;
            }
        }
        else if(nRole == Qt::SizeHintRole)
        {
            static QSize size(200,250);
            return size;
        }
    }

    return QVariant();
}

int TableBooks::rowCount(const QModelIndex &) const
{
    return books.size();
}

int TableBooks::columnCount(const QModelIndex &) const
{
    return 9;
}

QVariant TableBooks::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }
    if(orientation == Qt::Horizontal)
    {
        switch (section){
        case 0:
            return "Идентификатор";
        case 1:
            return "Название";
        case 2:
            return "Автор";
        case 3:
            return "Жанр";
        case 4:
            return "Год публикации";
        case 5:
            return "Описание";
        case 6:
            return "Идентификатор забронировавшего пользователя";
        case 7:
            return "Забронирована до";
        case 8:
            return "Фото";
        default:
            break;
        }
    }

    return QVariant();
}

bool TableBooks::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent)
    beginRemoveRows(QModelIndex(),row,row);
    //    emit delUser(users.at(row).user_id);
    books.remove(row);
    endRemoveRows();
    return true;
}

bool TableBooks::insertRow(int row, const Book &newBook)
{
    beginInsertRows(QModelIndex(),row, row);
    books.append(newBook);
    endInsertRows();
    return true;
}

bool TableBooks::setData(const QModelIndex &index, const QVariant &value, int nRole)
{
    if(!index.isValid())
    {
        return false;
    }

    if(nRole > Qt::UserRole)
    {
        Book& book = books[index.row()];
        switch (nRole) {
        case Title:
            book.title = value.toString();
            break;
        case Author:
            book.author = value.toString();
            break;
        case Genre:
            book.genre = value.toString();
            break;
        case BookId:
            book.book_id = value.toInt();
        case ReservationUserId:
            book.reservation_user_id = value.toInt();
            break;
        case YearPublication:
            book.year_publication= value.toInt();
            break;
        case Photo:
            book.photo = value.toString();
            break;
        case Description:
            book.description = value.toString();
            break;
        case TimeUnblocking:
            book.time_unblocking = value.toString();
            break;
        default:
            return false;
        }
    }
    return true;
}

//QStringList TableBooks::getNames() const
//{
//    QStringList list;
//    for(const auto& book: books)
//    {
//        list.append(book.title);
//    }
//    return list;
//}



