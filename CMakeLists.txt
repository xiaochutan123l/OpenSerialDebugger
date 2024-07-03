cmake_minimum_required(VERSION 3.5)

project(QMotorMaster VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# logo
if(WIN32)
  set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_SOURCE_DIR}/logo.rc")
endif()

## Add AddressSanitizer flags
#if (MSVC)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fsanitize=address")
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /fsanitize=address")
#else ()
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
#endif ()

# 添加 -bigobj 选项
if (MSVC)
  add_compile_options(/bigobj)
else ()
  add_compile_options(-Wa,-mbig-obj -O3)
endif ()

# debug
#add_definitions(-DUSE_FAKE_SERIAL)

#add_definitions(-DQCUSTOMPLOT_USE_OPENGL)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
                Widgets
                PrintSupport
                SerialPort
                #OpenGL
)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
                Widgets
                PrintSupport
                SerialPort
                #OpenGL
)

include_directories(./)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui

        qcustomplot.h
        qcustomplot.cpp

        clickablecombobox.h

        serialmonitor.h
        serialmonitor.cpp

        serialdatacontainer.h
        serialdatacontainer.cpp

        serialplotter.h
        serialplotter.cpp

        fakeserialport.h
        fakeserialport.cpp

        fakemotor.h
        fakemotor.cpp

        serialController.h
        serialController.cpp

        command.h
        command.cpp

        protocolparser.h
        protocolparser.cpp

        parametermanager.h
        parametermanager.cpp

        serialsendmonitor.h
        serialsendmonitor.cpp

        packethandler.h
        packethandler.cpp

        packet.h
        packet.cpp

        plotdatahandlerthread.h
        plotdatahandlerthread.cpp

    )
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QMotorMaster
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        icons.qrc
        # logo
        ${APP_ICON_RESOURCE_WINDOWS}
    )

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QMotorMaster APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(QMotorMaster SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(QMotorMaster
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(QMotorMaster
                        PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                        PRIVATE Qt${QT_VERSION_MAJOR}::PrintSupport
                        PRIVATE Qt${QT_VERSION_MAJOR}::SerialPort
                        #PRIVATE Qt${QT_VERSION_MAJOR}::OpenGL
)

# if (WIN32)
#     target_link_libraries(QMotorMaster PRIVATE opengl32)
# else()
#     target_link_libraries(QMotorMaster PRIVATE ${OPENGL_LIBRARIES})
# endif()

set_target_properties(QMotorMaster PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS QMotorMaster
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QMotorMaster)
endif()
