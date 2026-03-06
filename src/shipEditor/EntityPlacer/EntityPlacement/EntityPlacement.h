//
// Created by timon on 11/28/25.
//

#pragma once
#include <future>
#include <iostream>

#include "EntityFactory.h"
#include "EntityPlacementContext.h"
#include "json.hpp"
#include "math/Vectors.h"

class Entity;


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
                        [](EntityPlacement::Context * context)-> void{
                            return T::_editor_place(context);
                        });
        }
    };
};


#define EDITOR_PLACE_DECLARATION(ClassName,classId)\
private:\
inline static const EntityPlacement::RegisterEntity<ClassName> _editor_placement_registration_##ClassName = EntityPlacement::RegisterEntity<ClassName>{classId}; \
public: \
static void _editor_place(EntityPlacement::Context * context)

#define EDITOR_PLACE_DEFINITION(ClassName)\
void ClassName::_editor_place(EntityPlacement::Context * context)
