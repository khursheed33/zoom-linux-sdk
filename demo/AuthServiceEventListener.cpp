#include "AuthServiceEventListener.h"
#include <iostream>
#include <functional>

AuthServiceEventListener::AuthServiceEventListener(std::function<void()> onAuthSuccess)
    : onAuthSuccess_(onAuthSuccess) {}

void AuthServiceEventListener::onAuthenticationReturn(ZOOM_SDK_NAMESPACE::AuthResult ret) {
    if (ret == ZOOM_SDK_NAMESPACE::AUTHRET_SUCCESS) {
        std::cout << "Auth succeeded: JWT." << std::endl;
        if (onAuthSuccess_) onAuthSuccess_();
    } else {
        std::cout << "Auth failed: " << ret << std::endl;
    }
}

void AuthServiceEventListener::onLoginReturnWithReason(LOGINSTATUS ret, IAccountInfo* pAccountInfo, LoginFailReason reason) {}
void AuthServiceEventListener::onLogout() {}
void AuthServiceEventListener::onZoomIdentityExpired() {}
void AuthServiceEventListener::onZoomAuthIdentityExpired() {}