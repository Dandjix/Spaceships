//
// Created by timon on 7/9/25.
//

#include "Humanoid.h"

#include "physics/Physics.h"
#include "physics/shapes/RectPhysicsShape.h"
#include "physics/shapes/RoundPhysicsShape.h"
#include "rendering/util/RenderTexture.h"

Humanoid::Humanoid(Vector2Int position, std::optional<float> angle, Behavior *behavior)
    : BehavioredEntity(
          position,
          angle,
          new RectPhysicsShape(this, Vector2Int{20, 20}.scaleToWorldPosition())),
      behavior(behavior) {
    radius = 20;
    texture = nullptr;
}

void Humanoid::render(SDL_Renderer *renderer, const RenderingContext &context) {
    Rendering::Util::renderTexture(
        renderer,
        context,
        getPosition(),
        getAngle(),
        texture,
        Vector2Float(
            radius,
            radius
        )
    );
}

FROM_JSON_DEFINITION(Humanoid) {
    std::optional<float> angle = std::nullopt;
    if (json.contains("angle")) {
        angle = json["angle"];
    }

    Behavior *behavior = nullptr;


    if (json.contains("behavior")) {
        behavior = Behavior::fromJson(json["behavior"]);
    }

    return new Humanoid(
        Vector2Int{
            json["position"]["x"],
            json["position"]["y"]
        },
        angle,
        behavior
    );
}

EDITOR_PLACE_DEFINITION(Humanoid) {
    Vector2Int position = context->interface->getPosition();
    float angle = context->interface->getAngle();

    return std::async(std::launch::async, [position, angle]()-> Entity * {
        return new Humanoid(position, angle, nullptr);
    });
}
