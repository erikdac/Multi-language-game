#include "loadingwidget.h"
#include "ui_loadingwidget.h"
#include "window.h"

#include <iostream>

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

}

void LoadingWidget::pause() {

}

void LoadingWidget::process() {
    std::cout << "LOADING..." << std::endl;
    dynamic_cast<Window *> (this->parentWidget())->setGameUi();
}
