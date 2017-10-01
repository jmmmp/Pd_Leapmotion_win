#include "m_pd.h"
#include "Dispatcher.h"

using namespace Leap;

void Dispatcher::onInit(const Controller& controller) {
    post("Leap Motion for Pd Ver 0.2 for windows");
}

void Dispatcher::onConnect(const Controller& controller) {
    post("Leap:connected");
}

void Dispatcher::onDisconnect(const Controller& controller) {
    post("Leap:disconnected");
}

void Dispatcher::onExit(const Controller& controller) {
    post("Leap:exited");
}

void Dispatcher::onFrame(const Controller& controller) {
    frame = controller.frame();
}
