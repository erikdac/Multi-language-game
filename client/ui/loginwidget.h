#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include "gameengine/gamestate.h"
#include "json/json11.hpp"

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
    void process() override;

private:
    bool sendLoginData(json11::Json);
    bool isConnected();
    void checkResult();
    void popupBox(const QString);

private slots:
    void on_pushButton_clicked();
};

#endif
