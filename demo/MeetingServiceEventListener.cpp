#include "MeetingServiceEventListener.h"
#include <rawdata/zoom_rawdata_api.h>
#include <iostream>

using namespace ZOOMSDK; // Use the namespace locally

MeetingServiceEventListener::MeetingServiceEventListener(std::function<void()> onMeetingStarts,
                                                         std::function<void()> onMeetingEnds,
                                                         std::function<void()> onInMeeting)
    : onMeetingStarts_(onMeetingStarts), onMeetingEnds_(onMeetingEnds), onInMeeting_(onInMeeting) {}

void MeetingServiceEventListener::onMeetingStatusChanged(MeetingStatus status, int iResult) {
    std::cout << "onMeetingStatusChanged: " << status << ", iResult: " << iResult << std::endl;
    switch (status) {
    case MEETING_STATUS_INMEETING:
        printf("onMeetingStatusChanged() In Meeting.\n");
        if (onInMeeting_) onInMeeting_();
        break;
    case MEETING_STATUS_ENDED:
        printf("Meeting ends.\n");
        if (onMeetingEnds_) onMeetingEnds_();
        break;
    case MEETING_STATUS_CONNECTING:
        if (onMeetingStarts_) onMeetingStarts_();
        break;
    default:
        break;
    }
}