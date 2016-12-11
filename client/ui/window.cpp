#include "ui_window.h"
#include "window.h"
#include "loginwidget.h"
#include "loadingwidget.h"
#include "gamewidget.h"
#include "config.h"

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

    setLoginUi();

    this->run();
}

Window::~Window() {
    delete ui;
}

void Window::setLoginUi() {
    prepareState(LOGIN);
}

void Window::setLoadingUi() {
    prepareState(LOADING);
}

void Window::setGameUi() {
    prepareState(GAME);
}
