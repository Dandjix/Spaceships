//
// Created by timon on 1/24/26.
//

#pragma once
#include "json.hpp"

struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

    Color( unsigned char r, unsigned char g, unsigned char b, unsigned char a):r(r),g(g),b(b),a(a) {

    }

    static Color fromJson(const nlohmann::json & json) {
        return Color(json["r"],json["g"],json["b"],json["a"]);
    }

    nlohmann::json toJson() {
        nlohmann::json json = {};
        json["r"] = r;
        json["g"] = g;
        json["b"] = b;
        json["a"] = a;
        return json;
    }
};
