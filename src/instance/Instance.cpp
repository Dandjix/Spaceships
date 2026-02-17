//
// Created by timon on 2/13/26.
//

#include "Instance.h"

#include "behavior/BehavioredEntity.h"
#include "entities/scripts/ActiveWhenPausedEntity.h"
#include "entities/scripts/LateUpdateEntity.h"
#include "physics/Physics.h"
#include "physics/PhysicsUpdateVisitor/PhysicsUpdateVisitor.h"
#include "physics/PhysicsUpdateVisitor/PhysicsUpdateVisitorWall.h"
#include "physics/shapes/PhysicsShape.h"


bool EntityComparison::compareEntities(Entity *e1, Entity *e2) {
    return e1->getQueueOrder() < e2->getQueueOrder();
}

bool EntityComparison::comparePhysicsEntities(PhysicsEntity *e1, PhysicsEntity *e2) {
    //TODO : add a queue order for the physics entities if necessary
    return e1->getQueueOrder() < e2->getQueueOrder();
}


std::vector<Entity *> Instances::Instance::getEntities() const {
    auto vec = std::vector<Entity *>(entities.begin(), entities.end());

    std::sort(vec.begin(), vec.end(), EntityComparison::compareEntities);

    return vec;
}

std::vector<PhysicsEntity *> Instances::Instance::getPhysicsEntities() const {
    auto vec = std::vector<PhysicsEntity *>(physics_entities.begin(), physics_entities.end());

    std::sort(vec.begin(), vec.end(), EntityComparison::comparePhysicsEntities);

    return vec;
}

std::vector<BehavioredEntity *> Instances::Instance::getBehavioredEntities() const {
    //TODO if in need of performance, store in its own variable
    std::vector<BehavioredEntity *> to_return = {};
    for (auto entity: entities) {
        if (auto behaviored = dynamic_cast<BehavioredEntity *>(entity)) {
            to_return.push_back(behaviored);
        }
    }
    return to_return;
}

void Instances::Instance::registerEntities(const std::vector<Entity *> &to_register) {
    for (Entity *e: to_register) {
        e->registerInInstance(this);
    }
}

void Instances::Instance::unregisterEntities(const std::vector<Entity *> &to_unregister, bool delete_when_done) {
    for (Entity *e: to_unregister) {
        e->unregisterInInstance(this, delete_when_done);
    }
}

bool Instances::Instance::has_entity(Entity *entity) const {
    return entities.contains(entity);
}


Instances::Instance *Instances::Instance::finalizeRendering(const EntityRendering::Context &entity_rendering_context) {
    for (auto entity: entities) {
        entity->finalizeRendering(entity_rendering_context);
    }
    return this;
}


// Handling ------------------------------------------------------------------------------------------------------------

Instances::Instance::Instance(
    SpaceShip *space_ship,
    const std::vector<Entity *> &entities
) : space_ship(space_ship), entities() {
    for (Entity *e: entities) {
        e->registerInInstance(this);
    }
}

void Instances::Instance::physicsHandling(float target_delta_time, int subdivisions) {
    const PhysicsUpdateContext physicsContext = {
        target_delta_time / static_cast<float>(subdivisions),
        this
    };

    for (int i = 0; i < subdivisions; ++i) {
        auto working_physics_entities = getPhysicsEntities();

        hash_proximity_map.populate(working_physics_entities);

        for (auto entity: working_physics_entities) {
            if (entity->shape == nullptr)
                continue;

            std::unordered_set<PhysicsShape *> shapes;
            shapes.reserve(1);

            for (Vector2Int cell: entity->shape->getBoundingBox().encompassedTiles()) {
                for (PhysicsShape *physics_shape: hash_proximity_map.at_cell(cell)) {
                    shapes.insert(physics_shape);
                }
            }
            shapes.erase(entity->shape);
            for (PhysicsShape *other_shape: shapes) {
                // if (!entity->shape->getBoundingBox().intersects(other_shape->getBoundingBox())) //might be worth doing it, might not be. TODO : investigate
                //     continue;
                PhysicsUpdateVisitor *visitor = entity->shape->createVisitor();
                other_shape->consumeVisitor(visitor, this);
                delete visitor;
            }
        }
    }

    for (PhysicsEntity *e: getPhysicsEntities()) {
        PhysicsUpdateVisitorWall visitor = PhysicsUpdateVisitorWall(space_ship->spaceship_tiles);
        e->shape->consumeVisitor(&visitor, this);
    }


    for (PhysicsEntity *entity: getPhysicsEntities()) {
        entity->shape->physicsUpdate(physicsContext);
    }
}

void Instances::Instance::renderHandling(SDL_Renderer *renderer, const RenderingContext &renderingContext) {
    for (Entity *entity: getEntities()) {
        entity->render(renderer, renderingContext);
    }
    //render debug
    for (Entity *entity: getEntities()) {
        entity->debugRender(renderer, renderingContext);
    }
    //physics shapes debug
    for (PhysicsEntity *entity: getPhysicsEntities()) {
        entity->shape->debugRender(renderer, renderingContext);
    }
    //render the hash_proximity_map for debugging purposes
    // hash_proximity_map.render(renderer,renderingContext);
}

void Instances::Instance::eventHandling(const SDL_Event &event, const GameEvent::GameEventContext &event_context,
                                        bool paused) {
    if (!paused) {
        for (Entity *entity: getEntities()) {
            entity->handleEvent(event, event_context);
        }
    } else {
        for (auto entity: active_when_paused_entities) {
            entity->handleEvent(event, event_context);
        }
    }
}

void Instances::Instance::ProcessDeletionQueues() {
    entities.process_deletion_queue();
    physics_entities.process_deletion_queue();
    late_update_entities.process_deletion_queue();
    active_when_paused_entities.process_deletion_queue();
}

void Instances::Instance::updateHandling(const CameraTransformations::CameraInfo &camera_info, float deltaTime,
                                         GameEvent::MousePositionType mouse_position_type, bool paused) {
    UpdateContext updateContext = {
        camera_info,
        deltaTime,
        this,
        mouse_position_type
    };

    if (!paused) {
        // update
        for (Entity *entity: getEntities()) {
            entity->update(updateContext);
        }
        space_ship->spaceship_hooks->update(updateContext);

        ProcessDeletionQueues();
    } else {
        for (auto entity: active_when_paused_entities) {
            entity->update(updateContext);
        }
    }
}

void Instances::Instance::lateUpdateHandling(const CameraTransformations::CameraInfo &camera_info, float deltaTime,
                                             GameEvent::MousePositionType mouse_position_type, bool paused) {
    if (paused)
        return;

    UpdateContext updateContext = {
        camera_info,
        deltaTime,
        this,
        mouse_position_type
    };

    // late update
    for (LateUpdateEntity *entity: late_update_entities) {
        entity->lateUpdate(updateContext);
    }
}
