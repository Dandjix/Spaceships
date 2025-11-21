#pragma once
#include "PhysicsEntity.h"
#include "PhysicsShape.h"
#include "PhysicsUpdateVisitor/PhysicsUpdateVisitorRound.h"

class RoundPhysicsShape : public PhysicsShape
{
	public:
	float radius;

	PhysicsUpdateVisitor * createVisitor() override
	{
		return  new PhysicsUpdateVisitorRound(this);
	}
	void consumeVisitor(PhysicsUpdateVisitor* visitor, SpaceShip* space_ship) override
	{
		visitor->visitRound(this,space_ship);
	}

	RoundPhysicsShape(PhysicsEntity * owner, float radius) : PhysicsShape(owner), radius(radius)
	{

	}

	void debugRender(SDL_Renderer* renderer, const RenderingContext& context) override;

	BoundingBox<int> getBoundingBox() override;
};