#pragma once
#include "GUI.h"
#include "Vectors.h"
#include <optional>
#include <SDL3/SDL.h>
class GUIRect {
protected :
	int width; int height;
	Vector2Int offset;
	Anchor anchor;

public:
	GUIRect(Anchor anchor, Vector2Int offset,int width, int height)
	{
		this->anchor = anchor;
		this->offset = offset;
		this->width = width;
		this->height = height;
	}

	virtual void renderTexture(SDL_Renderer* renderer, Vector2Int TLCorner, Vector2Int dimensions) const = 0;

	void render(SDL_Renderer* renderer, const GUI_RenderingContext& context) const
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

		Vector2Int dimensions(w, h);

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

		Vector2Int screenPosition = topLeftCorner + offset;



		renderTexture(renderer, screenPosition, dimensions);
	}
};