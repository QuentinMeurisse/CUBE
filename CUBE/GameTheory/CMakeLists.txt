set(LIB GameTheory)

add_library(${LIB}
        Tiling.cpp Tiling.h
        SquareTiling.cpp SquareTiling.h
        RegularTiling.cpp RegularTiling.h
        GameTheoryUtility.cpp GameTheoryUtility.h
        GroupOfCells.cpp GroupOfCells.h
        UrbanBlockGame.cpp UrbanBlockGame.h
        Strategies/ChangeColorStrat.cpp Strategies/ChangeColorStrat.h
        Invariants/BuildingInvariant.cpp Invariants/BuildingInvariant.h
        Invariants/GreenSpaceInvariant.cpp Invariants/GreenSpaceInvariant.h
        GUI/TilingFrame.cpp GUI/TilingFrame.h
        RadiusConstraint.cpp RadiusConstraint.h
        Players/PorosityPlayer.cpp Players/PorosityPlayer.h
        Listeners/TilingListener.cpp Listeners/TilingListener.h
        Players/NumBuildingPlayer.cpp Players/NumBuildingPlayer.h
        Strategies/ChangeBlockColorStrat.cpp Strategies/ChangeBlockColorStrat.h
        Listeners/PrintListener.cpp Listeners/PrintListener.h
        Strategies/InflateStrat.cpp Strategies/InflateStrat.h
        Strategies/DeflateStrat.cpp Strategies/DeflateStrat.h
        Players/BuildingsPerimeterPlayer.cpp Players/BuildingsPerimeterPlayer.h
        Players/NumGreenSpacesPlayer.cpp Players/NumGreenSpacesPlayer.h
        Players/GreenSpaceRatePlayer.cpp Players/GreenSpaceRatePlayer.h 
        TwoRadiusConstraint.cpp TwoRadiusConstraint.h
        GUI/GameFrame.cpp GUI/GameFrame.h
        GUI/GameWorker.cpp GUI/GameWorker.h
        GUI/GUIListener.cpp GUI/GUIListener.h
        Invariants/UbranBlockGameInvariant.cpp Invariants/UbranBlockGameInvariant.h
        MultiSquareTilling.cpp MultiSquareTilling.h
        HexagonalTiling.cpp HexagonalTiling.h
        TriangularTiling.cpp TriangularTiling.h)

target_link_libraries(${LIB} PUBLIC GameCore)
target_include_directories(${LIB} PUBLIC
        "${PROJECT_BINARY_DIR}/GameCore"
        "${PROJECT_SOURCE_DIR}/GameCore")

target_link_libraries(${LIB} PUBLIC GameTheory3D)
target_include_directories(${LIB} PUBLIC
        "${PROJECT_BINARY_DIR}/GameTheory/3D"
        "${PROJECT_SOURCE_DIR}/GameTheory/3D")

target_link_libraries(${LIB} PUBLIC DataStructures)
target_include_directories(${LIB} PUBLIC
        "${PROJECT_BINARY_DIR}/DataStructures"
        "${PROJECT_SOURCE_DIR}/DataStructures")

list (APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_module)
find_package(OpenMesh REQUIRED)
include_directories(${OpenMesh_INCLUDE_DIRS})

target_link_libraries(${LIB} PUBLIC ${OpenMesh_LIBS} OpenMeshCore OpenMeshTools)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
target_link_libraries(${LIB} PUBLIC Qt5::Widgets)

target_link_libraries(${LIB} PUBLIC nlohmann_json)
add_subdirectory(3D)