#pragma once
#include "../entities/Entity.h"
#include "../physics/Physics.h"
class PhysicsEntity : public Entity
{
public :
	virtual void  physicsUpdate(const PhysicsUpdateContext & context){}
};