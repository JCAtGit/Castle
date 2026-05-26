include(FetchContent)

# nlohmann/json
FetchContent_Declare(json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)
FetchContent_MakeAvailable(json)

# itzmeanjan/sha3
FetchContent_Declare(
    sha3
    GIT_REPOSITORY https://github.com/itzmeanjan/sha3.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(sha3)

FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.4.1
)
FetchContent_MakeAvailable(CLI11)

