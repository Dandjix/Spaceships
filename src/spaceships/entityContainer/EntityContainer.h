#pragma once
//
// Created by timon on 2/1/26.
//

#include <format>
#include <vector>

template<class T>
class EntityContainer {
private:
    std::vector<T> container;

public:
    // Type aliases for iterator support
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;
    using value_type = T;
    using size_type = typename std::vector<T>::size_type;

    // Iterators
    iterator begin() { return container.begin(); }
    const_iterator begin() const { return container.begin(); }

    iterator end() { return container.end(); }
    const_iterator end() const { return container.end(); }

    // Modifiers
    void clear() { container.clear(); }

    void insert(T value) {
        container.push_back(value);
    }

    void erase(T key) {
        auto it = std::find(container.begin(), container.end(), key);

        if (it == container.end()) {
            throw std::runtime_error(
                std::format("Value not present in container")
            );
        }
        container.erase(it);
    }

    void erase_index(size_type index) {
        container.erase(container.begin()+index);
    }

    bool contains(const T& key) const {
        return std::find(container.begin(), container.end(), key) != container.end();
    }

    T at(size_type index) const {
        return container.at(index);
    }

    size_type size() const {
        return container.size();
    }
};
