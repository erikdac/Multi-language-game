#include "ui_screenwidget.h"
#include "screenwidget.h"
#include "config.h"
#include "window.h"
#include "gamewidget.h"
#include "network/connection.h"
#include "game/objects/player.h"
#include "game/map.h"

#include <iostream>
#include <QPainter>
#include <QOpenGLTexture>
#include <GL/glut.h>
#include <QMouseEvent>
#include <thread>
#include <chrono>
#include "unistd.h"

ScreenWidget::ScreenWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , ui(new Ui::ScreenWidget)
{
    ui->setupUi(this);

    QObject::connect(this, SIGNAL(repaint()), this, SLOT(repaint()));
}

ScreenWidget::~ScreenWidget() {
    delete ui;
}

void ScreenWidget::setMouseHandler(EventHandler<QMouseEvent *> * mouseHandler) {
    _mouseHandler = mouseHandler;
}

void ScreenWidget::intitializeGL() {
    glClearColor(255, 255, 255, 1);
}

void ScreenWidget::mousePressEvent(QMouseEvent * event) {
    if (_mouseHandler != 0) {
        _mouseHandler->addEvent(event);
    }
}

std::chrono::high_resolution_clock::time_point last_refresh = std::chrono::high_resolution_clock::now();

void ScreenWidget::refresh() {
    auto now = std::chrono::high_resolution_clock::now();
    int diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_refresh).count();
    if ((1000/MAX_FPS) - diff > 0) {
        return;
    }

    QEventLoop loop;
    connect(
        this, SIGNAL(done()),
        &loop, SLOT(quit())
    );
    emit repaint();
    loop.exec();
}

// Should only be called by refresh().
void ScreenWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (const Environment * e : _environment) {
        e->load_graphics();
    }

    for (const Actor * a : _actors) {
        a->load_graphics();
    }

    _self->load_graphics();

    emit done();
}
