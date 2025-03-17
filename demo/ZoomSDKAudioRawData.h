//GetAudioRawData
#include "rawdata/rawdata_audio_helper_interface.h"
#include "zoom_sdk.h"
#include "zoom_sdk_raw_data_def.h"

USING_ZOOM_SDK_NAMESPACE

class ZoomSDKAudioRawData :
	public IZoomSDKAudioRawDataDelegate
{
public:
	virtual void onMixedAudioRawDataReceived(AudioRawData* data_);
	virtual void onOneWayAudioRawDataReceived(AudioRawData* data_, uint32_t node_id);
	virtual void onShareAudioRawDataReceived(AudioRawData* data_);
	virtual void onOneWayInterpreterAudioRawDataReceived(AudioRawData* data_, const zchar_t* pLanguageName);
};