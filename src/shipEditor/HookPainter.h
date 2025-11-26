//
// Created by timon on 11/24/25.
//

#pragma once
#include <utility>

#include "entities/Entity.h"

class SpaceShipBlueprint;

namespace HookPainter{
    enum State {
        Off,
        Regions,
        Point
    };

    enum Precision {
        Center,
        Intersection,
        Free
    };

    class Painter : public Entity{
    private:
        std::optional<Vector2Int> start_position = std::nullopt;

        State state;
        Precision precision;
        std::function<std::string()> promptForName;



        void renderRegionHelper(SDL_Renderer *renderer, const RenderingContext &context);

        void renderPointHelper(SDL_Renderer *renderer, const RenderingContext &context);

    public:
        /**
         * Called when a region is placed, with arguments :
         * - region name
         * - region world TL
         * - region world dimensions
         */
        Event<std::string,Vector2Int,Vector2Int> on_region_placed;
        /**
         * Called when a point is placed, with arguments :
         * - point name
         * - point world position
         */
        Event<std::string,Vector2Int> on_point_placed;

        void setState(State state);
        State getState(){ return state;}

        void setPrecision(Precision precision);

        Vector2Int snapPosition(Vector2Int world_point);

        Precision getPrecision(){ return precision;}


        Painter(
            HookPainter::State state,
            HookPainter::Precision precision,
            std::function<std::string()> promptForName)
        :
        Entity({0,0},std::nullopt,nullptr),state(state),precision(precision), promptForName(std::move(promptForName))
        {}

        void render(SDL_Renderer *renderer, const RenderingContext &context) override;

        void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) override;

        void startPlacing(Vector2Int position);

        void cancelPlacement();

        void confirmPlacement(Vector2Int end_position);
    };

};



