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

protected:
    void intitializeGL();
    void paintGL();
    void mousePressEvent(QMouseEvent *);

private:
    Ui::ScreenWidget *ui;

signals:
    void repaint();
};

#endif
