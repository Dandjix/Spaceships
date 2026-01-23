//
// Created by timon on 1/23/26.
//

#include "UsageMap.h"

#include <iostream>

#include "TextureSet.h"


Textures::UsageMap &Textures::UsageMap::getInstance() {
    static UsageMap instance;
    return instance;
}

void Textures::UsageMap::initialize(const std::filesystem::path & textures_directory,SDL_Renderer * renderer) {
    auto& instance = getInstance();
    instance.textures_directory = textures_directory;
    instance.renderer = renderer;
}

Textures::TextureSet * Textures::UsageMap::subscribe(const std::string &key) {
    if (texture_sets.contains(key)) {
        auto set = texture_sets.at(key);
        set->user_count++;
        return set;
    }

    auto set = new TextureSet(renderer,textures_directory / key);
    texture_sets.insert({key,set});

    return set;
}

void Textures::UsageMap::unsubscribe(const std::string &key) {
    if (!texture_sets.contains(key)) {
        throw std::runtime_error("Unsubscribed a zero user texture set : "+key);
    }

    auto set = texture_sets.at(key);
    set->user_count--;

    if (set->user_count <= 0) {
        delete set;
        texture_sets.erase(key);
    }
}