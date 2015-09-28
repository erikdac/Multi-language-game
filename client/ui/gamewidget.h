#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>

namespace Ui {
    class GameWidget;
}

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = 0);
    ~GameWidget();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::GameWidget *ui;

    void openMenu();

public slots:
    void input(std::string result);

};
#endif
