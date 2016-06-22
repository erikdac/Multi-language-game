#include "stackedwidget.h"

StackedWidget::StackedWidget(QWidget * parent)
    : QStackedWidget(parent) {

}

void StackedWidget::addState(GameState * gameState) {
    gameState->init(this->parentWidget());
    _gameStates.push_back(gameState);
    this->addWidget(gameState);
}

void StackedWidget::setIndex(unsigned int index) {
    if (index < _gameStates.size()) {
        _gameStates[_index]->pause();
        _index = index;
        this->setCurrentIndex(_index);
        _gameStates[_index]->resume();
    }
}
