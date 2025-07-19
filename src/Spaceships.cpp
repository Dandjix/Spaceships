    #include <SDL3/SDL.h>
    #include <SDL3_ttf/SDL_ttf.h>
    #include <iostream>

    #include "userInterface/MainMenu.h"
    #include "game/Game.h"
    #include "shipEditor/ShipEditor.h"
    #include "userInterface/fonts.h"

    #define CLAY_IMPLEMENTATION
    #include "clay/clay.h"
    #include "clay/renderers/clay_renderer_SDL3.c"

    // ========== Global Clay Context ==========

    Clay_SDL3RendererData clayRendererData;
    Clay_Arena clayMemory;
    TTF_Font** clayFonts = nullptr;

    static void HandleClayErrors(Clay_ErrorData errorData) {
        std::cerr << "Clay Error: " << std::string(errorData.errorText.chars, errorData.errorText.length) << std::endl;
    }

    static inline Clay_Dimensions SDL_MeasureText(Clay_StringSlice text, Clay_TextElementConfig* config, void* userData) {
        TTF_Font** fonts = (TTF_Font**)userData;
        TTF_Font* font = fonts[config->fontId];
        int width = 0, height = 0;

        TTF_SetFontSize(font, config->fontSize);
        if (TTF_GetStringSize(font, text.chars, text.length, &width, &height) < 0) {
            SDL_Log("Text size error: %s", SDL_GetError());
        }

        return (Clay_Dimensions){ (float)width, (float)height };
    }


    int main(int argc, char* argv[]) {

        if (!TTF_Init()) {
            std::cout << "Failed to initialize SDL_ttf";
            // handle error
        }

        if (!SDL_Init(SDL_INIT_VIDEO)) {
            std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return 1;
        }

        if (!LoadFonts())
        {
            std::cout << "could not load fonts";
            return 1;
        }

        SDL_Window* window = SDL_CreateWindow("Spaceships", 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if (!window) {
            std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return 1;
        }

        SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
        if (!renderer) {
            std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }

        // Init Clay memory
        uint64_t totalMemorySize = Clay_MinMemorySize();
        clayMemory.memory = (char*)SDL_malloc(totalMemorySize);

        clayMemory.capacity = totalMemorySize;

        // Init Clay
        int winW, winH;
        SDL_GetWindowSize(window, &winW, &winH);
        Clay_Initialize(clayMemory, (Clay_Dimensions){(float)winW, (float)winH},
            (Clay_ErrorHandler){ .errorHandlerFunction = HandleClayErrors });

        // Setup text rendering
        clayFonts = (TTF_Font**)SDL_calloc(1, sizeof(TTF_Font*));
        clayFonts[0] = fonts["md"]; // e.g. from LoadFonts()

        clayRendererData.fonts = clayFonts;
        clayRendererData.renderer = renderer;
        clayRendererData.textEngine = TTF_CreateRendererTextEngine(renderer);
        Clay_SetMeasureTextFunction(SDL_MeasureText, clayFonts);


        MenuNavigation navigation = MainMenu;

        while (navigation != Quit)
        {
            switch (navigation)
            {
            case Game:
                navigation = RunGame(renderer, window);
                std::cout << "navigation : " << navigation <<"\n";
                break;
            case MainMenu:
                navigation = RunMainMenu(renderer, window);
                break;
            case ShipEditor:
                navigation = RunShipEditor(renderer, window);
                break;
            case Settings:
                throw std::logic_error("Function not yet implemented");
                break;
            }
        }
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

        SDL_free(clayMemory.memory);
        SDL_free(clayFonts);
        TTF_DestroyRendererTextEngine(clayRendererData.textEngine);
        return 0;
    }