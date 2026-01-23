//
// Created by timon on 1/23/26.
//

#pragma once
#include <filesystem>
#include <string>
#include <unordered_map>
#include <SDL3/SDL_render.h>

namespace Textures {
    struct TextureSet;

    class UsageMap {
    public:

        static Textures::UsageMap &getInstance();

        /**
         * This could technically get called multiple times, tho I do not recommend it.
         * @param textures_directory
         * @param renderer
         */
        static void initialize(const std::filesystem::path &textures_directory, SDL_Renderer *renderer);

        std::unordered_map<std::string,TextureSet *> texture_sets;
        std::filesystem::path textures_directory;
        SDL_Renderer * renderer;
        /**
         * @param key this is both the key for the textures as well as the name of the directory they are in
         * @return the loaded texture set
         */
        TextureSet * subscribe(const std::string & key);
        /**
         * @param key key (also the name of the directory) of the textures to unload if there are no more users
         */
        void unsubscribe(const std::string & key);

        UsageMap() = default;

        UsageMap(const std::filesystem::path & textures_directory,SDL_Renderer * renderer)
        :
        textures_directory(textures_directory),renderer(renderer)
        {}
    };
}

