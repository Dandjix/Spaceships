#pragma once
#include "SpaceShipBlueprint.h"
#include "Entity.h"
class BlueprintEditorAppearance : public Entity
{
protected:
	SpaceShipBlueprint* blueprint;

	void renderTile(SDL_Renderer* renderer, const RenderingContext& context,Tile tile, int x, int y);

public:
	BlueprintEditorAppearance(SpaceShipBlueprint * blueprint);

	void update(const UpdateContext& context) override;


	void render(SDL_Renderer* renderer, const RenderingContext& context) override;

};