#ifndef DIALOG_EDITBOOK_H
#define DIALOG_EDITBOOK_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class Dialog_EditBook;
}

class Dialog_EditBook : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_EditBook(QWidget *parent = nullptr);
    ~Dialog_EditBook();

private slots:
    void on_pushButtonOk_clicked();

//    void on_pushButtonCanlel_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::Dialog_EditBook *ui;
};

#endif // DIALOG_EDITBOOK_H
