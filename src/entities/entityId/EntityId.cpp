//
// Created by timon on 1/25/26.
//
#include "EntityId.h"
#include "loadGame/GameState.h"
#include "IIdentifiedEntity.h"
#include <iostream>
#include <unordered_set>

void EntityId::Manager::reset() {
    setNextEntityId(0);
}

void EntityId::Manager::reset(const GameState::transientGameState &transient_game_state) {
    //this part determines the max entityId
    auto max_entity_id = EntityId::FIRST_USABLE_VALUE;
    for (const auto &[key,entity]: transient_game_state.identified_entities) {
        auto id = entity->getEntityId();
        if (id >= max_entity_id)
            max_entity_id = id;
    }
    setNextEntityId(max_entity_id + 1);
}

std::vector<std::string> EntityId::Manager::getEntityIdKeys() {
    return {"entity_id", "linked_entity_id"};
}


void EntityId::Manager::incrementEntityIds(nlohmann::json &json) {
    entityId base_id = getNextEntityId();
    auto accepted = getEntityIdKeys();
    entityId new_max = 0;

    // Convert vector to set for O(1) lookup
    std::unordered_set<std::string> accepted_keys(accepted.begin(), accepted.end());

    // Recursive lambda to traverse and modify
    std::function<void(nlohmann::json &)> increment = [&](nlohmann::json &j) {
        if (j.is_object()) {
            for (auto it = j.begin(); it != j.end(); ++it) {
                // Check if this key is in our accepted list
                if (accepted_keys.contains(it.key())) {
                    // If value is a number, increment it
                    if (it.value().is_number_integer()) {
                        auto new_value = it.value() = it.value().get<entityId>() + base_id;
                        if (new_value > new_max)
                            new_max = new_value;
                        it.value() = new_value;
                    }
                }
                // Recurse into nested structures
                if (it.value().is_structured()) {
                    increment(it.value());
                }
            }
        } else if (j.is_array()) {
            for (auto &element: j) {
                if (element.is_structured()) {
                    increment(element);
                }
            }
        }
    };

    increment(json);

    setNextEntityId(new_max + 1);
}