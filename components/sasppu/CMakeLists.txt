set(SASPPU_GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/include/")

if (NOT EXISTS ${SASPPU_GEN_DIR})
    file(MAKE_DIRECTORY ${SASPPU_GEN_DIR})
endif()

set(SASPPU_GEN "${SASPPU_GEN_DIR}/sasppu_gen.h")

idf_component_register(
    SRCS 
        sasppu_help.c
        sasppu.c
    INCLUDE_DIRS
        .
        ${SASPPU_GEN_DIR}
    REQUIRES
)

# Generate sasppu_gen.h
add_custom_command(OUTPUT ${SASPPU_GEN}
                   COMMAND ${python} ${CMAKE_CURRENT_SOURCE_DIR}/sasppu_generated.py ${SASPPU_GEN}
                   MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/sasppu_generated.py
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                   COMMENT "Generating sasppu_gen.h"
                   )
        
add_custom_target(generate_sasppu_gen ALL DEPENDS ${SASPPU_GEN})
idf_component_get_property(sasppu_lib sasppu COMPONENT_LIB)
add_dependencies(${sasppu_lib} generate_sasppu_gen)

target_compile_options(${sasppu_lib} PRIVATE "-O3")

set_source_files_properties(${SASPPU_GEN} PROPERTIES HEADER_FILE_ONLY ON)

component_compile_options("-Wa,-I${CMAKE_CURRENT_SOURCE_DIR}")
