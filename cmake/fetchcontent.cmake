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

# Fetch the header‑only library picosha2
#FetchContent_Declare(
#    picosha2
#    GIT_REPOSITORY https://github.com/okdshin/PicoSHA2.git
#    GIT_TAG        master
#)
#FetchContent_MakeAvailable(picosha2)

FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.4.1    # Using a specific version is a good practice
)
FetchContent_MakeAvailable(CLI11)

