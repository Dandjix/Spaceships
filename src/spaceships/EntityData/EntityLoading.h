//
// Created by timon on 11/28/25.
//

#pragma once
#include <iostream>

#include "EntityFactory.h"
#include "json.hpp"


namespace EntityRendering {
    struct Context;
}

class Entity;

namespace GameState {
    struct transientGameState;
}

namespace EntityLoading {
    Entity * fromJson(nlohmann::json entry, GameState::transientGameState & transient_game_state, EntityRendering::Context * entity_rendering_context);

    template<typename T>
    struct RegisterEntity {
        explicit RegisterEntity(const std::string &class_name) {
            EntityFactory::getInstance().insert(class_name,
                [](const nlohmann::json& j, GameState::transientGameState& s) -> Entity* {
                    return T::fromJson(j, s);
                });
        }
    };

};


#define FROM_JSON_DECLARATION(ClassName,classId)\
std::string getJsonType() override{return classId;} \
private:\
inline static const EntityLoading::RegisterEntity<ClassName> registration_##ClassName = EntityLoading::RegisterEntity<ClassName>{classId}; \
public: \
static ClassName * fromJson(const nlohmann::json& json, GameState::transientGameState& transient_game_state)

#define FROM_JSON_DEFINITION(ClassName)\
ClassName * ClassName::fromJson(const nlohmann::json &json, GameState::transientGameState &transient_game_state)

