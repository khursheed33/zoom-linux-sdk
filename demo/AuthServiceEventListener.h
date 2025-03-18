#include "auth_service_interface.h"
#include <functional>

USING_ZOOM_SDK_NAMESPACE

class AuthServiceEventListener : public IAuthServiceEvent {
    std::function<void()> onAuthSuccess_;
public:
    AuthServiceEventListener(std::function<void()> onAuthSuccess) : onAuthSuccess_(onAuthSuccess) {}

    virtual void onAuthenticationReturn(AuthResult ret) {
        if (ret == AUTHRET_SUCCESS && onAuthSuccess_) onAuthSuccess_();
    }
    virtual void onLoginReturnWithReason(LOGINSTATUS ret, IAccountInfo* pAccountInfo, LoginFailReason reason) {}
    virtual void onLogout() {}
    virtual void onZoomIdentityExpired() {}
    virtual void onZoomAuthIdentityExpired() {}
};