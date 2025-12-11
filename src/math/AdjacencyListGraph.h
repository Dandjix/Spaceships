#pragma once
#include <list>
#include <unordered_map>
#include <utility>
#include <unordered_set>

template<typename T>
/// <summary>
/// A graph made for representing the rooms of a spaceship : it also has the distance  between each vertex
/// </summary>
class AdjacencyListGraph {
private:
    std::unordered_set<T> connected_recursive(const std::unordered_set<T> &vertices, int graphDistance) const {
        if (graphDistance == 1) {
            std::unordered_set<T> adjacentVertices;
            for (auto vertex: vertices) {
                //if (adjacencyListMap.find(vertex) == adjacencyListMap.end())
                //{
                //	continue;
                //}
                const std::list<std::pair<T, int> > &it = adjacencyListMap.at(vertex);
                for (const std::pair<T, int> &pair: it) {
                    adjacentVertices.insert(pair.first);
                }
            }
            return adjacentVertices;
        }
        std::unordered_set<T> next = connected_recursive(vertices, graphDistance - 1);
        std::unordered_set<T> adjacentVertices = next;
        for (auto &vertex: next) {
            const auto &neighbors = adjacencyListMap.at(vertex);
            for (const auto &pair: neighbors) {
                adjacentVertices.insert(pair.first);
            }
        }
        return adjacentVertices;
    }

protected:
    std::unordered_map<T, std::list<std::pair<T, int> > > adjacencyListMap;

public:
    ~AdjacencyListGraph() {
        for (auto room: adjacencyListMap) {
            delete room.first;
        }
    }

    void addVertex(T vertex) {
        adjacencyListMap[vertex] = std::list<std::pair<T, int> >();
    }

    void removeVertex(T vertex) {
        adjacencyListMap.erase(vertex);
    }

    bool hasVertex(T vertex) const {
        return adjacencyListMap.find(vertex) != adjacencyListMap.end();
    }

    void addEdge(T vertex1, T vertex2, int distance) {
        auto &adj_list_1 = adjacencyListMap[vertex1];
        adj_list_1.push_back({vertex2, distance});

        auto &adj_list_2 = adjacencyListMap[vertex2];
        adj_list_2.push_back({vertex1, distance});
    }

    void removeEdge(T vertex1, T vertex2) {
        auto &adj_list_1 = adjacencyListMap[vertex1];
        adj_list_1.remove_if([&](const std::pair<T, int> &edge) {
            return edge.first == vertex2;
        });

        auto &adj_list_2 = adjacencyListMap[vertex2];
        adj_list_2.remove_if([&](const std::pair<T, int> &edge) {
            return edge.first == vertex1;
        });
    }

    bool hasEdge(T vertex1, T vertex2) const {
        auto it = adjacencyListMap.find(vertex1);
        if (it == adjacencyListMap.end()) // If not found, return false
        {
            return false;
        }

        const auto &adj_list = it->second;
        for (auto pair: adj_list) {
            if (pair.first == vertex2)
                return true;
        }
        return false;
    }

    std::unordered_set<T> connected(T vertex, int graphDistance = 1) const {
        if (graphDistance < 1) {
            throw std::invalid_argument("graphDistance must be at least 1");
        }

        std::unordered_set<T> vertices = {vertex};

        auto result = connected_recursive(vertices, graphDistance);
        result.erase(vertex);
        return result;
    }

    std::vector<T> getVertices() const {
        std::vector<T> vertices;
        vertices.reserve(adjacencyListMap.size());
        for (auto it = adjacencyListMap.begin(); it != adjacencyListMap.end(); it++) {
            vertices.push_back(it->first);
        }
        return vertices;
    }

    //int pixelDistance(T vertex1, T vertex2) const
    //{
    //	return -1;
    //}
    //const int graphDistance(T vertex1, T vertex2)
    //{
    //	return -1;
    //}
    ///// <summary>
    ///// this implements Djikstra's algorithm
    ///// </summary>
    ///// <param name="from"></param>
    ///// <param name="to"></param>
    ///// <returns></returns>
    //std::list<T> shortestPath(T from, T to) const
    //{
    //	return std::list<T>();
    //}
};

extern template class AdjacencyListGraph<int>;
