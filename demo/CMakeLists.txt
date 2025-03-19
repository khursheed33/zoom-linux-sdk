cmake_minimum_required(VERSION 3.10)
project(MeetingSDKDemo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
find_package(ZLIB REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(GIO REQUIRED gio-2.0)

include_directories(
    ${GLIB_INCLUDE_DIRS}
    ${GIO_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include/h
    ${PROJECT_SOURCE_DIR}
)

link_directories(
    ${GLIB_LIBRARY_DIRS}
    ${GIO_LIBRARY_DIRS}
    ${PROJECT_SOURCE_DIR}/lib
)

add_executable(meetingSDKDemo
    meeting_sdk_demo.cpp
    MeetingReminderEventListener.cpp
    MeetingServiceEventListener.cpp
    NetworkConnectionHandler.cpp
    MeetingRecordingCtrlEventListener.cpp
    ZoomSDKRenderer.cpp
    ZoomSDKAudioRawData.cpp
    ZoomSDKVideoSource.cpp
    ZoomSDKVirtualAudioMicEvent.cpp
)

target_link_libraries(meetingSDKDemo
    ${GLIB_LIBRARIES}
    ${GIO_LIBRARIES}
    ZOOMSDK
    pthread
    ZLIB::ZLIB
)