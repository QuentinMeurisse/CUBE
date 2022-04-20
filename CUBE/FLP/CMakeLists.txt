set(LIB FLP)

add_library(${LIB}
        Genes.cpp Genes.h
        GeneticProblem.cpp GeneticProblem.h
        GeneticEasy.cpp GeneticEasy.h
        GeneticFLP.cpp GeneticFLP.h
        GeneticDisplay.cpp GeneticDisplay.h
        FLPWindow.cpp FLPWindow.h
        FLPSettingsWindow.cpp FLPSettingsWindow.h
        GeneticGUIListener.cpp GeneticGUIListener.h
        GeneticWorker.cpp GeneticWorker.h FLPColors.h)

target_link_libraries(${LIB} PUBLIC DataStructures)
target_include_directories(${LIB} PUBLIC
        "${PROJECT_BINARY_DIR}/DataStructures"
        "${PROJECT_SOURCE_DIR}/DataStructures")

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

