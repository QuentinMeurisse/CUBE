set(LIB GAP)

add_library(${LIB}
        Matrix.h
        GAPConfig.h
        GAPWindow.cpp GAPConfig.h
        GAPworker.cpp GAPworker.h)

target_link_libraries(${LIB} PUBLIC GraphGame)
target_include_directories(${LIB} PUBLIC
        "${PROJECT_BINARY_DIR}/GraphGame"
        "${PROJECT_SOURCE_DIR}/GraphGame")

find_package(Qt5 COMPONENTS Widgets REQUIRED)
target_link_libraries(${LIB} PUBLIC Qt5::Widgets)
find_package(Qt5 COMPONENTS Core REQUIRED)
get_target_property(QtCore_location Qt5::Core LOCATION)

find_package(Threads)

target_link_libraries (${LIB} PUBLIC Threads::Threads)
target_link_libraries(${LIB} PUBLIC nlohmann_json)