#ifndef LOADINGWIDGET_H
#define LOADINGWIDGET_H

#include "gameengine/gamestate.h"

#include <QWidget>

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
    void process() override;

private:
    Ui::LoadingWidget *ui;
};

#endif // LOADINGWIDGET_H
