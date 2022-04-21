set(LIB DataStructures)


add_library(${LIB}
            Graph.h
            GraphUtility.h
            Matrix.h
            RedBlackTree.h
            Segment.cpp Segment.h)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
target_link_libraries(${LIB} PUBLIC Qt5::Widgets)

target_link_libraries(${LIB} PUBLIC GraphGame)
target_include_directories(${LIB} PUBLIC
        "${PROJECT_BINARY_DIR}/GraphGame"
        "${PROJECT_SOURCE_DIR}/GraphGame")