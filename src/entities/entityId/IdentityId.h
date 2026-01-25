//
// Created by timon on 1/22/26.
//

#pragma once
#include <cstdint>
#include <iostream>

class IdentifiedEntity;
class Toggleable;
class Vehicle;
class Humanoid;
class Entity;

namespace EntityId {
    //instead of doing anything complex, since a uint64 is BIG, I'll just increment it each time one is assigned, and the max is going to
    //be stored in the save file. Decreasing this to a uint32 would be absolutely possible.
    using entityId = std::uint64_t;

    constexpr entityId FIRST_USABLE_VALUE = 1000;
    /**
     * This entityId is reserved for editor placed entities
     */
    constexpr entityId UNDEFINED_ENTITY_ID = 0;

    /**
     * entity ids below 1000 are reserved for special purposes, namely UNDEFINED_ENTITY_ID
     * @param id the id to check
     * @return
     */
    inline bool isValidEntityId(EntityId::entityId id) {
        return id >= FIRST_USABLE_VALUE;
    }

    class Manager {
    private:
        entityId next_entity_id = FIRST_USABLE_VALUE;

    public:
        Manager() = default;

        Manager(const Manager&) = delete;
        Manager& operator=(const Manager&) = delete;

        entityId createEntityId() {
            return next_entity_id++;
        }

        void setNextEntityId(entityId id) {
            next_entity_id = id;
        }

        void printNextEntityId() const {
            std::cout << "next entity id is : " << next_entity_id << std::endl;
        }

        static Manager& getInstance() {
            static Manager instance;
            return instance;
        }
    };
}
