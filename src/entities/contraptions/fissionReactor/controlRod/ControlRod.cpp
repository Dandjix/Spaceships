//
// Created by timon on 2/17/26.
//

#include "ControlRod.h"

FROM_JSON_DEFINITION(Contraptions::FissionReactor::ControlRod) {
    return new ControlRod(
        Vector2Int::fromJson(json["position"]),
        json["angle"],
        Vector2Int::fromJson(json["rail_start"]),
        Vector2Int::fromJson(json["rail_end"])
    );
}

nlohmann::json Contraptions::FissionReactor::ControlRod::toJson() {
    nlohmann::json json = PhysicsEntity::toJson();
    json["rail_start"] = rail_constraint.getStart().toJson();
    json["rail_end"] = rail_constraint.getEnd().toJson();
    json["rail_progress"] = rail_constraint.progress(this);
    return json;
}
