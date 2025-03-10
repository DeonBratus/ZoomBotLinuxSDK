#include "MeetingRecordingCtrlEventListener.h"

using namespace std;

MeetingRecordingCtrlEventListener::MeetingRecordingCtrlEventListener(void (*onIsGivenRecordingPermission)()) {
    onIsGivenRecordingPermission_ = onIsGivenRecordingPermission;
}

void MeetingRecordingCtrlEventListener::onRecordingStatus(RecordingStatus status) {
    // Реализация метода
}

void MeetingRecordingCtrlEventListener::onCloudRecordingStatus(RecordingStatus status) {
    // Реализация метода
}

void MeetingRecordingCtrlEventListener::onRecordPrivilegeChanged(bool bCanRec) {
    if (bCanRec) {
        if (onIsGivenRecordingPermission_) onIsGivenRecordingPermission_();
    }
}

void MeetingRecordingCtrlEventListener::onLocalRecordingPrivilegeRequestStatus(RequestLocalRecordingStatus status) {
    // Реализация метода
}

void MeetingRecordingCtrlEventListener::onRequestCloudRecordingResponse(RequestStartCloudRecordingStatus status) {
    // Реализация метода
}

void MeetingRecordingCtrlEventListener::onLocalRecordingPrivilegeRequested(IRequestLocalRecordingPrivilegeHandler* handler) {
    // Реализация метода
}

void MeetingRecordingCtrlEventListener::onStartCloudRecordingRequested(IRequestStartCloudRecordingHandler* handler) {
    // Реализация метода
}

void MeetingRecordingCtrlEventListener::onCloudRecordingStorageFull(time_t gracePeriodDate){};

void MeetingRecordingCtrlEventListener::onEnableAndStartSmartRecordingRequested(IRequestEnableAndStartSmartRecordingHandler* handler){};

void MeetingRecordingCtrlEventListener::onSmartRecordingEnableActionCallback(ISmartRecordingEnableActionHandler* handler){};

void MeetingRecordingCtrlEventListener::onTranscodingStatusChanged(TranscodingStatus status,const zchar_t* path){};