#include "gamewidget.h"
#include "ui_gamewidget.h"

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameWidget) {

    ui->setupUi(this);
}

GameWidget::~GameWidget() {
    delete ui;
}

void GameWidget::input(std::string result) {

}
