//
// Created by timon on 11/27/25.
//

#pragma once


#pragma once

#include "../../../entities/scripts/Entity.h"
#include "../../../entities/scripts/ShortLivedEntity.h"
#include "shipEditor/AirlockPainter/PainterStates/PainterStates.h"
#include "spaceships/Hooks/airlock/Airlock.h"


namespace AirlockPainter {

    enum class Mode {
        Off,
        Airlock,
    };

    enum class Precision {
        Center,
        Intersection,
        Free
    };

    class Painter : public ShortLivedEntity {
    private:
        friend class PainterState;

        Vector2Int start_position{};
        bool has_start = false;
        Vector2Int end_position{};
        Mode mode;
        Precision precision;
        Airlock::Orientation orientation;

        std::unique_ptr<PainterState> current_state;

    public:
        std::function<void()> promptForName;

        Event<std::string,Vector2Int,Vector2Int,Airlock::Orientation> on_airlock_placed;
        Event<Airlock::Orientation> on_orientation_changed;

        Painter(Mode mode,
                Precision precision,
                Airlock::Orientation orientation)
        : ShortLivedEntity({0,0},std::nullopt),
          mode(mode),
          precision(precision),
          orientation(orientation)
        {}

        void changeState(std::unique_ptr<AirlockPainter::PainterState> next);

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

        void setOrientation(Airlock::Orientation value) {
            orientation = value;
            on_orientation_changed.emit(orientation);
        }
        Airlock::Orientation getOrientation() const {return orientation;}

        Entity * initializeRendering(const EntityRendering::Context &context) override {return this;}

        Entity * finalizeRendering(const EntityRendering::Context &context) override {return this;}
    };

} // namespace Air
