#include "GUIRect.h"

bool GUIRect::is_inside(Vector2Float position)
{
    return  (
        position.x >= screenPosition.x  && position.x <= screenPosition.x + dimensions.x
        && position.y >= screenPosition.y  && position.y <= screenPosition.y + dimensions.y
    );

}

void GUIRect::update(const GUI_UpdateContext& context)
{
    int h, w;
    if (width == GUI_Fill)
        w = context.screenDimensions.x;
    else
        w = width;

    if (height == GUI_Fill)
        h = context.screenDimensions.y;
    else
        h = height;


    Vector2Int topLeftCorner;
    switch (anchor)
    {
    case Anchor::TL:
        topLeftCorner = Vector2Int(0, 0);
        break;
    case Anchor::TR:
        topLeftCorner = Vector2Int(context.screenDimensions.x, 0);
        break;
    case Anchor::BR:
        topLeftCorner = Vector2Int(context.screenDimensions.x, context.screenDimensions.y);
        break;
    case Anchor::BL:
        topLeftCorner = Vector2Int(0, context.screenDimensions.y);
        break;
    }

    screenPosition = topLeftCorner + offset;
    dimensions = Vector2Int(w, h);
}
