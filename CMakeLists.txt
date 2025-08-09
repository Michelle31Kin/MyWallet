cmake_minimum_required(VERSION 3.21)
project(myWallet LANGUAGES CXX VERSION 1.0)

# --------------------------------------------------------------------------------------------------------
# Create executable
# --------------------------------------------------------------------------------------------------------

file(GLOB SOURCES "src_cpp/*.cpp")
add_executable(${PROJECT_NAME} "main.cpp" ${SOURCES})

set(saucer_serializer "Glaze")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include_cpp/")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/embedded/")
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 23 CXX_EXTENSIONS OFF CXX_STANDARD_REQUIRED ON)

# --------------------------------------------------------------------------------------------------------
# Link libraries
# --------------------------------------------------------------------------------------------------------

include(FetchContent)

FetchContent_Declare(
    saucer 
    GIT_REPOSITORY "https://github.com/saucer/saucer" 
    GIT_TAG v6.0.1
)

FetchContent_MakeAvailable(saucer)

find_package(SQLite3 REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE saucer::saucer SQLite::SQLite3)