cmake_minimum_required(VERSION 3.16)

project(meetingSDKDemo CXX)
set(CMAKE_CXX_STANDARD 14) # Updated to C++14 for std::make_unique
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Find dependencies
find_package(PkgConfig REQUIRED)
find_package(ZLIB REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(GIO REQUIRED gio-2.0)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/h)
include_directories(${GLIB_INCLUDE_DIRS} ${GIO_INCLUDE_DIRS})
include_directories(/usr/include/glib-2.0)
include_directories(/usr/lib/x86_64-linux-gnu/glib-2.0/include) # System-specific, adjust if needed

# Zoom SDK include path (adjust if SDK headers are elsewhere)
include_directories(${CMAKE_SOURCE_DIR}/lib/zoom_meeting_sdk/h)

# Link directories
link_directories(${CMAKE_SOURCE_DIR}/lib/zoom_meeting_sdk)
link_directories(${CMAKE_SOURCE_DIR}/lib/zoom_meeting_sdk/qt_libs)
link_directories(${CMAKE_SOURCE_DIR}/lib/zoom_meeting_sdk/qt_libs/Qt/lib)
link_directories(${GLIB_LIBRARY_DIRS} ${GIO_LIBRARY_DIRS})

# Executable definition
add_executable(meetingSDKDemo
    ${CMAKE_SOURCE_DIR}/meeting_sdk_demo.cpp
    ${CMAKE_SOURCE_DIR}/MeetingReminderEventListener.cpp
    ${CMAKE_SOURCE_DIR}/MeetingServiceEventListener.cpp
    ${CMAKE_SOURCE_DIR}/NetworkConnectionHandler.cpp
    ${CMAKE_SOURCE_DIR}/AuthServiceEventListener.cpp
    ${CMAKE_SOURCE_DIR}/MeetingParticipantsCtrlEventListener.cpp
    ${CMAKE_SOURCE_DIR}/MeetingRecordingCtrlEventListener.cpp
    ${CMAKE_SOURCE_DIR}/ZoomSDKRenderer.cpp
    ${CMAKE_SOURCE_DIR}/ZoomSDKAudioRawData.cpp
    ${CMAKE_SOURCE_DIR}/ZoomSDKVideoSource.cpp
    ${CMAKE_SOURCE_DIR}/ZoomSDKVirtualAudioMicEvent.cpp
)

# Link libraries
target_link_libraries(meetingSDKDemo
    ${GLIB_LIBRARIES}
    ${GIO_LIBRARIES}
    ${ZLIB_LIBRARIES}
    meetingsdk  # Zoom SDK library
    pthread     # Threading support
    curl        # For network operations
)

# Remove unnecessary or redundant libraries (gcc_s, gcc, glib-2.0 already covered by GLIB_LIBRARIES)
# If these are needed for specific reasons, re-add with justification

# Create symbolic link for libmeetingsdk.so
execute_process(
    COMMAND ln -sf libmeetingsdk.so libmeetingsdk.so.1
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/zoom_meeting_sdk
)

# Copy config.txt to runtime directory
configure_file(${CMAKE_SOURCE_DIR}/config.txt ${CMAKE_SOURCE_DIR}/bin/config.txt COPYONLY)

# Copy Zoom SDK libraries to runtime directory
file(COPY ${CMAKE_SOURCE_DIR}/lib/zoom_meeting_sdk/libmeetingsdk.so
     DESTINATION ${CMAKE_SOURCE_DIR}/bin)
file(COPY ${CMAKE_SOURCE_DIR}/lib/zoom_meeting_sdk/libmeetingsdk.so.1
     DESTINATION ${CMAKE_SOURCE_DIR}/bin)

# Optional: Copy additional Qt libs if needed by Zoom SDK
file(COPY ${CMAKE_SOURCE_DIR}/lib/zoom_meeting_sdk/qt_libs/
     DESTINATION ${CMAKE_SOURCE_DIR}/bin/qt_libs/)