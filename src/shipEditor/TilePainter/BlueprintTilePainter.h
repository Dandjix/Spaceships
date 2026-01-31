#pragma once
#include <SDL3/SDL.h>

#include "../../entities/scripts/Entity.h"
#include "../../math/Vectors.h"
#include "../../game/Update.h"
#include "../../spaceships/SpaceShipBlueprint.h"
#include "../../player/Camera.h"
#include "../ShipBuildingGrid.h"
#include "../../spaceships/Tile.h"

class BlueprintTilePainter : public ShortLivedEntity {
protected :
    bool enabled = true;
    SpaceShipBlueprint *blueprint;
    ShipBuildingGrid *grid;
    Tile tileToPaint;

public:
    /**
     * When set to true, the painter fills an area of contiguous same tiles instead of changing just the one
     */
    bool fill;

    BlueprintTilePainter(SpaceShipBlueprint *blueprint, ShipBuildingGrid *grid, Tile tileToPaint, bool fill)
        : ShortLivedEntity(Vector2Int(0, 0), std::nullopt),
          blueprint(blueprint), grid(grid), tileToPaint(tileToPaint), fill(fill) {
    }

    void setTileToPaint(Tile tileToPaint);

    bool getEnabled();

    void setEnabled(bool enabled);

    void paintSingleTile(Vector2Int coords);

    void paintFill(Vector2Int coords);

    void update(const UpdateContext &context) override;

    void render(SDL_Renderer *renderer, const RenderingContext &context) override;

    void setBlueprint(SpaceShipBlueprint *blueprint);

    BlueprintTilePainter * initializeRendering(const EntityRendering::Context &context) override {return this;}

    BlueprintTilePainter * finalizeRendering(const EntityRendering::Context &context) override {return this;}
};
