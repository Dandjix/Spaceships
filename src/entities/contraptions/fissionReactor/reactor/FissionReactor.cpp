//
// Created by timon on 2/18/26.
//

#include "FissionReactor.h"

#include "entities/contraptions/fissionReactor/controlRod/ControlRod.h"
#include "physics/shapes/RoundStaticPhysicsShape.h"

constexpr int REACTOR_RADIUS = UNITS_PER_METER * 5.0f * Vectors::getFactor();

std::vector<Contraptions::FissionReactor::Reactor::ControlRodInfo> Contraptions::FissionReactor::Reactor::ControlRodInfo
::range(int nb_rods, float start_angle, float end_angle, float progress) {
    std::vector<ControlRodInfo> info;
    info.reserve(nb_rods);
    for (int i = 0; i < nb_rods; ++i) {
        float diff = (end_angle - start_angle)/static_cast<float>(nb_rods);
        float working_angle = start_angle + diff* static_cast<float>(i);
        info.push_back({working_angle,progress});
    }
    return info;
}

std::vector<Contraptions::FissionReactor::Reactor::ControlRodInfo> Contraptions::FissionReactor::Reactor::ControlRodInfo
::symmetricRange(int nb_rods, float start_angle, float end_angle, float progress) {
    std::vector<ControlRodInfo> info;
    info.reserve(nb_rods);
    for (int i = 0; i < nb_rods; ++i) {
        float diff = (end_angle - start_angle)/static_cast<float>(nb_rods - 1);
        float working_angle = start_angle + diff* static_cast<float>(i);
        info.push_back({working_angle,progress});
    }
    return info;
}

void Contraptions::FissionReactor::Reactor::constructControlRods(const std::vector<ControlRodInfo> &control_rod_info) {
    if (!control_rods.empty())
        throw std::runtime_error("tried constructing reactor rods more than once : rods vector not empty");

    int LENGTH_OF_ROD_STICKING_WHEN_FULLY_INSERTED = Scaling::metricToWorld(0.15f);

    control_rods.reserve(control_rod_info.size());

    Vector2Int rod_dimensions = Scaling::metricToWorld(Vector2Float(3, 0.20f));

    for (auto [relative_angle,progress]: control_rod_info) {
        float to_rotate = getAngle() - relative_angle;

        int center_distance = REACTOR_RADIUS
                              - rod_dimensions.x / 2
                              + LENGTH_OF_ROD_STICKING_WHEN_FULLY_INSERTED;

        int end_distance = REACTOR_RADIUS
                           + rod_dimensions.x / 2
                           + Scaling::metricToWorld(0.3f);

        Vector2Int rod_start = getPosition() + Vector2Int(end_distance, 0).rotate(to_rotate);
        Vector2Int rod_end = getPosition() + Vector2Int(center_distance, 0).rotate(to_rotate);

        auto *rod = new ControlRod(rod_start, rod_end, progress, rod_dimensions);

        control_rods.push_back(rod);
    }
}

Contraptions::FissionReactor::Reactor::Reactor
(
    Vector2Int position,
    float angle,
    const std::vector<ControlRodInfo> &control_rod_info,
    float output) : PhysicsEntity(
                        position,
                        angle,
                        new RoundStaticPhysicsShape(this, REACTOR_RADIUS)),
                    output(output) {
    constructControlRods(control_rod_info);
}

std::vector<Contraptions::FissionReactor::Reactor::ControlRodInfo>
Contraptions::FissionReactor::Reactor::generate_rod_info() const {
    std::vector<ControlRodInfo> info;
    info.reserve(control_rods.size());

    for (ControlRod *control_rod: control_rods) {
        float relative_angle = control_rod->computeAngle();
        float progress = control_rod->getProgress();
        info.push_back({relative_angle, progress});
    }

    return info;
}

void Contraptions::FissionReactor::Reactor::registerInInstance(Instances::Instance *world_instance) {
    PhysicsEntity::registerInInstance(world_instance);
    for (ControlRod *control_rod: control_rods) {
        control_rod->registerInInstance(world_instance);
    }
}

void Contraptions::FissionReactor::Reactor::unregisterInInstance(Instances::Instance *world_instance,
                                                                 bool delete_when_done) {
    PhysicsEntity::unregisterInInstance(world_instance, delete_when_done);
    for (ControlRod *control_rod: control_rods) {
        control_rod->unregisterInInstance(world_instance, delete_when_done);
    }
}

bool Contraptions::FissionReactor::Reactor::interacts(PhysicsEntity *other) {
    //if any of them is a rod of this reactor, don't interact.
    return !std::any_of(control_rods.begin(), control_rods.end(), [other](ControlRod *control_rod) {
        return other == control_rod;
    });
}

nlohmann::json Contraptions::FissionReactor::Reactor::toJson() {
    nlohmann::json json = PhysicsEntity::toJson();

    json["rod_info"] = ControlRodInfo::vectorToJson(generate_rod_info());
    json["output"] = output;

    return json;
}

FROM_JSON_DEFINITION(Contraptions::FissionReactor::Reactor) {
    return new Reactor(
        Vector2Int::fromJson(json["position"]),
        json["angle"],

        ControlRodInfo::vectorFromJson(json["rod_info"]),
        json["output"]
    );
}
