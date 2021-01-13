#ifndef TABLEUSERS_H
#define TABLEUSERS_H

#include <QAbstractTableModel>

struct User
{
    int user_id;
    QString login;
    QString pass;
    int rate_app;
};

class TableUsers: public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Roles
    {
        User_Id = Qt::UserRole + 1,
        Login,
        Pass,
        Rate_App
    };

    TableUsers(QObject* parent = nullptr);

    void createFromJsonArray(const QJsonArray& jA);

    QVariant data(const QModelIndex& index, int nRole) const;

    int rowCount(const QModelIndex& = QModelIndex()) const;

    int columnCount(const QModelIndex& = QModelIndex()) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    bool removeRow(int row, const QModelIndex &parent = QModelIndex());

    bool insertRow(int row, const User& newUser);

    bool setData(const QModelIndex& index, const QVariant& value, int nRole);

    QStringList getNames() const;

//signals:
//    void delUser(int);

private:
    QVector<User> users;
};

#endif // TABLEUSERS_H
