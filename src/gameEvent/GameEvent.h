//
// Created by timon on 11/23/25.
//

#pragma once


#include <SDL3/SDL_video.h>

#include "MousePositionType.h"
#include "game/CameraTransformations.h"


class GUIRect;
class SpaceShip;

namespace GameEvent {
    struct GameEventContext {
        const CameraTransformations::CameraInfo camera_info;
	    SpaceShip* spaceShip;
        MousePositionType mouse_position_type = Game;
        GUIRect * element_under_mouse;
        SDL_Window * window;
    };
}
