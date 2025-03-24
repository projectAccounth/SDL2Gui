### This section contains information about events in this tookit.

#### `class Event`
A base class for events. Can be inherited to store whatever information (for developer).

#### `class EventDispatcher`
A simple depatcher for events.\
**Methods and properties summary:**

##### `private using Callback = std::function<void(const Event&)>`
Wrapper type.

##### `private std::unordered_map<std::string, std::vector<Callback>> listeners`
The map which contains information about events and its listeners.

##### `public void subscribe(const std::string& eventType, Callback callback)`
Subscribes the associated callback function, which will be ran when the event `eventType` is dispatched (broadcasted).\
"Registers" the event if there's no listener listening from the event `eventType`.\
More than one listeners can be registered.

##### `public void dispatch(const std::string& eventType, const Event& event)`
Dispatch (broadcast) the event `eventType` globally to all listeners.

#### `class EventEmitter`
A simple implementation for an event emitter.\
Is the one used in classes' implementations.\
**Methods and properties summary:**

##### `private Callback = std::function<void(const std::vector<std::any>&)>`
Wrapper type.

##### `private std::unordered_map<std::string, std::vector<std::pair<size_t, Callback>>> listeners`
A map that stores information about event name, and its associated callbacks.

##### `private size_t nextListenerId`
