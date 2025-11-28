#pragma once
#include "../entities/Entity.h"
#include "entities/ShortLivedEntity.h"

class BlueprintEditorAppearance : public ShortLivedEntity
{
protected:
	SpaceShipBlueprint* blueprint;

	void renderTile(SDL_Renderer* renderer, const RenderingContext& context,Tile tile, int x, int y);

public:
	explicit BlueprintEditorAppearance(SpaceShipBlueprint * blueprint);

	void update(const UpdateContext& context) override;


	void render(SDL_Renderer* renderer, const RenderingContext& context) override;

};
