//
// Created by timon on 11/28/25.
//

#pragma once
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
};


#define FROM_JSON_DECLARATION(ClassName,classId)\
constexpr std::string getJsonType() override{return classId;} \
static ClassName * fromJson(const nlohmann::json& json, GameState::transientGameState& transient_game_state)

#define FROM_JSON_IMPLEMENTATION(ClassName,classId)\
ClassName * ClassName::fromJson(const nlohmann::json &json, GameState::transientGameState &transient_game_state)