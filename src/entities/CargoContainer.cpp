#include "CargoContainer.h"
#include <iostream>

// Initialize static member variables
SDL_Texture* CargoContainer::textures[static_cast<int>(CargoContainer::Variation::COUNT)] = {};
bool CargoContainer::texturesLoaded = false;

void CargoContainer::LoadTextures(SDL_Renderer* renderer) {
    if (texturesLoaded) return;

    textures[0] = IMG_LoadTexture(renderer, ENV_PROJECT_ROOT"assets/textures/objects/cargoContainers/blank.png");
    textures[1] = IMG_LoadTexture(renderer, ENV_PROJECT_ROOT"assets/textures/objects/cargoContainers/EMA.png");
    textures[2] = IMG_LoadTexture(renderer, ENV_PROJECT_ROOT"assets/textures/objects/cargoContainers/SL.png");
    textures[3] = IMG_LoadTexture(renderer, ENV_PROJECT_ROOT"assets/textures/objects/cargoContainers/SN.png");

    for (int i = 0; i < static_cast<int>(Variation::COUNT); i++) {
        if (textures[i] == NULL) {
            std::cout << "texture " << i << " is null.\n" << std::endl;
        }
    }

    texturesLoaded = true;
}

CargoContainer::CargoContainer(Vector2Int position, float angle, Variation variation)
    : BoxEntity(position, Vector2Float(61, 24), angle,nullptr), variation(variation) {
    color = getRandomColor();
}

void CargoContainer::update(const UpdateContext & context) {
    // CargoContainer might not need to move, so leave it empty.
}

void CargoContainer::render(SDL_Renderer* renderer, const RenderingContext& context) {
    SDL_Texture* texture = textures[static_cast<int>(variation)];
    if (!texture) {
        // Texture not found
        return;
    }

    SDL_SetTextureColorMod(texture, color.r, color.g, color.b);

    // Calculate the half-size of the container
    Vector2Float halfSize = scale * 0.5f;

    // Render the texture (this assumes you have a rendering function like `renderTexture` elsewhere)
    renderTexture(renderer, context, texture, halfSize);
}

SDL_Color CargoContainer::getRandomColor() {
    unsigned char r = 128 + rand() % 127;
    unsigned char g = 128 + rand() % 127;
    unsigned char b = 128 + rand() % 127;

    return { r, g, b, 255 };
}
