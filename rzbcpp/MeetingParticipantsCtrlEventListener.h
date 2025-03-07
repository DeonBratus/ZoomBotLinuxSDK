
#include "zoom_sdk.h"
#include <meeting_service_interface.h>
#include <meeting_service_components/meeting_audio_interface.h>
#include <meeting_service_components/meeting_participants_ctrl_interface.h>
USING_ZOOM_SDK_NAMESPACE
class MeetingParticipantsCtrlEventListener : public IMeetingParticipantsCtrlEvent {
    void (*onIsHost_)();
    void (*onIsCoHost_)();

public:
    MeetingParticipantsCtrlEventListener(void (*onIsHost)(), void (*onIsCoHost)());

    // Реализованные методы
    virtual void onUserJoin(IList<unsigned int >* lstUserID, const zchar_t* strUserList = nullptr) override;
    virtual void onUserLeft(IList<unsigned int >* lstUserID, const zchar_t* strUserList = nullptr) override;
    virtual void onHostChangeNotification(unsigned int userId) override;
    virtual void onLowOrRaiseHandStatusChanged(bool bLow, unsigned int userid) override;
    virtual void onUserNamesChanged(IList<unsigned int>* lstUserID) override;
    virtual void onCoHostChangeNotification(unsigned int userId, bool isCoHost) override;
    virtual void onInvalidReclaimHostkey() override;
    virtual void onAllHandsLowered() override;

    // Остальные методы
    virtual void onLocalRecordingStatusChanged(unsigned int user_id, RecordingStatus status) override;
    virtual void onAllowParticipantsRenameNotification(bool bAllow) override;
    virtual void onAllowParticipantsUnmuteSelfNotification(bool bAllow) override;
    virtual void onAllowParticipantsStartVideoNotification(bool bAllow) override;
    virtual void onAllowParticipantsShareWhiteBoardNotification(bool bAllow) override;
    virtual void onRequestLocalRecordingPrivilegeChanged(LocalRecordingRequestPrivilegeStatus status) override;
    virtual void onAllowParticipantsRequestCloudRecording(bool bAllow) override;
    virtual void onInMeetingUserAvatarPathUpdated(unsigned int userID) override;
    virtual void onParticipantProfilePictureStatusChange(bool bHidden) override;
    virtual void onFocusModeStateChanged(bool bEnabled) override;
    virtual void onFocusModeShareTypeChanged(FocusModeShareType type) override;
    virtual void onRobotRelationChanged(unsigned int authorizeUserID) override;
    virtual void onVirtualNameTagStatusChanged(bool bOn, unsigned int userID) override;
    virtual void onVirtualNameTagRosterInfoUpdated(unsigned int userID) override;
};