#pragma once
#include "../entities/Entity.h"
#include "../game/Rendering.h"
#include <SDL3/SDL.h>
#include "../player/Camera.h"
#include <functional>

class ShipBuildingGrid : public Entity
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
	ShipBuildingGrid(Vector2Int dimensions, Camera * camera, std::function<void(Vector2Int dimensions)> onResize);

	void render(SDL_Renderer* renderer, const RenderingContext& context) override;

	int getSizePx() const;

	Vector2Int getMouseCoordinates(bool strict = true) const;

	void startResizing();

	Camera* getCamera();

	void setDimensions(Vector2Int dimensions);

	void handleEvent(const SDL_Event & event) override;

	void update(const UpdateContext & context) override;
};