#include "ui_screenwidget.h"
#include "screenwidget.h"
#include "config.h"
#include "window.h"
#include "gamewidget.h"
#include "network/connection.h"
#include "game/gamestruct.h"
#include "gameengine/glwidget.h"

#include "iostream"
#include <QPainter>
#include <QOpenGLTexture>
#include <GL/glut.h>
#include <QMouseEvent>

ScreenWidget::ScreenWidget(QWidget *parent)
    : GLWidget(MAX_FPS, parent)
    , ui(new Ui::ScreenWidget)
{
    ui->setupUi(this);
}

ScreenWidget::~ScreenWidget() {
    delete ui;
}

void ScreenWidget::setMouseHandler(EventHandler<QMouseEvent *> * mouseHandler) {
    _mouseHandler = mouseHandler;
}

void ScreenWidget::mousePressEvent(QMouseEvent * event) {
    if (_mouseHandler != 0) {
        _mouseHandler->addEvent(event);
    }
}

// Should only be called by refresh().
// PROBLEM: Is called whenever stuff such as resize happens...
void ScreenWidget::paintGL() {
    // TODO: Find a way to remove ugly-hack.
    if (!_called_refresh) {
        return;
    }
    _called_refresh = false;

    glClear(GL_COLOR_BUFFER_BIT);

    for (Environment & e : gamestruct::environment()) {
        e.update();
        e.draw();
    }

    for (auto a : gamestruct::actors()) {
        a->update();
        a->draw();
    }

    gamestruct::self()->update();
    gamestruct::self()->draw();

    emit done();
}
