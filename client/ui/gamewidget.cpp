#include "ui_gamewidget.h"
#include "gamewidget.h"
#include "mainwindow.h"
#include "network/connection.h"

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameWidget)
{
    ui->setupUi(this);
}

GameWidget::~GameWidget() {
    delete ui;
}

void GameWidget::input(std::string input) {

}

void GameWidget::on_pushButton_clicked() {
    connection::disconnect();

    MainWindow *w = dynamic_cast<MainWindow *> (this->parentWidget());
    w->setUpLoginUi();
}
