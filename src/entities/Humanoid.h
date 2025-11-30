#pragma once

#include "../behavior/Behavior.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "behavior/BehavioredEntity.h"
#include "physics/PhysicsEntity.h"
#include "physics/RoundPhysicsShape.h"
#include "textures/Textures.h"

class Vehicle;

class Humanoid : public BehavioredEntity{
private:
    SDL_Texture * texture;
public:
    Event<Vehicle*> on_start_piloting_vehicle;
    Event<Vehicle*> on_stop_piloting_vehicle;
protected:
    float radius;
public:

    Humanoid(Vector2Int position,std::optional<float> angle, Behavior * behavior) :
        BehavioredEntity(position,angle,new RoundPhysicsShape(this, Scaling::scaleToWorld(20.0f)),behavior)
    {
        radius = 20;
        texture = nullptr;
    }
    void render(SDL_Renderer *renderer, const RenderingContext & context) override;

    nlohmann::json toJson() override {
        auto json = Entity::toJson();

        if (behavior!=nullptr) {
            json["behavior"] = behavior->toJson();

            if (behavior->isPlayerBehavior()) {
                json["is_player"] = true;
            }

        }
        return json;
    }

    static Humanoid * fromJson(nlohmann::json::const_reference json) {
        std::optional<float> angle = std::nullopt;
        if (json.contains("angle")) {
            angle = json["angle"];
        }

        Behavior * behavior = nullptr;


        if (json.contains("behavior")) {
            behavior = Behavior::fromJson(json["behavior"]);
        }

        return  new Humanoid(
            Vector2Int{
                json["position"]["x"],
                json["position"]["y"]
            },
            angle,
            behavior
        );
    }

    constexpr std::string getJsonType() override{return "humanoid";}

    constexpr bool is_player() override {
        if (behavior ==nullptr)
            return false;
        return behavior->isPlayerBehavior();
    }
};
