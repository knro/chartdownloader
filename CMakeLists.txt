cmake_minimum_required(VERSION 3.0)

project(chartdownloader LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(QT_VERSION_MAJOR 5)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/")
include(GNUInstallDirs)
include(FeatureSummary)

set(BIN_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/bin")

find_package(Qt5 COMPONENTS Core Quick Widgets Network Xml REQUIRED)
find_package(Poppler REQUIRED)

include_directories( ${POPPLER_QT_INCLUDE_DIR})

SET (chart_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/airport.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/chartinfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/downloadmanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/filedownloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/iconprovider.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/serviceprovider.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils.cpp
    # FAA Provider
    ${CMAKE_CURRENT_SOURCE_DIR}/src/faaservice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mycontenthandler.cpp
    # Australia Provider
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ausservice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/auscontenthandler.cpp
    )

if (APPLE)
    add_definitions(-DQ_OS_DARWIN)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.10")
    set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE)
    set(MACOSX_BUNDLE_ICON_FILE chartdownloader.icns)
    set(myApp_ICON ${CMAKE_CURRENT_SOURCE_DIR}/res/chartdownloader.icns)
    set_source_files_properties(${myApp_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${myApp_ICON} ${chart_SRCS} qml.qrc)
elseif (WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${chart_SRCS} qml.qrc)
else()
    add_executable(${PROJECT_NAME} ${chart_SRCS} qml.qrc)
endif (APPLE)

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Quick
    Qt5::Widgets
    Qt5::Network
    Qt5::Xml
    ${POPPLER_QT_LIBRARIES}
    )

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

if (APPLE)
find_path(MACDEPLOYQT_PATH macdeployqt PATH_SUFFIXES bin)
    if(NOT MACDEPLOYQT_PATH)
        message(FATAL_ERROR "Could not find macdeployqt for OSX bundling. You can point MACDEPLOYQT_PATH to it's path.")
    endif()

    # If macdeployqt is found just build and install.
    # OSX budle is ready.
    install(CODE "
            execute_process(COMMAND ${MACDEPLOYQT_PATH}/macdeployqt \"${BUNDLE_PATH}\" \"-qmldir=${CMAKE_SOURCE_DIR}/qml\" -verbose=2)
            message(STATUS \"Creating dmg of the app\")
            execute_process(COMMAND hdiutil create -fs HFS+ -volname ${PROJECT_NAME} -srcfolder \"${BUNDLE_PATH}\" -ov -format UDZO \"${PROJECT_NAME}.dmg\")
            " COMPONENT Runtime)
endif()

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
