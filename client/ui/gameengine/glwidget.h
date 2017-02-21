#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QObject>
#include <QOpenGLWidget>

class GLWidget : public QOpenGLWidget {

    Q_OBJECT

    int _max_fps;

public:
    GLWidget(const int, QWidget *);
    virtual ~GLWidget();

    void refresh();

protected:
    bool _called_refresh = false;

    void intitializeGL();

    virtual void paintGL() = 0;

signals:
    void repaint();
    void done();
};

#endif // GLWIDGET_H
