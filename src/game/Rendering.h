#pragma once
#include <optional>
#include <utility>
#include <SDL3/SDL.h>
#include "../math/Vectors.h"
#include "../spaceships/Tile.h"
#include "../game/CameraTransformations.h"
#include "math/Scaling.h"

struct RenderingContext {
    CameraTransformations::CameraInfo camera_info;
};

struct ExteriorRenderingContext {
    CameraTransformations::ExteriorCameraInfo camera_info;
};

namespace Rendering {
    inline Vector2Int get_zero(const RenderingContext &context) {
        Vector2Int zero = Vector2Int(0, 0) - (context.camera_info.cameraPosition).scaleToScreenPosition() / context.
                          camera_info.cameraScale; //scale zero

        Vector2Float screenCenter = Vectors::toVector2Float(context.camera_info.screenDimensions) / 2;
        Vector2Float diff = (screenCenter - Vectors::toVector2Float(zero)).rotate(context.camera_info.cameraAngle);
        //rotate zero
        zero = Vectors::toVector2Int(screenCenter - diff);
        return zero;
    }
};

namespace DebugRendering {
    inline void drawCross(SDL_Renderer *renderer, Vector2Float screenPosition, float size = 14.0f) {
        float halfLength = size / 2;

        Vector2Float corners[4] = {
            Vector2Float(screenPosition.x - halfLength, screenPosition.y - halfLength),
            Vector2Float(screenPosition.x + halfLength, screenPosition.y + halfLength),
            Vector2Float(screenPosition.x + halfLength, screenPosition.y - halfLength),
            Vector2Float(screenPosition.x - halfLength, screenPosition.y + halfLength),

        };

        SDL_RenderLine(renderer, corners[0].x, corners[0].y, corners[1].x, corners[1].y);
        SDL_RenderLine(renderer, corners[2].x, corners[2].y, corners[3].x, corners[3].y);
    }

    static void drawLine(SDL_Renderer *renderer, Vector2Float screenPosition1, Vector2Float screenPosition2) {
        SDL_RenderLine(renderer, screenPosition1.x, screenPosition1.y, screenPosition2.x, screenPosition2.y);

        //drawCross(renderer, screenPosition1);
        drawCross(renderer, screenPosition2);
    }

    static void drawCircle(SDL_Renderer *renderer, const RenderingContext &context, Vector2Int position, float radius) {
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

    static void drawRect(SDL_Renderer *renderer, const RenderingContext &context,
                         Vector2Float dimensions, Vector2Int position, float angle) {
        // Calculate half size with camera scale
        Vector2Float halfSize = dimensions * 0.5f / context.camera_info.cameraScale;

        Vector2Float center = context.camera_info.worldToScreenPoint(position);

        // Define corners in local space
        Vector2Float corners[4] = {
            {-halfSize.x, -halfSize.y},
            {halfSize.x, -halfSize.y},
            {halfSize.x, halfSize.y},
            {-halfSize.x, halfSize.y}
        };

        // Total rotation = object angle + camera angle
        float totalAngleDeg = angle + context.camera_info.cameraAngle;
        float angleRad = totalAngleDeg * (3.14159265f / 180.0f);
        float cosA = std::cos(angleRad);
        float sinA = std::sin(angleRad);

        // Rotate corners around center
        for (auto & corner : corners) {
            float x = corner.x;
            float y = corner.y;
            corner.x = x * cosA - y * sinA + center.x;
            corner.y = x * sinA + y * cosA + center.y;
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
    static void drawWorldRoomBoundingBox(SDL_Renderer *renderer, const RenderingContext &context, Vector2Int position,
                                         Vector2Int dimensions) {
        // Box corners in tile pixel space
        Vector2Int topLeft = position.scaleToWorldPosition() * Tiles::tileSizePx;
        Vector2Int size = dimensions.scaleToWorldPosition() * Tiles::tileSizePx;
        Vector2Int bottomRight = topLeft + size;

        Vector2Int TL = topLeft;
        Vector2Int TR = Vector2Int(bottomRight.x, topLeft.y);
        Vector2Int BR = bottomRight;
        Vector2Int BL = Vector2Int(topLeft.x, bottomRight.y);

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

    static void drawArrow(
        SDL_Renderer *renderer,
        Vector2Float screen_position,
        float angle, float scale = 48
    ) {
        std::vector<Vector2Float> points = {
            {-0.5, 1},
            {-0.5, -0},
            {-1, -0},
            {0, -1},
            {1, -0},
            {0.5, -0},
            {0.5, 1}
        };
        for (int i = 0; i < points.size(); i++) {
            int next_i = (i + 1) % points.size();

            Vector2Float p1, p2;
            p1 = screen_position + ((points[i] * scale)).rotate(angle);
            p2 = screen_position + ((points[next_i] * scale)).rotate(angle);

            SDL_RenderLine(renderer, p1.x, p1.y, p2.x, p2.y);
        }
    }
};
