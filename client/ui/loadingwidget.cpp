#include "loadingwidget.h"
#include "ui_loadingwidget.h"
#include "window.h"

#include <QtDebug>

LoadingWidget::LoadingWidget(QWidget *parent)
    : ui(new Ui::LoadingWidget)
{
    this->setParent(parent);
    ui->setupUi(this);
}

LoadingWidget::~LoadingWidget() {
    delete ui;
}

void LoadingWidget::resume() {
    // qinfo("Resumed");
}

void LoadingWidget::pause() {
    // qInfo("Paused");
}

void LoadingWidget::tick(float deltaTime) {
    dynamic_cast<Window *> (this->parentWidget())->setGameUi();
}
