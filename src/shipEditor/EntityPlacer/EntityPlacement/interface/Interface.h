#pragma once
#include <future>

#include "EntityPlacementForm.h"
#include "entities/scripts/ShortLivedEntity.h"
#include "entities/toggleables/door/Door.h"
#include "userInterface/elements/GUI/TextButton.h"
#include "userInterface/elements/prompts/scripts/IValuePrompt.h"
//
// Created by timon on 2/9/26.
//

// class GUITextPrompt;

namespace EntityPlacement {
    struct Context;
}

namespace GUI {
    class TextButton;
}

struct GUI_UpdateContext;
class GUITextPrompt;
class Entity;

namespace EntityPlacement {
    class Interface : public ShortLivedEntity {
    protected:
        //immediately available info
        Vector2Int position_to_place;
        float angle_to_place;

        //current form
        InterfaceForm::FormRequest form_request;
        //callback for when the form is filled
        std::function<Entity *(const InterfaceForm::FormResult &)> on_form_submitted;

        //different inputs
        /** first is the field name, second is the prompt. I do not advise adding a field to the prompts for the name,
         * even though a description likely contains the field name */
        std::vector<std::pair<std::string, GUI::Prompts::IValuePrompt<int> *> > int_prompts = {};
        std::vector<std::pair<std::string, GUI::Prompts::IValuePrompt<float> *> > float_prompts = {};
        std::vector<std::pair<std::string, GUI::Prompts::IValuePrompt<Vector2Int> *> > Vector2Int_prompts = {};
        std::vector<std::pair<std::string, GUI::Prompts::IValuePrompt<Vector2Float> *> > Vector2Float_prompts = {};
        /** groups both text and select prompts, so beware of name collisions */
        std::vector<std::pair<std::string, GUI::Prompts::IValuePrompt<std::string> *> > string_prompts;
        GUI::TextButton *send_button;

        //needed for prompt init/showing/hiding
        ElementContainer<GUIRect *> *gui_elements;
        SDL_Window *window;
        //needed for placing the entities in the world
        std::vector<Entity *> *world_entities;
        const EntityRendering::Context &entity_rendering_context;


        InterfaceForm::FormResult gatherResults();

        static int computeFormHeight(const EntityPlacement::InterfaceForm::FormRequest &request);

        void createPrompts(const InterfaceForm::FormRequest &request);

        void clearPrompts();

        [[nodiscard]] bool promptsAreValid() const;

    public:
        [[nodiscard]] Vector2Int getPlacementPosition() const { return position_to_place; }

        [[nodiscard]] float getPlacementAngle() const {
            return angle_to_place;
        }

        void setPositionToPlace(Vector2Int position_to_place_param) { position_to_place = position_to_place_param; }

        void setAngleToPlace(float angle_to_place_param) { angle_to_place = angle_to_place_param; }


        Interface(
            ElementContainer<GUIRect *> *gui_elements,
            SDL_Window *window, std::vector<Entity *> *world_entities,
            const EntityRendering::Context &entity_rendering_context
        );

        Entity *initializeRendering(const EntityRendering::Context &context) override { return this; }

        Entity *finalizeRendering(const EntityRendering::Context &context) override { return this; }

        void render(SDL_Renderer *renderer, const RenderingContext &context) override;

        void askForForm(const InterfaceForm::FormRequest &form_request_param,
                        std::function<Entity *(const InterfaceForm::FormResult &)>
                        place_entity);

        void sendForm();

        void placeImmediately(Entity *new_entity);
    };
};
