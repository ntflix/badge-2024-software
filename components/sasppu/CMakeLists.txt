set(SASPPU_GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/include/")

if (NOT EXISTS ${SASPPU_GEN_DIR})
    file(MAKE_DIRECTORY ${SASPPU_GEN_DIR})
endif()

set(SASPPU_GEN "${SASPPU_GEN_DIR}/sasppu_gen.inc")

idf_component_register(
    SRCS 
        sasppu_help.c
        sasppu_fast.S
        sasppu.c
    INCLUDE_DIRS
        .
    REQUIRES
)

# Generate sasppu_gen.inc
add_custom_command(OUTPUT ${SASPPU_GEN}
                   COMMAND ${python} ${CMAKE_CURRENT_SOURCE_DIR}/sasppu_generated.py ${SASPPU_GEN}
                   MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/sasppu_generated.py
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                   COMMENT "Generating sasppu_gen.inc"
                   )
        
add_custom_target(generate_sasppu_gen ALL DEPENDS ${SASPPU_GEN})
idf_component_get_property(sasppu_lib sasppu COMPONENT_LIB)
add_dependencies(${sasppu_lib} generate_sasppu_gen)

set_source_files_properties(${SASPPU_GEN} PROPERTIES HEADER_FILE_ONLY ON)

component_compile_options("-Wa,-I${SASPPU_GEN_DIR}")

