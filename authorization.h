#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QDialog>

#include "qdebug.h"

namespace Ui {
class Authorization;
}

enum ActionAuthorization
{
    AuthorizationAction = 0,
    RegistrationAction  = 1
};


class Authorization : public QDialog
{
    Q_OBJECT

public:
    explicit Authorization(QWidget *parent = nullptr);
    ~Authorization();

    QString getLogin();

private slots:
    void on_pushButtonOn_clicked();

    void on_pushButtonRegistration_clicked();

    void on_pushButtonExit_clicked();

signals:
    void action(ActionAuthorization, QString, QString);

private:
    Ui::Authorization *ui;
};

#endif // AUTHORIZATION_H
