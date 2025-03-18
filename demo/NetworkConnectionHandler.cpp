#include <iostream>
#include "NetworkConnectionHandler.h"
#include <functional>

using namespace std;

NetworkConnectionHandler::NetworkConnectionHandler(std::function<void()> postToDo)
    : postToDo_(postToDo) {}

void NetworkConnectionHandler::onProxyDetectComplete() {
    std::cout << "onProxyDetectComplete" << endl;
    if (postToDo_) postToDo_();
}

void NetworkConnectionHandler::onProxySettingNotification(IProxySettingHandler* handler) {
    std::cout << "onProxySettingNotification" << endl;
}

void NetworkConnectionHandler::onSSLCertVerifyNotification(ISSLCertVerificationHandler* handler) {
    std::cout << "onSSLCertVerifyNotification" << endl;
}