//
// Created by timon on 11/21/25.
//

#include "SpaceshipHooks.h"

#include "game/Update.h"
#include "spaceships/SpaceShip.h"

inline void SpaceshipHooks::update(const UpdateContext& context)
{
    for (auto entity : context.spaceShip->getEntities(RoomDistance::All))
    {
        for (auto entry : regions)
        {
            HookRegion * region = entry.second;
            if (region->pointIsInside(entity->getPosition()))
            {
                if (!region->hasEntity(entity))
                    region->registerEntity(entity);
            }
            else
            {
                if (region->hasEntity(entity))
                    region->unRegisterEntity(entity);
            }
        }
    }
}
