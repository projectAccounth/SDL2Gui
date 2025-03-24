#pragma once

#include "types.h"

namespace GUILib {
    // Basic event 
    class Event {
    public:
        virtual ~Event() = default;
    };
    // Basic event dispatcher
    class EventDispatcher {
    private:
        using Callback = std::function<void(const Event&)>;
        std::unordered_map<std::string, std::vector<Callback>> listeners;
    public:
        
        void subscribe(
            const std::string& eventType,
            Callback callback
        );

        void dispatch(
            const std::string& eventType,
            const Event& event
        );
    };

    class EventEmitter {
    private:
        using Callback = std::function<void(const std::vector<std::any>&)>;
        std::unordered_map<std::string, std::vector<std::pair<size_t, Callback>>> listeners;
        size_t nextListenerId = 0;
    public:
        using EventId = size_t;

        // Register an event listener and return an ID for deregistration
        template <typename... Args>
        EventId connect(const std::string& eventName, std::function<void(Args...)> callback) {
            EventId id = nextListenerId++;

            auto wrapper = [callback](const std::vector<std::any>& args) {
                if (args.size() != sizeof...(Args)) {
                    throw std::runtime_error("Event argument mismatch!");
                }
                std::apply(callback, unpackArguments<Args...>(args, std::index_sequence_for<Args...>{}));
            };

            listeners[eventName].push_back({ id, wrapper });
            return id;
        }

        // Register an event listener that fires only once
        template <typename... Args>
        EventId connectOnce(const std::string& eventName, std::function<void(Args...)> callback) {
            EventId id = nextListenerId++;

            auto wrapper = [this, eventName, id, callback](const std::vector<std::any>& args) {
                callback(std::apply(callback, unpackArguments<Args...>(args, std::index_sequence_for<Args...>{})));
                this->disconnect(eventName, id); // Remove itself after execution
                };

            listeners[eventName].emplace_back(id, std::move(wrapper));
            return id;
        }

        template <typename... Args>
        void fire(const std::string& eventName, Args&&... args) {
            if (!listeners.count(eventName)) return;
            std::vector<std::any> packedArgs = { std::forward<Args>(args)... };
            for (const auto& callback : listeners[eventName]) {
                callback.second(packedArgs);
            }
        }

        void disconnect(
            const std::string& eventName,
            EventId listenerId
        );

    private:
        template <typename... Args, std::size_t... I>
        static std::tuple<Args...> unpackArguments(const std::vector<std::any>& args, std::index_sequence<I...>) {
            return std::make_tuple(std::any_cast<Args>(args[I])...);
        }
    };
}