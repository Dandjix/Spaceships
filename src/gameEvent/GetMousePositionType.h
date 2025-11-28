//
// Created by timon on 11/24/25.
//

#pragma once
#include <vector>

#include "MousePositionType.h"
#include "math/Vectors.h"


class GUIRect;

namespace GameEvent{
    MousePositionType getMousePositionType(const std::vector<GUIRect *> &elements, Vector2Float mouse_position);
}




