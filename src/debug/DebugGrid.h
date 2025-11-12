#include "entities/Entity.h"

class DebugGrid : public Entity
{
private:
    Vector2Int origin;
    int resolution;

public:
    DebugGrid(int xOrigin, int yOrigin, int resolution);
    ~DebugGrid() override;

    void render(SDL_Renderer* renderer, const RenderingContext& context) override {}

    void debugRender(SDL_Renderer* renderer, const RenderingContext& context) override;
};
