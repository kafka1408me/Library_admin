#include "tableusers.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

TableUsers::TableUsers(QObject *parent):
    QAbstractTableModel(parent)
{
}

void TableUsers::createFromJsonArray(const QJsonArray &jA)
{
    qDebug() << "TableUsers::createFromJsonArray";

    int oldSize = users.size();

    bool removeRows = !users.isEmpty();

    if(removeRows)
    {
        beginRemoveRows(QModelIndex(),0,oldSize-1);
        users.clear();
        endRemoveRows();
    }

    QJsonObject jObject;

    users.reserve(jA.size());
    beginInsertRows(QModelIndex(), 0, jA.size()-1);

    for(auto j: jA)
    {
        User user;
        jObject = j.toObject();
        user.user_id = jObject.value("user_id").toInt();
        user.login = jObject.value("login").toString();
        user.pass = jObject.value("pass").toString();
        user.rate_app = jObject.value("rate_app").toInt();

        users.append(std::move(user));
    }
    endInsertRows();
}

QVariant TableUsers::data(const QModelIndex &index, int nRole) const
{
    if(!index.isValid())
    {
        return QVariant();
    }


    if(nRole > Qt::UserRole)
    {
        const User& user = users.at(index.row());
        switch (nRole)
        {
        case User_Id:
            return user.user_id;
        case Login:
            return user.login;
        case Pass:
            return user.pass;
        case Rate_App:
            return user.rate_app ? user.rate_app : QVariant();
        default:
            break;
        }
    }

    if(nRole == Qt::DisplayRole)
    {
        const User& user = users.at(index.row());
        switch (index.column())
        {
        case 0:
            return user.user_id;
        case 1:
            return user.login;
        case 2:
            return user.pass;
        case 3:
            return user.rate_app? user.rate_app : QVariant();
        default:
            break;
        }
    }
    return QVariant();
}

int TableUsers::rowCount(const QModelIndex &) const
{
    return users.size();
}

int TableUsers::columnCount(const QModelIndex &) const
{
    return 4;
}

QVariant TableUsers::headerData(int section, Qt::Orientation orientation, int role) const
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
            return "Логин";
        case 2:
            return "Пароль";
        case 3:
            return "Оценка мобильному приложению";
        default:
            break;
        }
    }

    return QVariant();
}

bool TableUsers::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent)
    beginRemoveRows(QModelIndex(),row,row);
//    emit delUser(users.at(row).user_id);
    users.remove(row);
    endRemoveRows();
    return true;
}

bool TableUsers::insertRow(int row, const User &newUser)
{
    beginInsertRows(QModelIndex(),row, row);
    users.append(newUser);
    endInsertRows();
    return true;
}

bool TableUsers::setData(const QModelIndex &index, const QVariant &value, int nRole)
{
    if(!index.isValid())
    {
        return false;
    }

    User& user = users[index.row()];
    switch (nRole)
    {
    case User_Id:
        user.user_id = value.toInt();
        break;
    case  Login:
        user.login = value.toString();
        break;
    case Pass:
        user.pass = value.toString();
        break;
    case Rate_App:
        user.rate_app = value.toInt();
        break;
    default:
        break;
    }
    emit dataChanged(index, index);

    return true;
}

QStringList TableUsers::getNames() const
{
    QStringList list;
    for(const auto& user: users)
    {
        list.append(user.login);
    }
    return list;
}
