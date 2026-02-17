//
// Created by timon on 2/7/26.
//

#include "ConvexStaticPhysicsShape.h"

#include "physics/PhysicsUpdateVisitor/PhysicsUpdateVisitorStaticConvex.h"

PhysicsUpdateVisitor * ConvexStaticPhysicsShape::createVisitor() {
    return new PhysicsUpdateVisitorStaticConvex(this);
}

void ConvexStaticPhysicsShape::consumeVisitor(PhysicsUpdateVisitor *visitor, Instances::Instance *instance) {
    visitor->visitStaticConvex(this, instance);
}
