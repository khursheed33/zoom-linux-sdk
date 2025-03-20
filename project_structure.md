gaddi@khursheed33 MINGW64 /d/Programs/ZOOM-SDK/zoom-linux-sdk ((127ae9d...))
$ ls -la
total 56
drwxr-xr-x 1 gaddi 197609     0 Mar 20 20:59 ./
drwxr-xr-x 1 gaddi 197609     0 Mar 19 14:38 ../
-rw-r--r-- 1 gaddi 197609    31 Mar 20 20:59 .env
-rw-r--r-- 1 gaddi 197609    31 Mar 20 20:59 .env.example
drwxr-xr-x 1 gaddi 197609     0 Mar 20 20:59 .git/
-rw-r--r-- 1 gaddi 197609   924 Mar 19 19:56 .gitignore
drwxr-xr-x 1 gaddi 197609     0 Mar 20 20:52 .vscode/
-rw-r--r-- 1 gaddi 197609   112 Mar 19 19:56 config.py
-rw-r--r-- 1 gaddi 197609  1827 Mar 19 19:56 db_manager.py
-rw-r--r-- 1 gaddi 197609  2088 Mar 19 19:56 docker_manager.py
-rw-r--r-- 1 gaddi 197609  1565 Mar 20 21:06 Dockerfile
-rw-r--r-- 1 gaddi 197609  5870 Mar 19 19:56 main.py
-rw-r--r-- 1 gaddi 197609    34 Mar 19 19:56 requirements.txt
-rw-r--r-- 1 gaddi 197609 12288 Mar 19 19:56 zoom_containers.db
drwxr-xr-x 1 gaddi 197609     0 Mar 20 23:02 zoom-linux-sdk/

gaddi@khursheed33 MINGW64 /d/Programs/ZOOM-SDK/zoom-linux-sdk ((127ae9d...))
$ cd zoom
zoom_containers.db  zoom-linux-sdk/     

gaddi@khursheed33 MINGW64 /d/Programs/ZOOM-SDK/zoom-linux-sdk ((127ae9d...))
$ cd zoom-linux-sdk/

gaddi@khursheed33 MINGW64 /d/Programs/ZOOM-SDK/zoom-linux-sdk/zoom-linux-sdk ((127ae9d...))
$ ls -la
total 44
drwxr-xr-x 1 gaddi 197609    0 Mar 20 23:02 ./
drwxr-xr-x 1 gaddi 197609    0 Mar 20 20:59 ../
-rw-r--r-- 1 gaddi 197609  116 Mar 19 19:56 .gitattributes
-rw-r--r-- 1 gaddi 197609  254 Mar 19 19:56 .gitignore
drwxr-xr-x 1 gaddi 197609    0 Mar 19 19:56 .vscode/
drwxr-xr-x 1 gaddi 197609    0 Mar 20 20:59 demo/
-rw-r--r-- 1 gaddi 197609  186 Mar 19 19:56 example.config.txt
-rw-r--r-- 1 gaddi 197609 1097 Mar 19 19:56 LICENSE.md
-rw-r--r-- 1 gaddi 197609 9704 Mar 19 19:56 README.md
-rw-r--r-- 1 gaddi 197609 5738 Mar 19 19:56 SETUP.md
drwxr-xr-x 1 gaddi 197609    0 Mar 20 20:59 speechsdk/
-rw-r--r-- 1 gaddi 197609   14 Mar 19 19:56 version.txt

gaddi@khursheed33 MINGW64 /d/Programs/ZOOM-SDK/zoom-linux-sdk/zoom-linux-sdk ((127ae9d...))
$ cd demo

gaddi@khursheed33 MINGW64 /d/Programs/ZOOM-SDK/zoom-linux-sdk/zoom-linux-sdk/demo ((127ae9d...))
$ ls -la
total 1633
drwxr-xr-x 1 gaddi 197609      0 Mar 20 20:59 ./
drwxr-xr-x 1 gaddi 197609      0 Mar 20 23:02 ../
-rw-r--r-- 1 gaddi 197609   1558 Mar 19 19:56 AuthServiceEventListener.cpp
-rw-r--r-- 1 gaddi 197609   1539 Mar 19 19:56 AuthServiceEventListener.h
-rw-r--r-- 1 gaddi 197609   3233 Mar 20 20:59 CMakeLists.txt
-rw-r--r-- 1 gaddi 197609    481 Mar 20 20:59 config.txt
-rw-r--r-- 1 gaddi 197609 557808 Mar 20 20:59 generate_jwt
-rw-r--r-- 1 gaddi 197609   5995 Mar 19 19:56 GenerateSDKJwtToken.cpp
-rw-r--r-- 1 gaddi 197609    987 Mar 19 19:56 GenerateSDKJwtToken.h
drwxr-xr-x 1 gaddi 197609      0 Mar 19 19:56 include/
-rw-r--r-- 1 gaddi 197609 978210 Mar 19 19:56 json.hpp
drwxr-xr-x 1 gaddi 197609      0 Mar 19 19:49 lib/
-rw-r--r-- 1 gaddi 197609  15017 Mar 20 20:59 meeting_sdk_demo.cpp
-rw-r--r-- 1 gaddi 197609   4809 Mar 19 19:56 MeetingParticipantsCtrlEventListener.cpp
-rw-r--r-- 1 gaddi 197609   5199 Mar 19 19:56 MeetingParticipantsCtrlEventListener.h
-rw-r--r-- 1 gaddi 197609   2543 Mar 19 19:56 MeetingRecordingCtrlEventListener.cpp
-rw-r--r-- 1 gaddi 197609   3521 Mar 19 19:56 MeetingRecordingCtrlEventListener.h
-rw-r--r-- 1 gaddi 197609   1895 Mar 19 19:56 MeetingReminderEventListener.cpp
-rw-r--r-- 1 gaddi 197609   1189 Mar 19 19:56 MeetingReminderEventListener.h
-rw-r--r-- 1 gaddi 197609   2536 Mar 19 19:56 MeetingServiceEventListener.cpp
-rw-r--r-- 1 gaddi 197609   2041 Mar 19 19:56 MeetingServiceEventListener.h
-rw-r--r-- 1 gaddi 197609    639 Mar 19 19:56 NetworkConnectionHandler.cpp
-rw-r--r-- 1 gaddi 197609   1092 Mar 19 19:56 NetworkConnectionHandler.h
-rwxr-xr-x 1 gaddi 197609    742 Mar 19 19:56 setup-pulseaudio.sh*
-rwxr-xr-x 1 gaddi 197609    579 Mar 19 19:56 setup-pulseaudio-centos.sh*
-rw-r--r-- 1 gaddi 197609   1331 Mar 19 19:56 ZoomSDKAudioRawData.cpp
-rw-r--r-- 1 gaddi 197609    563 Mar 19 19:56 ZoomSDKAudioRawData.h
-rw-r--r-- 1 gaddi 197609   3033 Mar 19 19:56 ZoomSDKRenderer.cpp
-rw-r--r-- 1 gaddi 197609    513 Mar 19 19:56 ZoomSDKRenderer.h
-rw-r--r-- 1 gaddi 197609   2113 Mar 19 19:56 ZoomSDKVideoSource.cpp
-rw-r--r-- 1 gaddi 197609    781 Mar 19 19:56 ZoomSDKVideoSource.h
-rw-r--r-- 1 gaddi 197609   2820 Mar 19 19:56 ZoomSDKVirtualAudioMicEvent.cpp
-rw-r--r-- 1 gaddi 197609   1045 Mar 19 19:56 ZoomSDKVirtualAudioMicEvent.h

gaddi@khursheed33 MINGW64 /d/Programs/ZOOM-SDK/zoom-linux-sdk/zoom-linux-sdk/demo/include/h ((127ae9d...))
$ ls -la
total 224
drwxr-xr-x 1 gaddi 197609      0 Mar 19 19:56 ./
drwxr-xr-x 1 gaddi 197609      0 Mar 19 19:56 ../
-rw-r--r-- 1 gaddi 197609  10677 Mar 19 19:56 auth_service_interface.h
drwxr-xr-x 1 gaddi 197609      0 Mar 19 19:56 meeting_service_components/
-rw-r--r-- 1 gaddi 197609  37589 Mar 19 19:56 meeting_service_interface.h
-rw-r--r-- 1 gaddi 197609   4905 Mar 19 19:56 network_connection_handler_interface.h
drwxr-xr-x 1 gaddi 197609      0 Mar 19 19:56 rawdata/
-rw-r--r-- 1 gaddi 197609 116753 Mar 19 19:56 setting_service_interface.h
-rw-r--r-- 1 gaddi 197609   4746 Mar 19 19:56 zoom_sdk.h
-rw-r--r-- 1 gaddi 197609  15069 Mar 19 19:56 zoom_sdk_def.h
-rw-r--r-- 1 gaddi 197609   1073 Mar 19 19:56 zoom_sdk_platform.h
-rw-r--r-- 1 gaddi 197609   5627 Mar 19 19:56 zoom_sdk_raw_data_def.h

gaddi@khursheed33 MINGW64 /d/Programs/ZOOM-SDK/zoom-linux-sdk/zoom-linux-sdk/demo/include/h ((127ae9d...))
$ cd rawdata/

gaddi@khursheed33 MINGW64 /d/Programs/ZOOM-SDK/zoom-linux-sdk/zoom-linux-sdk/demo/include/h/rawdata ((127ae9d...))
$ ls -la
total 28
drwxr-xr-x 1 gaddi 197609    0 Mar 19 19:56 ./
drwxr-xr-x 1 gaddi 197609    0 Mar 19 19:56 ../
-rw-r--r-- 1 gaddi 197609 3589 Mar 19 19:56 rawdata_audio_helper_interface.h
-rw-r--r-- 1 gaddi 197609 1396 Mar 19 19:56 rawdata_renderer_interface.h
-rw-r--r-- 1 gaddi 197609 3656 Mar 19 19:56 rawdata_share_source_helper_interface.h
-rw-r--r-- 1 gaddi 197609 2262 Mar 19 19:56 rawdata_video_source_helper_interface.h
-rw-r--r-- 1 gaddi 197609  801 Mar 19 19:56 zoom_rawdata_api.h

gaddi@khursheed33 MINGW64 /d/Programs/ZOOM-SDK/zoom-linux-sdk/zoom-linux-sdk/speechsdk/include/cxx_api ((127ae9d...))
$ ls -la
total 986
drwxr-xr-x 1 gaddi 197609     0 Mar 20 23:05 ./
drwxr-xr-x 1 gaddi 197609     0 Mar 20 18:31 ../
-rw-r--r-- 1 gaddi 197609  2010 Dec 16 02:39 azac_api_cxx_common.h
-rw-r--r-- 1 gaddi 197609   358 Dec 16 02:39 CMakeLists.txt
-rw-r--r-- 1 gaddi 197609  4543 Dec 16 02:39 speechapi_cxx.h
-rw-r--r-- 1 gaddi 197609 14802 Dec 16 02:39 speechapi_cxx_audio_config.h
-rw-r--r-- 1 gaddi 197609  8271 Dec 16 02:39 speechapi_cxx_audio_data_stream.h
-rw-r--r-- 1 gaddi 197609 15610 Dec 16 02:39 speechapi_cxx_audio_processing_options.h
-rw-r--r-- 1 gaddi 197609 42407 Dec 16 02:39 speechapi_cxx_audio_stream.h
-rw-r--r-- 1 gaddi 197609  7440 Dec 16 02:39 speechapi_cxx_audio_stream_format.h
-rw-r--r-- 1 gaddi 197609  5441 Dec 16 02:39 speechapi_cxx_auto_detect_source_lang_config.h
-rw-r--r-- 1 gaddi 197609  3268 Dec 16 02:39 speechapi_cxx_auto_detect_source_lang_result.h
-rw-r--r-- 1 gaddi 197609  2441 Dec 16 02:39 speechapi_cxx_class_language_model.h
-rw-r--r-- 1 gaddi 197609   557 Dec 16 02:39 speechapi_cxx_common.h
-rw-r--r-- 1 gaddi 197609 15617 Dec 16 02:39 speechapi_cxx_connection.h
-rw-r--r-- 1 gaddi 197609  1597 Dec 16 02:39 speechapi_cxx_connection_eventargs.h
-rw-r--r-- 1 gaddi 197609  4736 Dec 16 02:39 speechapi_cxx_connection_message.h
-rw-r--r-- 1 gaddi 197609  2095 Dec 16 02:39 speechapi_cxx_connection_message_eventargs.h
-rw-r--r-- 1 gaddi 197609 12129 Dec 16 02:39 speechapi_cxx_conversation.h
-rw-r--r-- 1 gaddi 197609 21469 Dec 16 02:39 speechapi_cxx_conversation_transcriber.h
-rw-r--r-- 1 gaddi 197609  4685 Dec 16 02:39 speechapi_cxx_conversation_transcription_eventargs.h
-rw-r--r-- 1 gaddi 197609  2286 Dec 16 02:39 speechapi_cxx_conversation_transcription_result.h
-rw-r--r-- 1 gaddi 197609 17672 Dec 16 02:39 speechapi_cxx_conversation_translator.h
-rw-r--r-- 1 gaddi 197609  8851 Dec 16 02:39 speechapi_cxx_conversation_translator_events.h
-rw-r--r-- 1 gaddi 197609  3688 Dec 16 02:39 speechapi_cxx_conversational_language_understanding_model.h
-rw-r--r-- 1 gaddi 197609 12986 Dec 16 02:39 speechapi_cxx_dialog_service_config.h
-rw-r--r-- 1 gaddi 197609 24045 Dec 16 02:39 speechapi_cxx_dialog_service_connector.h
-rw-r--r-- 1 gaddi 197609  5181 Dec 16 02:39 speechapi_cxx_dialog_service_connector_eventargs.h
-rw-r--r-- 1 gaddi 197609 11334 Dec 16 02:39 speechapi_cxx_embedded_speech_config.h
-rw-r--r-- 1 gaddi 197609 57985 Dec 16 02:39 speechapi_cxx_enums.h
-rw-r--r-- 1 gaddi 197609  4124 Dec 16 02:39 speechapi_cxx_event_logger.h
-rw-r--r-- 1 gaddi 197609   772 Dec 16 02:39 speechapi_cxx_eventargs.h
-rw-r--r-- 1 gaddi 197609  6920 Dec 16 02:39 speechapi_cxx_eventsignal.h
-rw-r--r-- 1 gaddi 197609  5188 Dec 16 02:39 speechapi_cxx_eventsignalbase.h
-rw-r--r-- 1 gaddi 197609  4760 Dec 16 02:39 speechapi_cxx_file_logger.h
-rw-r--r-- 1 gaddi 197609  2053 Dec 16 02:39 speechapi_cxx_grammar.h
-rw-r--r-- 1 gaddi 197609  3241 Dec 16 02:39 speechapi_cxx_grammar_list.h
-rw-r--r-- 1 gaddi 197609  1869 Dec 16 02:39 speechapi_cxx_grammar_phrase.h
-rw-r--r-- 1 gaddi 197609  4901 Dec 16 02:39 speechapi_cxx_hybrid_speech_config.h
-rw-r--r-- 1 gaddi 197609  4575 Dec 16 02:39 speechapi_cxx_intent_recognition_eventargs.h
-rw-r--r-- 1 gaddi 197609  3865 Dec 16 02:39 speechapi_cxx_intent_recognition_result.h
-rw-r--r-- 1 gaddi 197609 24434 Dec 16 02:39 speechapi_cxx_intent_recognizer.h
-rw-r--r-- 1 gaddi 197609  3343 Dec 16 02:39 speechapi_cxx_intent_trigger.h
-rw-r--r-- 1 gaddi 197609  2367 Dec 16 02:39 speechapi_cxx_keyword_recognition_eventargs.h
-rw-r--r-- 1 gaddi 197609  3978 Dec 16 02:39 speechapi_cxx_keyword_recognition_model.h
-rw-r--r-- 1 gaddi 197609  1550 Dec 16 02:39 speechapi_cxx_keyword_recognition_result.h
-rw-r--r-- 1 gaddi 197609  8374 Dec 16 02:39 speechapi_cxx_keyword_recognizer.h
-rw-r--r-- 1 gaddi 197609  4910 Dec 16 02:39 speechapi_cxx_language_understanding_model.h
-rw-r--r-- 1 gaddi 197609  1491 Dec 16 02:39 speechapi_cxx_log_level.h
-rw-r--r-- 1 gaddi 197609 11674 Dec 16 02:39 speechapi_cxx_meeting.h
-rw-r--r-- 1 gaddi 197609 18197 Dec 16 02:39 speechapi_cxx_meeting_transcriber.h
-rw-r--r-- 1 gaddi 197609  4610 Dec 16 02:39 speechapi_cxx_meeting_transcription_eventargs.h
-rw-r--r-- 1 gaddi 197609  3031 Dec 16 02:39 speechapi_cxx_meeting_transcription_result.h
-rw-r--r-- 1 gaddi 197609  5910 Dec 16 02:39 speechapi_cxx_memory_logger.h
-rw-r--r-- 1 gaddi 197609  7162 Dec 16 02:39 speechapi_cxx_participant.h
-rw-r--r-- 1 gaddi 197609  1171 Dec 16 02:39 speechapi_cxx_pattern_matching_entity.h
-rw-r--r-- 1 gaddi 197609  1055 Dec 16 02:39 speechapi_cxx_pattern_matching_intent.h
-rw-r--r-- 1 gaddi 197609 17288 Dec 16 02:39 speechapi_cxx_pattern_matching_model.h
-rw-r--r-- 1 gaddi 197609  3075 Dec 16 02:39 speechapi_cxx_phrase_list_grammar.h
-rw-r--r-- 1 gaddi 197609  9778 Dec 16 02:39 speechapi_cxx_pronunciation_assessment_config.h
-rw-r--r-- 1 gaddi 197609  4860 Dec 16 02:39 speechapi_cxx_pronunciation_assessment_result.h
-rw-r--r-- 1 gaddi 197609  3393 Dec 16 02:39 speechapi_cxx_properties.h
-rw-r--r-- 1 gaddi 197609 18951 Dec 16 02:39 speechapi_cxx_recognition_async_recognizer.h
-rw-r--r-- 1 gaddi 197609  1419 Dec 16 02:39 speechapi_cxx_recognition_base_async_recognizer.h
-rw-r--r-- 1 gaddi 197609  1563 Dec 16 02:39 speechapi_cxx_recognition_eventargs.h
-rw-r--r-- 1 gaddi 197609  8539 Dec 16 02:39 speechapi_cxx_recognition_result.h
-rw-r--r-- 1 gaddi 197609  1696 Dec 16 02:39 speechapi_cxx_recognizer.h
-rw-r--r-- 1 gaddi 197609  1923 Dec 16 02:39 speechapi_cxx_session.h
-rw-r--r-- 1 gaddi 197609  1733 Dec 16 02:39 speechapi_cxx_session_eventargs.h
-rw-r--r-- 1 gaddi 197609  1319 Dec 16 02:39 speechapi_cxx_smart_handle.h
-rw-r--r-- 1 gaddi 197609  3118 Dec 16 02:39 speechapi_cxx_source_lang_config.h
-rw-r--r-- 1 gaddi 197609  6990 Dec 16 02:39 speechapi_cxx_source_language_recognizer.h
-rw-r--r-- 1 gaddi 197609  2436 Dec 16 02:39 speechapi_cxx_speaker_identification_model.h
-rw-r--r-- 1 gaddi 197609  6984 Dec 16 02:39 speechapi_cxx_speaker_recognition_result.h
-rw-r--r-- 1 gaddi 197609  5274 Dec 16 02:39 speechapi_cxx_speaker_recognizer.h
-rw-r--r-- 1 gaddi 197609  2267 Dec 16 02:39 speechapi_cxx_speaker_verification_model.h
-rw-r--r-- 1 gaddi 197609 22586 Dec 16 02:39 speechapi_cxx_speech_config.h
-rw-r--r-- 1 gaddi 197609  4535 Dec 16 02:39 speechapi_cxx_speech_recognition_eventargs.h
-rw-r--r-- 1 gaddi 197609  2939 Dec 16 02:39 speechapi_cxx_speech_recognition_model.h
-rw-r--r-- 1 gaddi 197609  1229 Dec 16 02:39 speechapi_cxx_speech_recognition_result.h
-rw-r--r-- 1 gaddi 197609  2184 Dec 16 02:39 speechapi_cxx_speech_synthesis_bookmark_eventargs.h
-rw-r--r-- 1 gaddi 197609  1966 Dec 16 02:39 speechapi_cxx_speech_synthesis_eventargs.h
-rw-r--r-- 1 gaddi 197609  7277 Dec 16 02:39 speechapi_cxx_speech_synthesis_request.h
-rw-r--r-- 1 gaddi 197609  9876 Dec 16 02:39 speechapi_cxx_speech_synthesis_result.h
-rw-r--r-- 1 gaddi 197609  2386 Dec 16 02:39 speechapi_cxx_speech_synthesis_viseme_eventargs.h
-rw-r--r-- 1 gaddi 197609  3545 Dec 16 02:39 speechapi_cxx_speech_synthesis_word_boundary_eventargs.h
-rw-r--r-- 1 gaddi 197609 35094 Dec 16 02:39 speechapi_cxx_speech_synthesizer.h
-rw-r--r-- 1 gaddi 197609 11195 Dec 16 02:39 speechapi_cxx_speech_translation_config.h
-rw-r--r-- 1 gaddi 197609  3477 Dec 16 02:39 speechapi_cxx_speech_translation_model.h
-rw-r--r-- 1 gaddi 197609  3304 Dec 16 02:39 speechapi_cxx_string_helpers.h
-rw-r--r-- 1 gaddi 197609  4705 Dec 16 02:39 speechapi_cxx_synthesis_voices_result.h
-rw-r--r-- 1 gaddi 197609  6776 Dec 16 02:39 speechapi_cxx_translation_eventargs.h
-rw-r--r-- 1 gaddi 197609 16701 Dec 16 02:39 speechapi_cxx_translation_recognizer.h
-rw-r--r-- 1 gaddi 197609  5764 Dec 16 02:39 speechapi_cxx_translation_result.h
-rw-r--r-- 1 gaddi 197609  1909 Dec 16 02:39 speechapi_cxx_user.h
-rw-r--r-- 1 gaddi 197609  8161 Dec 16 02:39 speechapi_cxx_utils.h
-rw-r--r-- 1 gaddi 197609  5395 Dec 16 02:39 speechapi_cxx_voice_info.h
-rw-r--r-- 1 gaddi 197609  3188 Dec 16 02:39 speechapi_cxx_voice_profile.h
-rw-r--r-- 1 gaddi 197609 10831 Dec 16 02:39 speechapi_cxx_voice_profile_client.h
-rw-r--r-- 1 gaddi 197609  8180 Dec 16 02:39 speechapi_cxx_voice_profile_enrollment_result.h
-rw-r--r-- 1 gaddi 197609  7916 Dec 16 02:39 speechapi_cxx_voice_profile_phrase_result.h
-rw-r--r-- 1 gaddi 197609  5135 Dec 16 02:39 speechapi_cxx_voice_profile_result.h