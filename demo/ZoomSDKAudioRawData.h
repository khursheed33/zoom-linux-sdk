#include "rawdata/rawdata_audio_helper_interface.h"
#include "zoom_sdk.h"
#include "zoom_sdk_raw_data_def.h"
#include <functional>

USING_ZOOM_SDK_NAMESPACE

class ZoomSDKAudioRawData : public IZoomSDKAudioRawDataDelegate {
    std::function<void(const char*, unsigned int)> audioCallback_;
public:
    void setAudioCallback(std::function<void(const char*, unsigned int)> callback) {
        audioCallback_ = callback;
    }

    virtual void onMixedAudioRawDataReceived(AudioRawData* data_) {
        if (audioCallback_ && data_) {
            audioCallback_(data_->GetBuffer(), data_->GetBufferLen());
        }
    }
    virtual void onOneWayAudioRawDataReceived(AudioRawData* data_, uint32_t node_id) {}
    virtual void onShareAudioRawDataReceived(AudioRawData* data_) {}
    virtual void onOneWayInterpreterAudioRawDataReceived(AudioRawData* data_, const zchar_t* pLanguageName) {}
};