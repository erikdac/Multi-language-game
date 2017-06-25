#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include "gameengine/gamestate.h"
#include "external/json11/json11.hpp"

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
    void tick(float) override;

private:
    bool sendLoginData(json11::Json);
    bool isConnected();
    void checkResult(const std::string &);
    void popupBox(const QString);

private slots:
    void on_pushButton_clicked();
};

#endif
