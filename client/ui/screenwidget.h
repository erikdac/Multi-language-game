#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H

#include <QOpenGLWidget>

namespace Ui {
    class ScreenWidget;
}

class ScreenWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit ScreenWidget(QWidget *parent = 0);
    ~ScreenWidget();

    void resume();
    void pause();

protected:
    void intitializeGL();
    void paintGL();
    void mousePressEvent(QMouseEvent *);

private:
    Ui::ScreenWidget *ui;

    bool _keepRefreshing;
    void screenRefresher();

signals:
    void repaint();
};

#endif
