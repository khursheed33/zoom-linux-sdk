// MeetingRecordingCtrlEventListener.h
#pragma once
#include "meeting_service_components/meeting_recording_interface.h"
#include <functional>
#include <iostream>

using namespace ZOOMSDK;

class MeetingRecordingCtrlEventListener : public ZOOMSDK::IMeetingRecordingCtrlEvent {
private:
    std::function<void()> onPermission_;

public:
    MeetingRecordingCtrlEventListener(std::function<void()> cb) : onPermission_(cb) {}

    virtual void onRecordingPermissionRequest(bool bGranted) {
        std::cout << "Recording permission request: " << bGranted << std::endl;
        if (bGranted && onPermission_) onPermission_();
    }

    virtual void onRecordingStatus(ZOOMSDK::RecordingStatus status) {
        std::cout << "Recording status: " << static_cast<int>(status) << std::endl;
    }

    virtual void onCloudRecordingStatus(ZOOMSDK::RecordingStatus status) {
        std::cout << "Cloud recording status: " << static_cast<int>(status) << std::endl;
    }

    virtual void onRecordPrivilegeChanged(bool bCanRec) {
        std::cout << "Record privilege changed: " << bCanRec << std::endl;
    }

    virtual void onLocalRecordingPrivilegeRequestStatus(ZOOMSDK::RequestLocalRecordingStatus status) {
        std::cout << "Local recording privilege request status: " << static_cast<int>(status) << std::endl;
    }

    virtual void onRequestCloudRecordingResponse(ZOOMSDK::RequestStartCloudRecordingStatus status) {
        std::cout << "Request cloud recording response: " << static_cast<int>(status) << std::endl;
    }

    virtual void onLocalRecordingPrivilegeRequested(ZOOMSDK::IRequestLocalRecordingPrivilegeHandler* handler) {
        std::cout << "Local recording privilege requested" << std::endl;
    }

    virtual void onStartCloudRecordingRequested(ZOOMSDK::IRequestStartCloudRecordingHandler* handler) {
        std::cout << "Start cloud recording requested" << std::endl;
    }

    virtual void onCloudRecordingStorageFull(time_t gracePeriodDate) {
        std::cout << "Cloud recording storage full, grace period date: " << gracePeriodDate << std::endl;
    }

    virtual void onEnableAndStartSmartRecordingRequested(ZOOMSDK::IRequestEnableAndStartSmartRecordingHandler* handler) {
        std::cout << "Enable and start smart recording requested" << std::endl;
    }

    virtual void onSmartRecordingEnableActionCallback(ZOOMSDK::ISmartRecordingEnableActionHandler* handler) {
        std::cout << "Smart recording enable action callback" << std::endl;
    }

    virtual void onTranscodingStatusChanged(ZOOMSDK::TranscodingStatus status, const zchar_t* path) {
        std::cout << "Transcoding status changed: " << static_cast<int>(status) << ", path: " << (path ? path : "null") << std::endl;
    }
};