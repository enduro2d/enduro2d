function(add_e2d_tests NAME)
    set(TESTS_NAME untests_${NAME})

    #
    # external
    #

    find_package(Threads REQUIRED)

    if(APPLE)
        find_library(Foundation Foundation)
    endif(APPLE)

    #
    # sources
    #

    file(GLOB ${TESTS_NAME}_sources
        sources/*.*
        sources/${TESTS_NAME}/*.*)
    set(TESTS_SOURCES ${${TESTS_NAME}_sources})
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TESTS_SOURCES})

    #
    # executable
    #

    add_executable(${TESTS_NAME}
        ${TESTS_SOURCES})

    target_include_directories(${TESTS_NAME}
        PRIVATE "../headers")

    target_link_libraries(${TESTS_NAME}
        enduro2d
        "${CMAKE_THREAD_LIBS_INIT}")

    if(APPLE)
        target_link_libraries(${TESTS_NAME}
            ${Foundation})
    endif(APPLE)

    #
    # resources
    #

    if(MSVC)
      add_custom_command(TARGET ${TESTS_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/bin
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/bin)
    else()
      add_custom_command(TARGET ${TESTS_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/bin
        ${CMAKE_CURRENT_BINARY_DIR}/bin)
    endif()

    #
    # tests
    #

    add_test(${TESTS_NAME} ${TESTS_NAME})
endfunction(add_e2d_tests)

add_e2d_tests(base)
add_e2d_tests(core)
add_e2d_tests(math)
add_e2d_tests(utils)
