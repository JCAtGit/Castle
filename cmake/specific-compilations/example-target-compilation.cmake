# Example executable
add_executable(my_exe main.cpp)

# Compiler flags
target_compile_options(my_exe PRIVATE -Wall -Wextra)

# Preprocessor definitions
target_compile_definitions(my_exe PRIVATE MY_DEFINE=1)

# Include directories
target_include_directories(my_exe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link libraries (and their include/compile settings if they use PUBLIC/INTERFACE)
target_link_libraries(my_exe PRIVATE some_lib)



# --- Phase 0: gather sources
set(SOURCES source1.c source2.c) 	// set explicitly or
file(GLOB SOURCES "*.c")			// current dir only?
file(GLOB_RECURSE SOURCES "src/*.c")

# --- Phase 1: compile each source to bitcode ---

set(BC_FILES "")   # will collect .bc file names

foreach(src ${SOURCES})
    get_filename_component(name ${src} NAME_WE)
    set(bc_file ${CMAKE_CURRENT_BINARY_DIR}/${name}.bc)
    list(APPEND BC_FILES ${bc_file})

    add_custom_command(
        OUTPUT ${bc_file}
        COMMAND ${CMAKE_C_COMPILER} -emit-llvm -c ${CMAKE_CURRENT_SOURCE_DIR}/${src} -o ${bc_file}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${src}
        COMMENT "Generating bitcode ${bc_file}"
    )
endforeach()

# --- Phase 2: link all bitcode files into one ---
set(MERGED_BC ${CMAKE_CURRENT_BINARY_DIR}/merged.bc)

add_custom_command(
    OUTPUT ${MERGED_BC}
    COMMAND llvm-link ${BC_FILES} -o ${MERGED_BC}
    DEPENDS ${BC_FILES}
    COMMENT "Linking bitcode files with llvm-link"
)

# --- Phase 3: compile merged bitcode to executable ---
set(FINAL_EXE ${CMAKE_CURRENT_BINARY_DIR}/my_program${CMAKE_EXECUTABLE_SUFFIX})

add_custom_command(
    OUTPUT ${FINAL_EXE}
    COMMAND ${CMAKE_C_COMPILER} ${MERGED_BC} -o ${FINAL_EXE}
    DEPENDS ${MERGED_BC}
    COMMENT "Compiling merged bitcode to executable"
)

# --- create a top-level target that runs all phases ---
add_custom_target(build_phases ALL DEPENDS ${FINAL_EXE})