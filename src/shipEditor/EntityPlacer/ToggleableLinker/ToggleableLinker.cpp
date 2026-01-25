//
// Created by timon on 1/25/26.
//

#include "ToggleableLinker.h"

#include "entities/toggleableActivators/scripts/ToggleableActivator.h"
#include "entities/toggleables/scripts/Toggleable.h"

constexpr float SHORTEST_DISTANCE_PX = 256;

Toggleable *ToggleableLinker::getClosestToggleable(Vector2Int world_mouse_position) const {
    float smallest_distance = std::numeric_limits<float>().max();

    Toggleable *closest = nullptr;

    for (auto e: *entities) {
        auto t = dynamic_cast<Toggleable *>(e);

        if (t == nullptr) continue;

        auto distance = (world_mouse_position - t->getPosition()).length();

        if (distance <= smallest_distance) {
            smallest_distance = distance;
            closest = t;
        }
    }
    if (smallest_distance > Scaling::scaleToWorld(SHORTEST_DISTANCE_PX))
        return nullptr;

    return closest;
}

ToggleableActivator *ToggleableLinker::getClosestToggleableActivator(Vector2Int world_mouse_position) const {
    float smallest_distance = std::numeric_limits<float>().max();

    ToggleableActivator *closest = nullptr;

    for (auto e: *entities) {
        auto t = dynamic_cast<ToggleableActivator *>(e);

        if (t == nullptr) continue;

        auto distance = (world_mouse_position - t->asEntity()->getPosition()).length();

        if (distance <= smallest_distance) {
            smallest_distance = distance;
            closest = t;
        }
    }
    if (smallest_distance > Scaling::scaleToWorld(SHORTEST_DISTANCE_PX))
        return nullptr;

    return closest;
}

void ToggleableLinker::drawLineToExisting(SDL_Renderer * renderer,const RenderingContext & context,ToggleableActivator *toggleable_activator) {
    if (toggleable_activator->getLinkedEntity() == nullptr)
        return;

    auto screen_pos_1 = context.camera_info.worldToScreenPoint(toggleable_activator->asEntity()->getPosition());
    auto screen_pos_2 = context.camera_info.worldToScreenPoint(toggleable_activator->getLinkedEntity()->getPosition());

    SDL_RenderLine(renderer,screen_pos_1.x,screen_pos_1.y,screen_pos_2.x,screen_pos_2.y);
}

void ToggleableLinker::setState(State value) {
    state = value;

    switch (state) {
        case Disabled:
        case PickingActivator:
            activator_to_link_to = nullptr;
            toggleable_to_link_to = nullptr;
            break;
        case PickingToggleable:
            toggleable_to_link_to = nullptr;
            break;
    }
}

void drawLine(SDL_Renderer *renderer, const RenderingContext &context, const Entity *from, const Entity *to) {
    Vector2Float position1 = context.camera_info.worldToScreenPoint(from->getPosition());
    Vector2Float position2 = context.camera_info.worldToScreenPoint(to->getPosition());
    SDL_RenderLine(renderer, position1.x, position1.y, position2.x, position2.y);
}


void drawOctagon(SDL_Renderer *renderer, const RenderingContext &context, Vector2Int world_position) {
    constexpr int OCTAGON_RADIUS_PX = 48;
    constexpr int sides = 8;

    std::vector<Vector2Float> points;
    points.reserve(sides);

    for (int i = 0; i < 8; ++i) {
        Vector2Int world_pos = {0, Scaling::scaleToWorld(OCTAGON_RADIUS_PX)};
        world_pos = world_position + world_pos.rotate(12.5 + i * (360 / sides));
        points.push_back(context.camera_info.worldToScreenPoint(world_pos));
    }

    for (int i = 0; i < sides; ++i) {
        int next_i = (i + 1) % sides;
        SDL_RenderLine(renderer, points[i].x, points[i].y, points[next_i].x, points[next_i].y);
    }
}

void ToggleableLinker::render(SDL_Renderer *renderer, const RenderingContext &context) {
    switch (state) {
        case Disabled:
            break;
        case PickingActivator:
            if (closest_activator == nullptr)
                break;
            SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
            drawOctagon(renderer, context, closest_activator->asEntity()->getPosition());
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            drawLineToExisting(renderer, context, closest_activator);
            break;
        case PickingToggleable:
            if (closest_toggleable == nullptr)
                break;
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            drawOctagon(renderer, context, closest_toggleable->getPosition());
            drawLine(renderer, context, activator_to_link_to->asEntity(), closest_toggleable);
            break;
    }
}

void ToggleableLinker::update(const UpdateContext &context) {
    if (state == Disabled) {
        closest_activator = nullptr;
        closest_toggleable = nullptr;
        return;
    }

    float mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    Vector2Int mouse_world_position = context.camera_info.screenToWorldPoint({mouse_x, mouse_y});

    if (state == PickingActivator) {
        closest_activator = getClosestToggleableActivator(mouse_world_position);
        closest_toggleable = nullptr;
    } else //if (state == PickingToggleable)
    {
        closest_activator = nullptr;
        closest_toggleable = getClosestToggleable(mouse_world_position);
    }
}

void ToggleableLinker::done(ToggleableActivator *toggleable_activator, Toggleable *toggleable) {
    toggleable_activator->setLinkedEntity(toggleable);
}

void ToggleableLinker::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) {
    if (state == Disabled)
        return;

    if (SDL_GetModState() & SDL_KMOD_ALT)
        return;

    if (
        event.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
        event.button.button == SDL_BUTTON_RIGHT &&
        context.mouse_position_type == GameEvent::MousePositionType::Game &&
        state == PickingActivator &&
        closest_activator != nullptr &&
        closest_activator->getLinkedEntity() != nullptr
    ) {
        closest_activator->setLinkedEntity(nullptr);
    } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT && context.
               mouse_position_type == GameEvent::MousePositionType::Game) {
        switch (state) {
            case PickingActivator:
                if (closest_activator != nullptr) {
                    activator_to_link_to = closest_activator;
                    setState(PickingToggleable);
                }
                break;
            case PickingToggleable:
                if (closest_toggleable != nullptr) {
                    toggleable_to_link_to = closest_toggleable;

                    done(activator_to_link_to, toggleable_to_link_to);

                    setState(PickingActivator);
                }
                break;
            default:
                break;
        }
    } else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
        setState(PickingActivator);
    }
}

QueueOrder::Value ToggleableLinker::getQueueOrder() {
    return ShortLivedEntity::getQueueOrder();
}
