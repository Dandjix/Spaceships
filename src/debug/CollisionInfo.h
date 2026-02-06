#pragma once
#include "entities/scripts/ShortLivedEntity.h"
//
// Created by timon on 2/6/26.
//
namespace Debug {
    class CollisionInfo : public ShortLivedEntity {
    private:
        std::vector<std::pair<Vector2Int, Vector2Int> > next_lines;
        std::vector<Vector2Int> next_points;

    public:
        static CollisionInfo *instance;

        std::vector<std::pair<Vector2Int, Vector2Int> > lines;
        std::vector<Vector2Int> points;

        CollisionInfo() : ShortLivedEntity({0, 0}, 0) {
            instance = this;
        }

        void debugRender(SDL_Renderer *renderer, const RenderingContext &context) override;

        CollisionInfo *initializeRendering(const EntityRendering::Context &context) override { return this; }

        CollisionInfo *finalizeRendering(const EntityRendering::Context &context) override { return this; }

        void addLine(Vector2Int start, Vector2Int end);

        void addPoint(Vector2Int point);

        void addLines(const std::vector<std::pair<Vector2Int, Vector2Int> > &param_lines);

        void addPoints(const std::vector<Vector2Int> &param_points);

        void update(const UpdateContext &context) override;

        QueueOrder::Value getQueueOrder() override {
            return QueueOrder::LAST;
        }

        void render(SDL_Renderer *renderer, const RenderingContext &context) override {}
    };
}
