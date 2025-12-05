//
// Created by timon on 12/4/25.
//

#pragma once
#include "game/ElementContainer.h"
#include "userInterface/GUIRect.h"

#include "userInterface/elements/GUI/GUILabel.h"

namespace GUI {
    class Snackbar : public GUIRect {
    public:
        struct message {
            int ms_to_live;
            GUILabel *label;
        };

    protected:
        std::vector<message> messages = {};
        ElementContainer<GUIRect*> *gui_elements;
        float message_padding_px;
        QueueOrder::Value queue_order;

    private:
        std::vector<message> getMessages() {
            return messages;
        }

    public:
        void addMessage(const std::string &text, int ms_to_live = 4000, SDL_Color color = {255, 255, 255, 255}) {
            auto label = new GUILabel(anchor, {0, 0}, width, height, text, color, fonts["sm"]);
            messages.push_back({
                ms_to_live,
                label
            });
            gui_elements->add(label);
        }

        explicit Snackbar(ElementContainer<GUIRect*> * gui_elements, Anchor anchor = Anchor::Center,
                          Vector2Int offset = {0, 0},
                          int width = GUI_Fill, int height = GUI_Fill,
                          QueueOrder::Value queue_order = QueueOrder::MIDDLE, float message_padding_px = 5)
            : GUIRect(anchor, offset, width, height), gui_elements(gui_elements), queue_order(queue_order),
              message_padding_px(message_padding_px) {
        }

        void render(SDL_Renderer *renderer, const GUI_RenderingContext &context) const override {
        }

        void update(const GUI_UpdateContext &context) override {
            for (auto & message: messages) {
                message.ms_to_live -= static_cast<int>(context.deltaTime*1000);
                if (message.ms_to_live <= 0) {
                    gui_elements->removeAndDelete(message.label);
                }
            }


            float messages_y = message_padding_px;
            for (auto message: messages) {
                messages_y += static_cast<float>(message.label->getHeight()) + message_padding_px;
            }
            for (auto message: messages) {
                message.label->setOffset({0, static_cast<int>(messages_y)});
                messages_y -= static_cast<float>(message.label->getHeight()) + message_padding_px;
            }
        }

        QueueOrder::Value getQueueOrder() override {
            return queue_order;
        }
    };
}
