#include "entities/Entity.h"

class DebugGrid : public Entity
{
private:
    Vector2Int origin;
    int step;

    Vector2Int snap(Vector2Int worldPosition, bool ceil = false) const;

public:
    DebugGrid(int xOrigin, int yOrigin, int step);
    ~DebugGrid() override;

    void render(SDL_Renderer* renderer, const RenderingContext& context) override {}

    void debugRender(SDL_Renderer* renderer, const RenderingContext& context) override;
};
