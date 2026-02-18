#pragma once
#include "entities/scripts/Entity.h"
#include "math/Vectors.h"
#include "physics/Physics.h"

namespace Physics {
    class Constraint;
}

class SpaceShip;
class IInteractable;
class PhysicsShape;


class PhysicsEntity : public Entity {
public:
    PhysicsShape *const shape;

protected:
    /**
     *
     * @return The physics constraints to apply to this entity.
     * The order of the vector is the order they will be applied in (first first, last last)
     */
    [[nodiscard]] virtual std::vector<Physics::Constraint *> getConstraints() { return {}; }

public :
    PhysicsEntity(const Vector2Int &position, const std::optional<float> &angle, PhysicsShape *shape)
        : Entity(position, angle), shape(shape) {
    }

    ~PhysicsEntity() override;

    [[nodiscard]] virtual bool interacts(PhysicsEntity * other){return true;}

    void applyConstraints();

    /**
     * @return The weight in metric kilograms of the entity (ex : 24000 for a loaded cargo container)
     */
    virtual float get_weight() { return 1000.0f; }

    [[nodiscard]] virtual IInteractable *asIInteractable() { return nullptr; }

    void registerInInstance(Instances::Instance *world_instance) override;

    void unregisterInInstance(Instances::Instance *world_instance, bool delete_when_done) override;

    void debugRender(SDL_Renderer *renderer, const RenderingContext &context) override;
};
