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
};