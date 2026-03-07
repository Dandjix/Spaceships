#pragma once

#include <SDL3/SDL.h>
#include "game/CameraTransformations.h"
#include "game/ElementContainer.h"


class GUIRect;

enum class Anchor
{
	TL,
	TR,
	BR,
	BL,
	Center,
	BottomCenter,
	TopCenter,
	CenterLeft,
	CenterRight
};

enum Fill
{
	GUI_Fill = -1
};

struct GUI_RenderingContext
{
	CameraTransformations::CameraInfo camera_info;
};

struct GUI_UpdateContext
{
	CameraTransformations::CameraInfo camera_info;
	const float deltaTime;
	ElementContainer<GUIRect *> * gui_elements;
	SDL_Window * window;
	GUIRect * element_under_mouse;
};