#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "loginwidget.h"
#include "gamewidget.h"
#include "network/connection.h"
#include "onlinewidget.h"

#include <iostream>
#include <QStackedWidget>
#include <QGridLayout>

MainWindow::MainWindow(QStackedWidget *parent)
    : QStackedWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->addWidget(new LoginWidget(this));
    this->addWidget(new OnlineWidget(this));

    setUpLoginUi();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setUpLoginUi() {
    this->setCurrentIndex(0);
}

void MainWindow::setUpGameUi() {
    this->setCurrentIndex(1);
    (dynamic_cast<OnlineWidget *> (this->currentWidget()))->start();
}
