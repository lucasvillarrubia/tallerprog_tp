# cmake_minimum_required(VERSION 3.16)

#project(duckgame_lobby VERSION 0.1 LANGUAGES CXX)

# Keep Qt settings consistent with parent
# if(NOT DEFINED QT_VERSION_MAJOR)
#     set(QT_VERSION_MAJOR 6)
# endif()

# Use parent's compiler settings
# if (NOT DEFINED CMAKE_CXX_STANDARD)
#     set(CMAKE_CXX_STANDARD 20)
# endif()

# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)

#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

# if(NOT DEFINED CMAKE_PREFIX_PATH)
#     set(CMAKE_PREFIX_PATH "/home/lucas/Qt/6.8.0/gcc_64")
# endif()

# include of build ninja
# include("/build/Desktop_Qt_6_8_0-Debug/build.ninja")

#set(CMAKE_PREFIX_PATH "/home/lucas/Qt/6.8.0/gcc_64")
#set(QT_VERSION_MAJOR 6)
# find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# set(PROJECT_SOURCES
#         main.cpp
#         lobby.cpp
#         lobby.h
#         lobby.ui
# )

# target_sources(taller_client
#     PRIVATE
#     main.cpp
#     lobby.cpp
#     lobby.ui
#     PUBLIC
#     lobby.h
# )

# if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#     qt_add_executable(duckgame_lobby
#         MANUAL_FINALIZATION
#         ${PROJECT_SOURCES}
#     )
# # Define target properties for Android with Qt 6 as:
# #    set_property(TARGET duckgame_lobby APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
# #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
# else()
#     if(ANDROID)
#         add_library(duckgame_lobby SHARED
#             ${PROJECT_SOURCES}
#         )
# # Define properties for Android with Qt 5 after find_package() calls as:
# #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#     else()
#         add_executable(duckgame_lobby
#             ${PROJECT_SOURCES}
#         )
#     endif()
# endif()

# add_library(duckgame_lobby STATIC
#     ${PROJECT_SOURCES}
# )

# target_link_libraries(duckgame_lobby
#     PRIVATE
#     Qt${QT_VERSION_MAJOR}::Core
#     Qt${QT_VERSION_MAJOR}::Widgets
# )

# set_target_properties(taller_client PROPERTIES
#     AUTOMOC ON
#     AUTORCC ON
#     AUTOUIC ON
# )

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
# if (Qt${QT_VERSION_MAJOR}_VERSION VERSION_LESS 6.1.0)
#   set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.duckgame_lobby)
# endif()
#     set_target_properties(duckgame_lobby PROPERTIES
#     ${BUNDLE_ID_OPTION}
#     MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#     MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#     MACOSX_BUNDLE TRUE
#     WIN32_EXECUTABLE TRUE
# )

# include(GNUInstallDirs)
# install(TARGETS duckgame_lobby
#     BUNDLE DESTINATION .
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )

# if(QT_VERSION_MAJOR EQUAL 6)
#     qt_finalize_executable(duckgame_lobby)
# endif()
