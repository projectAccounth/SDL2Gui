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
        
        inline void subscribe(const std::string& eventType, Callback callback) {
            listeners[eventType].push_back(std::move(callback));
        }

        inline void dispatch(const std::string& eventType, const Event& event) {
            if (listeners.count(eventType)) {
                for (const auto& callback : listeners[eventType]) {
                    callback(event);
                }
            }
        }
    };

    class EventEmitter {
    private:
        using Callback = std::function<void(std::vector<std::any>)>;
        std::unordered_map<std::string, std::vector<Callback>> listeners;
    public:
        template <typename... Args>
        void connect(const std::string& eventName, std::function<void(Args...)> callback) {
            auto wrapper = [callback](const std::vector<std::any>& args) {
                if (args.size() != sizeof...(Args)) {
                    throw std::runtime_error("Event argument mismatch!");
                }
                // Unpack arguments and invoke the callback
                std::apply(callback, unpackArguments<Args...>(args, std::index_sequence_for<Args...>{}));
                };
            listeners[eventName].push_back(std::move(wrapper));
        }

        template <typename... Args>
        void fire(const std::string& eventName, Args&&... args) {
            if (listeners.count(eventName)) {
                std::vector<std::any> packedArgs = { std::forward<Args>(args)... };
                for (const auto& callback : listeners[eventName]) {
                    callback(packedArgs);
                    // Packing the actual arguments in the vector,
                    // which can then be used in the callback.
                }
            }
        }

        // TODO: implement something to disconnect the events;

    private:
        template <typename... Args, std::size_t... I>
        static std::tuple<Args...> unpackArguments(const std::vector<std::any>& args, std::index_sequence<I...>) {
            return std::make_tuple(std::any_cast<Args>(args[I])...);
        }
    };
}