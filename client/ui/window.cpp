#include "ui_window.h"
#include "window.h"
#include "loginwidget.h"
#include "onlinewidget.h"

#include <iostream>
#include <chrono>
#include "unistd.h"

static const unsigned int MAX_FPS = 30;

Window::Window(QWidget * parent)
    : StackedWidget(parent)
    , ui(new Ui::Window)
{
    ui->setupUi(this);

    addState(new LoginWidget(this));
    addState(new OnlineWidget(this));

    QObject::connect(
                this, SIGNAL(setIndex(unsigned int)),
                this, SLOT(setIndex(unsigned int))
    );

    setLoginUi();
    std::thread(&Window::gameLoop, this).detach();
}

Window::~Window() {
    delete ui;
    _isRunning = false;
}

void Window::gameLoop() {
    _isRunning = true;
    while (_isRunning) {
        auto begin = std::chrono::high_resolution_clock::now();
        if (_nextIndex != -1) {
            emit setIndex(_nextIndex);
            _nextIndex = -1;
        }
        currentState()->process();
        QCoreApplication::processEvents();
        auto end = std::chrono::high_resolution_clock::now();
        int diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        usleep(((1000/MAX_FPS) - diff) * 1000);
    }
}

void Window::setLoginUi() {
    _nextIndex = 0;
}

void Window::setGameUi() {
    _nextIndex = 1;
}
