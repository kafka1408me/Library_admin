#ifndef DIALOG_EDITCLIENT_H
#define DIALOG_EDITCLIENT_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class Dialog_EditClient;
}

class Dialog_EditClient : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_EditClient(QWidget *parent = nullptr);
    ~Dialog_EditClient();


    void setId(QString idIn);
    void setClient(QString clientIn);
    QString getId(void);
    QString getClient(void);

private slots:
    void on_pushButtonOk_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::Dialog_EditClient *ui;
};

#endif // DIALOG_EDITCLIENT_H
