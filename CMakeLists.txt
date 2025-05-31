cmake_minimum_required(VERSION 3.31)

set(CMAKE_CXX_STANDARD 23)

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
        Utils/Enums/GraphProblemType.h
        Utils/BinaryHeap.h
        Utils/Results/PathResult.h
        Utils/Results/MSTResult.h
        Utils/Results/FlowResult.h
        Utils/DefinitelyNotADataStructures/UnionFind.h
        Algorithms/Sorting.h
        Utils/DefinitelyNotADataStructures/DefinitelyNotAQueue.h
        Utils/Representations/Edge.h
        Menu/Abstractions/MenuBase.h
        Tests/TestRandomGraphGeneration.h
        Infrastructure/FileWriter.h
        Utils/Data/GraphDataHandler.h
        Utils/Enums/Algorithm.h
        Utils/Enums/GraphRepresentationType.h
        Tests/ReportGenerator.h
        Utils/Enums/SearchMethod.h
        Utils/Representations/GraphRepresentation.h)
