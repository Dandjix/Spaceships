#pragma once
#include <SDL3/SDL.h>
#include <optional>

#include "../behavior/Behavior.h"
#include "../math/Vectors.h"
#include "../game/Rendering.h"
#include "../game/Update.h"
#include  "../behavior/EntityAttributes.h"
#include  "../spaceships/SpaceShip.h"
#include "events/Event.h"






class Entity
{
protected:
    Vector2Int position;        // Using Vector2 to represent position
    std::optional<float> angle; // if an angle is null, the object will always face towards the camera

    Behavior * behavior;

    EntityAttributes * attributes;

    void renderTexture(SDL_Renderer *renderer, const RenderingContext &context, SDL_Texture *texture, Vector2Float destSize) const
    {
        destSize = destSize / context.camera_info.cameraScale;

        Vector2Float center = context.camera_info.worldToScreenPoint(position);

        SDL_FRect destRect = {
            center.x - destSize.x,
            center.y - destSize.y,
            destSize.x * 2.0f,
            destSize.y * 2.0f};
        float entity_angle;
        if (hasAngle())
        {
            entity_angle = getAngle() + context.camera_info.cameraAngle;
        }
        else
        {
            entity_angle = 0;
        }

        SDL_RenderTextureRotated(renderer, texture, nullptr, &destRect, entity_angle, nullptr, SDL_FLIP_NONE);
    }
public:

    Event<> on_die;


    /// <summary>
    /// creates an entity
    /// </summary>
    /// <param name="position">pixel position, converted internally to a position</param>
    Entity(Vector2Int position, std::optional<float> angle, Behavior * behavior)
        : position(position), angle(angle),behavior(behavior),attributes(new EntityAttributes()) {} // Constructor using Vector2

    virtual ~Entity()
    {
        on_die.emit();
    };

    virtual void update(const UpdateContext& context);
    virtual void handleEvent(const SDL_Event& event, const GameEvent::GameEventContext &context);
    virtual void render(SDL_Renderer *renderer, const RenderingContext &context) = 0;
    virtual void debugRender(SDL_Renderer *renderer, const RenderingContext &context) {}

    virtual void onRegistered(SpaceShip * newSpaceship) {}
    virtual void onUnRegistered(SpaceShip * oldSpaceship) {}

    /// <summary>
    /// lowest values first ! put something early neear the start, something late near the end.
    /// </summary>
    /// <returns></returns>
    virtual unsigned short int getQueueOrder()
    {
        return UINT16_MAX / 2;
    }

    [[nodiscard]] Vector2Int getPosition() const
    {
        return position;
    }

    // accessors
    [[nodiscard]] Behavior * getBehavior() const {return behavior;}
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
    void setBehavior(Behavior * entity_behavior) {this->behavior = entity_behavior;}

    [[nodiscard]] EntityAttributes * getAttributes() const {return attributes;}

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
};
