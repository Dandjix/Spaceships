#pragma once
#include "../entities/scripts/Entity.h"
#include "../game/Rendering.h"
#include <SDL3/SDL.h>
#include "../player/Camera.h"
#include <functional>

#include "../entities/scripts/ShortLivedEntity.h"

class ShipBuildingGrid : public ShortLivedEntity
{
private:
	void drawLines(Vector2Int dimensions, SDL_Renderer* renderer, const RenderingContext& context) const;

	std::function<void(Vector2Int dimensions)> onResize;

protected :
	int sizePx;
	Vector2Int dimensions;
	bool resizing = false;
	Camera * camera;



	void renderFixed(SDL_Renderer* renderer, const RenderingContext& context);

	void renderFluid(SDL_Renderer* renderer, const RenderingContext& context);

public:
	ShipBuildingGrid(Vector2Int dimensions, Camera * camera, const std::function<void(Vector2Int dimensions)> &onResize);

	void render(SDL_Renderer* renderer, const RenderingContext& context) override;

	int getSizePx() const;

	Vector2Int getMouseCoordinates(const CameraTransformations::CameraInfo & camera_info,bool strict = true) const;

	void startResizing();

	Camera* getCamera();

	void setDimensions(Vector2Int dimensions);

	void handleEvent(const SDL_Event & event, const GameEvent::GameEventContext &context) override;

	void update(const UpdateContext & context) override;
};
