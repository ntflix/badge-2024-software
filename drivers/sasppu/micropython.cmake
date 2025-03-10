# Create an INTERFACE library for our C module.
add_library(usermod_sasppu INTERFACE)

# Add our source files to the lib
target_sources(usermod_sasppu INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/sasppu_fast.S
    ${CMAKE_CURRENT_LIST_DIR}/sasppu_render.c
    ${CMAKE_CURRENT_LIST_DIR}/sasppu_mpy.c
    ${CMAKE_CURRENT_LIST_DIR}/sasppu_gen.inc
)

add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/sasppu_gen.inc
                   COMMAND python sasppu_generated.py
                   MAIN_DEPENDENCY ${CMAKE_CURRENT_LIST_DIR}/sasppu_generated.py
                   WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                   COMMENT "Generating sasppu_gen"
                   )
add_custom_target(sasppu_gen DEPENDS ${CMAKE_CURRENT_LIST_DIR}/sasppu_gen.inc)

add_dependencies(usermod_sasppu sasppu_gen)

# Add the current directory as an include directory.
target_include_directories(usermod_sasppu INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Link our INTERFACE library to the usermod target.
target_link_libraries(usermod INTERFACE usermod_sasppu)

