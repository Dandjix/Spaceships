//
// Created by timon on 1/18/26.
//

#pragma once
#include "entities/scripts/ShortLivedEntity.h"
#include "PainterStates/Painter.h"

namespace AirlockPainter {
    class OrientationChanger : public ShortLivedEntity {
    private:
        AirlockPainter::Painter *painter;

    public:
        OrientationChanger(AirlockPainter::Painter *painter_object)
            : ShortLivedEntity({0, 0}, 0), painter(painter_object) {
        }

        void render(SDL_Renderer *renderer, const RenderingContext &context) override {
        }

        void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) override;

        OrientationChanger * initializeRendering(const EntityRendering::Context &context) override {return this;}

        OrientationChanger * finalizeRendering(const EntityRendering::Context &context) override {return this;}
    };
}
