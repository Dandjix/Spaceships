#include "ParallaxObject.h"

#include <algorithm>

#include "spaceships/SpaceShip.h"

// Parallax position calculation:
//
// Let:
//   P = world position of the object
//   C = camera position
//   s = parallax strength (0 = background fixed to screen, 1 = moves fully with world)
//
// Then the parallax-adjusted position is:
//
//   P_parallax = C + (P - C) * s
//   // or equivalently:
//   P_parallax = P * s + C * (1 - s)
//
// thx chatGPT

float /**
 *
 * @param  fov : the fov. Wikipedia says it's about 120 in humans, but remains to be seen
 * @param  screen dimensions
 * @return the distance of the camera relative to the entity plane, a positive number
 */
getCameraDistance(float field_of_view, float screenWidth)
{
    float A = (screenWidth * static_cast<float>(Vectors::getFactor())) /2.0f;

    float T = std::tan(field_of_view/2);

    float O = A*T;

    return O;
}

void ParallaxObject::render(SDL_Renderer* renderer, const ExteriorRenderingContext& context, SpaceShip * ship) const {

    Vector2Float center  = context.camera_info.worldToScreenPoint(position);

    float texture_w, texture_h;

    SDL_GetTextureSize(texture,&texture_w,&texture_h);

    SDL_FRect destRect = {
        center.x - texture_w * 0.5f,
        center.y - texture_h * 0.5f,
        texture_w,
        texture_h
    };

SDL_RenderTextureRotated(renderer, texture, nullptr, &destRect, context.camera_info.getScreenObjectAngle(angle), nullptr, SDL_FLIP_NONE);
}

//
// Created by timon on 11/13/25.
//