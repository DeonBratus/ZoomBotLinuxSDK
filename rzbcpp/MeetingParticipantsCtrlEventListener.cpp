#include "MeetingParticipantsCtrlEventListener.h"
#include <iostream>

using namespace std;
#include <fstream>
#include <ctime>
#include <set>

MeetingParticipantsCtrlEventListener::MeetingParticipantsCtrlEventListener(
    void(*onIsHost)(),
    void(*onIsCoHost)(),
    void(*getUserInfo)(unsigned int userid),
    void(*getUserLeft)(unsigned int userid)
    ) {
    onIsHost_ = onIsHost;
    onIsCoHost_ = onIsCoHost;
    getJoinedUser_ = getUserInfo;
    getLeftUser_ = getUserLeft;

}
void MeetingParticipantsCtrlEventListener::onUserJoin(IList<unsigned int>* lstUserID, const zchar_t* strUserList) {
    if (getJoinedUser_) { 
        unsigned int connected_user = lstUserID->GetItem(0);
        getJoinedUser_(connected_user);

    }
}

void MeetingParticipantsCtrlEventListener::onUserLeft(IList<unsigned int>* lstUserID, const zchar_t* strUserList) {
    if (getLeftUser_) { 
        getLeftUser_(lstUserID->GetItem(0));

    }
}

void MeetingParticipantsCtrlEventListener::onHostChangeNotification(unsigned int userId) {
    if (onIsHost_) onIsHost_();
}

void MeetingParticipantsCtrlEventListener::onLowOrRaiseHandStatusChanged(bool bLow, unsigned int userid) {
    // Реализация метода
}

void MeetingParticipantsCtrlEventListener::onUserNamesChanged(IList<unsigned int>* lstUserID) {
    // Реализация метода
}

void MeetingParticipantsCtrlEventListener::onCoHostChangeNotification(unsigned int userId, bool isCoHost) {
    if (onIsCoHost_) onIsCoHost_();
}

void MeetingParticipantsCtrlEventListener::onInvalidReclaimHostkey() {
    // Реализация метода
}

void MeetingParticipantsCtrlEventListener::onAllHandsLowered() {
    // Реализация метода
}

void MeetingParticipantsCtrlEventListener::onLocalRecordingStatusChanged(unsigned int user_id, RecordingStatus status) {
    std::cout << "Local recording status changed" << std::endl;
}

void MeetingParticipantsCtrlEventListener::onAllowParticipantsRenameNotification(bool bAllow) {
    // Реализация метода
}

void MeetingParticipantsCtrlEventListener::onAllowParticipantsUnmuteSelfNotification(bool bAllow) {
    // Реализация метода
}

void MeetingParticipantsCtrlEventListener::onAllowParticipantsStartVideoNotification(bool bAllow) {
    // Реализация метода
}

void MeetingParticipantsCtrlEventListener::onAllowParticipantsShareWhiteBoardNotification(bool bAllow) {
    // Реализация метода
}

void MeetingParticipantsCtrlEventListener::onRequestLocalRecordingPrivilegeChanged(LocalRecordingRequestPrivilegeStatus status) {
    std::cout << "Local recording privilege changed" << std::endl;
}

void MeetingParticipantsCtrlEventListener::onAllowParticipantsRequestCloudRecording(bool bAllow) {
    // Реализация метода
}

void MeetingParticipantsCtrlEventListener::onInMeetingUserAvatarPathUpdated(unsigned int userID) {
    // Реализация метода
}

void MeetingParticipantsCtrlEventListener::onParticipantProfilePictureStatusChange(bool bHidden) {
    // Реализация метода
}

void MeetingParticipantsCtrlEventListener::onFocusModeStateChanged(bool bEnabled) {
    // Реализация метода
}

void MeetingParticipantsCtrlEventListener::onFocusModeShareTypeChanged(FocusModeShareType type) {
    // Реализация метода
}

void MeetingParticipantsCtrlEventListener::onRobotRelationChanged(unsigned int authorizeUserID) {
    // Реализация метода
}

void MeetingParticipantsCtrlEventListener::onVirtualNameTagStatusChanged(bool bOn, unsigned int userID) {
    // Реализация метода
}

void MeetingParticipantsCtrlEventListener::onVirtualNameTagRosterInfoUpdated(unsigned int userID) {
    // Реализация метода
}