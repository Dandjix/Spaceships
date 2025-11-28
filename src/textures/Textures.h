//
// Created by timon on 11/28/25.
//

#pragma once
#include <SDL3/SDL_render.h>

#include "json.hpp"


namespace Textures {

    struct  SavableTexture {
        SDL_Texture * texture;
        std::filesystem::path path_to_texture;
    };
};