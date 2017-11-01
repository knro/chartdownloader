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
set(MACOSX_BUNDLE_ICON_FILE chartdownloader.icns)
set(myApp_ICON ${CMAKE_CURRENT_SOURCE_DIR}/res/chartdownloader.icns)
set_source_files_properties(${myApp_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif (APPLE)

add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${myApp_ICON} ${chart_SRCS} qml.qrc)

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Quick
    Qt5::Widgets
    Qt5::Network
    Qt5::Xml
    ${POPPLER_QT_LIBRARIES}
    )

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
