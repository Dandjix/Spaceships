#pragma once
#include "Entity.h"
#include "Camera.h"
#include <SDL3/SDL.h>

class Cursor : public Entity {

protected :
	Camera* camera;

public :
	Cursor(Camera* camera) : Entity(Vector2Int(0,0),0.0f),camera(camera) {}

	void update(const UpdateContext& context) override {
		float mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		Vector2Int worldCursorPosition = camera->screenToWorldPoint(Vector2Float(mouseX, mouseY));

		setPosition(worldCursorPosition);
	}

	void render(SDL_Renderer* renderer, const RenderingContext& context) override {
		Vector2Float screenPos = context.toScreenPoint(getPosition());
		SDL_FRect rect = {
			screenPos.x,
			screenPos.y,
			50,
			50
		};
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &rect);
	}
};