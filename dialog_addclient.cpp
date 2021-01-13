#include "dialog_addclient.h"
#include "ui_dialog_addclient.h"

Dialog_AddClient::Dialog_AddClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_AddClient)
{
    ui->setupUi(this);
}

Dialog_AddClient::~Dialog_AddClient()
{
    delete ui;
}

void Dialog_AddClient::on_pushButtonOk_clicked()
{
    this->accept();
}

void Dialog_AddClient::on_pushButtonCancel_clicked()
{
    this->reject();
}

QString Dialog_AddClient::getLogin()
{
    return ui->lineEditClientName->text();
}

QString Dialog_AddClient::getPass()
{
    return ui->lineEditPass->text();
}

