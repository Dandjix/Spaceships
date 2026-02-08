//
// Created by timon on 2/8/26.
//

#include "DebugButton.h"

#include "entities/toggleables/door/Door.h"


void Debug::DebugButton::DoStuff(const GameEvent::GameEventContext &context) {
    for (auto entity: context.spaceShip->entities) {
        if (dynamic_cast<Door*>(entity) != nullptr) {
            entity->kill(context.spaceShip);
        }
    }
}

nlohmann::json Debug::DebugButton::toJson() {
    auto json = PhysicsEntity::toJson();
    return json;
}

Entity * Debug::DebugButton::initializeRendering(const EntityRendering::Context &context) {
    return this;
}

Entity * Debug::DebugButton::finalizeRendering(const EntityRendering::Context &context) {
    return this;
}

void Debug::DebugButton::render(SDL_Renderer *renderer, const RenderingContext &context) {
}

FROM_JSON_DEFINITION(Debug::DebugButton) {
    return new DebugButton(Vector2Int::fromJson(json["position"]),json["angle"]);
}