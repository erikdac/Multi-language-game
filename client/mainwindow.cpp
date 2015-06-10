#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwidget.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::MainWindow) {
    setUpLoginUi();
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setUpLoginUi() {
    new LoginWidget(this);
}

void MainWindow::setUpGameUi() {
    LoginWidget * loginWidget = findChild<LoginWidget*>();
    delete loginWidget;
}
