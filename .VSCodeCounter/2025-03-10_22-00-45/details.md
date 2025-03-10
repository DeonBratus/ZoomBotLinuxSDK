# Details

Date : 2025-03-10 22:00:45

Directory /home/debral/DevSpace/RecoZoomBot/

Total : 61 files,  5591 codes, 4887 comments, 2060 blanks, all 12538 lines

[Summary](results.md) / Details / [Diff Summary](diff.md) / [Diff Details](diff-details.md)

## Files
| filename | language | code | comment | blank | total |
| :--- | :--- | ---: | ---: | ---: | ---: |
| [README.md](/README.md) | Markdown | 21 | 0 | 8 | 29 |
| [rzbcpp/BotRecording.cpp](/rzbcpp/BotRecording.cpp) | C++ | 70 | 3 | 16 | 89 |
| [rzbcpp/BotRecording.h](/rzbcpp/BotRecording.h) | C++ | 29 | 0 | 6 | 35 |
| [rzbcpp/CMakeLists.txt](/rzbcpp/CMakeLists.txt) | CMake | 78 | 0 | 27 | 105 |
| [rzbcpp/ZoomBot.cpp](/rzbcpp/ZoomBot.cpp) | C++ | 150 | 8 | 37 | 195 |
| [rzbcpp/ZoomBot.h](/rzbcpp/ZoomBot.h) | C++ | 54 | 4 | 18 | 76 |
| [rzbcpp/bot\_recording\_wrapper.cpp](/rzbcpp/bot_recording_wrapper.cpp) | C++ | 16 | 0 | 3 | 19 |
| [rzbcpp/include/event\_listeners/AuthServiceEventListener.h](/rzbcpp/include/event_listeners/AuthServiceEventListener.h) | C++ | 13 | 12 | 13 | 38 |
| [rzbcpp/include/event\_listeners/MeetingParticipantsCtrlEventListener.h](/rzbcpp/include/event_listeners/MeetingParticipantsCtrlEventListener.h) | C++ | 39 | 2 | 4 | 45 |
| [rzbcpp/include/event\_listeners/MeetingRecordingCtrlEventListener.h](/rzbcpp/include/event_listeners/MeetingRecordingCtrlEventListener.h) | C++ | 19 | 1 | 14 | 34 |
| [rzbcpp/include/event\_listeners/MeetingReminderEventListener.h](/rzbcpp/include/event_listeners/MeetingReminderEventListener.h) | C++ | 10 | 6 | 6 | 22 |
| [rzbcpp/include/event\_listeners/MeetingServiceEventListener.h](/rzbcpp/include/event_listeners/MeetingServiceEventListener.h) | C++ | 18 | 7 | 9 | 34 |
| [rzbcpp/include/media\_handlers/ZoomSDKAudioRawData.h](/rzbcpp/include/media_handlers/ZoomSDKAudioRawData.h) | C++ | 16 | 1 | 3 | 20 |
| [rzbcpp/include/media\_handlers/ZoomSDKRenderer.h](/rzbcpp/include/media_handlers/ZoomSDKRenderer.h) | C++ | 14 | 1 | 5 | 20 |
| [rzbcpp/include/media\_handlers/ZoomSDKVideoSource.h](/rzbcpp/include/media_handlers/ZoomSDKVideoSource.h) | C++ | 21 | 1 | 7 | 29 |
| [rzbcpp/include/meeting\_zoom\_sdk/README.md](/rzbcpp/include/meeting_zoom_sdk/README.md) | Markdown | 0 | 0 | 1 | 1 |
| [rzbcpp/include/meeting\_zoom\_sdk/auth\_service\_interface.h](/rzbcpp/include/meeting_zoom_sdk/auth_service_interface.h) | C++ | 125 | 107 | 27 | 259 |
| [rzbcpp/include/meeting\_zoom\_sdk/meeting\_service\_components/meeting\_ai\_companion\_interface.h](/rzbcpp/include/meeting_zoom_sdk/meeting_service_components/meeting_ai_companion_interface.h) | C++ | 266 | 341 | 134 | 741 |
| [rzbcpp/include/meeting\_zoom\_sdk/meeting\_service\_components/meeting\_audio\_interface.h](/rzbcpp/include/meeting_zoom_sdk/meeting_service_components/meeting_audio_interface.h) | C++ | 67 | 91 | 24 | 182 |
| [rzbcpp/include/meeting\_zoom\_sdk/meeting\_service\_components/meeting\_breakout\_rooms\_interface\_v2.h](/rzbcpp/include/meeting_zoom_sdk/meeting_service_components/meeting_breakout_rooms_interface_v2.h) | C++ | 252 | 319 | 122 | 693 |
| [rzbcpp/include/meeting\_zoom\_sdk/meeting\_service\_components/meeting\_chat\_interface.h](/rzbcpp/include/meeting_zoom_sdk/meeting_service_components/meeting_chat_interface.h) | C++ | 314 | 364 | 128 | 806 |
| [rzbcpp/include/meeting\_zoom\_sdk/meeting\_service\_components/meeting\_configuration\_interface.h](/rzbcpp/include/meeting_zoom_sdk/meeting_service_components/meeting_configuration_interface.h) | C++ | 261 | 349 | 136 | 746 |
| [rzbcpp/include/meeting\_zoom\_sdk/meeting\_service\_components/meeting\_inmeeting\_encryption\_interface.h](/rzbcpp/include/meeting_zoom_sdk/meeting_service_components/meeting_inmeeting_encryption_interface.h) | C++ | 30 | 20 | 14 | 64 |
| [rzbcpp/include/meeting\_zoom\_sdk/meeting\_service\_components/meeting\_participants\_ctrl\_interface.h](/rzbcpp/include/meeting_zoom_sdk/meeting_service_components/meeting_participants_ctrl_interface.h) | C++ | 165 | 328 | 111 | 604 |
| [rzbcpp/include/meeting\_zoom\_sdk/meeting\_service\_components/meeting\_raw\_archiving\_interface.h](/rzbcpp/include/meeting_zoom_sdk/meeting_service_components/meeting_raw_archiving_interface.h) | C++ | 12 | 12 | 4 | 28 |
| [rzbcpp/include/meeting\_zoom\_sdk/meeting\_service\_components/meeting\_recording\_interface.h](/rzbcpp/include/meeting_zoom_sdk/meeting_service_components/meeting_recording_interface.h) | C++ | 159 | 178 | 62 | 399 |
| [rzbcpp/include/meeting\_zoom\_sdk/meeting\_service\_components/meeting\_reminder\_ctrl\_interface.h](/rzbcpp/include/meeting_zoom_sdk/meeting_service_components/meeting_reminder_ctrl_interface.h) | C++ | 104 | 65 | 18 | 187 |
| [rzbcpp/include/meeting\_zoom\_sdk/meeting\_service\_components/meeting\_sharing\_interface.h](/rzbcpp/include/meeting_zoom_sdk/meeting_service_components/meeting_sharing_interface.h) | C++ | 141 | 257 | 70 | 468 |
| [rzbcpp/include/meeting\_zoom\_sdk/meeting\_service\_components/meeting\_smart\_summary\_interface.h](/rzbcpp/include/meeting_zoom_sdk/meeting_service_components/meeting_smart_summary_interface.h) | C++ | 22 | 16 | 10 | 48 |
| [rzbcpp/include/meeting\_zoom\_sdk/meeting\_service\_components/meeting\_video\_interface.h](/rzbcpp/include/meeting_zoom_sdk/meeting_service_components/meeting_video_interface.h) | C++ | 166 | 276 | 78 | 520 |
| [rzbcpp/include/meeting\_zoom\_sdk/meeting\_service\_components/meeting\_waiting\_room\_interface.h](/rzbcpp/include/meeting_zoom_sdk/meeting_service_components/meeting_waiting_room_interface.h) | C++ | 94 | 105 | 36 | 235 |
| [rzbcpp/include/meeting\_zoom\_sdk/meeting\_service\_components/meeting\_webinar\_interface.h](/rzbcpp/include/meeting_zoom_sdk/meeting_service_components/meeting_webinar_interface.h) | C++ | 108 | 130 | 43 | 281 |
| [rzbcpp/include/meeting\_zoom\_sdk/meeting\_service\_interface.h](/rzbcpp/include/meeting_zoom_sdk/meeting_service_interface.h) | C++ | 415 | 261 | 95 | 771 |
| [rzbcpp/include/meeting\_zoom\_sdk/network\_connection\_handler\_interface.h](/rzbcpp/include/meeting_zoom_sdk/network_connection_handler_interface.h) | C++ | 50 | 51 | 19 | 120 |
| [rzbcpp/include/meeting\_zoom\_sdk/rawdata/rawdata\_audio\_helper\_interface.h](/rzbcpp/include/meeting_zoom_sdk/rawdata/rawdata_audio_helper_interface.h) | C++ | 39 | 26 | 13 | 78 |
| [rzbcpp/include/meeting\_zoom\_sdk/rawdata/rawdata\_renderer\_interface.h](/rzbcpp/include/meeting_zoom_sdk/rawdata/rawdata_renderer_interface.h) | C++ | 45 | 9 | 8 | 62 |
| [rzbcpp/include/meeting\_zoom\_sdk/rawdata/rawdata\_share\_source\_helper\_interface.h](/rzbcpp/include/meeting_zoom_sdk/rawdata/rawdata_share_source_helper_interface.h) | C++ | 39 | 35 | 15 | 89 |
| [rzbcpp/include/meeting\_zoom\_sdk/rawdata/rawdata\_video\_source\_helper\_interface.h](/rzbcpp/include/meeting_zoom_sdk/rawdata/rawdata_video_source_helper_interface.h) | C++ | 69 | 2 | 11 | 82 |
| [rzbcpp/include/meeting\_zoom\_sdk/rawdata/zoom\_rawdata\_api.h](/rzbcpp/include/meeting_zoom_sdk/rawdata/zoom_rawdata_api.h) | C++ | 20 | 5 | 2 | 27 |
| [rzbcpp/include/meeting\_zoom\_sdk/setting\_service\_interface.h](/rzbcpp/include/meeting_zoom_sdk/setting_service_interface.h) | C++ | 841 | 1,191 | 403 | 2,435 |
| [rzbcpp/include/meeting\_zoom\_sdk/zoom\_sdk.h](/rzbcpp/include/meeting_zoom_sdk/zoom_sdk.h) | C++ | 27 | 50 | 16 | 93 |
| [rzbcpp/include/meeting\_zoom\_sdk/zoom\_sdk\_def.h](/rzbcpp/include/meeting_zoom_sdk/zoom_sdk_def.h) | C++ | 325 | 62 | 26 | 413 |
| [rzbcpp/include/meeting\_zoom\_sdk/zoom\_sdk\_platform.h](/rzbcpp/include/meeting_zoom_sdk/zoom_sdk_platform.h) | C++ | 36 | 0 | 4 | 40 |
| [rzbcpp/include/meeting\_zoom\_sdk/zoom\_sdk\_raw\_data\_def.h](/rzbcpp/include/meeting_zoom_sdk/zoom_sdk_raw_data_def.h) | C++ | 58 | 60 | 30 | 148 |
| [rzbcpp/lib/README.md](/rzbcpp/lib/README.md) | Markdown | 0 | 0 | 1 | 1 |
| [rzbcpp/mainapp.cpp](/rzbcpp/mainapp.cpp) | C++ | 307 | 29 | 61 | 397 |
| [rzbcpp/src/event\_listeners/AuthServiceEventListener.cpp](/rzbcpp/src/event_listeners/AuthServiceEventListener.cpp) | C++ | 35 | 6 | 13 | 54 |
| [rzbcpp/src/event\_listeners/MeetingParticipantsCtrlEventListener.cpp](/rzbcpp/src/event_listeners/MeetingParticipantsCtrlEventListener.cpp) | C++ | 72 | 16 | 26 | 114 |
| [rzbcpp/src/event\_listeners/MeetingRecordingCtrlEventListener.cpp](/rzbcpp/src/event_listeners/MeetingRecordingCtrlEventListener.cpp) | C++ | 26 | 6 | 13 | 45 |
| [rzbcpp/src/event\_listeners/MeetingReminderEventListener.cpp](/rzbcpp/src/event_listeners/MeetingReminderEventListener.cpp) | C++ | 23 | 12 | 10 | 45 |
| [rzbcpp/src/event\_listeners/MeetingServiceEventListener.cpp](/rzbcpp/src/event_listeners/MeetingServiceEventListener.cpp) | C++ | 72 | 4 | 8 | 84 |
| [rzbcpp/src/media\_handlers/ZoomSDKAudioRawData.cpp](/rzbcpp/src/media_handlers/ZoomSDKAudioRawData.cpp) | C++ | 26 | 6 | 9 | 41 |
| [rzbcpp/src/media\_handlers/ZoomSDKRenderer.cpp](/rzbcpp/src/media_handlers/ZoomSDKRenderer.cpp) | C++ | 40 | 36 | 39 | 115 |
| [rzbcpp/src/media\_handlers/ZoomSDKVideoSource.cpp](/rzbcpp/src/media_handlers/ZoomSDKVideoSource.cpp) | C++ | 53 | 4 | 16 | 73 |
| [rzbpy/example.py](/rzbpy/example.py) | Python | 12 | 0 | 5 | 17 |
| [rzbpy/zbpy\_pkg/\_\_init\_\_.py](/rzbpy/zbpy_pkg/__init__.py) | Python | 2 | 0 | 0 | 2 |
| [rzbpy/zbpy\_pkg/interface.py](/rzbpy/zbpy_pkg/interface.py) | Python | 14 | 0 | 4 | 18 |
| [utils/installers/install\_zoom\_sdk.sh](/utils/installers/install_zoom_sdk.sh) | Shell Script | 26 | 11 | 12 | 49 |
| [utils/token\_generate/index.js](/utils/token_generate/index.js) | JavaScript | 27 | 1 | 5 | 33 |
| [utils/token\_generate/package-lock.json](/utils/token_generate/package-lock.json) | JSON | 24 | 0 | 1 | 25 |
| [utils/token\_generate/package.json](/utils/token_generate/package.json) | JSON | 14 | 0 | 1 | 15 |

[Summary](results.md) / Details / [Diff Summary](diff.md) / [Diff Details](diff-details.md)