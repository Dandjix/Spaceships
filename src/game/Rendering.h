#pragma once
#include <optional>
#include <utility>
#include <SDL3/SDL.h>
#include "../math/Vectors.h"
#include "../spaceships/Tile.h"
#include "../game/CameraTransformations.h"

struct RenderingContext
{
	CameraTransformations::CameraInfo camera_info;
};

struct ExteriorRenderingContext {
	CameraTransformations::ExteriorCameraInfo camera_info;
};

class Rendering
{
public :
	static Vector2Int get_zero(const RenderingContext& context)
	{
		Vector2Int zero = Vector2Int(0, 0) - (context.camera_info.cameraPosition).scaleToScreenPosition() / context.camera_info.cameraScale; //scale zero

		Vector2Float screenCenter = Vectors::toVector2Float(context.camera_info.screenDimensions) / 2;
		Vector2Float diff = (screenCenter - Vectors::toVector2Float(zero)).rotate(context.camera_info.cameraAngle); //rotate zero
		zero = Vectors::toVector2Int(screenCenter - diff);
		return zero;
	}
};

class DebugRendering
{
public:
	static void drawCross(SDL_Renderer * renderer,Vector2Int screenPosition, float size = 14.0f)
	{
		Vector2Float convertedPos = Vectors::toVector2Float(screenPosition);

		float halfLength = size/2;

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

	static void drawCircle(SDL_Renderer * renderer, const RenderingContext & context,Vector2Int position, float radius)
	{
		Vector2Float center = context.camera_info.worldToScreenPoint(position);

		float scaledRadius = Scaling::scaleToScreen(radius) / context.camera_info.cameraScale;

		// Draw debug circle
		const int segments = 16; // More = smoother circle
		float angleStep = 2.0f * 3.14159265f / segments;

		for (int i = 0; i < segments; ++i) {
			float theta1 = i * angleStep;
			float theta2 = (i + 1) % segments * angleStep;

			float x1 = center.x + cosf(theta1) * scaledRadius;
			float y1 = center.y + sinf(theta1) * scaledRadius;
			float x2 = center.x + cosf(theta2) * scaledRadius;
			float y2 = center.y + sinf(theta2) * scaledRadius;

			SDL_RenderLine(renderer, x1, y1, x2, y2);
		}
	}

	static void drawRect(SDL_Renderer * renderer,const RenderingContext& context,
		Vector2Float dimensions, Vector2Int position, float angle )
	{
		// Calculate half size with camera scale
		Vector2Float halfSize = dimensions * 0.5f / context.camera_info.cameraScale;

		Vector2Float center = context.camera_info.worldToScreenPoint(position);

		// Define corners in local space
		Vector2Float corners[4] = {
			{-halfSize.x, -halfSize.y},
			{ halfSize.x, -halfSize.y},
			{ halfSize.x,  halfSize.y},
			{-halfSize.x,  halfSize.y}
		};

		// Total rotation = object angle + camera angle
		float totalAngleDeg = angle + context.camera_info.cameraAngle;
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

	/// <summary>
	/// 
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="context"></param>
	/// <param name="position">The tile position of the top left corner of the room</param>
	/// <param name="dimensions">The tile dimensions of the room</param>
	static void drawWorldRoomBoundingBox(SDL_Renderer * renderer,const RenderingContext & context,Vector2Int position, Vector2Int dimensions)
	{
		// Box corners in tile pixel space
		Vector2Int topLeft = position.scaleToWorldPosition() * Tiles::tileSizePx;
		Vector2Int size = dimensions.scaleToWorldPosition() * Tiles::tileSizePx;
		Vector2Int bottomRight = topLeft + size;

		Vector2Int TL = topLeft;
		Vector2Int TR = Vector2Int(bottomRight.x, topLeft.y);
		Vector2Int BR = bottomRight;
		Vector2Int BL = Vector2Int(topLeft.x, bottomRight.y);

		// Center of the box (in world coords)
		Vector2Int center = (TL + BR) / 2.0f;


		// Apply camera transform: offset, scale
		auto transform = [&](Vector2Int pt) -> Vector2Float {
			return context.camera_info.worldToScreenPoint(pt);
		};

		Vector2Float rTL = transform(TL);
		Vector2Float rTR = transform(TR);
		Vector2Float rBR = transform(BR);
		Vector2Float rBL = transform(BL);

		// Draw the rotated rectangle
		SDL_RenderLine(renderer, rTL.x, rTL.y, rTR.x, rTR.y);
		SDL_RenderLine(renderer, rTR.x, rTR.y, rBR.x, rBR.y);
		SDL_RenderLine(renderer, rBR.x, rBR.y, rBL.x, rBL.y);
		SDL_RenderLine(renderer, rBL.x, rBL.y, rTL.x, rTL.y);
	}
};