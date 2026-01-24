#pragma once
#include "../entities/scripts/Entity.h"
#include "../entities/scripts/ShortLivedEntity.h"

enum class Tile;
class SpaceShipBlueprint;

class BlueprintEditorAppearance : public ShortLivedEntity
{
protected:
	SpaceShipBlueprint* blueprint;

	void renderTile(SDL_Renderer* renderer, const RenderingContext& context,Tile tile, int x, int y);

public:
	explicit BlueprintEditorAppearance(SpaceShipBlueprint * blueprint);

	void update(const UpdateContext& context) override;


	void render(SDL_Renderer* renderer, const RenderingContext& context) override;

	QueueOrder::Value getQueueOrder() override {
		return QueueOrder::LAST;
	}
};
