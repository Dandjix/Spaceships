//
// Created by timon on 11/23/25.
//

#pragma once


#include "MousePositionType.h"
#include "game/CameraTransformations.h"


namespace GameEvent {
    struct GameEventContext {
        const CameraTransformations::CameraInfo camera_info;
        MousePositionType mouse_position_type = Game;
    };
}
