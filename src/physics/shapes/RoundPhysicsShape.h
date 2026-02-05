#pragma once
#include "../PhysicsEntity.h"
#include "PhysicsShape.h"
#include "../PhysicsUpdateVisitor/PhysicsUpdateVisitorRound.h"

class RoundPhysicsShape : public PhysicsShape
{
	public:
	/**
	 * radius of the shape, in world units
	 */
	float radius;

	PhysicsUpdateVisitor * createVisitor() override
	{
		return  new PhysicsUpdateVisitorRound(this);
	}
	void consumeVisitor(PhysicsUpdateVisitor* visitor, SpaceShip* space_ship) override
	{
		visitor->visitRound(this,space_ship);
	}

	/**
	 * a round physics shape
	 * @param owner the owner entity
	 * @param radius the radius in world units
	 */
	RoundPhysicsShape(PhysicsEntity * owner, float radius) : PhysicsShape(owner), radius(radius)
	{

	}

	void debugRender(SDL_Renderer* renderer, const RenderingContext& context) override;

	BoundingBox<int> getBoundingBox() const override;

	bool is_inside(Vector2Int world_position) const override;
};