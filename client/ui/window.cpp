#include "ui_window.h"
#include "window.h"
#include "loginwidget.h"
#include "screenwidget.h"
#include "network/connection.h"
#include "onlinewidget.h"

#include <iostream>
#include <QStackedWidget>
#include <QGridLayout>

Window::Window(QWidget * parent)
    : StackedWidget(parent)
    , ui(new Ui::Window)
{
    ui->setupUi(this);

    addState(LoginWidget::instance());
    addState(OnlineWidget::instance());

    setUpLoginUi();
}

Window::~Window() {
    delete ui;
}

void Window::setUpLoginUi() {
    setIndex(0);
}

void Window::setUpGameUi() {
    setIndex(1);
}
