#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwidget.h"
#include "network/connection.h"
#include "network/network_reader.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::MainWindow) {

    Network_Reader * network_reader = new Network_Reader(this);
    setNetworkReader(network_reader);

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
