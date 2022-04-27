set(LIB GraphGame)

add_library(${LIB}
        GraphGame.cpp GraphGame.h
        Strats/SwapColorStrat.cpp Strats/SwapColorStrat.h
        Invariants/ColorNodeInvariant.cpp Invariants/ColorNodeInvariant.h
        Players/MovePlayer.cpp Players/MovePlayer.h
        GUI/GraphListener.cpp GUI/GraphListener.h
        GUI/GraphFrame.cpp GUI/GraphFrame.h
        Players/SwapPlayer.cpp Players/SwapPlayer.h
        Players/VariableMovePlayer.cpp Players/VariableMovePlayer.h
        GUI/GraphWindow.cpp GUI/GraphWindow.h
        GUI/GraphControlWindow.cpp GUI/GraphControlWindow.h
        GUI/GraphWorker.cpp GUI/GraphWorker.h)


target_link_libraries(${LIB} PUBLIC DataStructures)
target_include_directories(${LIB} PUBLIC
        "${PROJECT_BINARY_DIR}/DataStructures"
        "${PROJECT_SOURCE_DIR}/DataStructures")

target_link_libraries(${LIB} PUBLIC GameCore)
target_include_directories(${LIB} PUBLIC
        "${PROJECT_BINARY_DIR}/GameCore"
        "${PROJECT_SOURCE_DIR}/GameCore")

find_package(Qt5 COMPONENTS Widgets REQUIRED)
target_link_libraries(${LIB} PUBLIC Qt5::Widgets)
find_package(Qt5 COMPONENTS Core REQUIRED)
get_target_property(QtCore_location Qt5::Core LOCATION)

find_package(Threads)

target_link_libraries (${LIB} PUBLIC Threads::Threads)
target_link_libraries(${LIB} PUBLIC nlohmann_json)