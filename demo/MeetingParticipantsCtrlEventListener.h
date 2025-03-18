#include "zoom_sdk.h"
#include <meeting_service_components/meeting_participants_ctrl_interface.h>
#include <functional>

USING_ZOOM_SDK_NAMESPACE

class MeetingParticipantsCtrlEventListener : public IMeetingParticipantsCtrlEvent {
    std::function<void()> onIsHost_;
    std::function<void()> onIsCoHost_;
public:
    MeetingParticipantsCtrlEventListener(std::function<void()> onIsHost, std::function<void()> onIsCoHost)
        : onIsHost_(onIsHost), onIsCoHost_(onIsCoHost) {}

    virtual void onUserJoin(IList<unsigned int>* lstUserID, const zchar_t* strUserList = NULL) {}
    virtual void onUserLeft(IList<unsigned int>* lstUserID, const zchar_t* strUserList = NULL) {}
    virtual void onHostChangeNotification(unsigned int userId) { if (onIsHost_) onIsHost_(); }
    virtual void onLowOrRaiseHandStatusChanged(bool bLow, unsigned int userid) {}
    virtual void onUserNamesChanged(IList<unsigned int>* lstUserID) {}
    virtual void onCoHostChangeNotification(unsigned int userId, bool isCoHost) { if (isCoHost && onIsCoHost_) onIsCoHost_(); }
    virtual void onInvalidReclaimHostkey() {}
    virtual void onAllHandsLowered() {}
    virtual void onLocalRecordingStatusChanged(unsigned int user_id, RecordingStatus status) {}
    virtual void onAllowParticipantsRenameNotification(bool bAllow) {}
    virtual void onAllowParticipantsUnmuteSelfNotification(bool bAllow) {}
    virtual void onAllowParticipantsStartVideoNotification(bool bAllow) {}
    virtual void onAllowParticipantsShareWhiteBoardNotification(bool bAllow) {}
    virtual void onRequestLocalRecordingPrivilegeChanged(LocalRecordingRequestPrivilegeStatus status) {}
    virtual void onAllowParticipantsRequestCloudRecording(bool bAllow) {}
    virtual void onInMeetingUserAvatarPathUpdated(unsigned int userID) {}
    virtual void onParticipantProfilePictureStatusChange(bool bHidden) {}
    virtual void onFocusModeStateChanged(bool bEnabled) {}
    virtual void onFocusModeShareTypeChanged(FocusModeShareType type) {}
};