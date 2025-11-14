#pragma once
#include "../entities/Entity.h"
#include "../player/Camera.h"
#include "../math/Vectors.h"
class RayCaster : public Entity
{
protected:
	Camera* camera;
	Entity* player;
	std::optional<Vector2Int> hitPosition;
	std::optional<Vector2Int> originPosition;

public:
	RayCaster(Camera* camera, Entity * player) : Entity(Vector2Int(0, 0), 0.0f), camera(camera), player(player) {}

	// Inherited via Entity
	void update(const UpdateContext& context) override;
	void render(SDL_Renderer* renderer, const RenderingContext& context) override;

};