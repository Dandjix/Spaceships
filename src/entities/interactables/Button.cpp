//
// Created by timon on 1/19/26.
//

#include "Button.h"

#include "loadGame/GameState.h"

nlohmann::json Button::toJson() {
    auto json = PhysicsEntity::toJson();

    if (linked_entity != nullptr)
        json["linked_entity_id"] = linked_entity->entity_id;
    else
        json["linked_entity_id"] = "null";

    return json;
}

Button * Button::fromJson(const nlohmann::json &json,GameState::transientGameState & transient_game_state) {
    auto button = new Button(Vector2Int::fromJson(json["position"]),json["angle"],nullptr);
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
    //TODO : implement this
}

void Button::interact(Humanoid *activator) {
    if (linked_entity == nullptr)
        return;

    linked_entity->toggle();
}

bool Button::is_interactable(Humanoid *activator) {
    return IInteractable::distance_check(this,activator);
}
