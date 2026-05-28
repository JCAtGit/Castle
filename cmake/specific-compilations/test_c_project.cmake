# /home/user/eclipse-workspace/ClangASTTutorial/test/compilation-test/test_c_project
#add_executable(TestCProject test/compilation-test/test_c_project/main.cpp)
#target_compile_options(TestCProject PRIVATE -Wall -Wextra)

# Example executable
#add_executable(my_exe main.cpp)

# Compiler flags
#target_compile_options(my_exe PRIVATE -Wall -Wextra)

# Preprocessor definitions
target_compile_definitions(my_exe PRIVATE MY_DEFINE=1)

# Include directories
#target_include_directories(my_exe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link libraries (and their include/compile settings if they use PUBLIC/INTERFACE)
#target_link_libraries(my_exe PRIVATE some_lib)

#set_target_properties(my_exe PROPERTIES
#    CXX_STANDARD 17
#    CXX_STANDARD_REQUIRED ON
#    CXX_EXTENSIONS OFF
#)