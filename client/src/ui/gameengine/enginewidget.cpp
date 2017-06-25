#include "enginewidget.h"

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
        if (_nextState.empty() == false) {
            changeToState(_nextState);
            _nextState = "";
        }
        currentState()->tick(0.01); // TODO: should be time between two calls.
        int delay = (1000/_max_update_rate) - timeSince(begin);
        std::this_thread::sleep_for (std::chrono::milliseconds(delay));
    }
}

int EngineWidget::timeSince(TimePoint begin) const {
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}

void EngineWidget::prepareState(const std::string name) {
    _nextState = name;
}
