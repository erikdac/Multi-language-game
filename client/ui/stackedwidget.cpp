#include "stackedwidget.h"

#include <iostream>

StackedWidget::StackedWidget(QWidget * parent)
    : QStackedWidget(parent) {

}

void StackedWidget::addState(GameState * gameState) {
    _gameStates.push_back(gameState);
    this->addWidget(gameState);
}

void StackedWidget::setIndex(unsigned int index) {
    if (index < _gameStates.size()) {
        _gameStates[_index]->pause();
        _index = index;
        _gameStates[_index]->resume();
        this->setCurrentIndex(_index);
    }
}

GameState * StackedWidget::currentState() {
    return _gameStates[_index];
}
