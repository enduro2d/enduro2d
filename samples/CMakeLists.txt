function(add_e2d_sample NAME)
    set(SAMPLE_NAME sample_${NAME})

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

    file(GLOB ${SAMPLE_NAME}_sources
        sources/*.*
        sources/${SAMPLE_NAME}/*.*)
    set(SAMPLE_SOURCES ${${SAMPLE_NAME}_sources})
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SAMPLE_SOURCES})

    #
    # executable
    #

    add_executable(${SAMPLE_NAME}
        ${SAMPLE_SOURCES})

    target_include_directories(${SAMPLE_NAME}
        PRIVATE "../headers")

    target_link_libraries(${SAMPLE_NAME}
        enduro2d
        "${CMAKE_THREAD_LIBS_INIT}")

    if(APPLE)
        target_link_libraries(${SAMPLE_NAME}
            ${Foundation})
    endif(APPLE)

    #
    # resources
    #

    if(MSVC)
      add_custom_command(TARGET ${SAMPLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/bin
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/bin)
    else()
      add_custom_command(TARGET ${SAMPLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/bin
        ${CMAKE_CURRENT_BINARY_DIR}/bin)
    endif()
endfunction(add_e2d_sample)

add_e2d_sample(00)
add_e2d_sample(01)
