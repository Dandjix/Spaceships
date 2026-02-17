//
// Created by timon on 2/1/26.
//

#include "player/Camera.h"

#include "game/Update.h"
#include "spaceships/EntityData/EntityLoading.h"



Camera::Camera(Vector2Int position, float angle, float scale): LateUpdateEntity(position, angle), player(nullptr), scale(scale) {
}

Camera::~Camera() = default;

void Camera::setPlayer(Entity *p) {
    player = p;
    player_ownership_changed_id = player->on_ownership_change.subscribe(
        [this](Entity * new_owner) {
            player->on_ownership_change.unsubscribe(this->player_ownership_changed_id);
            setPlayer(new_owner);
        });
    setPosition(p->getPosition());  // Set the camera's position to follow the player initially
}

void Camera::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) {
    if (event.type == SDL_EVENT_MOUSE_WHEEL) {
        float zoomFactor = 0.1f; // Adjust zoom speed
        float scale_value= getScale() + event.wheel.y * zoomFactor;

        // Clamp scale
        scale_value = std::clamp(scale_value, 0.5f,5.0f);

        setScale(scale_value);
    }
}

QueueOrder::Value Camera::getQueueOrder() {
    return QueueOrder::LAST;
}

void Camera::setScale(float s) {
    if (s != scale)
    {
        scale = s;
    }
}

float Camera::getScale() const {
    return scale;
}

void Camera::setScreenDimensions(Vector2Int value) {
    this->screenDimensions = value;
}

void Camera::render(SDL_Renderer *renderer, const RenderingContext &context) {}

void Camera::lateUpdate(const UpdateContext &context) {
    setPosition(player->getPosition());  // Again in late update so that the camera position is synced with the physics
}

nlohmann::json Camera::toJson() {
    auto json = Entity::toJson();
    json["scale"] = scale;
    return json;
}

void Camera::registerInInstance(Instances::Instance *world_instance) {
    LateUpdateEntity::registerInInstance(world_instance);
    world_instance->cameras.insert(this);
}

void Camera::unregisterInInstance(Instances::Instance *world_instance, bool delete_when_done) {
    world_instance->cameras.add_to_erasing_queue(this,delete_when_done);
    LateUpdateEntity::unregisterInInstance(world_instance, delete_when_done);
}

void Camera::onRegistered(Instances::Instance *newInstance) {
    working_instance = newInstance;
}

Entity * Camera::initializeRendering(const EntityRendering::Context &context) {return this;}

Entity * Camera::finalizeRendering(const EntityRendering::Context &context) {return this;}

void Camera::update(const UpdateContext &context) {
    setPosition(player->getPosition());  // Update camera's position based on the player's position
    const bool* state = SDL_GetKeyboardState(nullptr);
    float deltaAngle = 0;
    if (state[SDL_SCANCODE_PAGEUP]) {
        deltaAngle -= 180* context.deltaTime;
    }
    if (state[SDL_SCANCODE_PAGEDOWN]) {
        deltaAngle += 180 * context.deltaTime;
    }
    float newAngle = getAngle() + deltaAngle;
    setAngle(newAngle);
    //if (getAngle() != newAngle)
    //{
    //    std::cout << "angle is " << newAngle <<"\n";
    //}
}

FROM_JSON_DEFINITION(Camera) {
    return new Camera(Vector2Int::fromJson(json["position"]),json["angle"],json["scale"]);
}