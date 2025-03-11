#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include "ZoomBot.h"
#include "BotRecording.h"

namespace py = pybind11;

PYBIND11_MODULE(zoombotpy, m) {
    m.doc() = "Python interface for Zoom Recording Bot";

    py::class_<BotRecording>(m, "ZoomRecoBot")
        .def(py::init<>(), "Create new recording bot instance")
        
        // Основные методы управления
        .def("init", &BotRecording::InitZoomSDK, "Init SDK for Bot")
        .def("join", &BotRecording::JoinForPython, "Join to meeting")
        .def("init_setting", &BotRecording::initAppSettings)
        .def("leave",&BotRecording::LeaveFromMeeting)
        .def("run_with_cli", &BotRecording::recorun, "Start the bot and join meeting")
        .def("stop_recording", &BotRecording::recostopCrutch, "Stop current recording")
        .def("start_recording",&BotRecording::CheckAndStartRawRecording)
        .def("request_recording_permission",&BotRecording::requestRecordingPermissionCrutch)
        
        // Свойства конфигурации
        .def_readwrite("meeting_id", &BotRecording::meeting_num_, 
                      "Meeting ID (numeric string)")
        .def_readwrite("meeting_pwd", &BotRecording::m_pwd_,
                      "Meeting password")
        .def_readwrite("meeting_token", &BotRecording::token_,
                      "JWT authentication token")
        .def_readwrite("bot_name", &BotRecording::bot_name,
                      "Display name for the bot");
        

    // Регистрация обработчиков исключений
    py::register_exception_translator([](std::exception_ptr p) {
        try {
            if (p) std::rethrow_exception(p);
        } catch (const std::runtime_error& e) {
            PyErr_SetString(PyExc_RuntimeError, e.what());
        } catch (const std::exception& e) {
            PyErr_SetString(PyExc_Exception, e.what());
        }
    });
}