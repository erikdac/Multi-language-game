#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include "gamestate.h"

#include <QWidget>

namespace Ui {
    class LoginWidget;
}

class LoginWidget : public GameState {

    Q_OBJECT

    Ui::LoginWidget *ui;

public:
    void init(QWidget *) override;
    void clear() override;

    void resume() override;
    void pause() override;

    static LoginWidget * instance() {
        static LoginWidget * _instance = new LoginWidget();
        return _instance;
    }

protected:
    LoginWidget();
    ~LoginWidget();

private:
    void checkResult();
    void popupBox(const QString);

private slots:
    void on_pushButton_clicked();
};

#endif
