cmake_minimum_required(VERSION 3.31)
project(Projekt2)

set(CMAKE_CXX_STANDARD 23)

add_executable(Projekt2 main.cpp
        Infrastructure/Node.h
        Infrastructure/Connection.h
        Algorithms/Dijkstra.h
        Utils/GigaKomarVector.h
        Algorithms/Kruskal.h
        Algorithms/Prim.h
        Algorithms/BellmanFord.h
        Algorithms/FordFulkerson.h
        Infrastructure/GraphDistanceSorter.h
        Infrastructure/Edge.h)
