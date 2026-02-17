//
// Created by timon on 11/20/25.
//

#pragma once
#include <SDL3/SDL_render.h>

#include "math/BoundingBox.h"
#include "math/Vectors.h"


class SpaceShip;
struct RenderingContext;
struct PhysicsUpdateContext;
class PhysicsEntity;
class PhysicsUpdateVisitor;

namespace Instances {
    class Instance;
}

class PhysicsShape
{
public:
    PhysicsEntity * owner_entity;

    explicit PhysicsShape(PhysicsEntity * owner_entity) : owner_entity(owner_entity){}
    virtual ~PhysicsShape() = default;

    [[nodiscard]] Vector2Int getCenter() const;
    [[nodiscard]] float getWeight() const;
    void moveCenter(Vector2Float delta) const;

    virtual void physicsUpdate(const PhysicsUpdateContext & context);
    void drawBoundingBox(SDL_Renderer* renderer, const RenderingContext& context);

    virtual void debugRender(SDL_Renderer * renderer, const RenderingContext & context);

    /**
     * Creates a visitor, which can be consumed by calling it with another physics entity, calling the right physics function
     * @return a pointer to the visitor to be consumed
     */
    virtual PhysicsUpdateVisitor * createVisitor()
    = 0;

    /**
     * Calls the correct physics collision function using the visitor beore deleting it (To be clear : this function deletes the instance)
     * /!\ You must call PhysicsEntity::consumeVisitor(visitor) AFTER your call to the correct method in the override.
     * @param visitor the visitor
     * @param instance the spaceship that both entities belong to
     */
    virtual void consumeVisitor(PhysicsUpdateVisitor* visitor, Instances::Instance *instance)=0;

    [[nodiscard]] virtual std::vector<Vector2Int> getVertices() const=0;

    [[nodiscard]] virtual BoundingBox<int> getBoundingBox() const =0;

    [[nodiscard]] virtual bool is_inside(Vector2Int world_position) const =0;
};
