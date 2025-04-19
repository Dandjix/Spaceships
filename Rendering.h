#pragma once
#include "Vectors.h"
#include <SDL3/SDL.h>

struct RenderingContext
{
	const Vector2Int cameraPos;
	const float cameraAngle;

	const Vector2Int screenDimensions;

	const float cameraScale;

	RenderingContext(Vector2Int cameraPos, float cameraAngle, Vector2Int screenDimensions, float cameraScale)
		: cameraPos(cameraPos), cameraAngle(cameraAngle), screenDimensions(screenDimensions),cameraScale(cameraScale){}
};

class DebugRendering
{
public:
	static void drawCross(SDL_Renderer * renderer,Vector2Int screenPosition)
	{
		Vector2Float convertedPos = Vectors::toVector2Float(screenPosition);

		float halfLength = 7;

		Vector2Float corners[4] = {
			Vector2Float(convertedPos.x - halfLength,convertedPos.y - halfLength),
			Vector2Float(convertedPos.x + halfLength,convertedPos.y + halfLength),
			Vector2Float(convertedPos.x + halfLength,convertedPos.y - halfLength),
			Vector2Float(convertedPos.x - halfLength,convertedPos.y + halfLength),

		};

		SDL_RenderLine(renderer, corners[0].x, corners[0].y, corners[1].x, corners[1].y);
		SDL_RenderLine(renderer, corners[2].x, corners[2].y, corners[3].x, corners[3].y);
	}

	static void drawLine(SDL_Renderer* renderer, Vector2Int screenPosition1, Vector2Int screenPosition2)
	{
		Vector2Float convertedPos1 = Vectors::toVector2Float(screenPosition1);
		Vector2Float convertedPos2 = Vectors::toVector2Float(screenPosition2);

		SDL_RenderLine(renderer, convertedPos1.x, convertedPos1.y, convertedPos2.x, convertedPos2.y);

		//drawCross(renderer, screenPosition1);
		drawCross(renderer, screenPosition2);
	}
};