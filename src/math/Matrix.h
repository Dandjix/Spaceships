//
// Created by timon on 12/11/25.
//

#pragma once
#include "Vectors.h"

namespace Matrix {

    struct Mat3 {
        float m[3][3];

        static Mat3 identity() {
            return { {
                {1,0,0},
                {0,1,0},
                {0,0,1}
            }};
        }

        static Mat3 translation(float x, float y) {
            return { {
                {1,0,x},
                {0,1,y},
                {0,0,1}
            }};
        }


        /**
         * Rotates the matrix
         * @param angle The angle in degrees
         * @return
         */
        static Mat3 rotation(float angle) {
            float c = std::cos(angle * static_cast<float>(std::numbers::pi)/180);
            float s = std::sin(angle * static_cast<float>(std::numbers::pi)/180);
            return { {
                {c,-s,0},
                {s, c,0},
                {0, 0,1}
            }};
        }

        static Mat3 scale(float s) {
            return { {
                {s, 0,0},
                {0, s,0},
                {0, 0,1}
            }};
        }

        Mat3 operator*(const Mat3& b) const {
            Mat3 r{};
            for(int i=0;i<3;i++)
                for(int j=0;j<3;j++)
                    r.m[i][j] = m[i][0]*b.m[0][j] + m[i][1]*b.m[1][j] + m[i][2]*b.m[2][j];
            return r;
        }

        Vector2Float operator*(Vector2Float v) const {
            float x = m[0][0]*v.x + m[0][1]*v.y + m[0][2];
            float y = m[1][0]*v.x + m[1][1]*v.y + m[1][2];
            return {x,y};
        }
    };

}
