//
// Created by timon on 1/23/26.
//

#pragma once
#include <filesystem>
#include <iostream>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "UsageMap.h"


namespace Textures {
    class UsageMap;

    struct TextureSet {
    private:
        static bool hasEnding(std::string const &fullString, std::string const &ending) {
            if (fullString.length() >= ending.length()) {
                return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
            } else {
                return false;
            }
        }

    protected:
        static std::unordered_map<std::string, SDL_Texture *> load(SDL_Renderer *renderer, const std::filesystem::path &path) {

            std::unordered_map<std::string, SDL_Texture *> textures = {};

            std::pmr::vector<std::string> accepted_texture_endings = {
                ".png"
            };

            for (const auto &entry: std::filesystem::directory_iterator(path)) {
                for (const auto &ending: accepted_texture_endings) {
                    auto filename = entry.path().filename().string();
                    if (entry.is_regular_file() && hasEnding(filename, ending)) {
                        const std::string & key = filename.substr(0,filename.size()-ending.size());
                        textures[key] = IMG_LoadTexture(renderer,entry.path().c_str());
                        break;
                    }
                }
            }
            return textures;
        }
    public:
        unsigned int user_count;
        std::unordered_map<std::string, SDL_Texture *> textures;

        /**
         * @param texture_key the name of the .png file without the .png extension (ie "door" for "door.png"
         * @return a pointer to the texture
         */
        virtual SDL_Texture * at(const std::string & texture_key) {
            return textures.at(texture_key);
        }

        TextureSet(SDL_Renderer * renderer, const std::filesystem::path &path,unsigned int user_count = 1)
            : user_count(user_count),
              textures(std::move(load(renderer, path)))
        {
        }

        virtual ~TextureSet() {
            for (const auto texture: textures | std::views::values) {
                SDL_DestroyTexture(texture);
            }
        }
    };
}
