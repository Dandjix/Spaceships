#pragma once
#include <list>
#include <unordered_map>
#include <map>
#include <utility>

template <typename T>
/// <summary>
/// A graph made for representing the rooms of a spaceship : it also has the distance  between each vertex
/// </summary>
class AdjacencyListGraph
{
protected:
	std::unordered_map<T, std::list<std::pair<T,int>>> adjacencyListMap;

public:
	void addVertex(T vertex)
	{
		adjacencyListMap[vertex] = std::list<std::pair<T, int>>();
	}
	void removeVertex(T vertex)
	{
		adjacencyListMap.erase(vertex);
	}
	const bool hasVertex(T vertex)
	{
		return adjacencyListMap.find(vertex) != adjacencyListMap.end();
	}

	void addEdge(T vertex1, T vertex2, int distance)
	{
		auto& adj_list_1 = adjacencyListMap[vertex1];
		adj_list_1.push_back({ vertex2, distance });

		auto& adj_list_2 = adjacencyListMap[vertex2];
		adj_list_2.push_back({ vertex1, distance });
	}
	void removeEdge(T vertex1, T vertex2)
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
	const bool hasEdge(T vertex1, T vertex2)
	{
		auto& adj_list = adjacencyListMap[vertex1];
		for (auto pair : adj_list)
		{
			if (pair.first == vertex2)
				return true;
		}
		return false;
	}

	const std:: list<T> connected(T vertex)
	{
		std::list<std::pair<T, int>>& it = adjacencyListMap[vertex];
		std::list<T> adjacentEdges;
		for (const std::pair<T, int>& pair : it)
		{
			adjacentEdges.push_back(pair.first);
		}
		return adjacentEdges;
	}

	//const int pixelDistance(T vertex1, T vertex2)
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
	//const std::list<T> shortestPath(T from, T to)
	//{
	//	return std::list<T>();
	//}
};

extern template class AdjacencyListGraph<int>;