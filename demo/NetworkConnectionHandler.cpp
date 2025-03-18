#include "NetworkConnectionHandler.h"
#include <iostream>

using namespace ZOOMSDK;

NetworkConnectionHandler::NetworkConnectionHandler(std::function<void()> postToDo)
    : postToDo_(postToDo) {}

void NetworkConnectionHandler::onProxyDetectComplete() {
    std::cout << "onProxyDetectComplete" << std::endl;
    if (postToDo_) postToDo_();
}

void NetworkConnectionHandler::onProxySettingNotification(IProxySettingHandler* handler) {
    std::cout << "onProxySettingNotification" << std::endl;
}

void NetworkConnectionHandler::onSSLCertVerifyNotification(ISSLCertVerificationHandler* handler) {
    std::cout << "onSSLCertVerifyNotification" << std::endl;
}