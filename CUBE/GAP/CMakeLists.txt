set(LIB GAP)

add_library(${LIB}
        GAPConfig.h
        GAPWindow.cpp GAPConfig.h
        GAPWorker.cpp GAPWorker.h GAPAlgo.h)

target_link_libraries(${LIB} PUBLIC DataStructures)
target_include_directories(${LIB} PUBLIC
        "${PROJECT_BINARY_DIR}/DataStructures"
        "${PROJECT_SOURCE_DIR}/DataStructures")

find_package(Qt5 COMPONENTS Widgets REQUIRED)
target_link_libraries(${LIB} PUBLIC Qt5::Widgets)
find_package(Qt5 COMPONENTS Core REQUIRED)
get_target_property(QtCore_location Qt5::Core LOCATION)

find_package(Threads)

target_link_libraries (${LIB} PUBLIC Threads::Threads)
target_link_libraries(${LIB} PUBLIC nlohmann_json)