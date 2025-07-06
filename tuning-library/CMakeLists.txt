cmake_minimum_required(VERSION 3.10)
project(tuning-library VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)

get_directory_property(parent_dir PARENT_DIRECTORY)
if("${parent_dir}" STREQUAL "")
    set(is_toplevel 1)
else()
    set(is_toplevel 0)
endif()
option(TUNING_LIBRARY_BUILD_TESTS "Add targets for building and running tests" ${is_toplevel})

if(TUNING_LIBRARY_BUILD_TESTS)
    add_executable(tuning-library-symbolcheck)
    target_link_libraries(tuning-library-symbolcheck PRIVATE ${PROJECT_NAME})
    target_sources(tuning-library-symbolcheck PRIVATE
            tests/symbolcheck1.cpp
            tests/symbolcheck2.cpp)

    add_executable(tuning-library-tests)
    target_include_directories(tuning-library-tests PRIVATE libs/catch2)
    target_link_libraries(tuning-library-tests PRIVATE ${PROJECT_NAME})
    target_sources(tuning-library-tests PRIVATE
            tests/alltests.cpp)

    add_executable(showmapping commands/showmapping.cpp)
    target_link_libraries(showmapping ${PROJECT_NAME})

    add_executable(parsecheck commands/parsecheck.cpp)
    target_link_libraries(parsecheck ${PROJECT_NAME})

    if (WIN32)
        add_custom_target(run-all-tests
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                COMMAND $<TARGET_FILE:tuning-library-symbolcheck>
                COMMAND $<TARGET_FILE:tuning-library-tests>
            )
    else()
        add_custom_target(run-all-tests
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                COMMAND $<TARGET_FILE:tuning-library-symbolcheck>
                COMMAND $<TARGET_FILE:tuning-library-tests>
                COMMAND LANG=es_ES  $<TARGET_FILE:tuning-library-tests>
                COMMAND LANG=fr_FR  $<TARGET_FILE:tuning-library-tests>
                COMMAND LANG=zh_CN  $<TARGET_FILE:tuning-library-tests>
                COMMAND LANG=  $<TARGET_FILE:tuning-library-tests>
            )
    endif()
    add_dependencies(run-all-tests tuning-library-tests tuning-library-symbolcheck)
endif()
