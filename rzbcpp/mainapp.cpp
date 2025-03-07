#include <ZoomBot.h>

#include <rawdata_audio_helper_interface.h>
#include <meeting_service_components/meeting_recording_interface.h>
#include <zoom_rawdata_api.h>

#include <ZoomSDKAudioRawData.h>
#include <MeetingRecordingCtrlEventListener.h>
#include <MeetingServiceEventListener.h>
#include <MeetingParticipantsCtrlEventListener.h>
#include <MeetingReminderEventListener.h>

USING_ZOOM_SDK_NAMESPACE


class BotRecording: public ZoomBot::ZoomBot {
    public:
    
        static BotRecording* r_instance;

        BotRecording(){r_instance = this;};

        IMeetingRecordingController* m_pRecordController;
        IZoomSDKAudioRawDataHelper* audioHelper;
        ZoomSDKAudioRawData* audio_source = new ZoomSDKAudioRawData();
        IMeetingParticipantsController* m_pParticipantsController;
        //check if you have permission to start raw recording
    void CheckAndStartRawRecording(bool isAudio) {

        if (isAudio) {
            m_pRecordController = meeting_service_->GetMeetingRecordingController();
            SDKError err2 = meeting_service_->GetMeetingRecordingController()->CanStartRawRecording();

            if (err2 == SDKERR_SUCCESS) {
                SDKError err1 = m_pRecordController->StartRawRecording();
                if (err1 != SDKERR_SUCCESS) {
                    std::cout << "Error occurred starting raw recording" << std::endl;
                }
                else {
                    //GetAudioRawData
                    if (isAudio) {
                        audioHelper = GetAudioRawdataHelper();
                        if (audioHelper) {
                            SDKError err = audioHelper->subscribe(audio_source);
                            if (err != SDKERR_SUCCESS) {
                                std::cout << "Error occurred subscribing to audio : " << err << std::endl;
                            }
                        }
                        else {
                            std::cout << "Error getting audioHelper" << std::endl;
                        }
                    }
                }
            }
            else {
                std::cout << "Cannot start raw recording: no permissions yet, need host, co-host, or recording privilege" << std::endl;
            }
        }
    }


    //callback when given host permission
    static void onIsHost() {
        printf("Is host now...\n");
        r_instance->CheckAndStartRawRecording(true);
    }

    //callback when given cohost permission
    static void onIsCoHost() {
        printf("Is co-host now...\n");
        r_instance->CheckAndStartRawRecording(true);
    }
    //callback when given recording permission
    static void onIsGivenRecordingPermission() {
        printf("Is given recording permissions now...\n");
        r_instance->CheckAndStartRawRecording(true);
    }


    //callback when the SDK is inmeeting
     void onInMeeting() {
        printf("onInMeeting Invoked\n");
        //double check if you are in a meeting
        if (meeting_service_->GetMeetingStatus() == ZOOM_SDK_NAMESPACE::MEETING_STATUS_INMEETING) {
            printf("In Meeting Now...\n");
        }

        //first attempt to start raw recording  / sending, upon successfully joined and achieved "in-meeting" state.
        CheckAndStartRawRecording(true);

    }
    static void onInMeetingStatic(){
        r_instance->onInMeeting();
    }

    static void onMeetingJoined(){};
    static void onMeetingEndsQuitApp(){};

    static void OnAuthSuccess() {
        std::cout << "recoauth" << std::endl;
        if (r_instance) {
            r_instance->recoCallback();
            }
    }

    void recoCallback(){
        std::cout << "recocallback" << std::endl;
        recojoin();
    }


    void recojoin() {
        std::cout << "recojoin" << std::endl;

        SDKError err;
        createServices();
        // Set the event listener for meeting status
        meeting_service_->SetEvent(new MeetingServiceEventListener(
            BotRecording::onMeetingJoined,
            BotRecording::onMeetingEndsQuitApp,
            BotRecording::onInMeetingStatic)
        );

        m_pParticipantsController = meeting_service_->GetMeetingParticipantsController();

        m_pParticipantsController->SetEvent(new MeetingParticipantsCtrlEventListener(
            BotRecording::onIsHost, BotRecording::onIsCoHost)
        );

	    m_pRecordController = meeting_service_->GetMeetingRecordingController();
    	m_pRecordController->SetEvent(new MeetingRecordingCtrlEventListener(
            BotRecording::onIsGivenRecordingPermission)
        );

        IMeetingReminderController* meetingremindercontroller = meeting_service_->GetMeetingReminderController();
	    MeetingReminderEventListener* meetingremindereventlistener = new MeetingReminderEventListener();
	    meetingremindercontroller->SetEvent(meetingremindereventlistener);


        ZoomBot::JoinToMeeting();
    }

    void recorun(){
        this->InitZoomSDK();
        this->JoinWithAuthToMeeting(BotRecording::OnAuthSuccess);
        this->initAppSettings();
    }
};

BotRecording* BotRecording::r_instance = nullptr;

int main() {

    std::string token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzZGtLZXkiOiJvRGNsaWpESlNvQ3Uwdko1MThkOUlBIiwiYXBwS2V5Ijoib0RjbGlqREpTb0N1MHZKNTE4ZDlJQSIsIm1uIjo3ODU1MTg4MzEwNSwicm9sZSI6MCwiaWF0IjoxNzQxMzAwOTYzLCJleHAiOjE3NDEzMDgxNjMsInRva2VuRXhwIjoxNzQxMzA4MTYzfQ.m0NZyx7PK-Bv-IibfYrzROhT1DFvf5zawd8bl6o0lko";
    std::string meeting_number = "72218148811";
    std::string pwd = "ZKCTw7SwDcxM2FAta4y9ATZQYfp6oU.1";
    
    BotRecording recbot;
    
    recbot.meeting_num_ = meeting_number;
    recbot.m_pwd_ = pwd;
    recbot.token_ = token;
    recbot.bot_name = "boostbee";
    recbot.recorun();

        // Запускаем главный цикл GLib для обработки событий
    GMainLoop* loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);
    g_main_loop_unref(loop);
    exit(0);

}