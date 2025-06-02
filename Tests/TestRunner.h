#ifndef TESTRUNNER_H
#define TESTRUNNER_H
#include <vector>
#include <filesystem>
#include <iostream>
#include <iomanip>

#include "../Infrastructure/FileWriter.h"
#include "ReportGenerator.h"

class TestRunner
{
public:
    /**
     * Run comprehensive performance tests for all algorithms and generate chart data
     * @param generationCount Number of test runs per configuration (for averaging)
     */
    static void Run(int generationCount = 10)
    {
        std::cout << "=== Starting Performance Tests ===" << std::endl;
        std::cout << "Generation count per test: " << generationCount << std::endl << std::endl;

        // Test configuration
        std::vector<int> densities = {20, 60, 99};
        std::vector<int> vertexCounts = {100, 250, 300, 400, 500, 600, 800};
        
        // Collect all data
        std::vector<GraphDataHandler> allData;
        
        std::cout << "Running tests for " << vertexCounts.size() << " vertex counts and " 
                  << densities.size() << " densities..." << std::endl;
        
        int totalTests = vertexCounts.size() * densities.size() * 12; // 12 = 6 algorithms * 2 representations
        int currentTest = 0;
        
        for (const int vertexCount : vertexCounts)
        {
            for (const int density : densities)
            {
                std::cout << "\n--- Testing: " << vertexCount << " vertices, " << density << "% density ---" << std::endl;
                
                // Test all algorithm-representation combinations
                runAlgorithmTests(allData, generationCount, vertexCount, density, currentTest, totalTests);
            }
        }
        
        std::cout << "\n=== Tests Complete. Generating Files ===" << std::endl;
        
        // Create output directory
        std::filesystem::create_directories("../Results");
        
        // Write chart data files
        writeChartFiles(allData);
        
        std::cout << "\n=== All Done! ===" << std::endl;
        std::cout << "Generated " << allData.size() << " data points." << std::endl;
        std::cout << "Files saved to ../Results/ directory." << std::endl;
        std::cout << "Run Python scripts to generate charts." << std::endl;
    }

    /**
     * Run tests for specific problem type only
     */
    static void RunForProblemType(GraphProblemType problemType, int generationCount = 10)
    {
        std::cout << "=== Starting Tests for " << getProblemTypeName(problemType) << " ===" << std::endl;
        
        std::vector<int> densities = {20, 60, 99};
        std::vector<int> vertexCounts = {100, 250, 300, 400, 500, 600, 800};
        std::vector<GraphDataHandler> data;
        
        auto algorithms = getAlgorithmsForProblemType(problemType);
        
        int totalTests = vertexCounts.size() * densities.size() * algorithms.size() * 2; // 2 representations
        int currentTest = 0;
        
        for (const int vertexCount : vertexCounts)
        {
            for (const int density : densities)
            {
                for (const auto& algorithm : algorithms)
                {
                    runSingleAlgorithmTest(data, algorithm, generationCount, vertexCount, density, currentTest, totalTests);
                }
            }
        }
        
        std::filesystem::create_directories("../Results");
        writeChartFilesForProblemType(data, problemType);
        
        std::cout << "\n=== Done! ===" << std::endl;
    }

private:
    static void runAlgorithmTests(std::vector<GraphDataHandler>& allData, int generationCount, 
                                 int vertexCount, int density, int& currentTest, int totalTests)
    {
        // MST Algorithms
        runSingleAlgorithmTest(allData, Algorithm::KRUSKAL, generationCount, vertexCount, density, currentTest, totalTests);
        runSingleAlgorithmTest(allData, Algorithm::PRIM, generationCount, vertexCount, density, currentTest, totalTests);
        
        // Shortest Path Algorithms
        runSingleAlgorithmTest(allData, Algorithm::DIJKSTRA, generationCount, vertexCount, density, currentTest, totalTests);
        runSingleAlgorithmTest(allData, Algorithm::BELLMAN_FORD, generationCount, vertexCount, density, currentTest, totalTests);
        
        // Max Flow Algorithms
        runSingleAlgorithmTest(allData, Algorithm::FORD_FULKERSON_DFS, generationCount, vertexCount, density, currentTest, totalTests);
        runSingleAlgorithmTest(allData, Algorithm::FORD_FULKERSON_BFS, generationCount, vertexCount, density, currentTest, totalTests);
    }
    
    static void runSingleAlgorithmTest(std::vector<GraphDataHandler>& data, Algorithm algorithm, 
                                      int generationCount, int vertexCount, int density, 
                                      int& currentTest, int totalTests)
    {
        // Test both representations
        for (const auto& representation : {GraphRepresentationType::ADJACENCY_LIST, GraphRepresentationType::ADJACENCY_MATRIX})
        {
            currentTest++;
            
            std::cout << "[" << std::setw(3) << currentTest << "/" << totalTests << "] " 
                      << getAlgorithmName(algorithm) << " (" << getRepresentationName(representation) << ")... ";
            
            auto results = ReportGenerator::GenerateType1Report(algorithm, density, generationCount, vertexCount, representation);
            data.insert(data.end(), results.begin(), results.end());
            
            std::cout << "Done" << std::endl;
        }
    }
    
    static void writeChartFiles(const std::vector<GraphDataHandler>& data)
    {
        std::cout << "\nWriting Type 1 chart files..." << std::endl;
        FileWriter::writeType1ChartData(data, GraphProblemType::MST, "../Results/MST");
        FileWriter::writeType1ChartData(data, GraphProblemType::SHORTEST_PATH, "../Results/SHORTEST_PATH");
        FileWriter::writeType1ChartData(data, GraphProblemType::MAX_FLOW, "../Results/MAX_FLOW");
        
        std::cout << "Writing Type 2 chart files..." << std::endl;
        FileWriter::writeType2ChartData(data, GraphProblemType::MST, "../Results/MST");
        FileWriter::writeType2ChartData(data, GraphProblemType::SHORTEST_PATH, "../Results/SHORTEST_PATH");
        FileWriter::writeType2ChartData(data, GraphProblemType::MAX_FLOW, "../Results/MAX_FLOW");
    }
    
    static void writeChartFilesForProblemType(const std::vector<GraphDataHandler>& data, GraphProblemType problemType)
    {
        std::string baseName = "../Results/" + getProblemTypeName(problemType);
        
        std::cout << "Writing chart files for " << getProblemTypeName(problemType) << "..." << std::endl;
        FileWriter::writeType1ChartData(data, problemType, baseName);
        FileWriter::writeType2ChartData(data, problemType, baseName);
    }
    
    static std::vector<Algorithm> getAlgorithmsForProblemType(GraphProblemType problemType)
    {
        switch (problemType) {
            case GraphProblemType::MST:
                return {Algorithm::KRUSKAL, Algorithm::PRIM};
            case GraphProblemType::SHORTEST_PATH:
                return {Algorithm::DIJKSTRA, Algorithm::BELLMAN_FORD};
            case GraphProblemType::MAX_FLOW:
                return {Algorithm::FORD_FULKERSON_DFS, Algorithm::FORD_FULKERSON_BFS};
            default:
                return {};
        }
    }
    
    static std::string getProblemTypeName(GraphProblemType problemType)
    {
        switch (problemType) {
            case GraphProblemType::MST: return "MST";
            case GraphProblemType::SHORTEST_PATH: return "SHORTEST_PATH";
            case GraphProblemType::MAX_FLOW: return "MAX_FLOW";
            default: return "UNKNOWN";
        }
    }
    
    static std::string getAlgorithmName(Algorithm algorithm)
    {
        switch (algorithm) {
            case Algorithm::KRUSKAL: return "Kruskal";
            case Algorithm::PRIM: return "Prim";
            case Algorithm::DIJKSTRA: return "Dijkstra";
            case Algorithm::BELLMAN_FORD: return "Bellman-Ford";
            case Algorithm::FORD_FULKERSON_DFS: return "Ford-Fulkerson (DFS)";
            case Algorithm::FORD_FULKERSON_BFS: return "Ford-Fulkerson (BFS)";
            default: return "Unknown";
        }
    }
    
    static std::string getRepresentationName(GraphRepresentationType representation)
    {
        switch (representation) {
            case GraphRepresentationType::ADJACENCY_MATRIX: return "Matrix";
            case GraphRepresentationType::ADJACENCY_LIST: return "List";
            default: return "Unknown";
        }
    }
};

#endif //TESTRUNNER_H