class NetworkConnectionHandler : public INetworkConnectionHandler {
    std::function<void()> postToDo_;
public:
    NetworkConnectionHandler(std::function<void()> postToDo) : postToDo_(postToDo) {}
    virtual void onProxyDetectComplete() { if (postToDo_) postToDo_(); }
    virtual void onProxySettingNotification(IProxySettingHandler* handler) {}
    virtual void onSSLCertVerifyNotification(ISSLCertVerificationHandler* handler) {}
};