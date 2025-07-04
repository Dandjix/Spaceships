#pragma once
#include <SDL3/SDL.h>

#include "../entities/Entity.h"
#include "../math/Vectors.h"
#include "../game/Update.h"
#include "../spaceships/SpaceShipBlueprint.h"
#include "../player/Camera.h"
#include "../shipEditor/ShipBuildingGrid.h"
#include "../spaceships/Tile.h"
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
