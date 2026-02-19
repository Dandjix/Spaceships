//
// Created by timon on 1/22/26.
//

#pragma once
#include <cstdint>
#include <iostream>

#include "json.hpp"


class IIdentifiedEntity;
class IToggleable;
class Vehicle;
class Humanoid;
class Entity;

namespace GameState {
    struct transientGameState;
}

namespace EntityId {
    //instead of doing anything complex, since a uint64 is BIG, I'll just increment it each time one is assigned, and the max is going to
    //be stored in the save file. Decreasing this to a uint32 would be absolutely possible.

    //entities can only reference entities on the same ship for now

    using entityId = std::uint64_t;

    constexpr entityId FIRST_USABLE_VALUE = 0;

    class Manager {
    private:
        entityId next_entity_id = FIRST_USABLE_VALUE;

    public:
        Manager() = default;

        Manager(const Manager&) = delete;
        Manager& operator=(const Manager&) = delete;

        /**
         * increments the counter and returns the new (hopefully unique) generated id
         * @return
         */
        entityId createEntityId() {
            return next_entity_id++;
        }

        /**
         * for use in loading a save and loading a ship
         * @param id
         */
        void setNextEntityId(entityId id) {
            next_entity_id = id;
        }

        void printNextEntityId() const {
            std::cout << "next entity id is : " << next_entity_id << std::endl;
        }
        void reset();

        void reset(const GameState::transientGameState & transient_game_state);

        /**
         * doesnt increment the counter
         * @return
         */
        [[nodiscard]] entityId getNextEntityId() {
            return next_entity_id;
        }

        [[nodiscard]] static std::vector<std::string>getEntityIdKeys();

        void incrementEntityIds(nlohmann::json &json);

        static Manager& getInstance() {
            static Manager instance;
            return instance;
        }
    };


}
