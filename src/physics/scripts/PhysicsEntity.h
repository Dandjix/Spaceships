#pragma once
#include "entities/scripts/Entity.h"
#include "math/Vectors.h"

class SpaceShip;
class IInteractable;
class PhysicsShape;


class PhysicsEntity : public Entity
{
public:
	PhysicsShape * const shape;

public :
	PhysicsEntity(const Vector2Int& position, const std::optional<float>& angle, PhysicsShape * shape)
		: Entity(position, angle), shape(shape)
	{
	}

	~PhysicsEntity() override;

	/**
	 * @return The weight in metric kilograms of the entity (ex : 24000 for a loaded cargo container)
	 */
	virtual float get_weight() { return 1000.0f; }

	[[nodiscard]] virtual IInteractable * asIInteractable(){return nullptr;}

	void registerInInstance(Instances::Instance *world_instance) override;

	void unregisterInInstance(Instances::Instance *world_instance, bool delete_when_done) override;
};



