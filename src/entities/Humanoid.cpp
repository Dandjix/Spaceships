//
// Created by timon on 7/9/25.
//

#include "Humanoid.h"

#include "physics/Physics.h"

void Humanoid::render(SDL_Renderer *renderer, const RenderingContext &context) {

    renderTexture(renderer,context,texture,Vector2Float(
        radius,
        radius
        )
    );
}