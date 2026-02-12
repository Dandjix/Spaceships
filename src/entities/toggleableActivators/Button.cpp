//
// Created by timon on 1/19/26.
//

#include "Button.h"

#include "loadGame/GameState.h"
#include "rendering/util/RenderTexture.h"
#include "textures/TextureSet.h"

constexpr float button_radius_px = 22.0f;

Button::Button(Vector2Int position, float angle, Toggleable *linked_device)
    : PhysicsEntity(
          position,
          angle,
          new RoundStaticPhysicsShape(
              this,
              Scaling::scaleToWorld(button_radius_px))),
      linked_entity(linked_device), texture(nullptr) {
}


nlohmann::json Button::toJson() {
    auto json = PhysicsEntity::toJson();

    if (linked_entity != nullptr)
        json["linked_entity_id"] = linked_entity->getEntityId();
    else
        json["linked_entity_id"] = "null";

    return json;
}

FROM_JSON_DEFINITION(Button) {
    auto button = new Button(Vector2Int::fromJson(json["position"]), json["angle"], nullptr);
    transient_game_state.transient_data_per_entity[button]["linked_entity_id"] = json["linked_entity_id"];
    return button;
}

void Button::finalizeJsonDeserialization(const GameState::transientGameState &transient_game_state) {
    PhysicsEntity::finalizeJsonDeserialization(transient_game_state);

    nlohmann::json linked = transient_game_state.transient_data_per_entity.at(this).at("linked_entity_id");
    if (linked != "null")
        linked_entity = transient_game_state.identified_entities.at(linked)->asToggleable();
    else
        linked_entity = nullptr;
}


void Button::render(SDL_Renderer *renderer, const RenderingContext &context) {
    Rendering::Util::renderTexture(
        renderer,
        context,
        getPosition(),
        getAngle(),
        texture,
        Vector2Float{button_radius_px*2, button_radius_px*2});
}

void Button::interact(Humanoid *activator, const GameEvent::GameEventContext &context) {
    if (linked_entity == nullptr)
        return;

    linked_entity->toggle();
}

bool Button::is_interactable(Humanoid *activator) {
    return IInteractable::distance_check(this, activator);
}

void Button::setLinkedEntity(Toggleable *entity_to_link) {
    linked_entity = entity_to_link;
}

Toggleable *Button::getLinkedEntity() {
    return linked_entity;
}

//TODO : create assets and more for humanoids
Button *Button::initializeRendering(const EntityRendering::Context &context) {
    return this;
}

Entity *Button::finalizeRendering(const EntityRendering::Context &context) {
    return this;
}
