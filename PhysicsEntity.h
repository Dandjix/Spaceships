#pragma once
#include "Entity.h"
#include "Physics.h"
class PhysicsEntity : public Entity
{
public :
	virtual void  physicsUpdate(const PhysicsUpdateContext & context);
};