#include "Painter.h"
#include "PainterStates.h"


using namespace AirlockPainter;



void Painter::changeState(std::unique_ptr<PainterState> next) {
    if (current_state) current_state->onExit(*this);
    current_state = std::move(next);
    if (current_state) current_state->onEnter(*this);
}

Vector2Int Painter::snapPosition(Vector2Int world_point) {

    Vector2Float world_point_pixels = Vectors::toVector2Float(world_point).scaleToScreenPosition() / Tiles::tileSizePx;
    Vector2Int adjusted_world_point;
    switch (precision) {
        case Precision::Center:
            adjusted_world_point = {
            static_cast<int>(std::floor(world_point_pixels.x)),
            static_cast<int>(std::floor(world_point_pixels.y))
        };
            adjusted_world_point =
                    adjusted_world_point.scaleToWorldPosition()
                    * Tiles::tileSizePx +
                    Vector2Int(Tiles::tileSizePx/2,Tiles::tileSizePx/2).scaleToWorldPosition();

            break;
        case Precision::Intersection:

            adjusted_world_point = {
            static_cast<int>(std::round(world_point_pixels.x)),
            static_cast<int>(std::round(world_point_pixels.y))
        };
            adjusted_world_point =
                    adjusted_world_point.scaleToWorldPosition()
                    * Tiles::tileSizePx;

            break;
        case Precision::Free:
            adjusted_world_point = world_point;
            break;
    }
    return adjusted_world_point;
}

void Painter::setMode(Mode newMode) {
    mode = newMode;

    switch (mode) {
        case Mode::Off:
            changeState(std::make_unique<StateOff>());
            break;
        case Mode::Airlock:
            changeState(std::make_unique<StateAirlockIdle>());
            break;
    }
}

void Painter::render(SDL_Renderer *r, const RenderingContext &ctx) {
    if (current_state)
        current_state->render(*this, r, ctx);
}

void Painter::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &ctx) {
    Entity::handleEvent(event,ctx);
    if (mode == Mode::Off || ctx.mouse_position_type != GameEvent::Game)
        return;

    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float mx,my; SDL_GetMouseState(&mx,&my);
        auto world = ctx.camera_info.screenToWorldPoint({mx,my});
        Vector2Int pos = snapPosition(world);

        if (current_state)
            current_state->handleClick(*this, pos);
    }
}

void Painter::confirmPlacement(const std::string &name) {

    // top-left calculation etc.
    Vector2Int start = getStart();
    Vector2Int TL{ std::min(start.x,end_position.x), std::min(start.y,end_position.y) };
    Vector2Int dim = {std::abs(start.x - end_position.x),std::abs(start.y - end_position.y) };

    on_airlock_placed.emit(name, TL, dim,orientation);

    clearStart();
    setMode(mode);  // restart state (idle again)
}

void Painter::setPrecision(Precision value) {
    precision = value;
}
