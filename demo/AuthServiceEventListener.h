#pragma once
#include "auth_service_interface.h"
#include <functional>
#include <iostream>

class AuthServiceEventListener : public ZOOMSDK::IAuthServiceEvent {
private:
    std::function<void()> onAuthComplete_;
public:
    AuthServiceEventListener(std::function<void()> cb) : onAuthComplete_(cb) {}
    virtual void onAuthenticationReturn(ZOOMSDK::AuthResult ret) {
        std::cout << "Auth result received: " << static_cast<int>(ret) << std::endl;
        if (ret == ZOOMSDK::AUTHRET_SUCCESS && onAuthComplete_) {
            std::cout << "Authentication successful, calling callback" << std::endl;
            onAuthComplete_();
        } else {
            std::cerr << "Authentication failed with result: " << static_cast<int>(ret) << std::endl;
        }
    }
    virtual void onLoginReturnWithReason(ZOOMSDK::LOGINSTATUS ret, ZOOMSDK::IAccountInfo* pAccountInfo, ZOOMSDK::LoginFailReason reason) {
        std::cout << "Login status: " << static_cast<int>(ret) << ", Reason: " << static_cast<int>(reason) << std::endl;
    }
    virtual void onLogout() { std::cout << "Logout event received" << std::endl; }
    virtual void onZoomIdentityExpired() { std::cout << "Zoom identity expired" << std::endl; }
    virtual void onZoomAuthIdentityExpired() { std::cout << "Zoom auth identity expired" << std::endl; }
};