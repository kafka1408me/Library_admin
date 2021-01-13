#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

#include "authorization.h"

#include "dialog_addbook.h"
#include "dialog_addclient.h"
#include "dialog_editbook.h"
#include "dialog_editclient.h"

#include "tableusers.h"
#include "tablebooks.h"

#include <QSortFilterProxyModel>

#include "mydelegateforlistview.h"
#include "connector.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum ClientsOrBooks
    {
        Clients,
        Books
    };


    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void slot_setSettings(const QString &login);

    void slot_authorizationWindow();

    void slot_actionAuthorization(ActionAuthorization action, const QString& login, const QString& pass);

    void slot_resultAuthorization(int user_id);

    void slot_resultRegistration(bool success, const QString& login);

    void slot_ClientsOrBookChanged(const QString& str);

    void slot_categorySearchChanged(const QString& categorySearch);

    void slot_addNewClient();

    void slot_addNewBook();

    void slot_setAllBooks(const QJsonArray& array);

    void slot_selectedBookChanged(const QItemSelection &selected);

    void slot_searchTextChanged(const QString& searchText);

    void slot_disconnected();

    void slot_tryGetUsers();

    void slot_setUsers(UserTypeErrors typeError, QJsonArray array);

signals:
    void tryAuthorization(QString, QString);

    void tryRegistration(QString, QString, bool);

    void authorizationSuccess(const QString& login);

    void tryGetAllBooks();


private:
    Ui::MainWindow *ui;
    Connector* connector;
    ::Authorization* authorization;
    QSortFilterProxyModel* proxyListUsers;
    QSortFilterProxyModel* proxyListBooks;
    QSortFilterProxyModel* proxyTableUsers;
    QSortFilterProxyModel* proxyTableBooks;
    TableUsers* modelUsers;
    TableBooks* modelBooks;

    ClientsOrBooks clienstOrBooksState = ClientsOrBooks::Clients;
//    QStringListModel* listModelUsers;
//    QStringListModel* listModelBooks;
};
#endif // MAINWINDOW_H
