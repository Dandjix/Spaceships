//
// Created by timon on 11/20/25.
//

#pragma once
#include "Physics.h"
#include "PhysicsEntity.h"
#include "math/BoundingBox.h"
#include "spaceships/SpaceShip.h"


class PhysicsUpdateVisitor;

class PhysicsShape
{
public:
    PhysicsEntity * owner_entity;

    explicit PhysicsShape(PhysicsEntity * owner_entity) : owner_entity(owner_entity){}
    virtual ~PhysicsShape() = default;

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
     * @param space_ship the spaceship that both entities belong to
     */
    virtual void consumeVisitor(PhysicsUpdateVisitor* visitor, SpaceShip* space_ship)=0;

    virtual BoundingBox<int> getBoundingBox()=0;

    virtual bool is_inside(Vector2Int world_position)=0;
};
