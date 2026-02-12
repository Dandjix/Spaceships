#pragma once
#include <format>
#include <functional>
#include <string>
#include <unordered_map>

#include "EntityPlacementContext.h"
#include "Interface.h"
#include "entities/scripts/Entity.h"
//
// Created by timon on 2/9/26.
//

namespace EntityPlacement {
    class Interface;
    /**
     * This is used to register an entity so that it can be spawned in the editor
     * deserialized
     */
    class EntityFactory {
    private:
        std::unordered_map<
            std::string,
            std::function<std::future<Entity *>(EntityPlacement::Context * entity_placement_context)>
        >
        factories;

    public:
        static EntityFactory &getInstance() {
            static EntityFactory instance;
            return instance;
        }

        void insert(const std::string &key,
                    const std::function<std::future<Entity *>(EntityPlacement::Context * context)> &value) {
            auto [_,success] = factories.insert({key, value});

            if (!success)
                throw std::logic_error(
                    std::format(
                        "You have multiple classes with the same classId : {}, avoid this using prefixes and such",
                        key
                    )
                );
        }

        std::vector<std::string> getOptions() {
            std::vector<std::string> options;
            options.reserve(size());
            for (const auto& key: factories | std::views::keys) {
                options.push_back(key);
            }
            return options;
        }

        // Type aliases for iterator support
        using const_iterator = std::unordered_map<
            std::string,
            std::function<std::future<Entity *>(EntityPlacement::Context * context)>
        >::const_iterator;
        using size_type = std::unordered_map<
            std::string,
            std::function<std::future<Entity *>(EntityPlacement::Context * context)>
        >::size_type;

        // Iterators
        const_iterator begin() const { return factories.begin(); }

        const_iterator end() const { return factories.end(); }

        size_type size() { return factories.size(); }

        [[nodiscard]] const std::function<std::future<Entity *>(EntityPlacement::Context * context)> &at(
            const std::string &key) const {
            if (!contains(key)) {
                print();
                throw std::logic_error(std::format("Entity placement factory does not contain the key : {}",key));
            }
            return factories.at(key);
        }

        [[nodiscard]] bool contains(const std::string &key) const {
            return factories.contains(key);
        }


        void print() const {
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
