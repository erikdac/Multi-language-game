#ifndef LOADINGWIDGET_H
#define LOADINGWIDGET_H

#include "gameengine/gamestate.h"
#include "external/json11/json11.hpp"

namespace Ui {
    class LoadingWidget;
}

class LoadingWidget : public GameState {
    Q_OBJECT

public:
    explicit LoadingWidget(QWidget *);
    ~LoadingWidget();

    void resume() override;
    void pause() override;
    void tick(float) override;

private:
    Ui::LoadingWidget *ui;

    void parsePacket(const json11::Json &);
};

#endif // LOADINGWIDGET_H
