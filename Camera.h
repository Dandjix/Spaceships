#pragma once
#include "Entity.h"
#include "Vectors.h"

class Camera : public Entity {  // Inherit from Entity
protected:
    Entity* player = nullptr;  // Pointer to the player entity

private :
    float scale;
public:
    /// <summary>
    /// Creates a Camera that follows the player.
    /// </summary>
    /// <param name="p">Pointer to the player entity</param>
    Camera(Vector2Int position, float angle, float scale) : Entity(position,angle),scale(scale) {}
    
    /// <summary>
    /// Set the player entity that the camera follows.
    /// </summary>
    /// <param name="p">Pointer to the new player entity</param>
    void setPlayer(Entity* p)
    {
        player = p;
        setPosition(p->getPosition());  // Set the camera's position to follow the player initially
    }

    void handleEvent(const SDL_Event& event) {
        if (event.type == SDL_EVENT_MOUSE_WHEEL) {
            float zoomFactor = 0.1f; // Adjust zoom speed
            float scale = getScale() + event.wheel.y * zoomFactor;

            // Clamp scale
            if (scale < 0.5f) scale = 0.5f;
            if (scale > 5.0f) scale = 5.0f;

            setScale(scale);
        }
    }
    
    void setScale(float s)
    {
        if (s != scale)
        {
            scale = s;
        }
    }

    float getScale()
    {
        return scale;
    }



    /// <summary>
    /// A camera renders nothing.
    /// </summary>
    void render(SDL_Renderer* renderer,const RenderingContext& context) override {}

    /// <summary>
    /// Update the camera's position to follow the player.
    /// </summary>
    /// <param name="deltaTime">Delta time for smooth movement</param>
    void update(float deltaTime) {
        setPosition(player->getPosition());  // Update camera's position based on the player's position
        const bool* state = SDL_GetKeyboardState(NULL);
        float deltaAngle = 0;
        if (state[SDL_SCANCODE_PAGEUP]) {
            deltaAngle -= 180*deltaTime;
        }
        if (state[SDL_SCANCODE_PAGEDOWN]) {
            deltaAngle += 180*deltaTime;
        }
        float newAngle = getAngle() + deltaAngle;
        setAngle(newAngle);
        //if (getAngle() != newAngle)
        //{
        //    std::cout << "angle is " << newAngle <<"\n";
        //}
    }

    /// <summary>
    /// Get the offset position of the camera relative to the screen's size.
    /// </summary>
    /// <param name="screenWidth">Screen width</param>
    /// <param name="screenHeight">Screen height</param>
    /// <returns>Offset position as Vector2Int</returns>
    Vector2Int getOffsetPosition(Vector2Int screenDimensions) {
        Vector2Int position = getPosition();
        int x = static_cast<int>( position.x - (screenDimensions.x / 2)*Vectors::getFactor()*getScale());  // Offset X to center the camera
        int y = static_cast<int>( position.y - (screenDimensions.y / 2)*Vectors::getFactor()*getScale());  // Offset Y to center the camera
        return Vector2Int(x, y);
    }
};
