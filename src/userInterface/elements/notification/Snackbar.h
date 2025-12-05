//
// Created by timon on 12/4/25.
//

#pragma once
#include "game/ElementContainer.h"
#include "userInterface/GUIRect.h"

#include "userInterface/elements/GUI/GUIFramedLabel.h"

namespace GUI {
    class Snackbar : public GUIRect {
    public:
        struct message {
            int ms_to_live;
            GUI::FramedLabel * label;
        };

    protected:
        std::vector<message> messages = {};
        ElementContainer<GUIRect *> *gui_elements;
        ElementContainer<GUIRect *> *gui_elements_deletion_queue;
        float message_padding_px;
        QueueOrder::Value queue_order;

    private:
        std::vector<message> getMessages() {
            return messages;
        }

    public:
        void addMessage(const std::string &text, int ms_to_live = 4000, SDL_Color color = {255, 255, 255, 255}) {
            auto label = new GUI::FramedLabel(anchor, {0, 0}, 240, 55, text, color, fonts["sm"]);
            messages.push_back({
                ms_to_live,
                label
            });
            gui_elements->add(label);
        }

        explicit Snackbar(ElementContainer<GUIRect *> *gui_elements,
                          ElementContainer<GUIRect *> *gui_elements_deletion_queue, Anchor anchor = Anchor::BottomCenter,
                          Vector2Int offset = {0, 0},
                          int width = GUI_Fill, int height = GUI_Fill,
                          QueueOrder::Value queue_order = QueueOrder::MIDDLE, float message_padding_px = 5)
            : GUIRect(anchor, offset, width, height), gui_elements(gui_elements),
              gui_elements_deletion_queue(gui_elements_deletion_queue), queue_order(queue_order),
              message_padding_px(message_padding_px) {
        }

        void render(SDL_Renderer *renderer, const GUI_RenderingContext &context) const override {
        }

        void update(const GUI_UpdateContext &context) override {
            std::vector<message> toDelete;
            for (auto &message: messages) {
                message.ms_to_live -= static_cast<int>(context.deltaTime * 1000);
                if (message.ms_to_live <= 0) {
                    toDelete.push_back(message);
                }
            }

            for (message msg: toDelete) {
                gui_elements_deletion_queue->add(msg.label);
                auto msg_position = std::find_if(messages.begin(), messages.end(), [&msg](const message &current) {
                    return msg.label == current.label;
                });
                if (msg_position != messages.end()) {
                    messages.erase(msg_position);
                }
            }
            toDelete.clear();

            float messages_y = message_padding_px;
            for (auto message: messages) {
                messages_y += static_cast<float>(message.label->getHeight()) + message_padding_px;
            }
            for (auto message: messages) {
                message.label->setOffset({0, - static_cast<int>(messages_y)});
                messages_y -= static_cast<float>(message.label->getHeight()) + message_padding_px;
            }
        }

        QueueOrder::Value getQueueOrder() override {
            return queue_order;
        }
    };
}
