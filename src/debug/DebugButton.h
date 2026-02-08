#pragma once
#include "entities/interactables/IInteractable.h"
#include "physics/scripts/PhysicsEntity.h"
#include "physics/shapes/RoundStaticPhysicsShape.h"
#include "spaceships/EntityData/EntityLoading.h"
//
// Created by timon on 2/8/26.
//

namespace Debug {
    class DebugButton : public PhysicsEntity, public virtual IInteractable {
    protected:
        void DoStuff() {
            std::cout << "you done pressed the button I tell you hwat" << std::endl;
        }

    public:
        DebugButton(const Vector2Int &position, float angle)
            : PhysicsEntity(position, angle, new RoundStaticPhysicsShape(this, Scaling::scaleToWorld(32))) {
        }

        void interact(Humanoid *activator) override { DoStuff();}
        bool is_interactable(Humanoid *activator) override { return true; }

        [[nodiscard]] PhysicsEntity *asEntity() override {return this;}
        [[nodiscard]] IInteractable * asIInteractable() override {return this;}

        nlohmann::json toJson() override;

        Entity *initializeRendering(const EntityRendering::Context &context) override;
        Entity *finalizeRendering(const EntityRendering::Context &context) override;
        void render(SDL_Renderer *renderer, const RenderingContext &context) override;

        FROM_JSON_DECLARATION(DebugButton, "debug_button");

        [[nodiscard]] std::string getInteractionText() const override {
            return "Push the crazy button";
        }
    };
}
