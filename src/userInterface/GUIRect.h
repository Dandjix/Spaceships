#pragma once

#include <optional>
#include <SDL3/SDL.h>

#include "GUI.h"
#include "../math/Vectors.h"
#include "../game/Update.h"

class GUIRect {
protected :
	int width; int height;
	Vector2Int offset;
	Anchor anchor;

	Vector2Int screenPosition;
	Vector2Int dimensions;

public:
	GUIRect(Anchor anchor, Vector2Int offset,int width, int height)
	{
		this->anchor = anchor;
		this->offset = offset;
		this->width = width;
		this->height = height;
	}

	virtual void render(SDL_Renderer* renderer, const GUI_RenderingContext& context) const = 0;

	virtual void update(const GUI_UpdateContext& context)
	{
		int h, w;
		if (width == GUI_Fill)
			w = context.screenDimensions.x;
		else
			w = width;

		if (height == GUI_Fill)
			h = context.screenDimensions.y;
		else
			h = height;


		Vector2Int topLeftCorner;
		switch (anchor)
		{
		case Anchor::TL:
			topLeftCorner = Vector2Int(0, 0);
			break;
		case Anchor::TR:
			topLeftCorner = Vector2Int(context.screenDimensions.x, 0);
			break;
		case Anchor::BR:
			topLeftCorner = Vector2Int(context.screenDimensions.x, context.screenDimensions.y);
			break;
		case Anchor::BL:
			topLeftCorner = Vector2Int(0, context.screenDimensions.y);
			break;
		}

		screenPosition = topLeftCorner + offset;
		dimensions = Vector2Int(w, h);
	}

	virtual void handleEvent(const SDL_Event & event){}
};