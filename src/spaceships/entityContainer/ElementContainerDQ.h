#pragma once
//
// Created by timon on 2/1/26.
//

#include <format>
#include <vector>

template<class T>
class ElementContainerDQ {
private:
    std::vector<T> container;
    std::vector<std::pair<T, bool> > deletion_queue;

public:
    ElementContainerDQ() = default;

    explicit ElementContainerDQ(std::vector<T> values) : container(values), deletion_queue({}) {
    }

    [[nodiscard]] std::vector<T> get() const { return container; }

    // Type aliases for iterator support
    using iterator = std::vector<T>::iterator;
    using const_iterator = std::vector<T>::const_iterator;
    using size_type = std::vector<T>::size_type;

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

    void insert(const std::vector<T> & values) {
        for (T v: values)
            container.push_back(v);
    }

    void add_to_erasing_queue(T value, bool delete_when_done = false) {
        deletion_queue.push_back({value, delete_when_done});
    }

    void add_to_erasing_queue(const std::vector<T> & values, bool delete_when_done = false) {
        for (T v: values)
            deletion_queue.push_back({v, delete_when_done});
    }

    void process_deletion_queue() {
        for (auto [value,delete_when_done]: deletion_queue) {
            erase_now(value);
        }
        for (auto [value,delete_when_done]: deletion_queue) {
            if (delete_when_done)
                delete value;
        }
        deletion_queue.clear();
    }

    void erase_now(T key) {
        auto it = std::find(container.begin(), container.end(), key);

        if (it == container.end()) {
            throw std::runtime_error(
                std::format("Value not present in container")
            );
        }
        container.erase(it);
    }

    void erase_index(size_type index) {
        container.erase(container.begin() + index);
    }

    bool contains(const T &key) const {
        return std::find(container.begin(), container.end(), key) != container.end();
    }

    T at(size_type index) const {
        return container.at(index);
    }

    size_type size() const {
        return container.size();
    }

    void sort(std::invocable<T, T> auto compare) {
        std::sort(container.begin(), container.end(), compare);
    }
};
