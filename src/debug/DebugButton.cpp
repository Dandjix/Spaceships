//
// Created by timon on 2/8/26.
//

#include "DebugButton.h"


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