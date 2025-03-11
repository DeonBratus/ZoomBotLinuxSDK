#include <ZoomBot.h>
#include <meeting_service_components/meeting_recording_interface.h>
#include <rawdata_audio_helper_interface.h>
#include <zoom_rawdata_api.h>

#include <MeetingAudioCtrlEventListener.h>

#include <unordered_map>
#include <string>
#include <functional>
#include <list>

USING_ZOOM_SDK_NAMESPACE

class BotRecording : public ZoomBot::ZoomBot {
public:
    BotRecording();

    // Основные методы
    void startCommandListener();
    void recorun();

    // Вспомогательные методы
    bool directoryExists(const std::string& path);
    bool createDirectory(const std::string& path);
    std::string formatTime(std::time_t t);
    std::string getCurrentTime();
    
    // Логирование
    void logAction(const std::string& action, unsigned int userID, const std::string& username);
    void logUserInfo(unsigned int userID, const std::string& username, const std::string& role);

    // Обработчики событий
    static void UserJoinCheck(unsigned int userid);
    static void UserLeftCheck(unsigned int userid);
    void CheckConnectedUsersFirst();
    
    // Управление записью
    void CheckAndStartRawRecording(bool isAudio, bool isVideo);
    static void CheckAndStartRawRecordingCrutch();
    void recostop();
    static void recostopCrutch();

    static void audioActiveHandler(IList<unsigned int>* audio_users);
    static void requestRecordingPermission();
    static void requestRecordingPermissionCrutch();

    // Системные обработчики
    static void onIsHost();
    static void onIsCoHost();
    static void onIsGivenRecordingPermission();
    static void onMeetingJoined();
    static void onMeetingEndsQuitApp();
    static void onInMeeting();

    void JoinForPython();

    bool enableReqRecordPermission{false};
    bool isStopRecording{false};
    bool isStartRecording{false};

private:
    void recojoin();
    void recoCallback();
    static void OnAuthSuccess();

    static BotRecording* self;
    std::string logDir;

    // Компоненты Zoom SDK
    IMeetingRecordingController* recored_controller;
    IMeetingAudioController* audio_controller;
    IMeetingParticipantsController* participants_controller;
    IZoomSDKAudioRawDataHelper* audioHelper;
    MeetingAudioCtrlEventListener* audioEventListener;

    // Данные пользователей
    std::unordered_map<unsigned int, std::string> current_user_dict;
    std::unordered_map<unsigned int, std::string> global_user_dict;
    ZOOMSDK::IList<unsigned int>* uids_list;
};