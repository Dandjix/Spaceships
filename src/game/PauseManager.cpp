//
// Created by timon on 12/3/25.
//

#include "PauseManager.h"

void PauseManager::update(const UpdateContext &context) {
}

void PauseManager::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_ESCAPE) {
            setPaused( !*paused );

        }
    }
}

void PauseManager::render(SDL_Renderer *renderer, const RenderingContext &context) {
}

/**
 * You should always set paused through this : this will call all of the event goodness
 * @param new_paused_value the new value. The game is frozen if true, else it plays normally
 */
void PauseManager::setPaused(bool new_paused_value) {
    *paused = new_paused_value;
    on_paused_change.emit(new_paused_value);
}
