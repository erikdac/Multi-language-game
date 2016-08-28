#include "stackedwidget.h"

#include <QEventLoop>
#include <cassert>

StackedWidget::StackedWidget(QWidget * parent)
    : QStackedWidget(parent) {

    QObject::connect(
        this, SIGNAL(executeChange(const unsigned int)),
        this, SLOT(changeUi(const unsigned int))
    );
}

void StackedWidget::addState(GameState * gameState) {
    _gameStates.push_back(gameState);
    this->addWidget(gameState);
}

void StackedWidget::changeToState(const unsigned int index) {
    QEventLoop loop;
    connect(
        this, SIGNAL(done()),
        &loop, SLOT(quit())
    );
    emit executeChange(index);
    loop.exec();
}

void StackedWidget::changeUi(const unsigned int index) {
    if (index < _gameStates.size()) {
        _gameStates[_index]->pause();
        _index = index;
        _gameStates[_index]->resume();
        this->setCurrentIndex(_index);
    } else {
        assert(false);
    }

    emit done();    // Required
}

GameState * StackedWidget::currentState() {
    return _gameStates[_index];
}
