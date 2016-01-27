#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QWidget {

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setUpLoginUi();
    void setUpGameUi();

private:
    Ui::MainWindow *ui;
    void changeWidget(QWidget *);
    void removePreviousUi();
};

#endif