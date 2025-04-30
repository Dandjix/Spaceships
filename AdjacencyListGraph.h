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
	bool hasVertex(T vertex) const
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
	bool hasEdge(T vertex1, T vertex2) const
	{
		auto it = adjacencyListMap.find(vertex1);
		if (it == adjacencyListMap.end())          // If not found, return false
		{
			return false;
		}

		const auto& adj_list = it->second;
		for (auto pair : adj_list)
		{
			if (pair.first == vertex2)
				return true;
		}
		return false;
	}

	std::vector<T> connected(T vertex) const
	{
		const std::list<std::pair<T, int>>& it = adjacencyListMap.at(vertex);
		std::list<T> adjacentEdges;
		for (const std::pair<T, int>& pair : it)
		{
			adjacentEdges.push_back(pair.first);
		}
		std::vector<T> v
		{ 
			std::make_move_iterator(std::begin(adjacentEdges)),
			std::make_move_iterator(std::end(adjacentEdges))
		};
		return v;
	}
	std::vector<T> getVertices() const
	{
		std::vector<T> vertices;
		vertices.reserve(adjacencyListMap.size());
		for (auto it = adjacencyListMap.begin(); it != adjacencyListMap.end(); it++)
		{
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