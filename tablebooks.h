#ifndef TABLEBOOKS_H
#define TABLEBOOKS_H

#include <QAbstractTableModel>


struct Book
{
    int book_id;
    int reservation_user_id = 0;
    int year_publication;
    QString photo;
    QString title;
    QString genre;
    QString author;
    QString description;
    QString time_unblocking;
};

class TableBooks: public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Roles
    {
        BookId = Qt::UserRole + 1,
        Title,
        Author,
        Genre,
        ReservationUserId,
        YearPublication,
        Photo,
        Description,
        TimeUnblocking,
    };

    TableBooks(QObject* parent = nullptr);

    void createFromJsonArray(const QJsonArray& jA);

    QVariant data(const QModelIndex& index, int nRole) const;

    int rowCount(const QModelIndex& = QModelIndex()) const;

    int columnCount(const QModelIndex& = QModelIndex()) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    bool removeRow(int row, const QModelIndex &parent = QModelIndex());

    bool insertRow(int row, const Book& newBook);

    bool setData(const QModelIndex& index, const QVariant& value, int nRole);

//    QStringList getNames() const;

private:
    QVector<Book> books;
};

#endif // TABLEBOOKS_H
