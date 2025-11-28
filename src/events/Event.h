//
// Created by timon on 11/21/25.
//

#pragma once
#include <functional>
#include <unordered_map>
#include <cstdint>

template <typename... Args>
class Event
{
public:
    using Callback = std::function<void(Args...)>;

    /**
     * Subscribes a callback to this event
     * @param cb the callback to subscribe
     * @return the id of the callback, used for unsubscribing
     */
    uint64_t subscribe(Callback cb)
    {
        uint64_t id = next_id++;
        subscribers[id] = std::move(cb);
        return id;
    }

    /**
     * Unsubscribes an event using
     * @param id the id that was returned from subscribe
     */
    void unsubscribe(uint64_t id)
    {
        subscribers.erase(id);
    }

    /**
     * Calls the callback for every subscriber
     * @param args the arguments of the callback
     */
    void emit(Args... args)
    {
        auto keys = subscribers;
        for (auto& [id, cb] : keys)
        {
            if (cb && subscribers.contains(id)) // still valid?
                cb(args...);
        }
    }

    /**
     * Clears the event of callbacks. Does not delete anything
     */
    void clear() {
        subscribers.clear();
    }

private:
    uint64_t next_id = 0;
    std::unordered_map<uint64_t, Callback> subscribers;
};
