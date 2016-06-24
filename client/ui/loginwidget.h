#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include "gamestate.h"

namespace Ui {
    class LoginWidget;
}

class LoginWidget : public GameState {

    Q_OBJECT

    Ui::LoginWidget * ui;

public:
    LoginWidget(QWidget *);
    ~LoginWidget();

    void resume() override;
    void pause() override;

private:
    void checkResult();
    void popupBox(const QString);

private slots:
    void on_pushButton_clicked();
};

#endif
