//
// Created by timon on 1/19/26.

#include "HashProximityMap.h"

#include <format>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "game/Rendering.h"
#include "../../physics/scripts/PhysicsEntity.h"
#include "physics/shapes/PhysicsShape.h"
#include "userInterface/fonts.h"

/**
 * @param world_position a world position  (a position inside the ship) to check for physics entities
 * @return a vector containing all physics entities at that position, (empty if nothing is there)
 */
std::vector<PhysicsShape *> HashProximityMap::at(Vector2Int world_position) const {

    Vector2Int coordinates = world_position / Scaling::scaleToWorld(Tiles::tileSizePx);

    if (!proximityMap.contains(coordinates))
        return {};

    return proximityMap.at(coordinates);
}

/**
 * Populates the map with all physics entities positions so that HashProximityMap::at may be called to get a unit's position.
 *
 *  This calls clear internally, no need to call it beforehand.
 *
 * @param entities the physics entities to populate the map with. Should certainly be the physics entities of the ship.
 */
void HashProximityMap::populate(const std::vector<PhysicsEntity *> &entities){
    clear();

    for (auto e: entities) {
        if (e->shape == nullptr)
            continue;

        auto bb = e->shape->getBoundingBox();

        int start_x = bb.topLeft().x / Scaling::scaleToWorld(Tiles::tileSizePx);
        int start_y = bb.topLeft().y / Scaling::scaleToWorld(Tiles::tileSizePx);

        int end_x = bb.bottomRight().x / Scaling::scaleToWorld(Tiles::tileSizePx);
        int end_y = bb.bottomRight().y / Scaling::scaleToWorld(Tiles::tileSizePx);

        for (int i = start_x; i <= end_x; ++i) {
            for (int j = start_y; j <= end_y; ++j) {
                proximityMap[{i,j}].push_back(e->shape);
            }
        }
    }
}

/**
 * Clears the map manually, if you want to for some unbeknown reason
 */
void HashProximityMap::clear() {
    proximityMap.clear();
}

/**
 * used for debugging
 * @param renderer
 * @param context
 */
void HashProximityMap::render(SDL_Renderer *renderer, const RenderingContext &context) const {
    for (const auto& [coords,shapes]: proximityMap) {

        std::string text = std::format("[{}]",shapes.size());
        SDL_Color color;
        {
            if (shapes.size()<=1) {
                color = {0,255,0,255};
            }
            else if (shapes.size()<=2) {
                color = {127,255,0,255};
            }
            else if (shapes.size()<=3) {
                color = {255,255,0,255};
            }
            else if (shapes.size()<=4) {
                color = {255,127,0,255};
            }
            else {
                color = {255,0,0,255};
            }
        }

        SDL_Surface* surface = TTF_RenderText_Solid(fonts["lg"],text.c_str(), text.length(), color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        int half_cell = Scaling::scaleToWorld(Tiles::tileSizePx/2);

        Vector2Int tile_center = (coords) *
            Scaling::scaleToWorld(Tiles::tileSizePx)
            +
            Vector2Int(half_cell,half_cell);

        Vector2Float screenPosition = context.camera_info.worldToScreenPoint(tile_center);

        SDL_FRect textRect;
        textRect.w = static_cast<float>(surface->w);
        textRect.h = static_cast<float>(surface->h);
        textRect.x = screenPosition.x - textRect.w / 2; // center horizontally
        textRect.y = screenPosition.y - textRect.h / 2; // center vertically

        SDL_RenderTexture(renderer, texture, nullptr, &textRect);

        SDL_DestroyTexture(texture);
        SDL_DestroySurface(surface);
    }
}
