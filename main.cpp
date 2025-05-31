#include <filesystem>

#include "Infrastructure/FileWriter.h"
#include "Menu/MainMenu.h"
#include "Tests/ReportGenerator.h"
#include "Tests/TestRandomGraphGeneration.h"

int main()
{
    MainMenu menu;
    menu.show();

    //Uncomment this to automatically generate report data
    // std::vector<int> densities = { 20, 50, 100 };
    // std::vector<GraphDataHandler> data;
    //
    // for (const int density : densities)
    // {
    //     data.append_range(ReportGenerator::GenerateType1Report(Algorithm::KRUSKAL, density, 10, 1000, GraphRepresentationType::ADJACENCY_LIST));
    //     data.append_range(ReportGenerator::GenerateType1Report(Algorithm::KRUSKAL, density, 10, 1000, GraphRepresentationType::ADJACENCY_MATRIX));
    //
    //     data.append_range(ReportGenerator::GenerateType1Report(Algorithm::PRIM, density, 10, 1000, GraphRepresentationType::ADJACENCY_LIST));
    //     data.append_range(ReportGenerator::GenerateType1Report(Algorithm::PRIM, density, 10, 1000, GraphRepresentationType::ADJACENCY_MATRIX));
    //
    //     data.append_range(ReportGenerator::GenerateType1Report(Algorithm::DIJKSTRA, density, 10, 1000, GraphRepresentationType::ADJACENCY_LIST));
    //     data.append_range(ReportGenerator::GenerateType1Report(Algorithm::DIJKSTRA, density, 10, 1000, GraphRepresentationType::ADJACENCY_MATRIX));
    //
    //     data.append_range(ReportGenerator::GenerateType1Report(Algorithm::BELLMAN_FORD, density, 10, 1000, GraphRepresentationType::ADJACENCY_LIST));
    //     data.append_range(ReportGenerator::GenerateType1Report(Algorithm::BELLMAN_FORD, density, 10, 1000, GraphRepresentationType::ADJACENCY_MATRIX));
    //
    //     data.append_range(ReportGenerator::GenerateType1Report(Algorithm::FORD_FULKERSON_DFS, density, 10, 1000, GraphRepresentationType::ADJACENCY_LIST));
    //     data.append_range(ReportGenerator::GenerateType1Report(Algorithm::FORD_FULKERSON_DFS, density, 10, 1000, GraphRepresentationType::ADJACENCY_MATRIX));
    //
    //     data.append_range(ReportGenerator::GenerateType1Report(Algorithm::FORD_FULKERSON_BFS, density, 10, 1000, GraphRepresentationType::ADJACENCY_LIST));
    //     data.append_range(ReportGenerator::GenerateType1Report(Algorithm::FORD_FULKERSON_BFS, density, 10, 1000, GraphRepresentationType::ADJACENCY_MATRIX));
    // }
    //
    // std::filesystem::create_directories("../Results");
    //
    // FileWriter::writeType1ChartData(data, GraphProblemType::MST, "../Results/Type1_MST");
    // FileWriter::writeType1ChartData(data, GraphProblemType::SHORTEST_PATH, "../Results/Type1_SHORTEST_PATH");
    // FileWriter::writeType1ChartData(data, GraphProblemType::MAX_FLOW, "../Results/Type1_MAX_FLOW");
    //
    // FileWriter::writeType2ChartData(data, GraphProblemType::MST, "../Results/Type2_MST");
    // FileWriter::writeType2ChartData(data, GraphProblemType::SHORTEST_PATH, "../Results/Type2_SHORTEST_PATH");
    // FileWriter::writeType2ChartData(data, GraphProblemType::MAX_FLOW, "../Results/Type2_MAX_FLOW");
    return 0;
}
