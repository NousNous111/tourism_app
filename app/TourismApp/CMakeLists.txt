cmake_minimum_required(VERSION 3.16)

project(TourismApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES
    main.cpp

    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    loginwindow.cpp
    loginwindow.h
    loginwindow.ui

    registerwindow.cpp
    registerwindow.h
    registerwindow.ui

    myorderswindow.cpp
    myorderswindow.h
    myorderswindow.ui

    database/databasemanager.h
    database/databasemanager.cpp

    models/user.h

    controllers/authcontroller.h
    controllers/authcontroller.cpp
    controllers/clientcontroller.h
    controllers/clientcontroller.cpp
    controllers/ordercontroller.h
    controllers/ordercontroller.cpp

    profilewindow.cpp
    profilewindow.h
    profilewindow.ui

    adminwindow.cpp
    adminwindow.h
    adminwindow.ui

    controllers/admincontroller.h
    controllers/admincontroller.cpp

    interestedpackageswindow.h
    interestedpackageswindow.cpp
    interestedpackageswindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TourismApp
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        controllers/clientcontroller.h
        controllers/clientcontroller.cpp
        controllers/ordercontroller.h
        controllers/ordercontroller.cpp
        profilewindow.h profilewindow.cpp profilewindow.ui
        adminwindow.h adminwindow.cpp adminwindow.ui
        controllers/admincontroller.h
        controllers/admincontroller.cpp
        interestedpackageswindow.h interestedpackageswindow.cpp interestedpackageswindow.ui
    )
else()
    if(ANDROID)
        add_library(TourismApp SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(TourismApp
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(TourismApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(TourismApp PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TourismApp)
endif()

set_target_properties(TourismApp PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)

install(TARGETS TourismApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TourismApp)
endif()