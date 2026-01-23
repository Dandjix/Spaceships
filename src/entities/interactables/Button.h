//
// Created by timon on 1/19/26.
//

#pragma once
#include "IInteractable.h"
#include "../toggleables/scripts/Toggleable.h"
#include "physics/shapes/RoundStaticPhysicsShape.h"


namespace GameState {
    struct transientGameState;
}

class Button : public PhysicsEntity, public IInteractable {
private:
    Toggleable * linked_entity;

public:
    Button(
        Vector2Int position,
        float angle,
        Toggleable * linked_device)

        : PhysicsEntity(
              position,
              angle,
              new RoundStaticPhysicsShape(this,Scaling::scaleToWorld(22.0f))),
          linked_entity(linked_device) {
    }

    nlohmann::json toJson() override;

    static Button * fromJson(const nlohmann::json & json,GameState::transientGameState & transient_game_state);
    void finalizeJsonDeserialization(const GameState::transientGameState &transient_game_state) override;

    std::string getJsonType() override { return "button"; }

    void render(SDL_Renderer *renderer, const RenderingContext &context) override;

    void interact(Humanoid *activator) override;

    bool is_interactable(Humanoid *activator) override;

    [[nodiscard]] PhysicsEntity *asEntity() override { return this; }

    [[nodiscard]] std::string getInteractionText() const override {
        return "[E] Toggle light";
    }

    [[nodiscard]] IInteractable * asIInteractable() override {return this;}
};
