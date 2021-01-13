#include "authorization.h"
#include "ui_authorization.h"

Authorization::Authorization(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Authorization)
{
    ui->setupUi(this);
    ui->pushButtonOn->setFocus();
}

Authorization::~Authorization()
{
    delete ui;
}

QString Authorization::getLogin()
{
    return ui->lineEditLog->text();
}

void Authorization::on_pushButtonOn_clicked()
{
    const QString login = ui->lineEditLog->text();
    const QString pass = ui->lineEditPas->text();
    emit action(ActionAuthorization::AuthorizationAction, login, pass);
}

void Authorization::on_pushButtonRegistration_clicked()
{
    const QString login = ui->lineEditLog->text();
    const QString pass = ui->lineEditPas->text();
    emit action(ActionAuthorization::RegistrationAction, login, pass);
}

void Authorization::on_pushButtonExit_clicked()
{
    qApp->exit(0);
}
