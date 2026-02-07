//
// Created by timon on 2/7/26.
//

#include "ConvexStaticPhysicsShape.h"

#include "physics/PhysicsUpdateVisitor/PhysicsUpdateVisitorStaticConvex.h"

PhysicsUpdateVisitor * ConvexStaticPhysicsShape::createVisitor() {
    return new PhysicsUpdateVisitorStaticConvex(this);
}

void ConvexStaticPhysicsShape::consumeVisitor(PhysicsUpdateVisitor *visitor, SpaceShip *space_ship) {
    visitor->visitStaticConvex(this, space_ship);
}
