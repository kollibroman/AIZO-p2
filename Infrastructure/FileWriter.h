#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <iostream>
#include "../Utils/Data/GraphDataHandler.h"
#include "../Utils/Enums/GraphProblemType.h"

class FileWriter
{
public:
    /**
     * Writes chart data for type 1 charts (separate charts for each graph representation)
     * Creates CSV files suitable for line charts with density as parameter and algorithm types
     * @param data Vector of GraphDataHandler containing measurement results
     * @param problemType Type of graph problem (MST or SHORTEST_PATH)
     * @param filename Base filename for output files
     */
    static void writeType1ChartData(const std::vector<GraphDataHandler>& data, 
                                   GraphProblemType problemType, 
                                   const std::string& filename);

    /**
     * Writes chart data for type 2 charts (separate charts for each graph density)
     * Creates CSV files suitable for line charts with algorithm-representation combinations
     * @param data Vector of GraphDataHandler containing measurement results
     * @param problemType Type of graph problem (MST or SHORTEST_PATH)
     * @param filename Base filename for output files
     */
    static void writeType2ChartData(const std::vector<GraphDataHandler>& data, 
                                   GraphProblemType problemType, 
                                   const std::string& filename);

private:
    /**
     * Groups data by representation type and organizes by density and algorithm
     */
    static std::map<GraphRepresentationType, std::map<int, std::map<Algorithm, long>>> 
        organizeDataByRepresentation(const std::vector<GraphDataHandler>& data);
    
    /**
     * Groups data by density and organizes by algorithm and representation
     */
    static std::map<int, std::map<Algorithm, std::map<GraphRepresentationType, long>>> 
        organizeDataByDensity(const std::vector<GraphDataHandler>& data);
    
    /**
     * Gets the relevant algorithms for the given problem type
     */
    static std::vector<Algorithm> getAlgorithmsForProblemType(GraphProblemType problemType);
    
    /**
     * Converts algorithm enum to string
     */
    static std::string algorithmToString(Algorithm algorithm);
    
    /**
     * Converts representation type enum to string
     */
    static std::string representationToString(GraphRepresentationType representation);
    
    /**
     * Writes CSV file for a specific representation (Type 1)
     */
    static void writeRepresentationCsv(const std::map<int, std::map<Algorithm, long>>& representationData,
                                      const std::vector<Algorithm>& algorithms,
                                      const std::string& filename,
                                      GraphRepresentationType representation);
    
    /**
     * Writes CSV file for a specific density (Type 2)
     */
    static void writeDensityCsv(const std::map<Algorithm, std::map<GraphRepresentationType, long>>& densityData,
                               const std::vector<Algorithm>& algorithms,
                               const std::string& filename,
                               int density);
};

// Implementation

inline void FileWriter::writeType1ChartData(const std::vector<GraphDataHandler>& data,
                                   GraphProblemType problemType, 
                                   const std::string& filename)
{
    if (data.empty()) {
        std::cerr << "Warning: No data provided to write" << std::endl;
        return;
    }
    
    // Organize data by representation type
    auto organizedData = organizeDataByRepresentation(data);
    
    // Get relevant algorithms for the problem type
    auto algorithms = getAlgorithmsForProblemType(problemType);
    
    if (algorithms.empty()) {
        std::cerr << "Error: No algorithms found for the specified problem type" << std::endl;
        return;
    }
    
    // Create separate CSV files for each representation
    for (const auto& [representation, representationData] : organizedData) {
        std::string repFilename = filename + "_type1_" + representationToString(representation) + ".csv";
        writeRepresentationCsv(representationData, algorithms, repFilename, representation);
    }
}

inline void FileWriter::writeType2ChartData(const std::vector<GraphDataHandler>& data,
                                   GraphProblemType problemType, 
                                   const std::string& filename)
{
    if (data.empty()) {
        std::cerr << "Warning: No data provided to write" << std::endl;
        return;
    }
    
    // Organize data by density
    auto organizedData = organizeDataByDensity(data);
    
    // Get relevant algorithms for the problem type
    auto algorithms = getAlgorithmsForProblemType(problemType);
    
    if (algorithms.empty()) {
        std::cerr << "Error: No algorithms found for the specified problem type" << std::endl;
        return;
    }
    
    // Create separate CSV files for each density
    for (const auto& [density, densityData] : organizedData) {
        std::string densityFilename = filename + "_type2_density" + std::to_string(density) + ".csv";
        writeDensityCsv(densityData, algorithms, densityFilename, density);
    }
}

std::map<GraphRepresentationType, std::map<int, std::map<Algorithm, long>>> 
inline FileWriter::organizeDataByRepresentation(const std::vector<GraphDataHandler>& data)
{
    std::map<GraphRepresentationType, std::map<int, std::map<Algorithm, long>>> organized;
    
    for (const auto& item : data) {
        organized[item.representation][item.Density][item.algorithm] = item.MeasuredTime;
    }
    
    return organized;
}

std::map<int, std::map<Algorithm, std::map<GraphRepresentationType, long>>> 
inline FileWriter::organizeDataByDensity(const std::vector<GraphDataHandler>& data)
{
    std::map<int, std::map<Algorithm, std::map<GraphRepresentationType, long>>> organized;
    
    for (const auto& item : data) {
        organized[item.Density][item.algorithm][item.representation] = item.MeasuredTime;
    }
    
    return organized;
}

std::vector<Algorithm> FileWriter::getAlgorithmsForProblemType(GraphProblemType problemType)
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

inline std::string FileWriter::algorithmToString(Algorithm algorithm)
{
    switch (algorithm) {
        case Algorithm::KRUSKAL: return "Kruskal";
        case Algorithm::PRIM: return "Prim";
        case Algorithm::DIJKSTRA: return "Dijkstra";
        case Algorithm::BELLMAN_FORD: return "Bellman-Ford";
        case Algorithm::FORD_FULKERSON_DFS: return "Ford-Fulkerson-DFS";
        case Algorithm::FORD_FULKERSON_BFS: return "Ford-Fulkerson-BFS";
        default: return "Unknown";
    }
}

inline std::string FileWriter::representationToString(GraphRepresentationType representation)
{
    switch (representation) {
        case GraphRepresentationType::ADJACENCY_MATRIX: return "AdjacencyMatrix";
        case GraphRepresentationType::ADJACENCY_LIST: return "AdjacencyList";
        default: return "Unknown";
    }
}

inline void FileWriter::writeRepresentationCsv(const std::map<int, std::map<Algorithm, long>>& representationData,
                                       const std::vector<Algorithm>& algorithms,
                                       const std::string& filename,
                                       GraphRepresentationType representation)
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing" << std::endl;
        return;
    }
    
    // Write header
    file << "Density";
    for (const auto& algorithm : algorithms) {
        file << "," << algorithmToString(algorithm);
    }
    file << std::endl;
    
    // Write data rows
    for (const auto& [density, algorithmData] : representationData) {
        file << density;
        
        for (const auto& algorithm : algorithms) {
            file << ",";
            auto it = algorithmData.find(algorithm);
            if (it != algorithmData.end()) {
                file << it->second;
            } else {
                file << ""; // Empty cell if no data for this algorithm at this density
            }
        }
        file << std::endl;
    }
    
    file.close();
    std::cout << "Type 1 chart data written to: " << filename << " for " << representationToString(representation) << std::endl;
}

inline void FileWriter::writeDensityCsv(const std::map<Algorithm, std::map<GraphRepresentationType, long>>& densityData,
                                 const std::vector<Algorithm>& algorithms,
                                 const std::string& filename,
                                 int density)
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing" << std::endl;
        return;
    }
    
    // Write header - Algorithm-Representation combinations
    file << "AlgorithmRepresentation,ExecutionTime" << std::endl;
    
    // Write data rows
    std::vector<GraphRepresentationType> representations = {
        GraphRepresentationType::ADJACENCY_MATRIX,
        GraphRepresentationType::ADJACENCY_LIST
    };
    
    for (const auto& algorithm : algorithms) {
        for (const auto& representation : representations) {
            std::string combinationName = algorithmToString(algorithm) + "_" + representationToString(representation);
            file << combinationName << ",";
            
            auto algIt = densityData.find(algorithm);
            if (algIt != densityData.end()) {
                auto repIt = algIt->second.find(representation);
                if (repIt != algIt->second.end()) {
                    file << repIt->second;
                } else {
                    file << ""; // Empty if no data
                }
            } else {
                file << ""; // Empty if no data
            }
            file << std::endl;
        }
    }
    
    file.close();
    std::cout << "Type 2 chart data written to: " << filename << " for density " << density << "%" << std::endl;
}

#endif //FILEWRITER_H