#include "dialog_addbook.h"
#include "ui_dialog_addbook.h"

Dialog_AddBook::Dialog_AddBook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_AddBook)
{
    ui->setupUi(this);
}

Dialog_AddBook::~Dialog_AddBook()
{
    delete ui;
}

void Dialog_AddBook::on_pushButtonOk_clicked()
{
    this->accept();
}

void Dialog_AddBook::on_pushButtonCancel_clicked()
{
    this->reject();
}

/*  ----Сменила автора при добавлении книги--- */
void Dialog_AddBook::on_comboAuthor_currentIndexChanged(int index)
{
    Q_UNUSED(index);


}

/*  ----Добавляем нового автора--- */
void Dialog_AddBook::on_pushAddAuthor_clicked()
{

}

/*  ----Добавляем фото для книги--- */
void Dialog_AddBook::on_pushViewBook_clicked()
{

}

/*  ----Вкючаем/выключаем бронь--- */
void Dialog_AddBook::on_checkBoxReserv_stateChanged(int arg1)
{
    Q_UNUSED(arg1);

}

QString Dialog_AddBook::getBook()
{
    return ui->lineEditBook->text();
}

QString Dialog_AddBook::getId()
{
//    return ui->editIdBook->text();
}

void Dialog_AddBook::settId(QString idIn)
{
//    ui->editIdBook->setText(idIn);
}
