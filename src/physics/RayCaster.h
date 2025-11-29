#pragma once
#include "../entities/Entity.h"
#include "../player/Camera.h"
#include "../math/Vectors.h"
#include "entities/ShortLivedEntity.h"

class RayCaster : public ShortLivedEntity
{
protected:
	Camera* camera;
	Entity* player;
	std::optional<Vector2Int> hitPosition;
	std::optional<Vector2Int> originPosition;
	std::vector<Vector2Int> checked_positions;

public:
	RayCaster(Camera* camera, Entity * player) : ShortLivedEntity(Vector2Int(0, 0), 0.0f), camera(camera), player(player) {}

	// Inherited via Entity
	void update(const UpdateContext& context) override;
	void render(SDL_Renderer* renderer, const RenderingContext& context) override;
};
