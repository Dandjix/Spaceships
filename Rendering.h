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

	static void drawWorldObjectBoundingBox(SDL_Renderer* renderer, const RenderingContext& context, Vector2Int position, Vector2Int scale, float angle) {
		// Convert object position and camera to float
		Vector2Float floatPosition = Vectors::toVector2Float(position);
		Vector2Float floatCameraPosition = Vectors::toVector2Float(context.cameraPos);
		Vector2Float worldCenter = floatPosition - floatCameraPosition;
		Vector2Float center = worldCenter / context.cameraScale;
		center = center.scaleToScreenPosition();

		// Apply camera rotation
		Vector2Float screenCenter = Vectors::toVector2Float(context.screenDimensions) / 2.0f;
		Vector2Float diff = (screenCenter - center).rotate(context.cameraAngle);
		center = screenCenter - diff;

		// Calculate half size with camera scale
		Vector2Float halfSize = Vectors::toVector2Float(scale) * 0.5f / context.cameraScale;

		// Define corners in local space
		Vector2Float corners[4] = {
			{-halfSize.x, -halfSize.y},
			{ halfSize.x, -halfSize.y},
			{ halfSize.x,  halfSize.y},
			{-halfSize.x,  halfSize.y}
		};

		// Total rotation = object angle + camera angle
		float totalAngleDeg = angle + context.cameraAngle;
		float angleRad = totalAngleDeg * (3.14159265f / 180.0f);
		float cosA = cos(angleRad);
		float sinA = sin(angleRad);

		// Rotate corners around center
		for (int i = 0; i < 4; ++i) {
			float x = corners[i].x;
			float y = corners[i].y;
			corners[i].x = x * cosA - y * sinA + center.x;
			corners[i].y = x * sinA + y * cosA + center.y;
		}

		// Draw rectangle from corners
		for (int i = 0; i < 4; ++i) {
			int next = (i + 1) % 4;
			SDL_RenderLine(renderer,
				corners[i].x, corners[i].y,
				corners[next].x, corners[next].y);
		}
	}

	static void drawWorldRect(SDL_Renderer* renderer, const RenderingContext& context, Vector2Int position, Vector2Int scale, float angle) {
		// Convert object position and camera to float
		Vector2Float floatPosition = Vectors::toVector2Float(position);
		Vector2Float floatCameraPosition = Vectors::toVector2Float(context.cameraPos);
		Vector2Float worldCenter = floatPosition - floatCameraPosition;
		Vector2Float center = worldCenter / context.cameraScale;
		center = center.scaleToScreenPosition();

		// Apply camera rotation
		Vector2Float screenCenter = Vectors::toVector2Float(context.screenDimensions) / 2.0f;
		Vector2Float diff = (screenCenter - center).rotate(context.cameraAngle);
		center = screenCenter - diff;

		// Calculate half size with camera scale
		//Vector2Float halfSize = Vectors::toVector2Float(scale) * 0.5f / context.cameraScale;

		// Define corners in local space
		Vector2Float corners[4] = {
			{0.0, 0.0},
			{ static_cast<float>(scale.x), 0.0},
			{ static_cast<float>(scale.x),  static_cast<float>(scale.y)},
			{0.0,  static_cast<float>(scale.y)}
		};

		// Total rotation = object angle + camera angle
		float totalAngleDeg = angle + context.cameraAngle;
		float angleRad = totalAngleDeg * (3.14159265f / 180.0f);
		float cosA = cos(angleRad);
		float sinA = sin(angleRad);

		// Rotate corners around center
		for (int i = 0; i < 4; ++i) {
			float x = corners[i].x;
			float y = corners[i].y;
			corners[i].x = x * cosA - y * sinA + center.x;
			corners[i].y = x * sinA + y * cosA + center.y;
		}

		// Draw rectangle from corners
		for (int i = 0; i < 4; ++i) {
			int next = (i + 1) % 4;
			SDL_RenderLine(renderer,
				corners[i].x, corners[i].y,
				corners[next].x, corners[next].y);
		}
	}
};