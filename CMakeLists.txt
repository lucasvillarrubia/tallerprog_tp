cmake_minimum_required(VERSION 3.13)

if (POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif()

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enableD
set(CMAKE_CXX_EXTENSIONS ON)

project(taller_tp
    VERSION 1.0
    HOMEPAGE_URL "https://github.com/URL-HERE"
    LANGUAGES CXX
    )

option(TALLER_TESTS "Enable / disable tests." ON)
option(TALLER_CLIENT "Enable / disable client program." ON)
option(TALLER_SERVER "Enable / disable server program." ON)
option(TALLER_MAKE_WARNINGS_AS_ERRORS "Enable / disable warnings as errors." ON)

message(CMAKE_CXX_COMPILER_ID="${CMAKE_CXX_COMPILER_ID}")

# Dependencia de yaml-cpp
include(FetchContent)
FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG master  # O usa una versión específica, como `yaml-cpp-0.7.0`
)
FetchContent_MakeAvailable(yaml-cpp)


# Define target: lib common between client, server and editor programs
# HEY TODO XXX: change the name to something that you like
# TODO: Hay que mover estos paths al cmake que está en "common/", por ahora están acá porque el CLion los crea rápido!
add_library(taller_common STATIC
        common/hands_on_sockets/socket.h
        # common/message.h
        common/hands_on_threads/queue.h
        common/hands_on_threads/thread.h
        common/hands_on_sockets/resolvererror.h
        common/hands_on_sockets/liberror.h
        common/hands_on_sockets/resolver.h
        common/hands_on_sockets/liberror.cpp
        # common/message.cpp
        common/hands_on_sockets/resolver.cpp
        common/hands_on_sockets/resolvererror.cpp
        common/hands_on_sockets/socket.cpp
        common/comms/receiver.cpp
        common/comms/receiver.h
        common/comms/sender.cpp
        common/comms/sender.h
        common/comms/protocol.cpp
        common/comms/protocol.h
        common/gamedata.h
        common/rectangulo.h
        common/coordinates.h
        common/drawingdata.h
        common/color.h
)

# add the source files that make the lib
# HEY!! TODO XXX: add more subfolders here
add_subdirectory(common/)


# set compiler warnings to the targets
include(cmake/CompilerWarnings.cmake)
set_project_warnings(taller_common ${TALLER_MAKE_WARNINGS_AS_ERRORS} FALSE)

# Say where the headers will be found required by library target
# Make this PUBLIC so any dependant linking lib will inherit this
# location (so our dependants will not have to explicitly request this)
target_include_directories(taller_common PUBLIC .)

# Install libSDL2pp (for client or editor)
# HEY!! TODO XXX: add more dependendies (either for client, server or editor)
if(TALLER_CLIENT)
    include(FetchContent)
    FetchContent_Declare(
        libSDL2pp
        URL https://github.com/libSDL2pp/libSDL2pp/archive/cc198c9a5657048bee67ece82de620b2d5661084.zip
    )
    # set up SDL2pp library
    set(SDL2PP_WITH_IMAGE YES)
    set(SDL2PP_WITH_TTF YES)
    set(SDL2PP_WITH_MIXER YES)
    FetchContent_MakeAvailable(libSDL2pp)
endif()

# Program section
# ---------------
if(TALLER_CLIENT)
    ## lobby!
    if(NOT DEFINED QT_VERSION_MAJOR)
        set(QT_VERSION_MAJOR 6)
    endif()

    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

    # set_target_properties(taller_client PROPERTIES
    #     AUTOMOC ON
    #     AUTORCC ON
    #     AUTOUIC ON
    # )
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    # find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
    # find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

    # HEY!! TODO XXX: Change the name of the executable to the one you like!
    # TODO: Hay que mover estos paths al cmake que está en "client/", por ahora están acá porque el CLion los crea rápido!
    add_executable(taller_client
            client/client.h
            client/client_comms/client_protocol.h
            client/client_exceptions.h
            client/client_comms/client_sender.h
            client/client_comms/client_receiver.h
            client/client.cpp
            client/client_comms/client_protocol.cpp
            client/renderer.h
            client/client_threads/event_listener.h
            client/local_player.h
            client/local_player.cpp
            client/client_comms/client_sender.cpp
            client/client_comms/client_receiver.cpp
            client/renderer.cpp
            client/client_threads/event_listener.cpp
            client/character.cpp
            client/character.h
            client/state_manager.cpp
            client/texture_manager.h
            client/state_manager.h
            client/client_threads/updater.cpp
            client/client_threads/updater.h
            client/lobby/lobby.cpp
            client/lobby/lobby.h
            client/lobby/lobby.ui
            # client/color.h
    )

    # Make them depend on taller_common lib
    add_dependencies(taller_client taller_common SDL2pp::SDL2pp)

    # Add source files and enable warnings
    # HEY!! TODO XXX: add more subfolders here
    add_subdirectory(client)
    # add_subdirectory(client/lobby)

    set_project_warnings(taller_client ${TALLER_MAKE_WARNINGS_AS_ERRORS} FALSE)

    # add libSDL2pp's headers
    target_include_directories(taller_client PRIVATE ${libSDL2pp_SOURCE_DIR})

    # Link the taller_common lib target
    target_link_libraries(
        taller_client
        taller_common
        SDL2pp::SDL2pp
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        yaml-cpp
    )

    # Creo que estoy agregando la carpeta de imágenes acá
    add_custom_target(copy_resources ALL
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources
    )

    # Para la instalación??
    #install(DIRECTORY resources/ DESTINATION ${CMAKE_INSTALL_PREFIX}/resources)

endif()

if(TALLER_SERVER)
    # HEY!! TODO XXX: Change the name of the executable to the one you like!
    # TODO: Hay que mover estos paths al cmake que está en "server/", por ahora están acá porque el CLion los crea rápido!
    add_executable(taller_server
            server/server.h
            server/server_comms/server_protocol.h
            # server/command.h
            server/server_threads/acceptor.h
            server/server_threads/gameplay.h
            server/monitored_list.h
            server/player.h
            server/server_comms/server_sender.h
            server/server_comms/server_receiver.h
            # server/weapon.h
            server/server_threads/acceptor.cpp
            server/server_threads/gameplay.cpp
            # server/monitored_list.cpp
            server/player.cpp
            server/server_comms/server_protocol.cpp
            server/server_comms/server_receiver.cpp
            server/server_comms/server_sender.cpp
            server/server.cpp
            server/server_threads/cleaner.cpp
            server/server_threads/cleaner.h
            server/duck.cpp
            server/duck.h
            server/state_manager.cpp
            server/state_manager.h
            server/server_threads/match_manager.cpp
            server/server_threads/match_manager.h
            server/match.cpp
            server/match.h
            server/terrain.cpp
            server/terrain.h
            server/server_guns/AK47.cpp
			server/server_guns/AK47.h
			server/server_guns/AK47Ammo.cpp
			server/server_guns/AK47Ammo.h
			server/server_guns/ammo.cpp
			server/server_guns/ammo.h
			server/server_guns/CowboyPistol.cpp
			server/server_guns/CowboyPistol.h
			server/server_guns/CowboyPistolAmmo.cpp
			server/server_guns/CowboyPistolAmmo.h
			server/server_guns/DuelPistol.cpp
			server/server_guns/DuelPistol.h
			server/server_guns/DuelPistolAmmo.cpp
			server/server_guns/DuelPistolAmmo.h
			server/server_guns/gun.cpp
			server/server_guns/gun.h
			server/server_guns/item.cpp
			server/server_guns/item.h
			server/server_guns/Magnum.cpp
			server/server_guns/Magnum.h
			server/server_guns/MagnumAmmo.cpp
			server/server_guns/MagnumAmmo.h
			server/server_guns/Sniper.cpp
			server/server_guns/Sniper.h
			server/server_guns/SniperAmmo.cpp
			server/server_guns/SniperAmmo.h
			server/server_guns/Banana.h
			server/server_guns/Banana.cpp
			server/server_guns/SpawnPlace.cpp
			server/server_guns/SpawnPlace.h
			server/server_guns/Grenade.cpp
			server/server_guns/Grenade.h
			server/server_guns/PewPewLaser.cpp
			server/server_guns/PewPewLaser.h
			server/server_guns/PewPewLaserAmmo.cpp
			server/server_guns/PewPewLaserAmmo.h
			server/server_guns/LaserRifle.cpp
			server/server_guns/LaserRifle.h
			server/server_guns/LaserRifleAmmo.cpp
			server/server_guns/LaserRifleAmmo.h
			server/server_guns/Shotgun.cpp
			server/server_guns/Shotgun.h
			server/server_guns/ShotgunAmmo.cpp
			server/server_guns/ShotgunAmmo.h)

    # Make them depend on taller_common lib
    add_dependencies(taller_server taller_common)

    # Add source files and enable warnings
    # HEY!! TODO XXX: add more subfolders here
    add_subdirectory(server)

    set_project_warnings(taller_server ${TALLER_MAKE_WARNINGS_AS_ERRORS} FALSE)

    # Link the taller_common lib target
    target_link_libraries(taller_server taller_common yaml-cpp)
endif()




# Testing section
# ---------------

if(TALLER_TESTS)
    # Let's use google test
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )

    # For Windows: Prevent overriding the parent project's compiler/linker settings
    # (borrowed from https://google.github.io/googletest/quickstart-cmake.html)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    include(GoogleTest)

    # target for testing
    # HEY!! TODO XXX: Change the name of the executable to the one you like!
    add_executable(taller_tests)

    # HEY!! TODO XXX: I'm assuming that the code under testing is in taller_common
    # Change this if you have to.
    add_dependencies(taller_tests taller_common)

    # Say where to find the xoz headers
    # HEY!! TODO XXX: add more subfolders here
    target_include_directories(taller_tests PUBLIC .)

    # Add the source files to the taller_tests target
    # HEY!! TODO XXX: add more subfolders here
    add_subdirectory(tests/)

    # Set the compiler warnings to the tests target (but be a little lesser strict)
    set_project_warnings(taller_tests ${TALLER_MAKE_WARNINGS_AS_ERRORS} TRUE)

    # Say where googletest headers can be found
    target_include_directories(taller_tests
        PUBLIC
        ${gtest_SOURCE_DIR}/include
        ${gmock_SOURCE_DIR}/include
        )

    # HEY!! TODO XXX: I'm assuming that the code under testing is in taller_common
    # Change this if you have to.
    target_link_libraries(taller_tests
        taller_common
        GTest::gtest_main
    )
endif()
