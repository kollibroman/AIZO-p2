#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <iostream>
#include "../Utils/Data/GraphDataHandler.h"
#include "../Utils/Enums/GraphProblemType.h"
#include <set>

class FileWriter
{
public:
    /**
     * Writes chart data for type 1 charts (separate charts for each graph representation)
     * Creates CSV files suitable for line charts with vertexCount as X-axis and algorithm-density combinations as lines
     * @param data Vector of GraphDataHandler containing measurement results
     * @param problemType Type of graph problem (MST or SHORTEST_PATH)
     * @param filename Base filename for output files
     */
    static void writeType1ChartData(const std::vector<GraphDataHandler>& data, 
                                   GraphProblemType problemType, 
                                   const std::string& filename);

    /**
     * Writes chart data for type 2 charts (separate charts for each graph density)
     * Creates CSV files suitable for line charts with vertexCount as X-axis and algorithm-representation combinations as lines
     * @param data Vector of GraphDataHandler containing measurement results
     * @param problemType Type of graph problem (MST or SHORTEST_PATH)
     * @param filename Base filename for output files
     */
    static void writeType2ChartData(const std::vector<GraphDataHandler>& data, 
                                   GraphProblemType problemType, 
                                   const std::string& filename);

private:
    /**
     * Groups data by representation type and organizes by vertex count and algorithm-density combinations
     */
    static std::map<GraphRepresentationType, std::map<int, std::map<std::string, long>>> 
        organizeDataByRepresentationForType1(const std::vector<GraphDataHandler>& data);
    
    /**
     * Groups data by density and organizes by vertex count and algorithm-representation combinations
     */
    static std::map<int, std::map<int, std::map<std::string, long>>> 
        organizeDataByDensityForType2(const std::vector<GraphDataHandler>& data);
    
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
    static void writeRepresentationCsvForType1(const std::map<int, std::map<std::string, long>>& representationData,
                                      const std::vector<Algorithm>& algorithms,
                                      const std::string& filename,
                                      GraphRepresentationType representation);
    
    /**
     * Writes CSV file for a specific density (Type 2)
     */
    static void writeDensityCsvForType2(const std::map<int, std::map<std::string, long>>& densityData,
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
    
    // Organize data by representation type for Type 1 format
    auto organizedData = organizeDataByRepresentationForType1(data);
    
    // Get relevant algorithms for the problem type
    auto algorithms = getAlgorithmsForProblemType(problemType);
    
    if (algorithms.empty()) {
        std::cerr << "Error: No algorithms found for the specified problem type" << std::endl;
        return;
    }
    
    // Create separate CSV files for each representation
    for (const auto& [representation, representationData] : organizedData) {
        std::string repFilename = filename + "_type1_" + representationToString(representation) + ".csv";
        writeRepresentationCsvForType1(representationData, algorithms, repFilename, representation);
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
    
    // Organize data by density for Type 2 format
    auto organizedData = organizeDataByDensityForType2(data);
    
    // Get relevant algorithms for the problem type
    auto algorithms = getAlgorithmsForProblemType(problemType);
    
    if (algorithms.empty()) {
        std::cerr << "Error: No algorithms found for the specified problem type" << std::endl;
        return;
    }
    
    // Create separate CSV files for each density
    for (const auto& [density, densityData] : organizedData) {
        std::string densityFilename = filename + "_type2_density" + std::to_string(density) + ".csv";
        writeDensityCsvForType2(densityData, algorithms, densityFilename, density);
    }
}

std::map<GraphRepresentationType, std::map<int, std::map<std::string, long>>> 
inline FileWriter::organizeDataByRepresentationForType1(const std::vector<GraphDataHandler>& data)
{
    std::map<GraphRepresentationType, std::map<int, std::map<std::string, long>>> organized;
    
    for (const auto& item : data) {
        // Create algorithm-density combination key
        std::string algoKey = algorithmToString(item.algorithm) + "_density" + std::to_string(item.Density);
        organized[item.representation][item.VertexCount][algoKey] = item.MeasuredTime;
    }
    
    return organized;
}

std::map<int, std::map<int, std::map<std::string, long>>> 
inline FileWriter::organizeDataByDensityForType2(const std::vector<GraphDataHandler>& data)
{
    std::map<int, std::map<int, std::map<std::string, long>>> organized;
    
    for (const auto& item : data) {
        // Create algorithm-representation combination key
        std::string algoRepKey = algorithmToString(item.algorithm) + "_" + representationToString(item.representation);
        organized[item.Density][item.VertexCount][algoRepKey] = item.MeasuredTime;
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

inline void FileWriter::writeRepresentationCsvForType1(const std::map<int, std::map<std::string, long>>& representationData,
                                       const std::vector<Algorithm>& algorithms,
                                       const std::string& filename,
                                       GraphRepresentationType representation)
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing" << std::endl;
        return;
    }
    
    // Collect all unique densities from the data
    std::set<int> uniqueDensities;
    for (const auto& [vertexCount, algoData] : representationData) {
        for (const auto& [algoKey, time] : algoData) {
            // Extract density from algorithm key
            size_t densityPos = algoKey.find("_density");
            if (densityPos != std::string::npos) {
                int density = std::stoi(algoKey.substr(densityPos + 8));
                uniqueDensities.insert(density);
            }
        }
    }
    
    // Write header: VertexCount followed by algorithm-density combinations
    file << "VertexCount";
    for (const auto& algorithm : algorithms) {
        for (int density : uniqueDensities) {
            file << "," << algorithmToString(algorithm) << "_density" << density;
        }
    }
    file << std::endl;
    
    // Write data rows
    for (const auto& [vertexCount, algoData] : representationData) {
        file << vertexCount;
        
        // For each algorithm-density combination
        for (const auto& algorithm : algorithms) {
            for (int density : uniqueDensities) {
                file << ",";
                std::string algoKey = algorithmToString(algorithm) + "_density" + std::to_string(density);
                auto it = algoData.find(algoKey);
                if (it != algoData.end()) {
                    file << it->second;
                } else {
                    file << ""; // Empty cell if no data for this combination
                }
            }
        }
        file << std::endl;
    }
    
    file.close();
    std::cout << "Type 1 chart data written to: " << filename << " for " << representationToString(representation) << std::endl;
}

inline void FileWriter::writeDensityCsvForType2(const std::map<int, std::map<std::string, long>>& densityData,
                                 const std::vector<Algorithm>& algorithms,
                                 const std::string& filename,
                                 int density)
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing" << std::endl;
        return;
    }
    
    // Write header: VertexCount followed by algorithm-representation combinations
    file << "VertexCount";
    std::vector<GraphRepresentationType> representations = {
        GraphRepresentationType::ADJACENCY_MATRIX,
        GraphRepresentationType::ADJACENCY_LIST
    };
    
    for (const auto& algorithm : algorithms) {
        for (const auto& representation : representations) {
            file << "," << algorithmToString(algorithm) << "_" << representationToString(representation);
        }
    }
    file << std::endl;
    
    // Write data rows
    for (const auto& [vertexCount, algoRepData] : densityData) {
        file << vertexCount;
        
        // For each algorithm-representation combination
        for (const auto& algorithm : algorithms) {
            for (const auto& representation : representations) {
                file << ",";
                std::string algoRepKey = algorithmToString(algorithm) + "_" + representationToString(representation);
                auto it = algoRepData.find(algoRepKey);
                if (it != algoRepData.end()) {
                    file << it->second;
                } else {
                    file << ""; // Empty cell if no data for this combination
                }
            }
        }
        file << std::endl;
    }
    
    file.close();
    std::cout << "Type 2 chart data written to: " << filename << " for density " << density << "%" << std::endl;
}

#endif //FILEWRITER_H