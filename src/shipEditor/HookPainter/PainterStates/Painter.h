//
// Created by timon on 11/27/25.
//

#pragma once


#pragma once

#include "entities/Entity.h"
#include "shipEditor/HookPainter/PainterStates/PainterStates.h"


namespace HookPainter {

    enum class Mode {
        Off,
        Regions,
        Point
    };

    enum class Precision {
        Center,
        Intersection,
        Free
    };

    class Painter : public Entity {
    private:
        friend class PainterState;

        Vector2Int start_position{};
        bool has_start = false;
        Vector2Int end_position{};
        Mode mode;
        Precision precision;

        std::unique_ptr<PainterState> current_state;

    public:
        std::function<void()> promptForName;

        Event<std::string,Vector2Int,Vector2Int> on_region_placed;
        Event<std::string,Vector2Int> on_point_placed;

        Painter(Mode mode,
                Precision precision)
        : Entity({0,0},std::nullopt,nullptr),
          mode(mode),
          precision(precision),
          promptForName(std::move(promptForName))
        {}

        // state control
        void changeState(std::unique_ptr<PainterState> next);

        // API exposed for states
        void setStart(Vector2Int pos) { start_position = pos; has_start = true; }
        void clearStart() { has_start = false; }
        bool hasStart() const { return has_start; }
        Vector2Int getStart() const { return start_position; }
        void setEndPosition(Vector2Int dim) { end_position = dim; }

        Vector2Int snapPosition(Vector2Int world_point);

        void setMode(Mode newMode);
        Mode getMode() const {return mode;}

        Precision getPrecision() const { return precision; }

        void render(SDL_Renderer *renderer,const RenderingContext &ctx) override;
        void handleEvent(const SDL_Event &event,const GameEvent::GameEventContext &ctx) override;

        void confirmPlacement(const std::string& name);

        void setPrecision(Precision value);
    };

} // namespace HookPainter
