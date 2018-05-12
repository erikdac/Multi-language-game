#include "stackedwidget.h"

#include <QEventLoop>
#include <cassert>
#include <map>

StackedWidget::StackedWidget(QWidget * parent)
    : QStackedWidget(parent) {

    QObject::connect(
        this, SIGNAL(executeChange(const int)),
        this, SLOT(changeUi(const int))
    );
}

StackedWidget::~StackedWidget() {

}

void StackedWidget::addState(const std::string name, GameState * gameState) {
    _gameStates[name] = this->count();
    this->addWidget(gameState);
}

void StackedWidget::changeToState(const std::string name) {
    assert(_gameStates.count(name));

    QEventLoop loop;
    connect(
        this, SIGNAL(done()),
        &loop, SLOT(quit())
    );
    emit executeChange(_gameStates[name]);
    loop.exec();
}

void StackedWidget::changeUi(const int index) {
    if (index < this->count()) {
        state(_index)->pause();
        _index = index;
        state(_index)->resume();
        this->setCurrentIndex(_index);
    } else {
        assert(false);
    }

    emit done();    // Required
}

GameState * StackedWidget::currentState() {
    return state(_index);
}

GameState * StackedWidget::state(const int index) {
    return static_cast<GameState *>(widget(index));
}
