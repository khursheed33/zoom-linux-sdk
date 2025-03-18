// MeetingRecordingCtrlEventListener.h
#pragma once
#include "zoom_sdk.h"
#include "meeting_service_components/meeting_recording_interface.h" // For IMeetingRecordingCtrlEvent
#include <functional>

using namespace ZOOMSDK;

class MeetingRecordingCtrlEventListener : public IMeetingRecordingCtrlEvent {
private:
    std::function<void()> onIsGivenRecordingPermission_;

public:
    MeetingRecordingCtrlEventListener(std::function<void()> onIsGivenRecordingPermission);

    virtual void onRecordingStatus(RecordingStatus status) {}
    virtual void onCloudRecordingStatus(RecordingStatus status) {}
    virtual void onRecordPrivilegeChanged(bool bCanRec); // Declaration only
    virtual void onLocalRecordingPrivilegeRequestStatus(RequestLocalRecordingStatus status) {}
    virtual void onRequestCloudRecordingResponse(RequestStartCloudRecordingStatus status) {}
    virtual void onLocalRecordingPrivilegeRequested(IRequestLocalRecordingPrivilegeHandler* handler) {}
    virtual void onStartCloudRecordingRequested(IRequestStartCloudRecordingHandler* handler) {}
    virtual void onCloudRecordingStorageFull(time_t gracePeriodDate) {}
    virtual void onEnableAndStartSmartRecordingRequested(IRequestEnableAndStartSmartRecordingHandler* handler) {}
    virtual void onSmartRecordingEnableActionCallback(ISmartRecordingEnableActionHandler* handler) {}
    virtual void onTranscodingStatusChanged(TranscodingStatus status, const zchar_t* path) {}
};