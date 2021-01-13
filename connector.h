#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QObject>
#include <QJsonArray>

#include "../ServerDB111/codes.h"

class QWebSocket;


enum AuthorizationFail
{
    BadLoginAndPassword = -2,
    UserIsNotAdmin      = -1
};

class Connector: public QObject
{
    Q_OBJECT
public:
    static Connector* getConnector();

    ~Connector();

    void tryConnectServer(const QString& ip, const int port);

private:
    Connector(QObject* parent = nullptr);

    void sendJson(const QJsonObject& obj);

public slots:
    void slot_tryAuthorization(const QString& login, const QString& pass);

    void slot_tryRegistration(const QString& login, const QString& pass, bool is_admin);

    void slot_tryGetAllBooks();

    void slot_tryGetUsers(UserType userType);

private slots:
    void slot_connected();

    void slot_TextMessageReceived(const QString& msg);

    void slot_disconnected();

signals:
    void connected();

    void disconnected();

    void authorization(int);

    void registration(bool, QString);

    void allBooks(QJsonArray);

    void users(UserTypeErrors typeError, QJsonArray);


private:
    QWebSocket* socket;
};

#endif // CONNECTOR_H
