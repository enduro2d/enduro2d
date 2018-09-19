function(add_e2d_sample NAME)
    set(SAMPLE_NAME sample_${NAME})

    #
    # external
    #

    find_package(OpenGL REQUIRED)
    find_package(Threads REQUIRED)

    if(APPLE)
        find_library(Cocoa Cocoa)
        find_library(IOKit IOKit)
        find_library(CoreVideo CoreVideo)
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
        ${OPENGL_gl_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT})

	if(WIN32 OR APPLE)
		target_link_libraries(${SAMPLE_NAME}
			glfw)
    endif(WIN32 OR APPLE)

    if(APPLE)
        target_link_libraries(${SAMPLE_NAME}
            ${Cocoa}
            ${IOKit}
            ${CoreVideo}
            ${Foundation})
    endif(APPLE)

    set_target_properties(${SAMPLE_NAME} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

    #
    # resources
    #

    if(MSVC)
        add_custom_command(TARGET ${SAMPLE_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/bin
            ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/bin)
    else(MSVC)
        add_custom_command(TARGET ${SAMPLE_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/bin
            ${CMAKE_CURRENT_BINARY_DIR}/bin)
    endif(MSVC)
endfunction(add_e2d_sample)

add_e2d_sample(00)
add_e2d_sample(01)
