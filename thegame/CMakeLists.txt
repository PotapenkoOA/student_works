cmake_minimum_required(VERSION 3.14)

project(thegame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(interfaces)

option(ENABLE_TESTING "Enable testing" OFF)
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

if(ENABLE_TESTING)
    enable_testing()
    if(ENABLE_COVERAGE)
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            message(INFO "Coverage is enable")
            add_compile_options(--coverage -fprofile-arcs -ftest-coverage)
            add_link_options(--coverage)
        else()
            message(WARNING "Coverage is not supported for this compiler")
        endif()
    endif()
endif()

add_executable(thegame main.cpp
    interfaces/icommand.h
    interfaces/icommandqueue.h
    epoint.h epoint.cpp
    interfaces/iepoint.h
    context.h
    start.h start.cpp
    move.h move.cpp
    reset.h reset.cpp
    field.h
    game.h
    start_player.h players_list.h calculate.h
)

find_package(Boost COMPONENTS program_options json REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(thegame PRIVATE ${Boost_LIBRARIES})

include(GNUInstallDirs)
install(TARGETS thegame
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_custom_command(
    TARGET thegame
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:thegame> ${CMAKE_BINARY_DIR}/bin
)

if(ENABLE_COVERAGE)
    find_program(LCOV_PATH lcov REQUIRED)

    find_program(GENHTML_PATH genhtml REQUIRED)

    add_custom_target(coverage
        COMMAND ${LCOV_PATH} --directory . --zerocounters

        COMMAND $<TARGET_FILE:thegame>

        COMMAND ${LCOV_PATH} --directory . --capture --output-file coverage.info

        COMMAND ${LCOV_PATH} --remove coverage.info '/usr/*' '*/tests/*' --output-file coverage_filtered.info

        COMMAND ${GENHTML_PATH} coverage_filtered.info --output-directory coverage_report

        COMMAND cmake -E echo "Coverage report: file://${CMAKE_BINARY_DIR}/coverage_report/index.html"

        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}

        DEPENDS test_my_app
    )

endif()

if(ENABLE_TESTING)
    add_subdirectory(tests)
endif()
