#include <QWebSocket>
#include <QJsonObject>
#include <QJsonDocument>

#include "connector.h"
//#include "codes.h"
#include "../ServerDB111/codes.h"
//#include <QJsonArray>

#define CONNECTOR  "Connector: "
#define RESULT_YES "yes"
#define RESULT_NO  "no"

Connector::Connector(QObject *parent):
    QObject(parent),
    socket(new QWebSocket)
{
    connect(socket, &QWebSocket::connected, this, &Connector::slot_connected);
    connect(socket, &QWebSocket::disconnected, this, &Connector::slot_disconnected);
    connect(socket, &QWebSocket::textMessageReceived, this, &Connector::slot_TextMessageReceived);

    qDebug() << CONNECTOR << "constructor";
}

Connector *Connector::getConnector()
{
    static Connector connector;
    return &connector;
}

Connector::~Connector()
{
    qDebug() << CONNECTOR << "destructor";
}

void Connector::tryConnectServer(const QString &ip, const int port)
{
    QString url = "ws://"+ ip + ":" + QString::number(port);
    qDebug() << CONNECTOR << "пытаюсь подключиться по адресу: " << url;
    socket->open(QUrl(url));
}

void Connector::sendJson(const QJsonObject& obj)
{
    QJsonDocument doc(obj);
    socket->sendTextMessage(doc.toJson(QJsonDocument::Compact));
}

void Connector::slot_tryAuthorization(const QString &login, const QString &pass)
{
    qDebug() << CONNECTOR << "пытаюсь авторизоваться. login = " << login << " ; password  = " << pass;

    QJsonObject obj;
    QJsonObject mainObj;

    obj.insert("type", Codes::Authorization);
    mainObj.insert("login", login);
    mainObj.insert("pass", pass);

    obj.insert("main", mainObj);

    sendJson(obj);
}

void Connector::slot_tryRegistration(const QString &login, const QString &pass, bool is_admin)
{
    qDebug() << CONNECTOR << "пытаюсь зарегистрироваться. login = " << login << " ; password  = " << pass;

    QJsonObject obj;
    QJsonObject mainObj;

    obj.insert("type", Codes::CreateUser);
    mainObj.insert("login", login);
    mainObj.insert("pass", pass);
    mainObj.insert("is_admin", is_admin);

    obj.insert("main", mainObj);

    sendJson(obj);
}

void Connector::slot_tryGetAllBooks()
{
    qDebug() << CONNECTOR << "пытаюсь получить информацию о всех книгах";

    QJsonObject obj;
    obj.insert("type", Codes::GetAllBooks);

    sendJson(obj);
}

void Connector::slot_tryGetUsers(UserType userType)
{
    qDebug() << CONNECTOR << "пытаюсь получить информаацию о пользователях";

    QJsonObject obj;
    QJsonObject mainObj;

    mainObj.insert("user_type", userType);
    obj.insert("type", Codes::GetUsers);
    obj.insert("main", mainObj);

    sendJson(obj);
}

void Connector::slot_connected()
{
    qDebug() << CONNECTOR << "удалось подключиться к серверу";
    qDebug() << CONNECTOR << "отправляю запрос для опознания";

    QJsonObject obj;
    QJsonObject mainObj;

    mainObj.insert("check", "client");
    obj.insert("type", Codes::Check);
    obj.insert("main", mainObj);

    sendJson(obj);
}

void Connector::slot_TextMessageReceived(const QString &msg)
{
    QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());

    QJsonObject obj = doc.object();
    const int type = obj.value("type").toInt();
    QJsonObject mainObj = obj.value("main").toObject();


    const QString result = mainObj.value("result").toString();
    switch (type)
    {
    case Codes::Authorization:
    {
        qDebug() << CONNECTOR << "получено сообщение типа Codes::Authorization";

        if(result == RESULT_YES)
        {
            const bool is_admin = mainObj.value("is_admin").toBool();

            if(!is_admin)
            {
                emit authorization(AuthorizationFail::UserIsNotAdmin);
            }
            else
            {
                const int user_id = mainObj.value("user_id").toInt();
                emit authorization(user_id);
                qDebug() << CONNECTOR << "успешная авторизация";
            }
        }
        else
        {
            emit authorization(AuthorizationFail::BadLoginAndPassword);
        }

        break;
    }
    case Codes::CreateUser:
    {
        qDebug() << CONNECTOR << "получено сообщение типа Codes::CreateUser";
        const QString login = mainObj.value("login").toString();

        bool success = false;
        if(result == RESULT_YES)
        {
            success = true;
        }

        emit registration(success, login);

        break;
    }
    case Codes::Check:
    {
        qDebug() << CONNECTOR << "получено сообщение типа Codes::Check";

        const QString check = mainObj.value("check").toString();
        qDebug() << CONNECTOR << "check = " << check;
        if(check == "server")
        {
            emit connected();
        }
        else
        {
            qDebug() << CONNECTOR << "закрываю соединение";
            socket->close();
        }

        break;
    }
    case Codes::GetAllBooks:
    {
        qDebug() << CONNECTOR << "получено сообщение типа Codes::GetAllBooks";

        if(result == RESULT_YES)
        {
            QJsonArray array = mainObj.value("books").toArray();
            emit allBooks(array);
        }
        else
        {
            qDebug() << CONNECTOR << "Codes::GetAllBooks : result = " << result;
        }
        break;
    }
    case Codes::GetUsers:
    {
        qDebug() << CONNECTOR << "получено сообщение типа Codes::GetUsers";

        if(result == RESULT_YES)
        {
            QJsonArray array = mainObj.value("users").toArray();
            emit users(UserTypeErrors::NoErrors, array);
        }
        else
        {
            UserTypeErrors typeError = UserTypeErrors(mainObj.value("type_error").toInt());
            emit users(typeError, {});
        }

        break;
    }
    default:
    {
        qDebug() << CONNECTOR << "получено сообщение неизвестного типа. type = " << type;
        break;
    }
    }
}

void Connector::slot_disconnected()
{
    qDebug() << CONNECTOR << "произошел разрыв соединения";
    emit disconnected();
}
