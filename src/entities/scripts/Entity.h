#pragma once
#include <SDL3/SDL.h>
#include <optional>

#include "../../behavior/Behavior.h"
#include "../../math/Vectors.h"
#include "../../game/Rendering.h"
#include "../../game/Update.h"
#include "../../spaceships/SpaceShip.h"
#include "events/Event.h"
#include "loadGame/GameState.h"

namespace QueueOrder {
    using Value = unsigned short;

    constexpr unsigned short FIRST = 0;
    constexpr unsigned short MIDDLE = USHRT_MAX*0.5F;
    constexpr unsigned short LAST = USHRT_MAX;
}

class Entity
{
protected:
    Vector2Int position;        // Using Vector2 to represent position
    std::optional<float> angle; // if an angle is null, the object will always face towards the camera

    void renderTexture(SDL_Renderer *renderer, const RenderingContext &context, SDL_Texture *texture, Vector2Float destSize) const;

public:

    Event<> on_die;

    Event<Entity*> on_ownership_change;


    /// <summary>
    /// creates an entity
    /// </summary>
    /// <param name="position">pixel position, converted internally to a position</param>
    Entity(Vector2Int position, std::optional<float> angle)
        : position(position), angle(angle) {
    } // Constructor using Vector2

    virtual ~Entity()
    {
        on_die.emit();
    }

    virtual nlohmann::json toJson()=0;
    virtual bool isJsonSerializable(){return true;}
    virtual constexpr std::string getJsonType()=0;

    virtual void update(const UpdateContext& context);
    virtual void handleEvent(const SDL_Event& event, const GameEvent::GameEventContext &context);
    virtual void render(SDL_Renderer *renderer, const RenderingContext &context) = 0;
    virtual void debugRender(SDL_Renderer *renderer, const RenderingContext &context) {}

    virtual void onRegistered(SpaceShip * newSpaceship) {}
    virtual void onUnRegistered(SpaceShip * oldSpaceship) {}

    /// <summary>
    /// lowest values first ! put something early neear the start, something late near the end. Base value is around 30000
    /// </summary>
    /// <returns></returns>
    virtual QueueOrder::Value getQueueOrder()
    {
        return QueueOrder::MIDDLE;
    }

    [[nodiscard]] Vector2Int getPosition() const
    {
        return position;
    }

    /**
     * Registers an entity in the spaceship so that it's behavior and appearance are handled in this spaceship. Does not call any event, this is done in the spaceship itself.
     *
     *  You should never use this directly, call spaceship.registerEntity (which calls this)
     *
     *  The use of this is to be overriden by physics entities, as they also add themselves to the physics entities list
     *
     * @param space_ship the spaceship in which the entity will be registered
     */
    virtual void registerInSpaceship(SpaceShip* space_ship);
    /**
     *  Unregisters an entity in the spaceship so that it's behavior and appearance are handled in this spaceship. Does not call any event, this is done in the spaceship itself.
     *
     *  You should never use this directly, call spaceship.unregisterEntity (which calls this)
     *
     *  The use of this is to be overriden by physics entities, as they also add themselves to the physics entities list
     *
     * @param space_ship the spaceship in which the entity will be unregistered
     */
    virtual void unregisterInSpacehip(SpaceShip* space_ship);;

    int getX() const { return position.x; }
    int getY() const { return position.y; }
    bool hasAngle() const { return angle.has_value(); }
    float getAngle() const { return angle.value_or(0); }
    void setPosition(Vector2Int newPosition) { position = newPosition; }
    void setAngle(float a, bool force = false)
    {
        if (!angle.has_value())
        {
            throw std::invalid_argument("this entity was instantiated without an angle, it is not possible to assign it one now.");
        }
        auto new_angle = static_cast<float>(fmod(a + 360, 360));
        angle = new_angle;
    }

    /**
     * Moves an object by a world coordinates delta
     * @param delta the vector to move, in world units
     * @param space_ship the spaceship in which to move, used by physics (wall collisions notably)
     */
    virtual void movePosition(Vector2Float delta, SpaceShip * space_ship);

    virtual void after_deserialized(GameState::GameState * game_state) {}
};
