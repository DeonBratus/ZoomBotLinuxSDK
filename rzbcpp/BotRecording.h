#ifndef BOT_RECORDING_H
#define BOT_RECORDING_H

#include "ZoomBot.h"
#include "ZoomSDKAudioRawData.h"
#include <meeting_service_components/meeting_recording_interface.h>
#include <zoom_rawdata_api.h>

#include "MeetingServiceEventListener.h"
#include "MeetingParticipantsCtrlEventListener.h"
#include "MeetingRecordingCtrlEventListener.h"

USING_ZOOM_SDK_NAMESPACE

class BotRecording : public ZoomBot {  // Исправлено наследование
public:
    static BotRecording* self;
    IMeetingRecordingController* m_pRecordController;
    IZoomSDKAudioRawDataHelper* audioHelper;
    ZoomSDKAudioRawData* audio_source;
    IMeetingParticipantsController* m_pParticipantsController;

    BotRecording();
    void CheckAndStartRawRecording(bool isAudio);
    void recoCallback();
    void recojoin();
    void recorun();
    void onInMeeting();
    static void onInMeetingStatic();
    static void onIsHost();
    static void onIsCoHost();
    static void onIsGivenRecordingPermission();
};

#endif // BOT_RECORDING_H