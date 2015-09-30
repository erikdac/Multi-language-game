#include "screenrefresher.h"

ScreenRefresher::ScreenRefresher() {

}

ScreenRefresher::~ScreenRefresher() {

}

void ScreenRefresher::run() {
    while(true) {
        emit animate();
        usleep(40000);
    }
}
