#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "mainwindow.h"

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

void GameWidget::on_pushButton_clicked()
{
    MainWindow *w = dynamic_cast<MainWindow *> (this->parentWidget());
    w->setUpLoginUi();

    // TODO: Logout from server!
}
