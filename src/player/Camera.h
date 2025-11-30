#pragma once
#include "../entities/Entity.h"
#include "../math/Vectors.h"
#include "../game/Rendering.h"

class Camera : public Entity {  // Inherit from Entity
protected:
    Entity* player;  // Pointer to the player entity
    Vector2Int screenDimensions = Vector2Int(0, 0);

    Events::Id player_ownership_changed_id = Events::null_id;
private :
    float scale;
public:
    /// <summary>
    /// Creates a Camera that follows the player.
    /// </summary>
    /// <param name="p">Pointer to the player entity</param>
    Camera(Vector2Int position, float angle, float scale) : Entity(position, angle), player(nullptr), scale(scale) {
    }

    SpaceShip * working_spaceship = nullptr;

    /// <summary>
    /// Set the player entity that the camera follows.
    /// </summary>
    /// <param name="p">Pointer to the new player entity</param>
    void setPlayer(Entity* p)
    {
        player = p;
        player_ownership_changed_id = player->on_ownership_change.subscribe(
            [this](Entity * new_owner) {
                player->on_ownership_change.unsubscribe(this->player_ownership_changed_id);
                setPlayer(new_owner);
            });
        setPosition(p->getPosition());  // Set the camera's position to follow the player initially
    }

    void handleEvent(const SDL_Event& event, const GameEvent::GameEventContext &context) override {
        if (event.type == SDL_EVENT_MOUSE_WHEEL) {
            float zoomFactor = 0.1f; // Adjust zoom speed
            float scale_value= getScale() + event.wheel.y * zoomFactor;

            // Clamp scale
            scale_value = std::clamp(scale_value, 0.5f,5.0f);

            setScale(scale_value);
        }
    }

    unsigned short getQueueOrder() override
    {
        return UINT16_MAX;
    }
    
    void setScale(float s)
    {
        if (s != scale)
        {
            scale = s;
        }
    }

    float getScale() const
    {
        return scale;
    }

    void setScreenDimensions(Vector2Int value)
    {
        this->screenDimensions = value;
    }


    /// <summary>
    /// A camera renders nothing.
    /// </summary>
    void render(SDL_Renderer* renderer,const RenderingContext& context) override {}

    /// <summary>
    /// Update the camera's position to follow the player.
    /// </summary>
    void update(const UpdateContext & context) override {
        setPosition(player->getPosition());  // Update camera's position based on the player's position
        const bool* state = SDL_GetKeyboardState(nullptr);
        float deltaAngle = 0;
        if (state[SDL_SCANCODE_PAGEUP]) {
            deltaAngle -= 180* context.deltaTime;
        }
        if (state[SDL_SCANCODE_PAGEDOWN]) {
            deltaAngle += 180 * context.deltaTime;
        }
        float newAngle = getAngle() + deltaAngle;
        setAngle(newAngle);
        //if (getAngle() != newAngle)
        //{
        //    std::cout << "angle is " << newAngle <<"\n";
        //}
    }

    nlohmann::json toJson() override {
        auto json = Entity::toJson();
        json["scale"] = scale;
        return json;
    }

    static Entity * fromJson(nlohmann::json json) {
        return new Camera(Vector2Int::fromJson(json["position"]),json["angle"],json["scale"]);
    }

    void registerInSpaceship(SpaceShip *space_ship) override {
        Entity::registerInSpaceship(space_ship);
        space_ship->cameras.push_back(this);
    }

    void unregisterInSpacehip(SpaceShip *space_ship) override {
        Entity::unregisterInSpacehip(space_ship);
        space_ship->cameras.erase(std::find(space_ship->cameras.begin(),space_ship->cameras.end(),this));
    }

    void onRegistered(SpaceShip *newSpaceship) override {
        working_spaceship = newSpaceship;
    }

    constexpr std::string getJsonType() override {return "camera";}
};
