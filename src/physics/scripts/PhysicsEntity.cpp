//
// Created by timon on 11/18/25.
//

#include "PhysicsEntity.h"

#include "entities/scripts/Entity.h"
#include "../shapes/PhysicsShape.h"
#include "entities/toggleables/door/DoorPanel.h"
#include "physics/constraints/scripts/PhysicsConstraint.h"
#include "spaceships/SpaceShip.h"

PhysicsEntity::~PhysicsEntity() {
    delete shape;
}

void PhysicsEntity::applyConstraints() {

    auto constraints = getConstraints();

    for (Physics::Constraint * c: constraints) {
        c->apply(this);
    }
}

inline void PhysicsEntity::registerInInstance(Instances::Instance *world_instance)
{
    Entity::registerInInstance(world_instance);
    world_instance->physics_entities.insert(this);
    // if (!isJsonSerializable())
    //     std::cout << "inserted a non serializable ! " << std::endl;
}

inline void PhysicsEntity::unregisterInInstance(Instances::Instance *world_instance, bool delete_when_done)
{
    Entity::unregisterInInstance(world_instance, delete_when_done);
    world_instance->physics_entities.add_to_erasing_queue(this);
}

void PhysicsEntity::debugRender(SDL_Renderer *renderer, const RenderingContext &context) {
    Entity::debugRender(renderer, context);

    for (Physics::Constraint * c: getConstraints()) {
        c->debugRender(this,renderer, context);
    }
}
