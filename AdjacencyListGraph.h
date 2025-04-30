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
	void addVertex(T vertex);
	void removeVertex(T vertex);
	const bool hasVertex(T vertex);

	void addEdge(T vertex1, T vertex2, int distance);
	void removeEdge(T vertex1, T vertex2);
	const bool hasEdge(T vertex1, T vertex2);

	const std:: list<T> connected(T vertex);

	const int pixelDistance(T vertex1, T vertex2);
	const int graphDistance(T vertex1, T vertex2);
	/// <summary>
	/// this implements A*
	/// </summary>
	/// <param name="from"></param>
	/// <param name="to"></param>
	/// <returns></returns>
	const std::list<T> shortestPath(T from, T to);
};