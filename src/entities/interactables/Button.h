//
// Created by timon on 1/19/26.
//

#pragma once
#include "IInteractable.h"
#include "physics/shapes/RoundStaticPhysicsShape.h"


class Button : public PhysicsEntity, public IInteractable{

private:
    std::function<void()> on_pressed_callback;

public:
    Button(const Vector2Int &position, const std::optional<float> &angle, const std::function<void()> &on_pressed_callback)
        : PhysicsEntity(position, angle, new RoundStaticPhysicsShape(this,Scaling::scaleToWorld(22))), on_pressed_callback(on_pressed_callback) {
    }

    nlohmann::json toJson() override;

    std::string getJsonType() override { return "button";}

    void render(SDL_Renderer *renderer, const RenderingContext &context) override;

    void interact(Humanoid *activator) override;

    bool is_interactable(Humanoid *activator) override;

    [[nodiscard]] PhysicsEntity *asEntity() override {return this;}
};