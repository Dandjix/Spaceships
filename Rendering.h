#pragma once
#include "Vectors.h"
#include <SDL3/SDL.h>
///
struct RenderingContext
{
	/// <summary>
	/// camera position in world units
	/// </summary>
	const Vector2Int cameraPos;
	const float cameraAngle;

	const Vector2Int screenDimensions;

	const float cameraScale;

	const Vector2Float toScreenPoint(Vector2Int worldPosition) const
	{
		Vector2Float floatPosition = Vectors::toVector2Float(worldPosition);

		Vector2Float floatCameraPosition = Vectors::toVector2Float(cameraPos);
		Vector2Float worldCenter = floatPosition - floatCameraPosition;
		Vector2Float center = (worldCenter) / cameraScale;
		center = center.scaleToScreenPosition();

		Vector2Float screenCenter = Vectors::toVector2Float(screenDimensions) / 2;
		Vector2Float diff = (screenCenter - center).rotate(cameraAngle);
		center = screenCenter - diff;

		return center;
	}

	RenderingContext(Vector2Int cameraPos, float cameraAngle, Vector2Int screenDimensions, float cameraScale)
		: cameraPos(cameraPos), cameraAngle(cameraAngle), screenDimensions(screenDimensions),cameraScale(cameraScale){}
};

class Rendering
{
public :
	static Vector2Int get_zero(const RenderingContext& context)
	{
		Vector2Int zero = Vector2Int(0, 0) - (context.cameraPos).scaleToScreenPosition() / context.cameraScale; //scale zero

		Vector2Float screenCenter = Vectors::toVector2Float(context.screenDimensions) / 2;
		Vector2Float diff = (screenCenter - Vectors::toVector2Float(zero)).rotate(context.cameraAngle); //rotate zero
		zero = Vectors::toVector2Int(screenCenter - diff);
		return zero;
	}
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