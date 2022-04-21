set(LIB CUBELauncher)

add_library(${LIB} CUBELauncher.cpp CUBELauncher.h)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
target_link_libraries(${LIB} PUBLIC Qt5::Widgets)

target_link_libraries(${LIB} PUBLIC GameTheory)
target_link_libraries(${LIB} PUBLIC GraphGame)
target_link_libraries(${LIB} PUBLIC FLP)
target_link_libraries(${LIB} PUBLIC GAP)
target_include_directories(${LIB} PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/GameTheory"
        "${PROJECT_SOURCE_DIR}/GameGraph"
        "${PROJECT_SOURCE_DIR}/FLP"
        "${PROJECT_SOURCE_DIR}/GAP")