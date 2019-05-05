function(add_e2d_tests NAME)
    set(TESTS_NAME untests_${NAME})

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

    add_executable(${TESTS_NAME} ${TESTS_SOURCES})
    target_link_libraries(${TESTS_NAME} enduro2d)
    set_target_properties(${TESTS_NAME} PROPERTIES FOLDER untests)

    #
    # resources
    #

    add_custom_command(TARGET ${TESTS_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/bin
        $<TARGET_FILE_DIR:${TESTS_NAME}>/bin)

    #
    # tests
    #

    add_test(${TESTS_NAME} ${TESTS_NAME})
endfunction(add_e2d_tests)

option(E2D_BUILD_BASE_UNTESTS "Build base untests" ON)
option(E2D_BUILD_CORE_UNTESTS "Build core untests" ON)
option(E2D_BUILD_HIGH_UNTESTS "Build high untests" ON)
option(E2D_BUILD_MATH_UNTESTS "Build math untests" ON)
option(E2D_BUILD_UTILS_UNTESTS "Build utils untests" ON)

if(E2D_BUILD_BASE_UNTESTS)
    add_e2d_tests(base)
endif()

if(E2D_BUILD_CORE_UNTESTS)
    add_e2d_tests(core)
endif()

if(E2D_BUILD_HIGH_UNTESTS)
    add_e2d_tests(high)
endif()

if(E2D_BUILD_MATH_UNTESTS)
    add_e2d_tests(math)
endif()

if(E2D_BUILD_UTILS_UNTESTS)
    add_e2d_tests(utils)
endif()
