#include <QtWidgets>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connection.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent ( QCloseEvent * event )
{
    disconnect();
    event->accept();
}
