#pragma once

#include "types.h"

namespace GUILib {
    /// Basic event.
    class Event {
    public:
        virtual ~Event() = default;
    };
    /// Basic event dispatcher.
    /**
     * Example usage of the class and the event
     * @code
     * class MyEvent : public Event {
     * public:
     *    int data; // anything you want goes here!
     *    MyEvent(int data) : data(data) {}
     * };
     * 
     * EventDispatcher dispatcher;
     * 
     * dispatcher.subscribe("myEvent", [](const Event& event) {
     *    auto& myEvent = dynamic_cast<const MyEvent&>(event);
     *    std::cout << "Data: " << myEvent.data << std::endl;
     * });
     * 
     * dispatcher.dispatch("myEvent", MyEvent(42)); // Data: 42
     * @endcode
     */
    class EventDispatcher {
    private:
        using Callback = std::function<void(const Event&)>;
        std::unordered_map<std::string, std::vector<Callback>> listeners;
    public:
        
        /// @brief Subscribes a listener to an event.
        /// @param eventType The type of the event.
        /// @param callback The callback function.
        /**
         * Example usage:
         * @code
         * class MyEvent : public Event {
         * public:
         *    int data;
         *    MyEvent(int data) : data(data) {}
         * };
         * 
         * dispatcher.subscribe("myEvent", [](const Event& event) {
         *     auto& myEvent = dynamic_cast<const MyEvent&>(event);
         *     // do something!
         * });
         * @endcode
         */
        void subscribe(
            const std::string& eventType,
            Callback callback
        );

        /// @brief Dispatches an event.
        /// @param eventType The type of the event.
        /// @param event The event to be dispatched.
        /**
         * Example usage:
         * @code
         * class MyEvent : public Event {
         * public:
         *    int data;
         *    MyEvent(int data) : data(data) {}
         * };
         * 
         * dispatcher.dispatch("myEvent", MyEvent(2));
         * @endcode
         */
        void dispatch(
            const std::string& eventType,
            const Event& event
        );
    };

    /// @brief Basic event emitter.
    class EventEmitter {
    private:
        /// @brief The wrapper type for callbacks.
        using Callback = std::function<void(const std::vector<std::any>&)>;
        /// @brief The callback map with associated event name.
        std::unordered_map<std::string, std::vector<std::pair<size_t, Callback>>> listeners;
        
        size_t nextListenerId = 0;
    public:
        using EventId = size_t;

        /// @brief Register an event listener.
        /// @param eventName The name of the event.
        /// @param callback The callback function.
        /// @return An EventId that can be further used for deregistration.
        /**
         * This function registers a new event listener, with the passed callback being triggered once fire() is called.
         * @code
         * EventEmitter emitter;
         *
         * emitter.connect<int, int>("myEvent", [](int a, int b) {
         *     std::cout << "a + b = " << a + b << std::endl;
         * });
         *
         * // or
         *
         * emitter.connect("myEvent", std::function<void(int, int)>([](int a, int b) {
         *     // do something
         * });
         *
         * emitter.fire("myEvent", 1, 2); // Outputs "a + b = 3"
         * // Note that the first usage will trigger an IDE error as it cannot instantiate the template!
         * @endcode
         */
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

        /// @brief Register an event listener that fires only once.
        /// @param eventName The name of the event.
        /// @param callback The callback function.
        /// @return An EventId that can be further used for deregistration.
        /**
         * Fires a new event, which will then trigger all listeners registered with event name of eventName.
         * @code
         * EventEmitter emitter;
         * UIUnit objectPosition;
         * 
         * emitter.connectOnce<UIUnit>("myEvent", [](UIUnit pos) {
         *     std::cout << "Object position: " << pos.sizeX << ", " << pos.sizeY << std::endl;
         * });
         * 
         * // or
         * 
         * emitter.connectOnce("myEvent", std::function<void(UIUnit)>([](UIUnit pos) {
         *     // do something...
         * });
         * 
         * // supposing that we only do that once.
         * 
         * emitter.fire("myEvent", objectPosition); // Outputs "Object position: x, y"
         * emitter.fire("myEvent", objectPosition); // Nothing prints
         * // Note that the first usage will trigger an IDE error as it cannot instantiate the template!
         * @endcode
         */
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

        /**
         * @brief Fires an event, invoking all registered listeners for the specified event name.
         * 
         * @tparam Args The types of the arguments to pass to the event listeners.
         * @param eventName The name of the event to fire.
         * @param args The arguments to pass to the event listeners.
         * 
         * This function triggers all callbacks associated with the given event name,
         * passing the provided arguments to each callback (registered with connect()). If no listeners are registered
         * for the event, the function does nothing.
         * 
         * @code
         * EventEmitter emitter;
         * 
         * emitter.connect<int, int>("myEvent", [](int a, int b) {
         *     std::cout << "a + b = " << a + b << std::endl;
         * });
         * 
         * // or
         * 
         * emitter.connect("myEvent", std::function<void(int, int)>([](int a, int b) {
         *     // do something
         * });
         * 
         * emitter.fire("myEvent", 1, 2); // Outputs "a + b = 3"
         * // Note that the first usage will trigger an IDE error as it cannot instantiate the template!
         * @endcode
         */
        template <typename... Args>
        void fire(const std::string& eventName, Args&&... args) {
            if (!listeners.count(eventName)) return;
            std::vector<std::any> packedArgs = { std::forward<Args>(args)... };
            for (const auto& callback : listeners[eventName]) {
                callback.second(packedArgs);
            }
        }

        /// @brief Deregister an event listener.
        /// @param eventName The name of the event.
        /// @param listenerId The ID recieved from registering the listener.
        /**
         * @code
         * EventEmitter emitter;
         * 
         * EventId listenerId = emitter.connect("myEvent", []() {
         *     // do something
         * });
         * 
         * // later...
         * 
         * emitter.disconnect("myEvent", listenerId);
         * 
         * emitter.fire("myEvent"); // No callbacks will be triggered
         * @endcode
         */
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