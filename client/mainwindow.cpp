#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwidget.h"
#include "gamewidget.h"
#include "network/connection.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::MainWindow) {

//    connectToServer();

    setUpLoginUi();
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setUpLoginUi() {
    removePreviousUi();
    LoginWidget * loginWidget = new LoginWidget(this);
    connection::setActiveWidget(loginWidget);
    loginWidget->show();
}

void MainWindow::setUpGameUi() {
    removePreviousUi();
    GameWidget * gameWidget = new GameWidget(this);
    gameWidget->show();
    connection::setActiveWidget(gameWidget);
}

void MainWindow::removePreviousUi() {
    while ( QWidget* w = findChild<QWidget*>() )
        delete w;
}
