#include "ParallaxObject.h"

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

// static const float parallax_strength = 0.0;


void ParallaxObject::render(SDL_Renderer* renderer, const RenderingContext& context)
{
    // P_parallax = C + (P - C) * s

    // depth : distance in world units to the entity plane.

    Vector2Int parallax_position = context.cameraPos + (position - context.cameraPos)
    // * (depth / Vectors::getFactor());
    * 1;

    parallax_position /= context.cameraScale;


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

    texture_w /= context.cameraScale;
    texture_h /= context.cameraScale;


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