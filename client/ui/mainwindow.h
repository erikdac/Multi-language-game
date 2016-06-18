#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStackedWidget>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QStackedWidget {

    Q_OBJECT

public:
    explicit MainWindow(QStackedWidget *parent = 0);
    ~MainWindow();
    void setUpLoginUi();
    void setUpGameUi();

private:
    Ui::MainWindow *ui;
};

#endif
