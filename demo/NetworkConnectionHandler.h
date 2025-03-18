// NetworkConnectionHandler.h
#include <network_connection_handler_interface.h> // Zoom SDK header
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