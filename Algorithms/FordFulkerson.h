#ifndef FORDFULKERSON_H
#define FORDFULKERSON_H
#include "../Utils/Results/FlowResult.h"
#include "../Utils/DefinitelyNotADataStructures/DefinitelyNotAQueue.h"
#include "../Utils/DefinitelyNotADataStructures/DefinitelyNotAVector.h"

enum class SearchMethod {
    BFS,
    DFS
};

class FordFulkersonList {
public:
    static FlowResult findMaxFlow(const AdjacencyList& graph, int source, int sink, SearchMethod method = SearchMethod::BFS) {
        int V = graph.getVertexCount();
        FlowResult result;
        result.maxFlow = 0;
        result.flowGraph = DefinitelyNotAVector<DefinitelyNotAVector<int>>(V);
        result.residualGraph = DefinitelyNotAVector<DefinitelyNotAVector<int>>(V);
        
        // Initialize graphs with zero vectors
        for (int i = 0; i < V; i++) {
            result.flowGraph[i] = DefinitelyNotAVector<int>(V, 0);
            result.residualGraph[i] = DefinitelyNotAVector<int>(V, 0);
        }

        // Initialize residual graph
        for (int u = 0; u < V; u++) {
            for (const auto& edge : graph.getAdjacent(u)) {
                result.residualGraph[u][edge.destination] = edge.weight;
            }
        }

        DefinitelyNotAVector<int> parent(V);
        bool pathFound = false;
        
        while (true) {
            // Choose search method based on parameter
            if (method == SearchMethod::BFS) {
                pathFound = bfs(result.residualGraph, source, sink, parent, V);
            } else {
                DefinitelyNotAVector<bool> visited(V, false);
                pathFound = dfs(result.residualGraph, source, sink, parent, visited, V);
            }
            
            if (!pathFound) break;
            
            int pathFlow = INT_MAX;
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                pathFlow = std::min(pathFlow, result.residualGraph[u][v]);
            }

            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                result.flowGraph[u][v] += pathFlow;
                result.flowGraph[v][u] -= pathFlow;
                result.residualGraph[u][v] -= pathFlow;
                result.residualGraph[v][u] += pathFlow;
            }

            result.maxFlow += pathFlow;
        }

        return result;
    }

private:
    static bool bfs(const DefinitelyNotAVector<DefinitelyNotAVector<int>>& rGraph, 
                   int s, int t, DefinitelyNotAVector<int>& parent, int V) {
        DefinitelyNotAVector<bool> visited(V, false);
        DefinitelyNotAQueue<int> queue;
        queue.push(s);
        visited[s] = true;
        parent[s] = -1;

        while (!queue.empty()) {
            int u = queue.front();
            queue.pop();

            for (int v = 0; v < V; v++) {
                if (!visited[v] && rGraph[u][v] > 0) {
                    queue.push(v);
                    parent[v] = u;
                    visited[v] = true;
                }
            }
        }

        return visited[t];
    }
    
    static bool dfs(const DefinitelyNotAVector<DefinitelyNotAVector<int>>& rGraph,
                    int u, int t, DefinitelyNotAVector<int>& parent, 
                    DefinitelyNotAVector<bool>& visited, int V) {
        if (u == t) {
            return true;
        }
        
        visited[u] = true;
        
        for (int v = 0; v < V; v++) {
            if (!visited[v] && rGraph[u][v] > 0) {
                parent[v] = u;
                if (dfs(rGraph, v, t, parent, visited, V)) {
                    return true;
                }
            }
        }
        
        return false;
    }
};

class FordFulkersonMatrix {
public:
    static FlowResult findMaxFlow(const AdjacencyMatrix& graph, int source, int sink, SearchMethod method = SearchMethod::BFS) {
        int V = graph.getVertexCount();
        FlowResult result;
        result.maxFlow = 0;
        result.flowGraph = DefinitelyNotAVector<DefinitelyNotAVector<int>>(V);
        result.residualGraph = DefinitelyNotAVector<DefinitelyNotAVector<int>>(V);
        
        // Initialize graphs with zero vectors
        for (int i = 0; i < V; i++) {
            result.flowGraph[i] = DefinitelyNotAVector<int>(V, 0);
            result.residualGraph[i] = DefinitelyNotAVector<int>(V, 0);
        }

        // Initialize residual graph
        for (int u = 0; u < V; u++) {
            for (int v = 0; v < V; v++) {
                int weight = graph.getWeight(u, v);
                if (weight != graph.getNoEdgeValue()) {
                    result.residualGraph[u][v] = weight;
                }
            }
        }

        DefinitelyNotAVector<int> parent(V);
        bool pathFound = false;
        
        while (true) {
            // Choose search method based on parameter
            if (method == SearchMethod::BFS) {
                pathFound = bfs(result.residualGraph, source, sink, parent, V);
            } else {
                DefinitelyNotAVector<bool> visited(V, false);
                pathFound = dfs(result.residualGraph, source, sink, parent, visited, V);
            }
            
            if (!pathFound) break;
            
            int pathFlow = INT_MAX;
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                pathFlow = std::min(pathFlow, result.residualGraph[u][v]);
            }

            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                result.flowGraph[u][v] += pathFlow;
                result.flowGraph[v][u] -= pathFlow;
                result.residualGraph[u][v] -= pathFlow;
                result.residualGraph[v][u] += pathFlow;
            }

            result.maxFlow += pathFlow;
        }

        return result;
    }

private:
    static bool bfs(const DefinitelyNotAVector<DefinitelyNotAVector<int>>& rGraph, 
                   int s, int t, DefinitelyNotAVector<int>& parent, int V) {
        DefinitelyNotAVector<bool> visited(V, false);
        DefinitelyNotAQueue<int> queue;
        queue.push(s);
        visited[s] = true;
        parent[s] = -1;

        while (!queue.empty()) {
            int u = queue.front();
            queue.pop();

            for (int v = 0; v < V; v++) {
                if (!visited[v] && rGraph[u][v] > 0) {
                    queue.push(v);
                    parent[v] = u;
                    visited[v] = true;
                }
            }
        }

        return visited[t];
    }
    
    static bool dfs(const DefinitelyNotAVector<DefinitelyNotAVector<int>>& rGraph,
                    int u, int t, DefinitelyNotAVector<int>& parent, 
                    DefinitelyNotAVector<bool>& visited, int V) {
        if (u == t) {
            return true;
        }
        
        visited[u] = true;
        
        for (int v = 0; v < V; v++) {
            if (!visited[v] && rGraph[u][v] > 0) {
                parent[v] = u;
                if (dfs(rGraph, v, t, parent, visited, V)) {
                    return true;
                }
            }
        }
        
        return false;
    }
};
#endif //FORDFULKERSON_H