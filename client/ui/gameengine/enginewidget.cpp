#include "enginewidget.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <cassert>

EngineWidget::EngineWidget(const int max_update_rate, QWidget * parent)
    : StackedWidget(parent)
    , _max_update_rate(max_update_rate)
    , _isRunning(false)
    , _nextState("")
{

}

EngineWidget::~EngineWidget() {
    _isRunning = false;
}

void EngineWidget::run() {
    _isRunning = true;
    std::thread(&EngineWidget::gameLoop, this).detach();
}

void EngineWidget::gameLoop() {
    assert(_isRunning == true);
    while (_isRunning) {
        auto begin = std::chrono::high_resolution_clock::now();
        if (_nextState.size() > 0) {
            changeToState(_nextState);
            _nextState = "";
        }
        currentState()->process();
        auto end = std::chrono::high_resolution_clock::now();
        int diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        int delay = (1000/_max_update_rate) - diff;
        std::this_thread::sleep_for (std::chrono::milliseconds(delay));
    }
}

void EngineWidget::prepareState(const std::string name) {
    _nextState = name;
}
