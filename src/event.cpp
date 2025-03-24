#include "event.h"

using namespace GUILib;

void EventDispatcher::subscribe(const std::string& eventType, Callback callback) {
    listeners[eventType].push_back(std::move(callback));
}

void EventDispatcher::dispatch(const std::string& eventType, const Event& event) {
    if (listeners.count(eventType)) {
        for (const auto& callback : listeners[eventType]) {
            callback(event);
        }
    }
}

void EventEmitter::disconnect(const std::string& eventName, EventId listenerId) {
    if (listeners.count(eventName)) {
        auto& vec = listeners[eventName];
        vec.erase(std::remove_if(vec.begin(), vec.end(),
            [listenerId](const auto& pair) { return pair.first == listenerId; }),
            vec.end());

        // Remove the event entry if there are no more listeners
        if (vec.empty()) {
            listeners.erase(eventName);
        }
    }
}