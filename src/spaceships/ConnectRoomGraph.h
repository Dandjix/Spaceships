#pragma once
#include "../math/AdjacencyListGraph.h"
#include "Room.h"

class ConnectRoomGraph
{
private :
	static Vector2Int pixelAvg(const std::vector < std::pair < Vector2Int,Vector2Int>> & boundingBoxes)
	{
		Vector2Int summ = Vector2Int(0, 0);
		for (auto bb : boundingBoxes)
		{
			summ = summ + bb.first * Tiles::tileSizePx + bb.second*Tiles::tileSizePx * 0.5f;
		}
		return summ / boundingBoxes.size();
	}

	static int pixelDistance(Room * r1, Room * r2)
	{
		return (pixelAvg(r1->getBoundingBoxes()) - pixelAvg(r2->getBoundingBoxes())).length();
	}

public:
	static void Connect(AdjacencyListGraph<Room*>& graph) // Pass by reference
	{
		const auto& rooms = graph.getVertices();

		for (Room* room : rooms)
		{
			for (Room* otherRoom : rooms)
			{
				if (room == otherRoom)
					continue;

				if (AreRoomsOverlapping(room, otherRoom))
				{
					if (!graph.hasEdge(room, otherRoom))
					{
						//Vector2Int firstCenter = room->getBoundingBoxes()
						int distance = pixelDistance(room, otherRoom);

						graph.addEdge(room, otherRoom, distance); // Add edge with dummy distance
					}
				}
			}
		}
	}

private:
	static bool AreRoomsOverlapping(Room* a, Room* b)
	{
		for (auto [posA, dimA] : a->getBoundingBoxes())
		{
			int leftA = posA.x;
			int rightA = posA.x + dimA.x;
			int topA = posA.y;
			int bottomA = posA.y + dimA.y;

			for (auto [posB, dimB] : b->getBoundingBoxes())
			{
				int leftB = posB.x;
				int rightB = posB.x + dimB.x;
				int topB = posB.y;
				int bottomB = posB.y + dimB.y;

				bool xOverlap = leftA < rightB && rightA > leftB;
				bool yOverlap = topA < bottomB && bottomA > topB;

				if (xOverlap && yOverlap)
				{
					return true;
				}
			}
		}
		return false;
	}
};
