#include <meeting_service_components/meeting_recording_interface.h>
#include "zoom_sdk.h"
#include <functional>

USING_ZOOM_SDK_NAMESPACE

class MeetingRecordingCtrlEventListener : public IMeetingRecordingCtrlEvent {
    std::function<void()> onIsGivenRecordingPermission_;
public:
    MeetingRecordingCtrlEventListener(std::function<void()> onIsGivenRecordingPermission)
        : onIsGivenRecordingPermission_(onIsGivenRecordingPermission) {}

    virtual void onRecordingStatus(RecordingStatus status) {}
    virtual void onCloudRecordingStatus(RecordingStatus status) {}
    virtual void onRecordPrivilegeChanged(bool bCanRec) {
        if (bCanRec && onIsGivenRecordingPermission_) {
            onIsGivenRecordingPermission_();
        }
    }
    virtual void onLocalRecordingPrivilegeRequestStatus(RequestLocalRecordingStatus status) {}
    virtual void onRequestCloudRecordingResponse(RequestStartCloudRecordingStatus status) {}
    virtual void onLocalRecordingPrivilegeRequested(IRequestLocalRecordingPrivilegeHandler* handler) {}
    virtual void onStartCloudRecordingRequested(IRequestStartCloudRecordingHandler* handler) {}
    virtual void onCloudRecordingStorageFull(time_t gracePeriodDate) {}
    virtual void onEnableAndStartSmartRecordingRequested(IRequestEnableAndStartSmartRecordingHandler* handler) {}
    virtual void onSmartRecordingEnableActionCallback(ISmartRecordingEnableActionHandler* handler) {}
    virtual void onTranscodingStatusChanged(TranscodingStatus status, const zchar_t* path) {}
};