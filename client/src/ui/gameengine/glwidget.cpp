#include "glwidget.h"

#include <QEventLoop>
#include <QPainter>
#include <QOpenGLTexture>
#include <QMouseEvent>
#include <thread>
#include <chrono>
#include "unistd.h"

GLWidget::GLWidget(const int max_fps, QWidget * parent)
    : QOpenGLWidget(parent)
    , _max_fps(max_fps)
{

    QObject::connect(this, SIGNAL(repaint()), this, SLOT(repaint()));
}

GLWidget::~GLWidget() {

}

void GLWidget::initializeGL() {
    glClearColor(255, 255, 255, 1);
}

auto last_refresh = std::chrono::high_resolution_clock::now();

void GLWidget::refresh() {
    auto now = std::chrono::high_resolution_clock::now();
    int diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_refresh).count();
    if ((1000/_max_fps) - diff > 0) {
        return;
    }

    QEventLoop loop;
    connect(
        this, SIGNAL(done()),
        &loop, SLOT(quit())
    );
    _called_refresh = true;
    emit repaint();
    loop.exec();

    last_refresh = std::chrono::high_resolution_clock::now();
}
