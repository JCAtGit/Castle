if(NOT LLVM_FOUND)
    # If not found, search again with REQUIRED and provide common HINTS
    find_package(LLVM 21.1.8 REQUIRED CONFIG
        HINTS
            # Linux (Debian/Ubuntu) typical paths
            /usr/lib/llvm-21
            /usr/lib/llvm21
            /usr/lib64/llvm21

            # Linux (Fedora/RHEL) typical paths
            /usr/lib64/llvm
            # macOS Homebrew
            /usr/local/opt/llvm
            /opt/homebrew/opt/llvm
            # macOS MacPorts
            /opt/local/libexec/llvm
            # Windows (typical install)
            "C:/Program Files/LLVM"
            "C:/Program Files (x86)/LLVM"
            # Environment variable hints
            $ENV{LLVM_ROOT}
            ${LLVM_ROOT}
    )
endif()

find_package(Clang REQUIRED CONFIG)

if(LLVM_FOUND)
    message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION} in ${LLVM_DIR}")
    include_directories(${LLVM_INCLUDE_DIRS})
    add_definitions(${LLVM_DEFINITIONS})
else()
    message(FATAL_ERROR "LLVM (version 21.1.8) not found. Please set LLVM_DIR or LLVM_ROOT to the directory containing LLVMConfig.cmake")
endif()

set(LLVM_LINK_COMPONENTS
  Support
)

link_directories(/usr/lib64/llvm21/lib64)
include_directories(/usr/lib64/llvm21/include)

# This applies to every target defined AFTER this line
link_libraries(
    clang-cpp
    LLVM
    nlohmann_json::nlohmann_json
	CLI11::CLI11
	gtest_main
)

include_directories(
    ${gtest_SOURCE_DIR}/include
    ${gtest_BINARY_DIR}/include
)
