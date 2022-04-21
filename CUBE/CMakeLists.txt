cmake_minimum_required(VERSION 3.10)
project(CUBE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

add_subdirectory(GameCore)
add_subdirectory(GameTheory)
add_subdirectory(GraphGame)
add_subdirectory(FLP)
add_subdirectory(GAP)
add_subdirectory(CUBELauncher)
add_subdirectory(DataStructures)
add_subdirectory(lib/fmt)
add_subdirectory(lib/nlohmann_json)

add_executable(CUBE main.cpp)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
get_target_property(QtCore_location Qt5::Core LOCATION)

target_link_libraries(CUBE PUBLIC CUBELauncher)
target_include_directories(CUBE PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/CUBELauncher")



