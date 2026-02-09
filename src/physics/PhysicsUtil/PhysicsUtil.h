#pragma once
#include <vector>

#include "math/Vectors.h"
//
// Created by timon on 2/9/26.
//


namespace Physics {
    namespace Util {
        std::vector<std::pair<Vector2Int, Vector2Int> > getDiagonals(Vector2Int center,
                                                                     const std::vector<Vector2Int> &polygon);

        std::vector<std::pair<Vector2Int, Vector2Int> > getSides(std::vector<Vector2Int> polygon);


        struct PolygonInfo {
            // Vector2Int center;
            std::vector<Vector2Int> vertices;

            [[nodiscard]] Vector2Int getCenter() const {
                Vector2<long> summ;
                for (auto vertex: vertices) {
                    summ += {vertex.x, vertex.y};
                }
                summ /= vertices.size();
                return {static_cast<int>(summ.x), static_cast<int>(summ.y)};
            }

            PolygonInfo(Vector2Int center, const std::vector<Vector2Int> &vertices) : vertices(vertices) {
            }
        };

        struct CircleInfo {
            Vector2Int center;
            int radius;

            [[nodiscard]] Vector2Int getCenter() const {
                return center;
            }

            CircleInfo(Vector2Int center, float radius) : center(center), radius(radius) {
            }
        };


        bool circleConvexAreColliding(const PolygonInfo &convex, const CircleInfo &round);


        //https://www.youtube.com/watch?v=7Ik2vowGcU0&t=1320s
        bool areCollidingSAT(const PolygonInfo *poly1, const PolygonInfo *poly2);


        bool areCollidingDiagonals(const PolygonInfo *poly1, const PolygonInfo *poly2);

        struct SATReturn {
            float overlap;
            bool are_colliding;
        };


        SATReturn SeparatedAxisTheorem(PolygonInfo *poly1, PolygonInfo *poly2);
    }
};
