#pragma once
#include <format>
#include <functional>
#include <string>
#include <unordered_map>

#include "json.hpp"
#include "loadGame/GameState.h"
//
// Created by timon on 2/1/26.
//

namespace EntityLoading {
    class EntityFactory {
    private:
        std::unordered_map<
            std::string,
            std::function<Entity *(const nlohmann::json &, GameState::transientGameState &)>
        >
        factories;

    public:
        static EntityFactory &getInstance() {
            static EntityFactory instance;
            return instance;
        }

        void insert(const std::string &key,
                    const std::function<Entity *(const nlohmann::json &, GameState::transientGameState &)> &value) {
            auto [_,success] = factories.insert({key, value});

            if (!success)
                throw std::logic_error(
                    std::format(
                        "You have multiple classes with the same classId : {}, avoid this using prefixes and such",
                        key
                    )
                );
        }

        [[nodiscard]] const std::function<Entity *(const nlohmann::json &, GameState::transientGameState &)> &at(
            const std::string &key) const {
            return factories.at(key);
        }

        [[nodiscard]] bool contains(const std::string &key) const {
            return factories.contains(key);
        }


        void print() {
            std::string keys;
            int count = 0;

            for (const auto &k: factories | std::views::keys) {
                keys += k + ", ";
                count++;
            }

            std::cout << "I have keys : " << keys << "for a total of " << count << std::endl;
        }
    };
}
