#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwidget.h"

#include <QStackedWidget>

QStackedWidget * stackedWidget;

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::MainWindow)
{
    stackedWidget = new QStackedWidget;
    setUpLoginUi();
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUpLoginUi()
{

    LoginWidget * loginWidget = new LoginWidget(this);
}
