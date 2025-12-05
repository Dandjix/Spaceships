#pragma once

#include <optional>
#include <SDL3/SDL.h>

#include "GUI.h"
#include "../math/Vectors.h"
#include "../entities/scripts/Entity.h"

class GUIRect {
protected :
	int width; int height;
	Vector2Int offset;
	Anchor anchor;
public:
	[[nodiscard]] int getWidth() const {return width;}
	[[nodiscard]] int getHeight() const {return height;}
	[[nodiscard]] Vector2Int getOffset() const {return offset;}
	void setOffset(Vector2Int value) {offset = value;}
	[[nodiscard]] Anchor getAnchor() const {return anchor;}
protected:
	/**
	 * position of the top left corner of the element
	 */
	Vector2Int screenPosition;

	/**
	 * size of the element
	 */
	Vector2Int dimensions;

public:
	virtual ~GUIRect() = default;

	GUIRect(Anchor anchor, Vector2Int offset,int width, int height)
	{
		this->anchor = anchor;
		this->offset = offset;
		this->width = width;
		this->height = height;
	}

	virtual bool is_inside(Vector2Float position);

	virtual void render(SDL_Renderer* renderer, const GUI_RenderingContext& context) const = 0;

	virtual void update(const GUI_UpdateContext& context);

	virtual void handleEvent(const SDL_Event & event, const GameEvent::GameEventContext & context){}

	virtual QueueOrder::Value getQueueOrder() {
		return QueueOrder::MIDDLE;
	}
};


