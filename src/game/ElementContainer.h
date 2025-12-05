//
// Created by timon on 12/4/25.
//

#pragma once
#include <algorithm>
#include <unordered_set>
#include <vector>
#include <bits/std_function.h>

template<typename T>
class ElementContainer {
protected:
    std::vector<T> elements;

public:
    [[nodiscard]] std::vector<T> get() const;

    void add(std::vector<T> to_add);

    void add(T to_add);

    void remove(std::vector<T> to_remove);

    void remove(T to_remove);

    void removeAndDelete(std::vector<T> to_remove);

    void removeAndDelete(T to_remove);

    void sort(std::function<bool(T, T)> sorter);

    void clear() {
        elements.clear();
    }

    explicit ElementContainer(std::vector<T> elements) : elements(elements) {
    }

    ElementContainer() : elements({}) {
    }
};

template<typename T>
std::vector<T> ElementContainer<T>::get() const {
    return elements;
}

template<typename T>
void ElementContainer<T>::add(std::vector<T> to_add) {
    for (auto elem: to_add) {
        elements.push_back(elem);
    }
}

template<typename T>
void ElementContainer<T>::add(T to_add) {
    elements.push_back(to_add);
}

template<typename T>
void ElementContainer<T>::remove(std::vector<T> to_remove) {
    for (auto r: to_remove) {
        remove(r);
    }
}

template<typename T>
void ElementContainer<T>::remove(T to_remove) {
    elements.erase(
        std::remove_if(elements.begin(), elements.end(),
                       [&](const T &item) {
                           return to_remove == item;
                       }),
        elements.end()
    );
}

template<typename T>
void ElementContainer<T>::removeAndDelete(T to_remove) {
    elements.erase(
        std::remove_if(elements.begin(), elements.end(),
                       [&](const T &item) {
                           return to_remove == item;
                       }),
        elements.end()
    );

    delete to_remove;
}

template<typename T>
void ElementContainer<T>::sort(std::function<bool(T, T)> sorter) {
    std::ranges::stable_sort(elements, sorter);
}

template<typename T>
void ElementContainer<T>::removeAndDelete(std::vector<T> to_remove) {
    std::unordered_set<T> remove_set(to_remove.begin(), to_remove.end());


    elements.erase(
        std::remove_if(elements.begin(), elements.end(),
                       [&](const T &item) {
                           return remove_set.contains(item);
                       }),
        elements.end()
    );

    for (auto e: to_remove) {
        delete e;
    }
}
