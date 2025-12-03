//
// Created by timon on 12/3/25.
//

#include "RoundStaticPhysicsShape.h"

#include "physics/PhysicsUpdateVisitor/PhysicsUpdateVisitorStaticRound.h"

PhysicsUpdateVisitor * RoundStaticPhysicsShape::createVisitor() {
    return new PhysicsUpdateVisitorStaticRound(this);
}
