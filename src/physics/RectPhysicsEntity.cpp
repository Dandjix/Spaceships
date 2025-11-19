//
// Created by timon on 11/18/25.
//

#include "RectPhysicsEntity.h"


void RectPhysicsEntity::beVisitedBy(PhysicsEntity* other, PhysicsUpdateVisitor* visitor, SpaceShip * space_ship)
{
    return visitor->visitRect(other,this,space_ship);
}