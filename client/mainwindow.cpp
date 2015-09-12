#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwidget.h"
#include "connection.hpp"
#include "reader.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::MainWindow) {

    Reader * reader = new Reader(this);
    setReader(reader);

    connectToServer();

    setUpLoginUi();
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setUpLoginUi() {
    LoginWidget * loginWidget = new LoginWidget(this);
    setActiveWidget(loginWidget);
}

void MainWindow::setUpGameUi() {
    LoginWidget * loginWidget = findChild<LoginWidget*>();
    delete loginWidget;
}
