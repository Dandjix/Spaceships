#include "AdjacencyListGraph.h"
#include <list>
// vertices --
template <typename T>
const bool AdjacencyListGraph<T>::hasVertex(T vertex)
{
	return adjacencyListMap.find(vertex) != adjacencyListMap.end();
}

template <typename T>
void AdjacencyListGraph<T>::removeVertex(T vertex)
{
	adjacencyListMap.erase(vertex);
}

template <typename T>
void AdjacencyListGraph<T>::addVertex(T vertex)
{
	adjacencyListMap[vertex] = std::list<std::pair<T,int>>();
}

// edges --
template <typename T>
const std::list<T> AdjacencyListGraph<T>::connected(T vertex)
{
	std::list<std::pair<T,int>> & it = adjacencyListMap[vertex];
	std::list<T> adjacentEdges;
	for (const std::pair<T, int>& pair : it)
	{
		adjacentEdges.push_back(pair.first);
	}
	return adjacentEdges;
}

template <typename T>
const bool AdjacencyListGraph<T>::hasEdge(T vertex1, T vertex2)
{
	auto& adj_list = adjacencyListMap[vertex1];
	for (auto pair : adj_list)
	{
		if (pair.first == vertex2)
			return true;
	}
	return false;
}

template <typename T>
void AdjacencyListGraph<T>::removeEdge(T vertex1, T vertex2)
{
	auto& adj_list_1 = adjacencyListMap[vertex1];
	adj_list_1.remove_if([&](const std::pair<T, int>& edge) {
		return edge.first == vertex2;
	});

	auto& adj_list_2 = adjacencyListMap[vertex2];
	adj_list_2.remove_if([&](const std::pair<T, int>& edge) {
		return edge.first == vertex1;
	});

}

template <typename T>
void AdjacencyListGraph<T>::addEdge(T vertex1, T vertex2, int distance)
{
	auto& adj_list_1 = adjacencyListMap[vertex1];
	adj_list_1.push_back({ vertex2, distance });

	auto& adj_list_2 = adjacencyListMap[vertex2];
	adj_list_2.push_back({ vertex1, distance });
}
// complicated computations --
template <typename T>
const int AdjacencyListGraph<T>::graphDistance(T vertex1, T vertex2)
{
	return -1;
}

template <typename T>
const int AdjacencyListGraph<T>::pixelDistance(T vertex1, T vertex2)
{
	return -1;
}

template <typename T>
const std::list<T> AdjacencyListGraph<T>::shortestPath(T from, T to)
{
	return std::list<T>();
}


template class AdjacencyListGraph<int>;