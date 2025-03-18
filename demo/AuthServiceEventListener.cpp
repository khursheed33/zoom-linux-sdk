#include "AuthServiceEventListener.h"
#include <iostream>

using namespace ZOOMSDK;

AuthServiceEventListener::AuthServiceEventListener(std::function<void()> onAuthSuccess)
    : onAuthSuccess_(onAuthSuccess) {}

void AuthServiceEventListener::onAuthenticationReturn(AuthResult ret) {
    std::cout << "onAuthenticationReturn: " << ret << std::endl;
    if (ret == AUTHRET_SUCCESS && onAuthSuccess_) onAuthSuccess_();
}