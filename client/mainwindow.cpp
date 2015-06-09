#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwidget.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::MainWindow) {
    setUpLoginUi();
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setUpLoginUi() {
    LoginWidget * loginWidget = new LoginWidget(this);
}
