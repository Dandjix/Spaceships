#include "RayCaster.h"
#include "Camera.h"
#include "Physics.h"
#include "Vectors.h"
void RayCaster::update(const UpdateContext& context)
{
	const bool* state = SDL_GetKeyboardState(NULL);

	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);


	Vector2Int mousePosition = camera->screenToWorldPoint(Vector2Float(mouseX,mouseY));
	Vector2Float direction = Vectors::toVector2Float(mousePosition - player->getPosition());

	direction.normalize();


	if (state[SDL_SCANCODE_SPACE])
	{
		auto hit = Physics::RayCast(player->getPosition(), direction, context.spaceShip);
		if (hit.has_value())
		{
			SDL_Log("hit !");
		}
		else
		{
			SDL_Log("no hit :(");
		}
		hitPosition = hit;
	}
	hitPosition = std::nullopt;
}

void RayCaster::render(SDL_Renderer* renderer, const RenderingContext& context)
{
	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	Vector2Float mouseScreenPos = Vector2Float(mouseX, mouseY);
	Vector2Int mousePos = context.toWorldPosition(mouseScreenPos);

	Vector2Int playerPos = player->getPosition();
	Vector2Float playerScreenPos = context.toScreenPoint(playerPos);

	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 0);
	DebugRendering::drawCross(renderer, Vectors::toVector2Int(context.toScreenPoint(mousePos)));

	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
	SDL_RenderLine(renderer, mouseScreenPos.x, mouseScreenPos.y, playerScreenPos.x, playerScreenPos.y);


	SDL_SetRenderDrawColor(renderer, 0, 255, 255,0);
	if (hitPosition.has_value())
	{
		DebugRendering::drawCross(renderer, Vectors::toVector2Int(context.toScreenPoint(hitPosition.value())));
	}
}