//
// Created by timon on 1/19/26.
//

#pragma once
#include "../interactables/IInteractable.h"
#include "../toggleables/scripts/Toggleable.h"
#include "physics/shapes/RoundStaticPhysicsShape.h"
#include "scripts/ToggleableActivator.h"


namespace GameState {
    struct transientGameState;
}

class Button : public PhysicsEntity, public IInteractable, public virtual ToggleableActivator{
private:
    Toggleable * linked_entity;

    SDL_Texture * texture;
public:
    Button(
        Vector2Int position,
        float angle,
        Toggleable * linked_device);

    nlohmann::json toJson() override;

    static Button * fromJson(const nlohmann::json & json,GameState::transientGameState & transient_game_state);
    void finalizeJsonDeserialization(const GameState::transientGameState &transient_game_state) override;

    std::string getJsonType() override { return "button"; }

    void render(SDL_Renderer *renderer, const RenderingContext &context) override;

    void interact(Humanoid *activator) override;

    bool is_interactable(Humanoid *activator) override;

    [[nodiscard]] PhysicsEntity * asEntity() override { return this; }
    [[nodiscard]] IInteractable * asIInteractable() override {return this;}

    [[nodiscard]] std::string getInteractionText() const override {
        return "[E] Toggle light";
    }

    void setLinkedEntity(Toggleable *entity_to_link) override;

    [[nodiscard]] Toggleable * getLinkedEntity() override;

    Button *initializeRendering(const EntityRendering::Context &context) override;

    Entity *finalizeRendering(const EntityRendering::Context &context) override;
};
