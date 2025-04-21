#include "MenuNavigation.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include "Vectors.h"
#include "FreeCamera.h"
#include "DebugGrid.h"
#include "SaveAndLoadShips.h"

void RenderSidebar(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, std::vector<SDL_FRect>& buttonRects)
{
    buttonRects.clear();

    const int sidebarWidth = 200;
    const SDL_Color backgroundColor = { 30, 30, 30, 255 }; // Dark gray
    const SDL_Color textColor = { 255, 255, 255, 255 }; // White
    const int optionHeight = 30;
    const std::vector<std::string> options = {"Void","Wall","HDoor","VDoor","Floor","Save","Load"};

    int screenWidth, screenHeight;

    SDL_GetWindowSize(window, &screenWidth, &screenHeight);

    // Draw sidebar background
    SDL_FRect sidebarRect = { 0, 0, (float)sidebarWidth, (float)screenHeight };
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderFillRect(renderer, &sidebarRect);

    // Draw each option
    for (int i = 0; i < options.size(); ++i) {
        SDL_FRect optionRect = { 10, (float)(10 + i * (optionHeight + 10)), sidebarWidth - 20, (float)optionHeight };
        buttonRects.push_back(optionRect);
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255); // Lighter gray for button
        SDL_RenderFillRect(renderer, &optionRect);

        // Render label
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, options[i].c_str(), options[i].size(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        int textW = textSurface->w, textH = textSurface->h;
        SDL_FRect textRect = {
            optionRect.x + (optionRect.w - textW) / 2.0f,
            optionRect.y + (optionRect.h - textH) / 2.0f,
            (float)textW, (float)textH
        };
        SDL_RenderTexture(renderer, textTexture, NULL, &textRect);

        SDL_DestroyTexture(textTexture);
        SDL_DestroySurface(textSurface);
    }
}


MenuNavigation RunShipEditor(SDL_Renderer * renderer, SDL_Window * window, TTF_Font* font)
{


    FreeCamera camera(Vector2Int(0, 0), 0, 1,600);

    DebugGrid grid(0, 0, 64);
    Uint64 now = SDL_GetTicks();
    Uint64 last = 0;

    float deltaTime = 0.0f;

    MenuNavigation destination = ShipEditor;
    std::vector<SDL_FRect> buttonRects;

    while (destination == ShipEditor) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                destination = Quit;
            }
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                float mouseX = event.button.x;
                float mouseY = event.button.y;

                for (int i = 0; i < buttonRects.size(); ++i) {
                    SDL_FRect& rect = buttonRects[i];
                    if (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
                        mouseY >= rect.y && mouseY <= rect.y + rect.h) {

                        switch (i)
                        {
                        case 0:
                            SDL_Log("Void");
                            break;
                        case 1:
                            SDL_Log("Wall");
                            break;
                        case 2:
                            SDL_Log("HDoor");
                            break;
                        case 3:
                            SDL_Log("VDoor");
                            break;
                        case 4:
                            SDL_Log("Floor");
                            break;
                        case 5:
                            //SDL_Log("Save");
                            SaveShip("123");
                            break;
                        case 6:
                            std::string blueprint = LoadShip();
                            std::cout << "bp : " << blueprint;
                            break;
                        }
                        
                    }
                }
            }

            camera.handleEvent(event);
        }
        last = now;
        now = SDL_GetTicks();
        deltaTime = (now - last) / 1000.0f; // Convert ms to seconds


        // update
        grid.update(deltaTime);
        camera.update(deltaTime);

        //render variable calculation
        int screenWidth, screenHeight;
        SDL_GetWindowSize(window, &screenWidth, &screenHeight);
        Vector2Int screenDimensions = Vector2Int(screenWidth, screenHeight);
        Vector2Int cameraPos = camera.getOffsetPosition(screenDimensions);

        RenderingContext renderingContext(cameraPos, camera.getAngle(), screenDimensions, camera.getScale());

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        //render
        grid.render(renderer, renderingContext);
        camera.render(renderer, renderingContext);

        //render debug
        grid.debugRender(renderer, renderingContext);
        RenderSidebar(renderer, window, font, buttonRects);

        SDL_RenderPresent(renderer);

    }

    return destination;
}