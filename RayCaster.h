#pragma once
#include "Entity.h"
#include "Camera.h"
#include "Vectors.h"
class RayCaster : public Entity
{
protected:
	Camera* camera;
	Entity* player;
	std::optional<Vector2Int> hitPosition;

public:
	RayCaster(Camera* camera, Entity * player) : Entity(Vector2Int(0, 0), 0.0f), camera(camera), player(player) {}

	// Inherited via Entity
	void update(const UpdateContext& context) override;
	void render(SDL_Renderer* renderer, const RenderingContext& context) override;

};