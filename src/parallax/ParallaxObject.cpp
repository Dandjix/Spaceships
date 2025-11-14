#include "ParallaxObject.h"

#include <algorithm>

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

    float T = std::tanf(field_of_view/2);

    float O = A*T;

    return O;
}

void ParallaxObject::render(SDL_Renderer* renderer, const RenderingContext& context)
{
    // P_parallax = C + (P - C) * s

    // depth : distance in world units to the entity plane.

    auto cameraDistance = getCameraDistance(
        120.0f,
        static_cast<float>(context.screenDimensions.x)*context.cameraScale
        );

    float factor = cameraDistance / (cameraDistance + depth);
    // Clamp to avoid weirdness
    factor = std::clamp(factor, 0.0f, 1.0f);

    Vector2Int parallax_position = context.cameraPos + (position - context.cameraPos)
    // * (depth / Vectors::getFactor());
    * factor;


    // parallax_position /= context.cameraScale;


    Vector2Float center;
    // all indented is a copy of the contents of toScreenPoint adapted
        Vector2Float floatPosition = Vectors::toVector2Float(parallax_position);

        Vector2Float floatCameraPosition = Vectors::toVector2Float(context.cameraPos);
        Vector2Float worldCenter = floatPosition - floatCameraPosition;
        Vector2Float scaledWorldCenter = ((worldCenter) / context.cameraScale).scaleToScreenPosition();

        Vector2Float screenCenter = Vectors::toVector2Float(context.screenDimensions) / 2;
        Vector2Float diff = (screenCenter - scaledWorldCenter).rotate(context.cameraAngle);
        scaledWorldCenter = screenCenter - diff;

        // return scaledWorldCenter;
    // center  = context.toScreenPoint(parallax_position);
    center = scaledWorldCenter;


    // std::cout << "p_pos : "<< parallax_position << ", center : " << center << std::endl;

    float texture_w, texture_h;

    SDL_GetTextureSize(texture,&texture_w,&texture_h);

    texture_w /= (context.cameraScale / (cameraDistance / (cameraDistance + depth))) / sizeMultiplier;
    texture_h /= (context.cameraScale / (cameraDistance / (cameraDistance + depth))) / sizeMultiplier;


    SDL_FRect destRect = {
        center.x - texture_w * 0.5f,
        center.y - texture_h * 0.5f,
        texture_w,
        texture_h
    };

    SDL_RenderTextureRotated(renderer, texture, nullptr, &destRect, angle + context.cameraAngle, nullptr, SDL_FLIP_NONE);
}

//
// Created by timon on 11/13/25.
//