#include "MeetingParticipantsCtrlEventListener.h"
#include <iostream>

using namespace ZOOMSDK;

MeetingParticipantsCtrlEventListener::MeetingParticipantsCtrlEventListener(std::function<void()> onIsHost,
                                                                           std::function<void()> onIsCoHost)
    : onIsHost_(onIsHost), onIsCoHost_(onIsCoHost) {}

void MeetingParticipantsCtrlEventListener::onHostChangeNotification(unsigned int userId) {
    std::cout << "onHostChangeNotification: " << userId << std::endl;
    if (onIsHost_) onIsHost_();
}

void MeetingParticipantsCtrlEventListener::onCoHostChangeNotification(unsigned int userId, bool isCoHost) {
    std::cout << "onCoHostChangeNotification: " << userId << ", isCoHost: " << isCoHost << std::endl;
    if (isCoHost && onIsCoHost_) onIsCoHost_();
}