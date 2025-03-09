#include <ZoomBot.h>
#include <rawdata_audio_helper_interface.h>
#include <meeting_service_components/meeting_recording_interface.h>
#include <zoom_rawdata_api.h>
#include <ZoomSDKAudioRawData.h>
#include <ZoomSDKVideoSource.h>
#include <ZoomSDKRenderer.h>
#include <MeetingRecordingCtrlEventListener.h>
#include <MeetingServiceEventListener.h>
#include <MeetingParticipantsCtrlEventListener.h>
#include <MeetingReminderEventListener.h>

#include <iostream>
#include <fstream>
#include <ctime>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <iomanip>
#include <sstream>
#include <sys/stat.h> // Для mkdir
#include <unistd.h>   // Для access

USING_ZOOM_SDK_NAMESPACE

class MeetingAudioCtrlEventListener : public IMeetingAudioCtrlEvent {
public:
    MeetingAudioCtrlEventListener() {}
    virtual void onUserAudioStatusChange(IList<IUserAudioStatus*>* lstAudioStatusChange, const zchar_t* strAudioStatusList = nullptr) override {}
    virtual void onUserActiveAudioChange(IList<unsigned int>* lstActiveAudio) override {}
    virtual void onHostRequestStartAudio(IRequestStartAudioHandler* handler_) override {}
    virtual void onJoin3rdPartyTelephonyAudio(const zchar_t* audioInfo) override {}
    virtual void onMuteOnEntryStatusChange(bool bEnabled) override {}
};

class BotRecording: public ZoomBot::ZoomBot {
public:
    BotRecording() {
        self = this;
        audioEventListener = new MeetingAudioCtrlEventListener();
    }


    // Функция для проверки существования директории
    bool directoryExists(const std::string& path) {
        return access(path.c_str(), F_OK) == 0;
    }

    // Функция для создания директории, если она не существует
    bool createDirectory(const std::string& path) {
        if (!directoryExists(path)) {
            #ifdef _WIN32
                return mkdir(path.c_str()) == 0;
            #else
                return mkdir(path.c_str(), 0777) == 0; // Права 0777 для Unix-систем
            #endif
        }
        return true;
    }

    std::string formatTime(std::time_t t) {
        std::tm* tm_ptr = std::localtime(&t);
        char buffer[20];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_ptr);
        return std::string(buffer);
    }

    std::string getCurrentTime() {
        time_t now = time(0);
        struct tm tstruct;
        char buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
        return std::string(buf);
    }

    void logAction(const std::string& action, unsigned int userID, const std::string& username) {
        // Формируем путь к папке
        std::string logDir = "meeting_" + self->meeting_num_ + "/";
        
        // Создаем папку, если она не существует
        if (!createDirectory(logDir)) {
            std::cerr << "Failed to create directory: " << logDir << std::endl;
            return;
        }

        // Формируем путь к файлу
        std::string logFilePath = logDir + "action_logs.txt";

        // Открываем файл для записи
        std::ofstream logFile(logFilePath, std::ios::app);

        if (logFile.is_open()) {
            logFile << action << "," << userID << "," << username << "," << getCurrentTime() << std::endl;
            logFile.close();
        } else {
            std::cerr << "Unable to open log file!" << std::endl;
        }
    }

    // Функция для записи информации о пользователях
    void logUserInfo(unsigned int userID, const std::string& username, const std::string& role) {
        // Формируем путь к папке
        std::string logDir = "meeting_" + self->meeting_num_ + "/";
        
        // Создаем папку, если она не существует
        if (!createDirectory(logDir)) {
            std::cerr << "Failed to create directory: " << logDir << std::endl;
            return;
        }

        // Формируем путь к файлу
        std::string logFilePath = logDir + "user_info.txt";

        // Открываем файл для записи
        std::ofstream userFile(logFilePath, std::ios::app);
        if (userFile.is_open()) {
            userFile << userID << "," << username << "," << getCurrentTime() << "," << role << std::endl;
            userFile.close();
        } else {
            std::cerr << "Unable to open user info file: " << logFilePath << std::endl;
        }
    }

    static void UserJoinCheck(unsigned int userid) {
        self->g_uids = self->m_pParticipantsController->GetParticipantsList();
        IUserInfo* userel = self->m_pParticipantsController->GetUserByUserID(userid);
        if (userel) {
            self->current_user_dict[userid] = userel->GetUserName();
            self->global_user_dict[userid] = userel->GetUserName(); // don't erase

            std::cout << self->current_user_dict[userel->GetUserID()] << std::endl;
            std::cout << "JOIN," << userel->GetUserID() << "," << userel->GetUserName() << std::endl;

            self->logAction("JOIN", userel->GetUserID(), userel->GetUserName());

            self->logUserInfo(userel->GetUserID(), userel->GetUserName(), "participant");

            std::cout << "USER IN CONFERENCE LIST: ";
            for (const auto& user : self->current_user_dict) {
                std::cout << user.second << std::endl;
            }
        }
    }

    static void UserLeftCheck(unsigned int userid) {
        std::cout << self->current_user_dict[userid] << " LEFT," << userid << std::endl;
        // Логирование действия
        self->logAction("LEFT", userid, self->current_user_dict[userid]);
        self->current_user_dict.erase(userid);
    }

    void FirstUserJoinFix() {
        g_uids = m_pParticipantsController->GetParticipantsList();
        for (int i = 0; i < g_uids->GetCount(); i++) {
            IUserInfo* userinfo = m_pParticipantsController->GetUserByUserID(g_uids->GetItem(i));
            self->current_user_dict[userinfo->GetUserID()] = userinfo->GetUserName();
            std::cout << "WAS," << userinfo->GetUserID() << "," << userinfo->GetUserName() << std::endl;
            self->logAction("WAS",  userinfo->GetUserID(), userinfo->GetUserName());


            // Логирование информации о пользователе
            self->logUserInfo(userinfo->GetUserID(), userinfo->GetUserName(), "participant");
        }
    }


    void CheckAndStartRawRecording(bool isAudio, bool isVideo) {
        if (isAudio) {
            m_pRecordController = meeting_service_->GetMeetingRecordingController();
            SDKError err2 = meeting_service_->GetMeetingRecordingController()->CanStartRawRecording();

            if (err2 == SDKERR_SUCCESS) {
                SDKError err1 = m_pRecordController->StartRawRecording();
                if (err1 != SDKERR_SUCCESS) {
                    std::cout << "Error occurred starting raw recording" << std::endl;
                } else {
                    // GetAudioRawData
                    if (isAudio) {
                        audioHelper = GetAudioRawdataHelper();
                        if (audioHelper) {
                            SDKError err = audioHelper->subscribe(audio_source);
                            if (err != SDKERR_SUCCESS) {
                                std::cout << "Error occurred subscribing to audio : " << err << std::endl;
                            }
                        } else {
                            std::cout << "Error getting audioHelper" << std::endl;
                        }
                    }
                }
            } else {
                std::cout << "Cannot start raw recording: no permissions yet, need host, co-host, or recording privilege" << std::endl;
            }
        }
    }


    // Callback when given host permission
    static void onIsHost() {
        printf("Is host now...\n");
        self->CheckAndStartRawRecording(true, true);
    }

    // Callback when given cohost permission
    static void onIsCoHost() {
        printf("Is co-host now...\n");
        self->CheckAndStartRawRecording(true, true);
    }

    // Callback when given recording permission
    static void onIsGivenRecordingPermission() {
        printf("Is given recording permissions now...\n");
        self->CheckAndStartRawRecording(true, true);
    }

    void onInMeeting() {
        // Double check if you are in a meeting
        if (meeting_service_->GetMeetingStatus() == MEETING_STATUS_INMEETING) {
            printf("In Meeting Now...\n");
        }
        // First attempt to start raw recording / sending, upon successfully joined and achieved "in-meeting" state.
        CheckAndStartRawRecording(true, true);
        FirstUserJoinFix();
    }

    static void onInMeetingStatic() { self->onInMeeting(); }

    static void onMeetingJoined() {}

    static void onMeetingEndsQuitApp() {}

    static void OnAuthSuccess() { if (self) {self->recoCallback();} }

    void recoCallback() {recojoin();}

    void recojoin() {
        SDKError err;
        createServices();

        // Set the event listener for meeting status
        meeting_service_->SetEvent(
            new MeetingServiceEventListener(
                BotRecording::onMeetingJoined,
                BotRecording::onMeetingEndsQuitApp,
                BotRecording::onInMeetingStatic)
        );

        m_pParticipantsController = meeting_service_->GetMeetingParticipantsController();
        m_pParticipantsController->SetEvent(
            new MeetingParticipantsCtrlEventListener(
                BotRecording::onIsHost,
                BotRecording::onIsCoHost,
                BotRecording::UserJoinCheck,
                BotRecording::UserLeftCheck)
        );

        m_pRecordController = meeting_service_->GetMeetingRecordingController();
        m_pRecordController->SetEvent(
            new MeetingRecordingCtrlEventListener(
                BotRecording::onIsGivenRecordingPermission)
        );

        IMeetingReminderController* meetingremindercontroller = meeting_service_->GetMeetingReminderController();
        MeetingReminderEventListener* meetingremindereventlistener = new MeetingReminderEventListener();
        meetingremindercontroller->SetEvent(meetingremindereventlistener);

        m_pAudioController = meeting_service_->GetMeetingAudioController(); // Get audio controller
        m_pAudioController->SetEvent(audioEventListener); // Set audio event listener

        ZoomBot::JoinToMeeting();
    }

    void recorun() {
        this->InitZoomSDK();
        this->JoinWithAuthToMeeting(BotRecording::OnAuthSuccess);
        this->initAppSettings();
    }

    static BotRecording* self;

    IZoomSDKRenderer* videoHelper;
    ZoomSDKRenderer* videoSource = new ZoomSDKRenderer();

    IMeetingRecordingController* m_pRecordController;
    IZoomSDKAudioRawDataHelper* audioHelper;
    IMeetingAudioController* m_pAudioController;
    ZoomSDKAudioRawData* audio_source = new ZoomSDKAudioRawData();
    IMeetingParticipantsController* m_pParticipantsController;
    MeetingAudioCtrlEventListener* audioEventListener;

    std::unordered_map<unsigned int, std::string> current_user_dict;
    std::unordered_map<unsigned int, std::string> global_user_dict;
    ZOOMSDK::IList<unsigned int>* g_uids;

};

BotRecording* BotRecording::self = nullptr;

int main() {
    std::string token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzZGtLZXkiOiJvRGNsaWpESlNvQ3Uwdko1MThkOUlBIiwiYXBwS2V5Ijoib0RjbGlqREpTb0N1MHZKNTE4ZDlJQSIsIm1uIjo3MzYxNDM1ODQ5OCwicm9sZSI6MCwiaWF0IjoxNzQxNTMzMDk3LCJleHAiOjE3NDE1NDAyOTcsInRva2VuRXhwIjoxNzQxNTQwMjk3fQ.jeAyYqNc39DC1lp7Y0iZmm3wjC0rP0i_CkLnNO7j0bk";
    std::string meeting_number = "79789006489";
    std::string pwd = "flaKPtYu4EO5lxwK0lkHD5xs00jyaL.1";

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