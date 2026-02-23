#pragma once
#include <string>
#include <unordered_map>

#include "math/Vectors.h"
//
// Created by timon on 2/21/26.
//

namespace EntityPlacement::InterfaceForm {

    enum SupportedType{
        INT,
        FLOAT,
        VECTOR2INT,
        VECTOR2FLOAT,
        STRING
    };

    inline const std::unordered_map<SupportedType, float> heights = {
        {INT,64.0f},
        {FLOAT,64.0f},
        {VECTOR2INT,64.0f},
        {VECTOR2FLOAT,64.0f},
        {STRING,64.0f},
    };

    struct FormRequest {
    protected:
        std::vector<std::pair<std::string,SupportedType>> query;

    public:
        using iterator = std::vector<std::pair<std::string,SupportedType>>::iterator;
        iterator begin(){return query.begin();}
        iterator end(){return query.end();}

        float computeHeight() {
            float height = 0;
            for (auto type: query | std::views::values) {
                if (!heights.contains(type))
                    height += 64.0f;
                else
                    height+= heights.at(type);
            }
            return height;
        }
    };

    struct FormResult {
        std::unordered_map<std::string,int> results_int;
        std::unordered_map<std::string,float> results_float;
        std::unordered_map<std::string,Vector2Int> results_Vector2Int;
        std::unordered_map<std::string,Vector2Float> results_Vector2Float;
        std::unordered_map<std::string,std::string> results_string;
    };
}
