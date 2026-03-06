#pragma once
#include <string>
#include <unordered_map>

#include "math/Vectors.h"
//
// Created by timon on 2/21/26.
//

namespace EntityPlacement::InterfaceForm {
    enum SupportedType {
        INT,
        FLOAT,
        VECTOR2INT,
        VECTOR2FLOAT,
        STRING
    };

    struct FormRequest {
    public:
        std::string entity_name; //used for the send button mostly
        std::vector<std::pair<std::string, SupportedType> > query;
        std::vector<std::pair<std::string, std::vector<std::string> > > choices;

        FormRequest() = default;

        FormRequest(
            std::string entity_name,
            const std::vector<std::pair<std::string, SupportedType> > &query,
            const std::vector<std::pair<std::string, std::vector<std::string> > > &choices)
            : entity_name(std::move(entity_name)), query(query), choices(choices) {
        }
    };

    struct FormResult {
        std::unordered_map<std::string, int> results_int;
        std::unordered_map<std::string, float> results_float;
        std::unordered_map<std::string, Vector2Int> results_Vector2Int;
        std::unordered_map<std::string, Vector2Float> results_Vector2Float;
        std::unordered_map<std::string, std::string> results_string;
    };
}
