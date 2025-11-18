#include "RoundPhysicsEntity.h"

void RoundPhysicsEntity::physicsUpdate(const PhysicsUpdateContext& context)
{
    // const std::vector<PhysicsEntity *> & all_entities = context.space_ship->getPhysicsEntities(RoomDistance::Close);
    //
    // for (auto e : all_entities)
    // {
    //     if (e == this)
    //     {
    //         continue;
    //     }
    //
    //
    // }

    std::cout << "my position is " << position << std::endl;
}
