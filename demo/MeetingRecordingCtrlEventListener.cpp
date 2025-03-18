#include "MeetingRecordingCtrlEventListener.h"
#include <iostream>

using namespace ZOOMSDK;

MeetingRecordingCtrlEventListener::MeetingRecordingCtrlEventListener(std::function<void()> onIsGivenRecordingPermission)
    : onIsGivenRecordingPermission_(onIsGivenRecordingPermission) {}

void MeetingRecordingCtrlEventListener::onRecordPrivilegeChanged(bool bCanRec) {
    std::cout << "onRecordPrivilegeChanged: " << bCanRec << std::endl;
    if (bCanRec && onIsGivenRecordingPermission_) onIsGivenRecordingPermission_();
}