#ifndef DIALOG_ADDCLIENT_H
#define DIALOG_ADDCLIENT_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class Dialog_AddClient;
}

class Dialog_AddClient : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_AddClient(QWidget *parent = nullptr);
    ~Dialog_AddClient();

    QString getLogin();

    QString getPass();

private slots:
    void on_pushButtonOk_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::Dialog_AddClient *ui;
};

#endif // DIALOG_ADDCLIENT_H
