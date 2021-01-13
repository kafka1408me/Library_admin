#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connector.h"
#include <QDebug>
#include <QMessageBox>

#define MAINWINDOW "MainWindow: "

#define ABRACADABRA "abracadabra"

#define SEARCH_CATEGORY_TITLE "название"
#define SEARCH_CATEGORY_AUTHOR "автор"
#define SEARCH_CATEGORY_CLIENT "логин"

#define CLIENTS_STR "Клиенты"
#define BOOKS_STR   "Книги"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(nullptr)
{
    connector = Connector::getConnector();

    connect(connector, &Connector::connected, this, &MainWindow::slot_authorizationWindow);
    connect(connector, &Connector::authorization, this, &MainWindow::slot_resultAuthorization);
    connect(connector, &Connector::registration, this, &MainWindow::slot_resultRegistration);


    connect(this, &MainWindow::authorizationSuccess, connector, &Connector::slot_tryGetAllBooks);
    connect(this, &MainWindow::authorizationSuccess, this, &MainWindow::slot_tryGetUsers);
    connect(this, &MainWindow::authorizationSuccess, this, &MainWindow::slot_setSettings);
    connect(this, &MainWindow::tryRegistration, connector, &Connector::slot_tryRegistration);
    connect(this, &MainWindow::tryAuthorization, connector, &Connector::slot_tryAuthorization);
    connect(this, &MainWindow::tryGetAllBooks, connector, &Connector::slot_tryGetAllBooks);
    connect(connector, &Connector::allBooks, this, &MainWindow::slot_setAllBooks);
    connect(connector, &Connector::disconnected, this, &MainWindow::slot_disconnected);
    connect(connector, &Connector::users, this, &MainWindow::slot_setUsers);


    connector->tryConnectServer("localhost", 4735);
}

MainWindow::~MainWindow()
{
    if(ui)
    {
        delete ui;
    }
}

void MainWindow::slot_setSettings(const QString& login)
{
    ui = new Ui::MainWindow;
    ui->setupUi(this);

    ui->lblYorLogin->setText(login);



    /*  ----widgetBooks---- */
    modelBooks = new TableBooks(this);

    proxyListBooks = new QSortFilterProxyModel(this);
    proxyListBooks->setSourceModel(modelBooks);
    proxyListBooks->setFilterCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);
    proxyListBooks->setSortRole(TableBooks::Roles::Title);

    proxyTableBooks = new QSortFilterProxyModel(this);
    proxyTableBooks->setSourceModel(modelBooks);
    proxyTableBooks->setFilterRole(TableBooks::Roles::BookId);
    proxyTableBooks->setFilterWildcard(ABRACADABRA);

    QVector<int> rolesBooks{TableBooks::Roles::Author, TableBooks::Roles::Title};

    ui->viewNameBooks->setModel(proxyListBooks);
    MyDelegateForListView* delegateBook = new MyDelegateForListView(proxyListBooks, rolesBooks, this);
    ui->viewNameBooks->setItemDelegate(delegateBook);

    ui->viewTableBooks->setModel(proxyTableBooks);

    /* --------------------- */

    ////////////////////////////////////

    /*  ----widgetClients----   */
    modelUsers = new TableUsers(this);

    proxyListUsers = new QSortFilterProxyModel(this);
    proxyListUsers->setSourceModel(modelUsers);
    proxyListUsers->setFilterCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);
    proxyListUsers->setFilterRole(TableUsers::Login);
    proxyListUsers->setSortRole(TableUsers::Roles::Login);

    proxyTableUsers = new QSortFilterProxyModel(this);
    proxyTableUsers->setFilterRole(TableUsers::Roles::Login);
    proxyTableUsers->setFilterWildcard( ABRACADABRA);
    proxyTableUsers->setSourceModel(modelUsers);

    QVector<int> rolesUsers{TableUsers::Roles::Login};

    ui->viewNameClients->setModel(proxyListUsers);
    MyDelegateForListView* delegateUser = new MyDelegateForListView(proxyListUsers, rolesUsers, this);
    ui->viewNameClients->setItemDelegate(delegateUser);

    ui->viewTableClients->setModel(proxyTableUsers);

    /* --------------------- */

    connect(ui->viewNameBooks->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::slot_selectedBookChanged);


    connect(ui->cmbxClientsOrBooks, &QComboBox::currentTextChanged, this, &MainWindow::slot_ClientsOrBookChanged);
    connect(ui->cmbxCategorySearch, &QComboBox::currentTextChanged, this, &MainWindow::slot_categorySearchChanged);
    connect(ui->lineSearchRequest, &QLineEdit::textChanged, this, &MainWindow::slot_searchTextChanged);

    connect(ui->pushAddBooks, &QPushButton::clicked, this, &MainWindow::slot_addNewBook);
    connect(ui->pushAddClients,  &QPushButton::clicked, this, &MainWindow::slot_addNewClient);

    ui->cmbxClientsOrBooks->addItems(QStringList() << BOOKS_STR << CLIENTS_STR);
    slot_ClientsOrBookChanged(BOOKS_STR);

    show();
}

void MainWindow::slot_authorizationWindow()
{
    authorization = new ::Authorization;
    connect(authorization, &Authorization::action, this, &MainWindow::slot_actionAuthorization);
    connect(this, &MainWindow::authorizationSuccess, authorization, &Authorization::deleteLater);

    authorization->exec();
}

void MainWindow::slot_actionAuthorization(ActionAuthorization action, const QString &login, const QString &pass)
{
    qDebug() << MAINWINDOW << "slot_actionAuthorization. action = " << action;

    if(action == ActionAuthorization::AuthorizationAction)
    {
        emit tryAuthorization(login, pass);
    }
    else
    {
        emit tryRegistration(login, pass, true);
    }
}

void MainWindow::slot_resultAuthorization(int user_id)
{
    qDebug() << "slot_resultAuthorization. user_id = " << user_id;

    if(user_id < 0)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Ошибка авторизации");

        QString text;

        if(user_id == AuthorizationFail::BadLoginAndPassword)
        {
            text = "Неверные логин или пароль";
        }
        else
        {
            text = "Пользователь не имеет прав администратора\n Вы не можете войти в этот аккаунт";
        }
        msgBox.setText(text);
        msgBox.exec();
    }
    else
    {
        emit authorizationSuccess(authorization->getLogin());
    }
}

void MainWindow::slot_resultRegistration(bool success, const QString &login)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Регистрация");

    QString text;
    if(success)
    {
        text = "Пользователь \'" + login + "\' успешно создан";
        msgBox.setIcon(QMessageBox::Information);
    }
    else
    {
        text = "Не удалось создать пользователя \'"+login+"\'\nВероятно, пользователь уже существует";
        msgBox.setIcon(QMessageBox::Warning);
    }
    msgBox.setText(text);
    msgBox.exec();
}

void MainWindow::slot_ClientsOrBookChanged(const QString &str)
{
    ui->cmbxCategorySearch->clear();

    const QString searchText = ui->lineSearchRequest->text();

    if(str == BOOKS_STR)
    {
        proxyListBooks->setFilterFixedString(searchText);
        clienstOrBooksState = ClientsOrBooks::Books;
        static QStringList booksCategorySearch({SEARCH_CATEGORY_TITLE,SEARCH_CATEGORY_AUTHOR});
        ui->widgetClients->hide();
        ui->widgetBooks->show();
        ui->cmbxCategorySearch->addItems(booksCategorySearch);
        qDebug() << MAINWINDOW << "Books";
    }
    else
    {
        proxyListUsers->setFilterFixedString(searchText);
        clienstOrBooksState = ClientsOrBooks::Clients;
        static QStringList clientCategorySearch({SEARCH_CATEGORY_CLIENT});
        ui->widgetClients->show();
        ui->widgetBooks->hide();
        ui->cmbxCategorySearch->addItems(clientCategorySearch);
        qDebug() << MAINWINDOW <<"Clients";
    }
}

void MainWindow::slot_categorySearchChanged(const QString &categorySearch)
{
    qDebug() << "MainWindow::slot_categorySearchChanged";
    QString searchText = ui->lineSearchRequest->text();

    if(clienstOrBooksState == ClientsOrBooks::Clients)
    {
        proxyListUsers->setFilterFixedString(searchText);
    }
    else
    {
        if(categorySearch == SEARCH_CATEGORY_TITLE)
        {
            proxyListBooks->setFilterRole(TableBooks::Roles::Title);
        }
        else
        {
            proxyListBooks->setFilterRole(TableBooks::Roles::Author);
        }
        proxyListBooks->setFilterFixedString(searchText);
    }
}

void MainWindow::slot_addNewClient()
{
    qDebug() << "MainWindow::slot_addNewClient";

    Dialog_AddClient* dialogAddClient = new Dialog_AddClient;
    int result = dialogAddClient->exec();

    if(result == Dialog_AddClient::Accepted)
    {
        QString login = dialogAddClient->getLogin();
        QString pass = dialogAddClient->getPass();
        emit tryRegistration(login, pass, false);
    }
    delete dialogAddClient;
}

void MainWindow::slot_addNewBook()
{
    qDebug() << "MainWindow::slot_addNewBook";
}

void MainWindow::slot_setAllBooks(const QJsonArray &array)
{
    qDebug() << MAINWINDOW << "slot_setAllBooks";

    modelBooks->createFromJsonArray(array);

    if(modelBooks->rowCount())
    {
        ui->viewNameBooks->selectionModel()->select(modelBooks->index(0,0), QItemSelectionModel::Select);
    }

    ui->viewTableBooks->resizeRowsToContents();
    ui->viewTableBooks->resizeColumnToContents(8);
}

void MainWindow::slot_selectedBookChanged(const QItemSelection &selected)
{
    qDebug() << "MainWindow::slot_selectedBookChanged";
    auto indexes = selected.indexes();

    if(!indexes.isEmpty())
    {
        const int bookId = ui->viewNameBooks->selectionModel()->currentIndex().data(TableBooks::Roles::BookId).toInt();

        proxyTableBooks->setFilterRegExp(QString::number(bookId) + "$");
        ui->viewTableBooks->resizeRowsToContents();
        ui->viewTableBooks->resizeColumnToContents(8);
        return;

    }
    proxyTableBooks->setFilterWildcard(ABRACADABRA);

    ui->viewTableBooks->resizeRowsToContents();
    ui->viewTableBooks->resizeColumnToContents(8);  // столбец с картинкой должен отображаться полностью
}

void MainWindow::slot_searchTextChanged(const QString &searchText)
{
    qDebug() << "MainWindow::slot_searchTextChanged";

    if(clienstOrBooksState == ClientsOrBooks::Clients)
    {
        proxyListUsers->setFilterFixedString(searchText);
    }
    else
    {
        proxyListBooks->setFilterFixedString(searchText);
    }
}

void MainWindow::slot_disconnected()
{
    QMessageBox msgBox;
    setEnabled(false);
    msgBox.setWindowTitle("Ошибка подключения");
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText("Отсутствует соединение с сервером\nПрограмма будет завершена");
    msgBox.exec();
    qApp->exit(1);
}

void MainWindow::slot_tryGetUsers()
{
    qDebug() << "MainWindow::slot_tryGetUsers";
    connector->slot_tryGetUsers(UserType::OrdinaryUsers);
}

void MainWindow::slot_setUsers(UserTypeErrors typeError, QJsonArray array)
{
    qDebug() << "MainWindow::slot_resultGetUsers";
    if(typeError == UserTypeErrors::NoErrors)
    {
        modelUsers->createFromJsonArray(array);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Произошла ошибка");
        msgBox.setIcon(QMessageBox::Warning);
        QString text = "Не удалось получить список пользователей:\n";
        if(typeError == UserTypeErrors::QueryError)
        {
            text += "Произошла ошибка на сервере";
        }
        else
        {
            text += "Ошибка в протоколе";
        }
        msgBox.setText(text);
        msgBox.exec();
    }
}

