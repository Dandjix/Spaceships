#pragma once
#include "Camera.h"

class FreeCamera : public Camera
{
protected:
    float speed;
public:
    FreeCamera(Vector2Int position,float angle,float scale, float speed);

    void handleEvent(const SDL_Event& event, const GameEvent::GameEventContext &context) override;

    void update(const UpdateContext & context) override;
};