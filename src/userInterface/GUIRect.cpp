#include "GUIRect.h"

bool GUIRect::is_inside(Vector2Float position) {
    return (
        position.x >= screenPosition.x && position.x <= screenPosition.x + dimensions.x
        && position.y >= screenPosition.y && position.y <= screenPosition.y + dimensions.y
    );
}

void GUIRect::update(const GUI_UpdateContext &context) {
    int h, w;
    if (width == GUI_Fill)
        w = context.camera_info.screenDimensions.x;
    else
        w = width;

    if (height == GUI_Fill)
        h = context.camera_info.screenDimensions.y;
    else
        h = height;


    Vector2Int topLeftCorner;
    switch (anchor) {
        case Anchor::TL:
            topLeftCorner = Vector2Int(0, 0);
            break;
        case Anchor::TR:
            topLeftCorner = Vector2Int(
                context.camera_info.screenDimensions.x - w,
                0
            );
            break;
        case Anchor::BR:
            topLeftCorner = Vector2Int(
                context.camera_info.screenDimensions.x - w,
                context.camera_info.screenDimensions.y - h
            );
            break;
        case Anchor::BL:
            topLeftCorner = Vector2Int(
                0,
                context.camera_info.screenDimensions.y - h
            );
            break;
        case Anchor::Center:
            topLeftCorner = Vector2Int(
                context.camera_info.screenDimensions.x * 0.5f - w * 0.5f,
                context.camera_info.screenDimensions.y * 0.5f - h * 0.5f
            );
            break;
        case Anchor::BottomCenter:
            topLeftCorner = Vector2Int(
                context.camera_info.screenDimensions.x * 0.5f - w * 0.5f,
                context.camera_info.screenDimensions.y - h
            );
            break;
        case Anchor::TopCenter:
            topLeftCorner = Vector2Int(
                context.camera_info.screenDimensions.x * 0.5f - w * 0.5f,
                0
            );
            break;
        case Anchor::CenterLeft:
            topLeftCorner = Vector2Int(
                0,
                context.camera_info.screenDimensions.y * 0.5f - h * 0.5f
            );
            break;
        case Anchor::CenterRight:
            topLeftCorner = Vector2Int(
                context.camera_info.screenDimensions.x -w,
                context.camera_info.screenDimensions.y * 0.5f - h * 0.5f
            );
            break;
    }

    screenPosition = topLeftCorner + offset;
    dimensions = Vector2Int(w, h);
}
