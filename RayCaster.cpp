#include "RayCaster.h"
#include "Camera.h"
#include "Physics.h"
#include "Vectors.h"
void RayCaster::update(const UpdateContext& context)
{
	const bool* state = SDL_GetKeyboardState(NULL);

	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);


	Vector2Int mousePosition = camera->screenToWorldPointPerfectInverse(Vector2Float(mouseX,mouseY));
	Vector2Float direction = Vectors::toVector2Float(mousePosition - player->getPosition());

	direction.normalize();


	if (state[SDL_SCANCODE_SPACE])
	{
		auto hit = Physics::RayCast(player->getPosition(), direction, context.spaceShip);
		//if (hit.has_value())
		//{
		//	SDL_Log("hit at %d %d",hit.value().x,hit.value().y);
		//}
		//else
		//{
		//	SDL_Log("no hit :(");
		//}
		hitPosition = hit;
		//SDL_Log("hit position set. has value ? : %b", hitPosition.has_value());
	}
	else
	{
		hitPosition = std::nullopt;
	}
}

void RayCaster::render(SDL_Renderer* renderer, const RenderingContext& context)
{
	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	Vector2Float mouseScreenPos = Vector2Float(mouseX, mouseY);
	Vector2Int mousePos = context.toWorldPositionPerfectInverse(mouseScreenPos);

	Vector2Int playerPos = player->getPosition();
	Vector2Float playerScreenPos = context.toScreenPoint(playerPos);

	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	DebugRendering::drawCross(renderer, Vectors::toVector2Int(context.toScreenPoint(mousePos)));

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	DebugRendering::drawCross(renderer, Vectors::toVector2Int(context.toScreenPoint(playerPos + Vector2Int(1200,1200))));
	DebugRendering::drawCross(renderer, Vectors::toVector2Int(context.toScreenPoint(playerPos - Vector2Int(1200,1200))));

	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_RenderLine(renderer, mouseScreenPos.x, mouseScreenPos.y, playerScreenPos.x, playerScreenPos.y);


	SDL_SetRenderDrawColor(renderer, 0, 255, 255,255);
	if (hitPosition.has_value())
	{
		Vector2Float screenHit = context.toScreenPoint(hitPosition.value());
		SDL_Log("screen hit at %f %f", screenHit.x, screenHit.y);
		DebugRendering::drawCross(renderer, Vectors::toVector2Int(screenHit));
	}
}