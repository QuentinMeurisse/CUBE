set(LIB GameTheory3D)
set(SOURCES GroupOfCells3D.cpp GroupOfCells3D.h
        SegmentTree.h
            CoMod3DInvariant.cpp CoMod3DInvariant.h
            Building3DInvariant.cpp Building3DInvariant.h
            GreenSpaces3DInvariant.cpp GreenSpaces3DInvariant.h
            HeightConstraint.cpp HeightConstraint.h
            Inflate3DStrat.cpp Inflate3DStrat.h
            Buildings3DPerimeterPlayer.cpp Buildings3DPerimeterPlayer.h
            NumBuilding3DPlayer.cpp NumBuilding3DPlayer.h
            IncreaseHeightStrat.cpp IncreaseHeightStrat.h
            HeightPlayer.cpp HeightPlayer.h)



add_library(${LIB} ${SOURCES})

target_link_libraries(${LIB} PUBLIC GameTheory)

target_link_libraries(${LIB} PUBLIC GameCore)

target_link_libraries(${LIB} PUBLIC "/usr/local/lib/libOpenMeshCore.so")
target_link_libraries(${LIB} PUBLIC "/usr/local/lib/libOpenMeshTools.so")
target_link_libraries(${LIB} PUBLIC nlohmann_json)




