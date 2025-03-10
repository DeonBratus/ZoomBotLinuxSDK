#include <meeting_service_components/meeting_recording_interface.h>
#include "zoom_sdk.h"

USING_ZOOM_SDK_NAMESPACE

class MeetingRecordingCtrlEventListener : public IMeetingRecordingCtrlEvent {
    void (*onIsGivenRecordingPermission_)();

public:
    MeetingRecordingCtrlEventListener(void (*onIsGivenRecordingPermission)());

    // Методы без "= 0", так как они реализованы в .cpp файле
    virtual void onRecordingStatus(RecordingStatus status) override;

    virtual void onCloudRecordingStatus(RecordingStatus status) override;

    virtual void onRecordPrivilegeChanged(bool bCanRec) override;

    virtual void onLocalRecordingPrivilegeRequestStatus(RequestLocalRecordingStatus status) override;

    virtual void onRequestCloudRecordingResponse(RequestStartCloudRecordingStatus status) override;

    virtual void onLocalRecordingPrivilegeRequested(IRequestLocalRecordingPrivilegeHandler* handler) override;
    
    virtual void onStartCloudRecordingRequested(IRequestStartCloudRecordingHandler* handler) override;

	virtual void onCloudRecordingStorageFull(time_t gracePeriodDate) override;

	virtual void onEnableAndStartSmartRecordingRequested(IRequestEnableAndStartSmartRecordingHandler* handler) override;

	virtual void onSmartRecordingEnableActionCallback(ISmartRecordingEnableActionHandler* handler) override;
	
	virtual void onTranscodingStatusChanged(TranscodingStatus status,const zchar_t* path) override;
};