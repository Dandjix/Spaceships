//
// Created by timon on 1/22/26.
//

#pragma once
#include <cstdint>


class IdentifiedEntity;
class Toggleable;
class Vehicle;
class Humanoid;
class Entity;

namespace EntityId {
    using entityId = std::uint64_t;

    static entityId max_entity_id = 0;

    //instead of doing anything complex, since a uint64 is BIG, I'll just increment it each time one is assigned, and the max is going to
    //be stored in the save file. Decreasing this to a uint32 would be absolutely possible.

    //I don't think every entity should have an identifier
    static entityId createEntityId() {
        max_entity_id++;
        return max_entity_id-1;
    }

    static IdentifiedEntity * findIdentifiedEntity(entityId entity_id);
}