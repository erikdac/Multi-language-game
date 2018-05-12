#include "ui_window.h"
#include "window.h"
#include "loginwidget.h"
#include "loadingwidget.h"
#include "gamewidget.h"
#include "config.h"
#include "game/parser.h"

#include <QtDebug>

static const std::string LOGIN      = "login";
static const std::string LOADING    = "loading";
static const std::string GAME       = "game";

Window::Window(QWidget * parent)
    : EngineWidget(MAX_UPDATE_RATE, parent)
    , ui(new Ui::Window)
{
    ui->setupUi(this);

    addState(LOGIN, new LoginWidget(this));
    addState(LOADING, new LoadingWidget(this));
    addState(GAME, new GameWidget(this));

    parser::initialize();

    setLoginUi();

    this->run();
}

Window::~Window() {
    delete ui;
}

void Window::setLoginUi() {
    qInfo("Setting login UI");
    prepareState(LOGIN);
}

void Window::setLoadingUi() {
    qInfo("Setting loading UI");
    prepareState(LOADING);
}

void Window::setGameUi() {
    qInfo("Setting game UI");
    prepareState(GAME);
}
