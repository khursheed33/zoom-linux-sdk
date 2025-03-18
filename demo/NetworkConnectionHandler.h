// NetworkConnectionHandler.h
#pragma once
#include "zoom_sdk.h" // Include foundational Zoom SDK header
#include <functional>

using namespace ZOOMSDK;

class NetworkConnectionHandler : public INetworkConnectionHandler {
    std::function<void()> postToDo_;

public:
    NetworkConnectionHandler(std::function<void()> postToDo);

    virtual void onProxyDetectComplete();
    virtual void onProxySettingNotification(IProxySettingHandler* handler);
    virtual void onSSLCertVerifyNotification(ISSLCertVerificationHandler* handler);
};