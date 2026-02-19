#pragma once
#include <format>

#include "json.hpp"
#include "game/Update.h"
#include "modes/ModeIdle.h"
#include "modes/ModeLower.h"
#include "modes/ModeRaise.h"
//
// Created by timon on 2/19/26.
//

namespace Contraptions::FissionReactor::Behavior {
    class Mode;
}

namespace Contraptions::FissionReactor::Behavior {
    class StateMachine {
    public:
        enum ModeEnum {
            LOWER,
            RAISE,
            IDLE,
            // EMERGENCY_SHUTDOWN,
            // POWERED_OFF,
        };

    protected:
        ModeIdle idle_mode;
        ModeRaise raise_mode;
        ModeLower lower_mode;
        Mode *current_mode;
        ModeEnum current_mode_enum;

        // Mode emergency_shutdown_mode;
        // Mode lower_mode;

    public:
        explicit StateMachine(ModeEnum current_mode_enum)
            : current_mode_enum(current_mode_enum),
              idle_mode(0, 0.35f),
        raise_mode(0.05f),
        lower_mode(0.1f)
        {
            current_mode = determineMode(current_mode_enum);
        }

        void update(const UpdateContext & context, Reactor * reactor) {
            current_mode->update(context,reactor);
        }

    private:
        static std::unordered_map<ModeEnum, std::string> enum_name_map() {
            return {
                {LOWER, "lower"},
                {RAISE, "raise"},
                {IDLE, "idle"},
                // {EMERGENCY_SHUTDOWN, "emergency_shutdown"},
                // {POWERED_OFF, "powered_off"},
            };
        }

        static ModeEnum reverse_lookup(const std::unordered_map<ModeEnum, std::string> &map,
                                       std::string value_to_find) {
            for (const auto& [key,value]: map) {
                if (value == value_to_find) {
                    return key;
                }
            }
            throw std::logic_error(std::format("value : {} not found in map", value_to_find));
        }

        Mode *determineMode(ModeEnum mode_enum) {
            switch (mode_enum) {
                case LOWER:
                    return &lower_mode;
                case RAISE:
                    return &raise_mode;
                case IDLE:
                    return &idle_mode;
            }
            throw std::logic_error("Mode not handled");
        }

    public:
        void setMode(ModeEnum mode_enum) {
            current_mode = determineMode(mode_enum);
        }

        [[nodiscard]] nlohmann::json toJson() const {
            nlohmann::json json = {};
            json["mode"] = enum_name_map().at(current_mode_enum);
            return json;
        }

        static StateMachine fromJson(const nlohmann::json &json) {
            ModeEnum mode_enum = reverse_lookup(enum_name_map(), to_string(json["mode"]));
            return StateMachine(mode_enum);
        }
    };
}
