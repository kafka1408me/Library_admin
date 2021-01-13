#ifndef DIALOG_ADDBOOK_H
#define DIALOG_ADDBOOK_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class Dialog_AddBook;
}

class Dialog_AddBook : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_AddBook(QWidget *parent = nullptr);
    ~Dialog_AddBook();

    QString getBook();
    QString getId();
    void    settId(QString idIn);

private slots:
    void on_pushButtonOk_clicked();

    void on_pushButtonCancel_clicked();

    void on_comboAuthor_currentIndexChanged(int index);

    void on_pushAddAuthor_clicked();

    void on_pushViewBook_clicked();

    void on_checkBoxReserv_stateChanged(int arg1);

private:
    Ui::Dialog_AddBook *ui;
};

#endif // DIALOG_ADDBOOK_H
