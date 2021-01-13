#include "dialog_editclient.h"
#include "ui_dialog_editclient.h"

Dialog_EditClient::Dialog_EditClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_EditClient)
{
    ui->setupUi(this);
}

Dialog_EditClient::~Dialog_EditClient()
{
    delete ui;
}

void Dialog_EditClient::on_pushButtonOk_clicked()
{
    this->accept();
}

void Dialog_EditClient::on_pushButtonCancel_clicked()
{
    this->reject();
}

void Dialog_EditClient::setId(QString idIn)
{
    ui->lineEditClientId->setText(idIn);
}

void Dialog_EditClient::setClient(QString clientIn)
{
    ui->lineEditClientName->setText(clientIn);
}

QString Dialog_EditClient::getId(void)
{
    return ui->lineEditClientId->text();
}

QString Dialog_EditClient::getClient(void)
{
    return ui->lineEditClientName->text();
}
