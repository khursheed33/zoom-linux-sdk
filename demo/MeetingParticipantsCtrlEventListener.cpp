// MeetingParticipantsCtrlEventListener.cpp
#include "MeetingParticipantsCtrlEventListener.h"
#include <iostream>

using namespace ZOOMSDK;

MeetingParticipantsCtrlEventListener::MeetingParticipantsCtrlEventListener(std::function<void()> onIsHost,
                                                                           std::function<void()> onIsCoHost)
    : onIsHost_(onIsHost), onIsCoHost_(onIsCoHost) {}

void MeetingParticipantsCtrlEventListener::onUserJoin(ZOOMSDK::IList<unsigned int>* pUserList) {
    std::cout << "User joined event" << std::endl;
}

void MeetingParticipantsCtrlEventListener::onUserLeft(ZOOMSDK::IList<unsigned int>* pUserList) {
    std::cout << "User left event" << std::endl;
}

void MeetingParticipantsCtrlEventListener::onHostChangeNotification(unsigned int userId) {
    std::cout << "onHostChangeNotification: " << userId << std::endl;
    if (onIsHost_) onIsHost_();
}

void MeetingParticipantsCtrlEventListener::onCoHostChangeNotification(unsigned int userId, bool isCoHost) {
    std::cout << "onCoHostChangeNotification: " << userId << ", isCoHost: " << isCoHost << std::endl;
    if (isCoHost && onIsCoHost_) onIsCoHost_();
}

void MeetingParticipantsCtrlEventListener::onLowOrRaiseHandStatusChanged(bool bLow, unsigned int userid) {
    std::cout << "Hand status changed for user " << userid << ": " << (bLow ? "low" : "raised") << std::endl;
}

void MeetingParticipantsCtrlEventListener::onUserNameChanged(ZOOMSDK::IList<unsigned int>* pUserList) {
    std::cout << "User name changed event" << std::endl;
}