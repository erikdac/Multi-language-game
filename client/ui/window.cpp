#include "ui_window.h"
#include "window.h"
#include "loginwidget.h"
#include "onlinewidget.h"

Window::Window(QWidget * parent)
    : StackedWidget(parent)
    , ui(new Ui::Window)
{
    ui->setupUi(this);

    addState(new LoginWidget(this));
    addState(new OnlineWidget(this));

    setLoginUi();
}

Window::~Window() {
    delete ui;
}

void Window::setLoginUi() {
    setIndex(0);
}

void Window::setGameUi() {
    setIndex(1);
}
