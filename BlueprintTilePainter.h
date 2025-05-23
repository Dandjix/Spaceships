#pragma once
#include "Entity.h"
#include "Vectors.h"
#include "Update.h"
#include <SDL3/SDL.h>
#include "SpaceShipBlueprint.h"
#include "Camera.h"
#include "ShipBuildingGrid.h"
#include "Tile.h"
class BlueprintTilePainter : public Entity {
protected :
	bool enabled = true;
	SpaceShipBlueprint * blueprint;
	ShipBuildingGrid* grid;
	Tile tileToPaint;
public:

	BlueprintTilePainter(SpaceShipBlueprint* blueprint, ShipBuildingGrid* grid, Tile tileToPaint);

	void setTileToPaint(Tile tileToPaint);

	bool getEnabled();

	void setEnabled(bool enabled);

	void update(const UpdateContext& context) override;

	void render(SDL_Renderer* renderer, const RenderingContext& context) override;

	void setBlueprint(SpaceShipBlueprint* blueprint);
};
