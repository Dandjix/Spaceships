#include "RayCaster.h"
#include "../player/Camera.h"
#include "../physics/Physics.h"
#include "../math/Vectors.h"
void RayCaster::update(const UpdateContext& context)
{
	const bool* state = SDL_GetKeyboardState(nullptr);

	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);


	Vector2Int mousePosition = camera->screenToWorldPoint(Vector2Float(mouseX,mouseY));
	Vector2Float direction = Vectors::toVector2Float(mousePosition - player->getPosition());

	direction.normalize();


	if (state[SDL_SCANCODE_SPACE])
	{
		auto hit = Physics::RayCast(player->getPosition(), direction, context.spaceShip);
		// if (hit.has_value())
		// {
		// 	// SDL_Log("hit at %d %d",hit.value().x,hit.value().y);
		// }
		// else
		// {
		// 	// SDL_Log("no hit :(");
		// }
		originPosition = player->getPosition();
		hitPosition = hit;
		//SDL_Log("hit position set. has value ? : %b", hitPosition.has_value());
	}
	// else
	// {
	// 	hitPosition = std::nullopt;
	// }
}

void RayCaster::render(SDL_Renderer* renderer, const RenderingContext& context)
{
	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	Vector2Float mouseScreenPos = Vector2Float(mouseX, mouseY);
	Vector2Int mousePos = context.toWorldPosition(mouseScreenPos);

	Vector2Int playerPos = player->getPosition();
	Vector2Float playerScreenPos = context.toScreenPoint(playerPos);

	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	DebugRendering::drawCross(renderer, Vectors::toVector2Int(context.toScreenPoint(mousePos)));

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	DebugRendering::drawCross(renderer, Vectors::toVector2Int(context.toScreenPoint(playerPos + Vector2Int(1200,1200))));
	DebugRendering::drawCross(renderer, Vectors::toVector2Int(context.toScreenPoint(playerPos - Vector2Int(1200,1200))));

	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_RenderLine(renderer, mouseScreenPos.x, mouseScreenPos.y, playerScreenPos.x, playerScreenPos.y);


	if (originPosition.has_value() && hitPosition.has_value())
	{
		Vector2Float hitPoint = context.toScreenPoint(originPosition.value());
		Vector2Float originPoint = context.toScreenPoint(hitPosition.value());
		SDL_SetRenderDrawColorFloat(renderer,0,1,0,1.0f);
		SDL_RenderLine(renderer,originPoint.x,originPoint.y,hitPoint.x,hitPoint.y);
	}
	if (originPosition.has_value())
	{
		Vector2Float originHit = context.toScreenPoint(originPosition.value());
		SDL_SetRenderDrawColorFloat(renderer,0,0,1,1.0f);
		DebugRendering::drawCross(renderer, Vectors::toVector2Int(originHit),15);
	}
	if (hitPosition.has_value())
	{
		Vector2Float screenHit = context.toScreenPoint(hitPosition.value());
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float size = 15.0f + static_cast <float> (rand()) / static_cast <float> (RAND_MAX)*10.0f;
		SDL_SetRenderDrawColorFloat(renderer,r,g,b,1.0f);
		DebugRendering::drawCross(renderer, Vectors::toVector2Int(screenHit),size);
	}

}