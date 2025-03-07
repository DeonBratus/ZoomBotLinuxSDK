#include <pybind11/pybind11.h>
#include "ZoomBot.h"
#include "BotRecording.h"  // Ваш класс BotRecording из mainapp.cpp

namespace py = pybind11;

PYBIND11_MODULE(zoombotpy, m) {
    py::class_<BotRecording>(m, "ZoomRecoBot")
        .def(py::init<>())
        .def("run", &BotRecording::recorun)
        .def_readwrite("meeting_id", &BotRecording::meeting_num_)
        .def_readwrite("meeting_pwd", &BotRecording::m_pwd_)
        .def_readwrite("meeting_token", &BotRecording::token_)
        .def_readwrite("bot_name", &BotRecording::bot_name);

    m.def("create_bot", []() {
        return new BotRecording();
    }, py::return_value_policy::take_ownership);
}