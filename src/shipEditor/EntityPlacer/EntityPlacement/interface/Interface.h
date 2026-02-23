#pragma once
#include "EntityPlacementForm.h"
#include "math/Color.h"
#include "math/Vectors.h"
//
// Created by timon on 2/9/26.
//

class GUITextPrompt;

namespace EntityPlacement {
    class Interface : public ShortLivedEntity {
    protected:
        Vector2Int position_to_place;
        float angle_to_place;
        InterfaceForm::FormRequest form_request;
        std::vector<GUITextPrompt * > prompts;

    public:
        [[nodiscard]] Vector2Int getPlacementPosition() const { return position_to_place; }

        [[nodiscard]] float getPlacementAngle() const {
            return angle_to_place;
        }

        void setPositionToPlace(Vector2Int position_to_place_param) { position_to_place = position_to_place_param; }

        void setAngleToPlace(float angle_to_place_param) { angle_to_place = angle_to_place_param; }

        Interface()
            : ShortLivedEntity({0, 0}, 0), position_to_place({0, 0}), angle_to_place(0) {
        }

        [[nodiscard]] std::future<InterfaceForm::FormResult> askForForm(
            const InterfaceForm::FormRequest &form_request_param) {
            form_request = form_request_param;
            for
        }

        Entity * initializeRendering(const EntityRendering::Context &context) override {return this;}

        Entity * finalizeRendering(const EntityRendering::Context &context) override {return this;}

        void render(SDL_Renderer *renderer, const RenderingContext &context) override {
            //we render a bg rect

            float total_height = form_request.computeHeight();

            auto center = (context.camera_info.screenDimensions) / 2;
            Vector2Float dimensions = {1000.0f,total_height};
            auto rect = SDL_FRect(center.x - dimensions.x/2,center.y - dimensions.y/2,0,0);

            SDL_SetRenderDrawColor(renderer,225,225,225,255);
            SDL_RenderRect(renderer,&rect);
        }
    };
};
