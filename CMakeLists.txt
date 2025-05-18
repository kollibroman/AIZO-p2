cmake_minimum_required(VERSION 3.31)

set(CMAKE_CXX_STANDARD 23)
include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG v6.1.9
)

FetchContent_MakeAvailable(ftxui)

if(NOT ftxui_POPULATED)
    FetchContent_Populate(ftxui)
    add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

project(Projekt2 LANGUAGES CXX VERSION 1.0.0)
add_executable(Projekt2 main.cpp
        Algorithms/Dijkstra.h
        Utils/DefinitelyNotADataStructures/DefinitelyNotAVector.h
        Algorithms/Kruskal.h
        Algorithms/Prim.h
        Algorithms/BellmanFord.h
        Algorithms/FordFulkerson.h
        Menu/MainMenu.h
        Menu/MSTMenu.h
        Menu/SSPMenu.h
        Menu/MaxFlowMenu.h
        Infrastructure/RandomGraphGenerator.h
        Infrastructure/FileReader.h
        Utils/RepresentationStorer.h
        Utils/DefinitelyNotADataStructures/DefinitelyNotATuple.h
        Utils/Representations/AdjacencyList.h
        Utils/Representations/AdjancencyMatrix.h
        Infrastructure/GraphConverter.h
        Utils/GraphData.h
        Utils/GraphProblemType.h
        Utils/BinaryHeap.h
        Utils/Results/PathResult.h
        Utils/Results/MSTResult.h
        Utils/Results/FlowResult.h
        Utils/DefinitelyNotADataStructures/UnionFind.h
        Algorithms/Sorting.h
        Utils/DefinitelyNotADataStructures/DefinitelyNotAQueue.h
        Utils/Representations/Edge.h)

target_link_libraries(Projekt2
        # Chose a submodule
        PRIVATE ftxui::component
        PRIVATE ftxui::dom
        PRIVATE ftxui::screen
)
