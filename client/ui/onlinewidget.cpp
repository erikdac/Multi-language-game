#include "onlinewidget.h"
#include "ui_onlinewidget.h"
#include "gamewidget.h"
#include "network/connection.h"
#include "mainwindow.h"
#include "game/objects/player.h"
#include "game/keyboardcontroller.h"
#include "game/screenrefresher.h"
#include "game/map.h"
#include "game/objects/graphics.h"

#include <QGridLayout>
#include <QLabel>
#include <QKeyEvent>

OnlineWidget::OnlineWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OnlineWidget)
{
    ui->setupUi(this);

    QGridLayout * gameLayout = findChild<QGridLayout *>("GameLayout");
    gameLayout->addWidget(new GameWidget(this));

    setFocus();
}

OnlineWidget::~OnlineWidget()
{
    delete ui;
}

void OnlineWidget::input(std::string input) {
    std::string error;
    Json data = Json::parse(input, error);

    std::cout << "ONLINEWIDGET: " << data.dump() << std::endl;

    std::string type = data["Type"].string_value();
    if(type == "Disconnect") {
        logout();
    }
    else if(type == "Map") {
        parse_map(data);
    }
}

void OnlineWidget::logout() {
    GameWidget * gameWidget = findChild<GameWidget*>();
    gameWidget->stop_refreshing();
    connection::disconnect();
    MainWindow *w = dynamic_cast<MainWindow *> (this->parentWidget());
    w->setUpLoginUi();
}

void OnlineWidget::keyPressEvent(QKeyEvent *event) {

    if(event->isAutoRepeat()) {
        return;
    }

    switch(event->key()) {
        case Qt::Key_W:
            setKeyboardController('w');
        break;
        case Qt::Key_A:
            setKeyboardController('a');
        break;
        case Qt::Key_S:
            setKeyboardController('s');
        break;
        case Qt::Key_D:
            setKeyboardController('d');
        break;
        case Qt::Key_Escape:
            openMenu();
        break;
    }
}

void OnlineWidget::setKeyboardController(char key) {
    if(_keyMap.find(key) == _keyMap.end()) {
        KeyboardController * temp = new KeyboardController(key);
        _keyMap[key] = temp;
        temp->start();
    }
}

void OnlineWidget::keyReleaseEvent(QKeyEvent *event) {
    if(event->isAutoRepeat() == false) {
        char c;
        switch(event->key()) {
            case Qt::Key_W: c = 'w'; break;
            case Qt::Key_A: c = 'a'; break;
            case Qt::Key_S: c = 's'; break;
            case Qt::Key_D: c = 'd'; break;
            default: return;
        }

        KeyboardController * temp = _keyMap[c];
        temp->stop();
        _keyMap.erase(c);
    }
}

// TODO: Implement a game menu instead of just logging out.
void OnlineWidget::openMenu() {
    logout();
}
