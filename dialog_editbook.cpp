#include "dialog_editbook.h"
#include "ui_dialog_editbook.h"

Dialog_EditBook::Dialog_EditBook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_EditBook)
{
    ui->setupUi(this);
}

Dialog_EditBook::~Dialog_EditBook()
{
    delete ui;
}

void Dialog_EditBook::on_pushButtonOk_clicked()
{
    this->accept();
}

void Dialog_EditBook::on_pushButtonCancel_clicked()
{
    this->reject();
}
