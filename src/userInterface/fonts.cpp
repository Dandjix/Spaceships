#include "fonts.h"

// Define the actual storage here
std::unordered_map<std::string, TTF_Font*> fonts;

bool LoadFonts() {
    fonts["sm"] = TTF_OpenFont(ENV_PROJECT_ROOT"assets/fonts/square-deal/square-deal.ttf", 16);
    fonts["md"] = TTF_OpenFont(ENV_PROJECT_ROOT"assets/fonts/square-deal/square-deal.ttf", 24);
    fonts["lg"] = TTF_OpenFont(ENV_PROJECT_ROOT"assets/fonts/square-deal/square-deal.ttf", 32);

    for (auto& [key, font] : fonts) {
        if (!font) {
            SDL_Log("Failed to load font: %s", key.c_str());
            return false;
        }
    }

    return true;
}

void UnloadFonts() {
    for (auto& [_, font] : fonts)
        TTF_CloseFont(font);
    fonts.clear();
}
