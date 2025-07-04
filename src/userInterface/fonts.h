#pragma once
#include <string>
#include <unordered_map>
#include <SDL3_ttf/SDL_ttf.h>

// Declare (but don't define)
extern std::unordered_map<std::string, TTF_Font*> fonts;

bool LoadFonts();
void UnloadFonts();
