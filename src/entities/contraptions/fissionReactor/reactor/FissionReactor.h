#pragma once
//
// Created by timon on 2/18/26.
//
#include <vector>

#include "entities/contraptions/fissionReactor/reactorBehavior/FissionReactorBehavior.h"
#include "physics/scripts/PhysicsEntity.h"

namespace Contraptions::FissionReactor {
    class ControlRod;
}

namespace Contraptions::FissionReactor {
    class Reactor : public PhysicsEntity {
    public:
        struct ControlRodInfo {
            float relative_angle;
            float rail_progress;

            static nlohmann::json vectorToJson(const std::vector<ControlRodInfo> &info_vector) {
                nlohmann::json json = nlohmann::json::array();
                for (const ControlRodInfo &i: info_vector) {
                    nlohmann::json entry = {};
                    entry["relative_angle"] = i.relative_angle;
                    entry["rail_progress"] = i.rail_progress;
                    json.push_back(entry);
                }
                return json;
            }

            static std::vector<ControlRodInfo> vectorFromJson(const nlohmann::json &json) {
                std::vector<ControlRodInfo> info_vector;
                info_vector.reserve(json.size());
                for (auto info_json: json) {
                    info_vector.push_back({
                        info_json["relative_angle"],
                        info_json["rail_progress"]
                    });
                }
                return info_vector;
            }

            static std::vector<ControlRodInfo> range(int nb_rods, float start_angle = 0, float end_angle = 360,
                                                     float progress = 0.5f);

            static std::vector<ControlRodInfo> symmetricRange(int nb_rods, float start_angle = 0, float end_angle = 360,
                                                              float progress = 0.5f);
        };

    protected:
        FissionReactorBehavior behavior;
        std::vector<ControlRod *> control_rods;
        float output;

        void constructControlRods(const std::vector<ControlRodInfo> &control_rod_info);

    public:
        std::vector<ControlRod *> getControlRods() { return control_rods; }

        [[nodiscard]] float getOutput() const { return output; }

        Reactor(Vector2Int position, float angle, const std::vector<ControlRodInfo> &control_rod_info, float output);

        Entity *initializeRendering(const EntityRendering::Context &context) override { return this; }

        Entity *finalizeRendering(const EntityRendering::Context &context) override { return this; }

        void render(SDL_Renderer *renderer, const RenderingContext &context) override {
        }

        std::vector<Contraptions::FissionReactor::Reactor::ControlRodInfo> generate_rod_info() const;

        nlohmann::json toJson() override;

        FROM_JSON_DECLARATION(Reactor, "fission_reactor");

        void registerInInstance(Instances::Instance *world_instance) override;

        void unregisterInInstance(Instances::Instance *world_instance, bool delete_when_done) override;

        [[nodiscard]] bool interacts(PhysicsEntity *other) override;

        void update(const UpdateContext &context) override {
            behavior.update(context,this);
        }
    };
}
