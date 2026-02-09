//
// Created by timon on 11/28/25.
//

#pragma once
#include <future>
#include <iostream>

#include "EntityFactory.h"
#include "json.hpp"
#include "math/Vectors.h"

namespace EntityPlacement {
    struct Context;
}

class Entity;

namespace GameState {
    struct transientGameState;
}

namespace EntityPlacement {
    Entity *place(Vector2Int position, float angle);

    template<typename T>
    struct RegisterEntity {
        explicit RegisterEntity(const std::string &class_name) {
            EntityPlacement::
                    EntityFactory::
                    getInstance()
                    .insert(
                        class_name,
                        [](EntityPlacementInterface & interface)-> std::future<Entity * >{
                            return T::_editor_place(interface);
                        });
        }
    };
};


#define EDITOR_PLACE_DECLARATION(ClassName,classId)\
private:\
inline static const EntityPlacement::RegisterEntity<ClassName> _editor_placement_registration_##ClassName = EntityPlacement::RegisterEntity<ClassName>{classId}; \
public: \
static std::future<Entity *> _editor_place(EntityPlacement::EntityPlacementInterface & interface)

#define EDITOR_PLACE_DEFINITION(ClassName)\
std::future<Entity *> ClassName::_editor_place(EntityPlacement::EntityPlacementInterface & interface)
