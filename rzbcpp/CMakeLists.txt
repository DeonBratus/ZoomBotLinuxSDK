# cmake_minimum_required(VERSION 3.16)
# project(zoom_bot_pybind CXX)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)
# set(CMAKE_VERBOSE_MAKEFILE ON)  # Для отладки

# # Найти pybind11
# find_package(pybind11 REQUIRED)

# # Пути к заголовкам (Arch Linux)
# include_directories(
#     ${CMAKE_SOURCE_DIR}                  # Корень проекта
#     /usr/include/glib-2.0                # GLib
#     /usr/lib/glib-2.0/include            # glibconfig.h
#     ${CMAKE_SOURCE_DIR}/include/h        # Zoom SDK
#     ${CMAKE_SOURCE_DIR}/include/h/rawdata
# )

# # Пути к библиотекам
# link_directories(
#     ${CMAKE_SOURCE_DIR}/lib              # Библиотеки Zoom SDK
# )

# # Создать Python-модуль
# pybind11_add_module(zoombotpy
#     bot_recording_wrapper.cpp
#     ZoomBot.cpp
#     BotRecording.cpp
#     AuthServiceEventListener.cpp
#     MeetingServiceEventListener.cpp
#     MeetingParticipantsCtrlEventListener.cpp
#     MeetingRecordingCtrlEventListener.cpp
#     MeetingReminderEventListener.cpp
#     ZoomSDKAudioRawData.cpp
# )

# # Линковка библиотек
# target_link_libraries(zoombotpy
#     PRIVATE
#     meetingsdk           # Основная библиотека Zoom
#     glib-2.0             # GLib
#     pthread              # Потоки
#     curl                 # Для сетевых запросов (если требуется)
# )

# # Оптимизация и предупреждения
# target_compile_options(zoombotpy
#     PRIVATE
#     -Wall -Wextra -Wpedantic
#     -O3                  # Максимальная оптимизация
# )

### FOR BUILD WITHOUT PYTHON WRAPPER

cmake_minimum_required(VERSION 3.16)

project(zoom_app CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Укажите пути к заголовочным файлам
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)

include_directories(${CMAKE_SOURCE_DIR}/include/media_handlers)

include_directories(${CMAKE_SOURCE_DIR}/include/event_listeners)

include_directories(${CMAKE_SOURCE_DIR}/include/meeting_zoom_sdk)
include_directories(${CMAKE_SOURCE_DIR}/include/meeting_zoom_sdk/rawdata)

# Укажите пути к библиотекам
link_directories(${CMAKE_SOURCE_DIR}/lib)

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Создайте исполняемый файл
add_executable(zoom_app mainapp.cpp
    ${CMAKE_SOURCE_DIR}/ZoomBot.h
    ${CMAKE_SOURCE_DIR}/ZoomBot.cpp
    ${SOURCES}


)

include_directories(/usr/include/glib-2.0/)
include_directories(/usr/include/glib-2.0/glib)
include_directories(/usr/lib/x86_64-linux-gnu/glib-2.0/include/)
include_directories(/usr/lib64/glib-2.0/include/)



target_link_libraries(zoom_app gcc_s gcc)
target_link_libraries(zoom_app meetingsdk)
target_link_libraries(zoom_app glib-2.0)
target_link_libraries(zoom_app curl)
target_link_libraries(zoom_app pthread)


# Подключите библиотеку Zoom SDK
target_link_libraries(zoom_app meetingsdk)

