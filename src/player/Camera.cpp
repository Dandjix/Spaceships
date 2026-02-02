//
// Created by timon on 2/1/26.
//

#include "player/Camera.h"

#include "spaceships/EntityData/EntityLoading.h"

FROM_JSON_DEFINITION(Camera)
{
    return new Camera(Vector2Int::fromJson(json["position"]),json["angle"],json["scale"]);
}