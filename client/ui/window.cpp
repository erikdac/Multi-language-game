#include "ui_window.h"
#include "window.h"
#include "loginwidget.h"
#include "loadingwidget.h"
#include "gamewidget.h"

#include <iostream>
#include <chrono>
#include <thread>

Window::Window(const int max_update_rate, QWidget * parent)
    : StackedWidget(parent)
    , _max_update_rate(max_update_rate)
    , ui(new Ui::Window)
{
    ui->setupUi(this);

    addState(new LoginWidget(this));
    addState(new LoadingWidget(this));
    addState(new GameWidget(this));

    setLoginUi();
}

Window::~Window() {
    _isRunning = false;
    delete ui;
}

void Window::run() {
    _isRunning = true;
    std::thread(&Window::gameLoop, this).detach();
}

void Window::gameLoop() {
    assert(_isRunning == true);
    while (_isRunning) {
        auto begin = std::chrono::high_resolution_clock::now();
        if (_nextIndex != -1) {
            changeToState(_nextIndex);
            _nextIndex = -1;
        }
        currentState()->process();
        auto end = std::chrono::high_resolution_clock::now();
        int diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        int delay = (1000/_max_update_rate) - diff;
        std::this_thread::sleep_for (std::chrono::milliseconds(delay));
    }
}

void Window::setLoginUi() {
    _nextIndex = 0;
}

void Window::setLoadingUi() {
    _nextIndex = 1;
}

void Window::setGameUi() {
    _nextIndex = 2;
}
