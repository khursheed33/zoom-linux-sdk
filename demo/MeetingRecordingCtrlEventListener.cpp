#include "MeetingRecordingCtrlEventListener.h"
#include <functional>

using namespace std;

MeetingRecordingCtrlEventListener::MeetingRecordingCtrlEventListener(std::function<void()> onIsGivenRecordingPermission)
    : onIsGivenRecordingPermission_(onIsGivenRecordingPermission) {}

void MeetingRecordingCtrlEventListener::onRecordingStatus(RecordingStatus status) {}
void MeetingRecordingCtrlEventListener::onCloudRecordingStatus(RecordingStatus status) {}

void MeetingRecordingCtrlEventListener::onRecordPrivilegeChanged(bool bCanRec) {
    if (bCanRec && onIsGivenRecordingPermission_) {
        onIsGivenRecordingPermission_();
    }
}

void MeetingRecordingCtrlEventListener::onLocalRecordingPrivilegeRequestStatus(RequestLocalRecordingStatus status) {}
void MeetingRecordingCtrlEventListener::onRequestCloudRecordingResponse(RequestStartCloudRecordingStatus status) {}
void MeetingRecordingCtrlEventListener::onLocalRecordingPrivilegeRequested(IRequestLocalRecordingPrivilegeHandler* handler) {}
void MeetingRecordingCtrlEventListener::onStartCloudRecordingRequested(IRequestStartCloudRecordingHandler* handler) {}
void MeetingRecordingCtrlEventListener::onCloudRecordingStorageFull(time_t gracePeriodDate) {}
void MeetingRecordingCtrlEventListener::onEnableAndStartSmartRecordingRequested(IRequestEnableAndStartSmartRecordingHandler* handler) {}
void MeetingRecordingCtrlEventListener::onSmartRecordingEnableActionCallback(ISmartRecordingEnableActionHandler* handler) {}
void MeetingRecordingCtrlEventListener::onTranscodingStatusChanged(TranscodingStatus status, const zchar_t* path) {}