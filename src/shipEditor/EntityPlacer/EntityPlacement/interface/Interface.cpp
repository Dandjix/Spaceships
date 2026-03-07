//
// Created by timon on 2/28/26.
//

#include "Interface.h"

#include "game/Rendering.h"
#include "userInterface/GUI.h"
#include "userInterface/elements/prompts/FloatPrompt.h"
#include "userInterface/elements/prompts/IntPrompt.h"
#include "userInterface/elements/prompts/Select.h"
#include "userInterface/elements/prompts/TextPrompt.h"
#include "userInterface/elements/prompts/Vector2FloatPrompt.h"
#include "userInterface/elements/prompts/Vector2IntPrompt.h"

constexpr int FIELD_HEIGHT_PX = 50;
constexpr int FIELD_WIDTH_PX = 250;

EntityPlacement::InterfaceForm::FormResult EntityPlacement::Interface::gatherResults() {
    InterfaceForm::FormResult result = {};

    for (const auto &[field_name, prompt]: int_prompts)
        result.results_int.insert({field_name, prompt->getValue()});
    for (const auto &[field_name, prompt]: float_prompts)
        result.results_float.insert({field_name, prompt->getValue()});
    for (const auto &[field_name, prompt]: Vector2Int_prompts)
        result.results_Vector2Int.insert({field_name, prompt->getValue()});
    for (const auto &[field_name, prompt]: Vector2Float_prompts)
        result.results_Vector2Float.insert({field_name, prompt->getValue()});
    for (const auto &[field_name, prompt]: string_prompts)
        result.results_string.insert({field_name, prompt->getValue()});

    return result;
}

int EntityPlacement::Interface::computeFormHeight(const InterfaceForm::FormRequest &request) {
    return static_cast<int>(request.choices.size()) * FIELD_HEIGHT_PX
           + static_cast<int>(request.query.size()) * FIELD_HEIGHT_PX
           + 1 * FIELD_HEIGHT_PX; //confirm button
}

void EntityPlacement::Interface::createPrompts(
    const InterfaceForm::FormRequest &request) {
    int height = computeFormHeight(request);

    Anchor anchor = Anchor::Center;
    Vector2Int prompt_offset = {0, height / 2};

    for (const auto &[field_name,choices]: request.choices) {
        string_prompts.push_back({
            field_name,
            new GUI::Prompts::Select(anchor, prompt_offset, FIELD_WIDTH_PX, FIELD_HEIGHT_PX, choices)
        });
        prompt_offset.y += FIELD_HEIGHT_PX;
    }

    for (const auto &[field_name,type]: request.query) {
        switch (type) {
            case InterfaceForm::INT:
                int_prompts.push_back({
                        field_name,
                        new GUI::Prompts::IntPrompt(anchor,
                                                    prompt_offset,
                                                    FIELD_WIDTH_PX,
                                                    FIELD_HEIGHT_PX,
                                                    window,
                                                    true)
                    }
                );
                break;
            case InterfaceForm::FLOAT:
                float_prompts.push_back({
                    field_name,
                    new GUI::Prompts::FloatPrompt(anchor,
                                                  prompt_offset,
                                                  FIELD_WIDTH_PX,
                                                  FIELD_HEIGHT_PX,
                                                  window,
                                                  true)
                });

                break;
            case InterfaceForm::VECTOR2INT:
                Vector2Int_prompts.push_back({
                    field_name,
                    new GUI::Prompts::Vector2IntPrompt(anchor,
                                                       prompt_offset,
                                                       FIELD_WIDTH_PX,
                                                       FIELD_HEIGHT_PX,
                                                       window,
                                                       true)
                });

                break;
            case InterfaceForm::VECTOR2FLOAT:
                Vector2Float_prompts.push_back({
                    field_name,
                    new GUI::Prompts::Vector2FloatPrompt(anchor,
                                                         prompt_offset,
                                                         FIELD_WIDTH_PX,
                                                         FIELD_HEIGHT_PX,
                                                         window,
                                                         true)
                });

                break;
            case InterfaceForm::STRING:
                string_prompts.push_back({
                    field_name,
                    new GUI::Prompts::TextPrompt(anchor,
                                                 prompt_offset,
                                                 FIELD_WIDTH_PX,
                                                 FIELD_HEIGHT_PX,
                                                 window,
                                                 true)
                });

                break;
        }
        prompt_offset.y += FIELD_HEIGHT_PX;
    }

    // activate (place in the ui)
    for (auto prompt: int_prompts | std::views::values)
        gui_elements->insert(prompt->asGUIRect());
    for (auto prompt: float_prompts | std::views::values)
        gui_elements->insert(prompt->asGUIRect());
    for (auto prompt: Vector2Int_prompts | std::views::values)
        gui_elements->insert(prompt->asGUIRect());
    for (auto prompt: Vector2Float_prompts | std::views::values)
        gui_elements->insert(prompt->asGUIRect());
    for (auto prompt: string_prompts | std::views::values)
        gui_elements->insert(prompt->asGUIRect());

    //it is safer to delete the send button and create it each time since it avoids a possible memory leak


    send_button = new GUI::TextButton(
        anchor,
        prompt_offset,
        FIELD_WIDTH_PX,
        FIELD_HEIGHT_PX,
        std::format("Place {}", form_request.entity_name),
        false,
        [this]() {
            return promptsAreValid();
        }
    );
    send_button->on_pressed.subscribe([this]() {
        sendForm();
    });
    gui_elements->insert(send_button);
}

void EntityPlacement::Interface::clearPrompts() {
    for (auto prompt: int_prompts | std::views::values)
        prompt->asGUIRect()->kill(gui_elements);
    for (auto prompt: float_prompts | std::views::values)
        prompt->asGUIRect()->kill(gui_elements);
    for (auto prompt: Vector2Int_prompts | std::views::values)
        prompt->asGUIRect()->kill(gui_elements);
    for (auto prompt: Vector2Float_prompts | std::views::values)
        prompt->asGUIRect()->kill(gui_elements);
    for (auto prompt: string_prompts | std::views::values)
        prompt->asGUIRect()->kill(gui_elements);

    send_button->kill(gui_elements);

    int_prompts = {};
    float_prompts = {};
    Vector2Int_prompts = {};
    Vector2Float_prompts = {};
    string_prompts = {};
}

bool EntityPlacement::Interface::promptsAreValid() const {
    //TODO finish this
    return true;
}

EntityPlacement::Interface::Interface(
    ElementContainerDQ<GUIRect *> *gui_elements,
    SDL_Window *window,
    std::vector<Entity *> *world_entities,
    const EntityRendering::Context &entity_rendering_context
)
    : ShortLivedEntity({0, 0}, 0),
      position_to_place({0, 0}),
      angle_to_place(0),
      send_button(new GUI::TextButton(Anchor::Center, {0, 0}, 150, 45, "--placeholder--")),
      gui_elements(gui_elements),
      window(window),
      world_entities(world_entities),
      entity_rendering_context(entity_rendering_context) {
}

void EntityPlacement::Interface::render(SDL_Renderer *renderer, const RenderingContext &context) {
    //we render a bg rect

    int total_height = computeFormHeight(form_request);

    auto center = (context.camera_info.screenDimensions) / 2;
    Vector2Float dimensions = {1000.0f, static_cast<float>(total_height)};
    auto rect = SDL_FRect(center.x - dimensions.x / 2, center.y - dimensions.y / 2, 0, 0);

    SDL_SetRenderDrawColor(renderer, 225, 225, 225, 255);
    SDL_RenderRect(renderer, &rect);
}

void EntityPlacement::Interface::askForForm(
    const InterfaceForm::FormRequest &form_request_param,
    std::function<Entity *(const InterfaceForm::FormResult &)> place_entity
) {
    createPrompts(form_request_param);
    on_form_submitted = std::move(place_entity);
}

void EntityPlacement::Interface::sendForm() {
    InterfaceForm::FormResult result = gatherResults();
    Entity *entity = on_form_submitted(result);
    placeImmediately(entity);
    clearPrompts();
}

void EntityPlacement::Interface::placeImmediately(Entity *new_entity) {
    new_entity->initializeRendering(entity_rendering_context);
    world_entities->push_back(new_entity);
}
