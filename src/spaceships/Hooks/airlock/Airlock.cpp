//
// Created by timon on 12/13/25.
//

#include "Airlock.h"


void Airlock::registerEntity(Entity* entity)
{
    Events::Id id = entity->on_die.subscribe([this,entity]{
        unRegisterEntity(entity);
    });
    entities_inside.insert({entity,id});
    on_enter.emit(entity);
}

void Airlock::unRegisterEntity(Entity* entity)
{
    auto id = entities_inside[entity];
    entities_inside.erase(entity);
    entity->on_die.unsubscribe(id);
    on_leave.emit(entity);
}

bool Airlock::pointIsInside(Vector2Int world_position) const
{
    return (
        world_position.x >= TL.x && world_position.y >= TL.y
        && world_position.x <= TL.x + dimensions.x && world_position.y <= TL.y + dimensions.y
    );
}

bool Airlock::hasEntity(Entity* entity) const
{
    return entities_inside.contains(entity);
}

void Airlock::update(const UpdateContext &context) {
    auto entities = context.spaceShip->getEntities(RoomDistance::All);

    for (auto entity: entities) {
        if (pointIsInside(entity->getPosition())) {
            if (!hasEntity(entity)) {
                registerEntity(entity);
            }
        }
        else {
            if (hasEntity(entity)) {
                unRegisterEntity(entity);
            }
        }
    }
}

void Airlock::debugRender(SDL_Renderer *renderer, const RenderingContext &context) {

    Vector2Float TL_screen, BR_screen, TR_screen, BL_screen;

    TL_screen = context.camera_info.worldToScreenPoint(TL);
    BR_screen = context.camera_info.worldToScreenPoint(TL + dimensions);
    TR_screen = context.camera_info.worldToScreenPoint(Vector2Int(TL.x + dimensions.x,TL.y));
    BL_screen = context.camera_info.worldToScreenPoint(Vector2Int(TL.x,TL.y + dimensions.y));

    std::vector<Vector2Float> corners = {
        TL_screen,TR_screen,BR_screen,BL_screen
    };

    SDL_SetRenderDrawColor(renderer,0,255,255,255);

    for (int i = 0; i < corners.size(); ++i) {
        int next_i = (i + 1) % corners.size();

        SDL_RenderLine(renderer,corners[i].x,corners[i].y,corners[next_i].x,corners[ next_i].y);
    }

}

nlohmann::json Airlock::toJson() const {
    nlohmann::json json = {};

    json["TL"] = TL.toJson();
    json["orientation"] = orientation;
    json["dimensions"] = dimensions.toJson();

    return json;
}

Airlock * Airlock::fromJson(const nlohmann::json &json) {
    return new Airlock(Vector2Int::fromJson(json["TL"]),Vector2Int::fromJson(json["dimensions"]),json["orientation"]);
}
