// AuthServiceEventListener.h
#pragma once
#include "zoom_sdk.h" // Foundational SDK header
#include "auth_service_interface.h" // Defines IAuthServiceEvent and related types
#include <functional>

using namespace ZOOMSDK;

class AuthServiceEventListener : public IAuthServiceEvent {
private:
    std::function<void()> onAuthSuccess_;

public:
    AuthServiceEventListener(std::function<void()> onAuthSuccess);

    virtual void onAuthenticationReturn(AuthResult ret); // Declaration only
    virtual void onLoginReturnWithReason(LOGINSTATUS ret, IAccountInfo* pAccountInfo, LoginFailReason reason) {}
    virtual void onLogout() {}
    virtual void onZoomIdentityExpired() {}
    virtual void onZoomAuthIdentityExpired() {}
};