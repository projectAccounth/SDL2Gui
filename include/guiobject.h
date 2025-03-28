#pragma once

#include "types.h"
#include "event.h"

namespace GUILib {

	namespace Reserved {
        /// helper function, __cplusplus refused to work
        double clamp(double val, double min, double max);
		/// helper function to check whether the point is in the rect
		bool isPointInRect(const SDL_Point& point, const SDL_Rect& rect);
    }

	/// @brief A struct to represent the size of a GUI object.
	typedef struct UIUnit {
		/// @brief The size of the object.
		double sizeX, sizeY;
		/// @brief Whether the size is using scale or not.
		bool isUsingScale;

		/// @brief Gets the absolute size of the object.
		/// @param containerSize The size of the container.
		/// @return The absolute size of the object, in pixels.
		SDL_Point getAbsoluteSize(const SDL_Point& containerSize) const;
    } UIUnit;
	 
	/// @brief A basic GUI object.
	/// @brief Can be used as a base for all GUI objects.
	class GuiObject {
	protected:
		/// @brief The rect of the object.
		SDL_Rect objRect;
		/// @brief The renderer of the object.
		SDL_Renderer*& ref;
		/// @brief The parent of the object.
		GuiObject* parent;

		/// @brief The offset of the drag.
		int dragOffsetX,
		    dragOffsetY;

		/// @brief The position of the object.
		UIUnit position;
		/// @brief The size of the object.
		UIUnit size;

		/// @brief Whether the object is being dragged.
		bool isDragging;
		/// @brief Whether the object is visible/or active.
		bool visible, active;
		/// @brief Whether the object can be dragged.
		bool canBeDragged;

		/// @brief The event emitter of the object.
		EventEmitter events;

		/// @brief Updates the object, the position and size.
		void update(SDL_Renderer* renderer);

		/// @brief The children of the object.
		std::vector<GuiObject*> children;

		/// @brief The children's rendering state.
		bool shouldRenderChildren;

		[[maybe_unused]]
		/// @brief The class name of the object.
		std::string className;

		/// @brief The class name.
		static inline const std::string CLASS_NAME = "GuiObject";
	public:
		GuiObject();
		GuiObject(
			GuiObject* parent,
			SDL_Renderer*& renderer,
			UIUnit size = UIUnit(),
			UIUnit position = UIUnit(),
			bool isVisible = true,
			bool isActive = true
		);

		/// @brief Returns the children's rendering state.
		/// @return The boolean that indicates whether the children should be rendered.
		bool getChildrenRenderingState() const;

		/// @brief Sets the children's rendering state (whether the children should be rendered).
		/// @param value The value.
		void setChildrenRenderingState(bool value);

		/// @brief Adds a child to the object.
		/// Fires the "onChildAdded" event.
		/// @param child The child to be added.
		void addChild(GuiObject* child);

		/// @brief Removes a child from the object.
		/// Fires the "onChildRemoved" event.
		/// @param child The child to be removbed. Can be nullptr.
		void removeChild(GuiObject* child);

		/// @brief Returns the rect of the object.
		/// @return The rect of the object.
		SDL_Rect getRect() const;

		/// @brief Returns the size of the object.
		/// @return The size of the object.
		UIUnit getSize() const;

		/// @brief Returns the position of the object.
		/// @return The position of the object.
		UIUnit getPosition() const;

		/// @brief Moves the object to a new position.
		/// Fires the "onPositionChange" event.
		/// @param newPos The new position.
		void move(const UIUnit& newPos);

		/// @brief Resizes the object.
		/// Fires the "onSizeChange" event.
		/// @param newSize The new size.
		void resize(const UIUnit& newSize);

		/// @brief Returns the activity state of the object.
		/// @return The activity state.
		bool isActive() const;

		/// @brief Sets the activity state of the object.
		/// Fires the "onActiveChange" event.
		/// @param value The new activity state.
		void setActive(bool value);

		/// @brief Returns the visibility of the object.
		/// @return The visibility state.
		bool isVisible() const;

		/// @deprecated No longer in use.
		/// @param value The new visibility state.
		/// Fires the "onVisibilityChange" event.
		/// @brief Toggles the visibility of the object.
		void toggleVisibility(bool value);

		/// @brief Sets the visibility of the object.
		/// Fires the "onVisibilityChange" event.
		/// @param value The new visibility state.
		void setVisible(bool value);

		/// @brief Handles the event for the object.
		/// @param event The event that will be processed.
		virtual void handleEvent(const SDL_Event& event);

		/// @brief Returns the parent of the object.
		/// @return The parent of the object.
		const GuiObject* getParent() const;

		/// @brief Sets the parent of the object.
		/// @param newParent The new parent of the object.
		void setParent(GuiObject* newParent);

		/// @brief Checks if the object has a parent.
		/// @return True if the object has a parent, false otherwise.
		bool hasParent() const;

		/// @brief Checks if the object is draggable.
		/// @return True if the object is draggable, false otherwise.
		bool isDraggable() const;

		/// @brief Sets the draggable state of the object.
		/// Fires the "onDraggableChange" event.
		/// @param val The new draggable state.
		void setDraggable(bool val);

		/// @brief Renders the object, and all its children, if it has.
		virtual void render();

		/// @brief Connects a callback to an event.
		/// @param eventName The name of the event.
		/// @param callback The callback function. Must contain the same arguments as the trigger function.
		template <typename... Args>
		void on(
			const std::string& eventName,
			std::function<void(Args...)> callback
		) {
			// can't hide the implementation as this is a template function
			events.connect(eventName, std::move(callback));
		}

		/// @brief Triggers an event.
		/// @param eventName The name of the event.
		/// @param args The arguments that will be fired with the event. See EventEmitter for more details.
		template <typename... Args>
		void trigger(
			const std::string& eventName,
			Args&&... args
		) {
			events.fire(eventName, std::forward<Args>(args)...); // Extra O(n)
		}

		/// @brief Updates the reference to the renderer of the object.
		/// Fires the "onRendererChange" event.
		/// @param renderer The new renderer.
		void updateRenderer(SDL_Renderer*& renderer);

		/// @brief Gets the current renderer.
		/// @return The current renderer.
		SDL_Renderer*& getCurrentRenderer() const;
		
		/// @brief Assignment operator.
		/// @param other The right-hand-side.
		GuiObject& operator=(const GuiObject& other);

		/// @brief Equality check.
		/// @param other The right-hand-side.
		bool operator==(const GuiObject& other) const;

		/// @brief Returns the class name of the object.
		/// @return The class name.
		std::string getClassName() const;

		/// @brief Destructor.
		virtual ~GuiObject();
	};
}
