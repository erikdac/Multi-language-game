#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "loginwidget.h"
#include "gamewidget.h"
#include "network/connection.h"
#include "onlinewidget.h"

#include <iostream>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    setUpLoginUi();
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setUpLoginUi() {
    removePreviousUi();
    LoginWidget * loginWidget = new LoginWidget(this);
    changeWidget(loginWidget);
}

void MainWindow::setUpGameUi() {
    removePreviousUi();
    OnlineWidget * onlineWidget = new OnlineWidget(this);
    changeWidget(onlineWidget);
}

void MainWindow::changeWidget(QWidget *widget) {
    connection::setActiveWidget(widget);
    QGridLayout * layout = new QGridLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(widget);
    setLayout(layout);
}

void MainWindow::removePreviousUi() {
    delete this->layout();
    while ( QWidget* w = findChild<QWidget*>() )
        delete w;
}
