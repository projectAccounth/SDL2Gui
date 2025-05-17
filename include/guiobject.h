#pragma once

#include "types.h"
#include "event.h"

namespace GUILib {

	namespace Reserved {
		using TextureType = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;
		/// helper function, __cplusplus refused to work
		double clamp(double val, double min, double max);
		/// helper function to check whether the point is in the rect
		bool isPointInRect(const SDL_Point& point, const SDL_Rect& rect);
		/// creates box texture
		[[nodiscard]]
		TextureType createSolidBoxTexture(SDL_Renderer* r, SDL_Color c, int w, int h);

		/// helper function to rotate a point based on a origin point
		/// @param px X position of the point
		/// @param py Y position of the point
		/// @param ox X position of the origin
		/// @param oy Y position of the origin
		/// @returns A pair of rotated positions
		[[nodiscard]]
        std::pair<float, float> rotatePoint(float px, float py, float ox, float oy, float angle_rad);
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
		[[nodiscard]] SDL_Point getAbsoluteSize(const SDL_Point& containerSize) const;


		[[nodiscard]] bool operator==(const UIUnit& other) const;

	} UIUnit;
	
	/// @brief A basic GUI object.
	/// @brief Can be used as a base for all GUI objects.
	class GuiObject : public std::enable_shared_from_this<GuiObject> {
	protected:
		/// @brief The rect of the object.
		SDL_Rect objRect;
		/// @brief The reference to the renderer of the object. Stored internally.
		SDL_Renderer* ref;
		/// @brief The parent of the object.
		std::weak_ptr<GuiObject> parent;

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
		std::vector<std::shared_ptr<GuiObject>> children;

		/// @brief The children's rendering state.
		bool shouldRenderChildren;

		[[maybe_unused]]
		/// @brief The class name of the object.
		std::string className;

		/// @brief The class name.
		static inline const std::string CLASS_NAME = "GuiObject";
		
		/// @brief Serves as the rotating center of the object.
		UIUnit renderingPivotOffset;

		/// @brief Rotation of the object clockwise, in degrees.
		double degreeRotation;

		/// @brief The name of the object.
		std::string name;

		
		GuiObject(
			std::shared_ptr<GuiObject> parent, //!< what
			SDL_Renderer*& renderer,
			UIUnit size = UIUnit(),
			UIUnit position = UIUnit(),
			bool isVisible = true,
			bool isActive = true
		);
	public:
		// Default constructor. Not recommended to use.
		GuiObject();
		GuiObject(const GuiObject&) noexcept;
		GuiObject(GuiObject&&) noexcept;

		/// @brief A builder class for objects. Is the recommended way to create objects dependant of the factory.
		/// @tparam DerivedBuilder The derived (overridden) builder class.
		/// @tparam ProductType The final type of the object.
		template<typename DerivedBuilder, typename ProductType>
		class Builder {
		protected:
			/// @brief The internal type.
			std::shared_ptr<ProductType> obj;
		public:

			Builder() : obj(std::make_shared<ProductType>()) {}

			/// @brief Sets the parent.
			/// @param p The parent.
			/// @returns The current modified builder.
			virtual DerivedBuilder& setParent(std::shared_ptr<GuiObject> p)
			{
				obj->setParent(p);
				return static_cast<DerivedBuilder&>(*this);
			}

			/// @brief Sets the renderer.
			/// @param r The renderer.
			/// @returns The current modified builder.
			virtual DerivedBuilder& setRenderer(SDL_Renderer* r)
			{
				obj->updateRenderer(r);
				return static_cast<DerivedBuilder&>(*this);
			}

			/// @brief Sets the position.
			/// @param p The position.
			/// @returns The current modified builder.
			virtual DerivedBuilder& setPosition(UIUnit p)
			{
				obj->move(p);
				return static_cast<DerivedBuilder&>(*this);
			}

			/// @brief Sets the size.
			/// @param s The size.
			/// @returns The current modified builder.
			virtual DerivedBuilder& setSize(UIUnit s)
			{
				obj->resize(s);
				return static_cast<DerivedBuilder&>(*this);
			}

			/// @brief Sets the rotation.
			/// @param r The degree rotation.
			/// @returns The current modified builder.
			virtual DerivedBuilder& setRotation(double r)
			{
				obj->setRotation(r);
				return static_cast<DerivedBuilder&>(*this);
			}

			/// @brief Adds an existing child to an object.
			/// @param child The child.
			/// @returns The current modified builder.
			virtual DerivedBuilder& addChild(const std::shared_ptr<GuiObject>& child)
			{
				obj->addChild(std::move(child));
				return static_cast<DerivedBuilder&>(*this);
			}

			/// @brief Sets the visibility. Will be default to false if not set.
			/// @param val The value.
			/// @returns The current modified builder.
			virtual DerivedBuilder& setVisible(bool val)
			{
				obj->setVisible(val);
				return static_cast<DerivedBuilder&>(*this);
			}

			/// @brief Sets the activity of the object. Will be default to false if not set.
			/// @param val The value.
			/// @returns The current modified builder.
			virtual DerivedBuilder& setActive(bool val)
			{
				obj->setActive(val);
				return static_cast<DerivedBuilder&>(*this);
			}

			/// @brief Finalizes the object.
			/// @returns The final created object.
			[[nodiscard]] virtual std::shared_ptr<ProductType> build() const
			{
				return std::static_pointer_cast<ProductType>(obj);
			}

			virtual ~Builder() = default;
		};

        /// @brief Checks whether the passed object is an ascendant of this.
        /// @param other The object to check.
        /// @returns Whether the object `other` is the ascendant of this. Does not need to be the next.
        bool isAscendantOf(const std::shared_ptr<const GuiObject>& other) const;

		/// @brief Checks whether the passed object is a descendant of this.
		/// @param other The object to check.
		/// @returns Whether the object `other` is the descendant of this. Does not need to be the next.
        bool isDescendantOf(const std::shared_ptr<const GuiObject>& other) const;

        /// @brief Returns the children's rendering state.
		/// @return The boolean that indicates whether the children should be rendered.
		bool getChildrenRenderingState() const;

		/// @brief Sets the children's rendering state (whether the children should be rendered).
		/// @param value The value.
		void setChildrenRenderingState(bool value);

		/// @brief Adds a child to the object.
		/// Fires the "onChildAdded" event.
		/// @param child The child to be added.
		bool addChild(const std::shared_ptr<GuiObject>& child);

		/// @brief Removes a child from the object.
		/// Fires the "onChildRemoved" event.oi
		/// @param child The child to be removed. Can be nullptr.
		bool removeChild(const std::shared_ptr<GuiObject>& child);

		/// @brief Returns the rect of the object.
		/// @return The rect of the object.
		SDL_Rect getRect() const;

		/// @brief Changes the pivot offset of the object.
		/// Fires the "onPivotOffsetChange" event.
		/// @param offset The new offset.
		void setPivotOffset(const UIUnit& offset);

		/// @brief Returns the offset from the origin of the object to the offset point.
		/// @returns The offset pivot, in an SDL_Point.
		SDL_Point getPivotOffsetPoint() const;

		/// @brief Returns the offset from the origin of the object to the offset point.
		/// @returns The offset pivot, in an SDL_Point.
		UIUnit getPivotOffset() const;

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
		const std::weak_ptr<GuiObject>& getParent() const;

		/// @brief Sets the parent of the object.
		/// @param newParent The new parent of the object.
		bool setParent(const std::shared_ptr<GuiObject>& newParent);

		/// @brief Checks if the object has a parent.
		/// @return True if the object has a parent, false otherwise.
		bool hasParent() const noexcept;

		/// @brief Checks if the object is draggable.
		/// @return True if the object is draggable, false otherwise.
		bool isDraggable() const noexcept;

		/// @brief Sets the draggable state of the object.
		/// Fires the "onDraggableChange" event.
		/// @param val The new draggable state.
		void setDraggable(bool val);

		/// @brief Renders the object, and all its children, if it has.
		virtual void render() = 0;

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
			events.fire(eventName, std::forward<Args>(args)...);
		}

		void resetListeners(const std::string& eventName);

		/// @brief Updates the reference to the renderer of the object.
		/// Fires the "onRendererChange" event.
		/// @param renderer The new renderer.
		void updateRenderer(SDL_Renderer* renderer);

		/// @brief Gets the current renderer.
		/// @return The current renderer.
		[[nodiscard]] SDL_Renderer* getCurrentRenderer() const;
		
		/// @brief Assignment operator.
		/// @param other The right-hand-side.
		GuiObject& operator=(const GuiObject& other);

		GuiObject& operator=(GuiObject&& other) noexcept;

		/// @brief Equality check.
		/// @param other The right-hand-side.
		bool operator==(const GuiObject& other) const;

		/// @brief Returns the class name of the object.
		/// @return The class name.
		inline virtual std::string getClassName() const { return "GuiObject"; };

		/// @brief Returns the rotation of the object, in the unit of degrees.
		/// @return The rotation of the object.
		[[nodiscard]] double getRotation() const;

		/// @brief Sets the rotation of the object.
		/// Fires the "onRotationChange" event.
		/// @param rotation The new rotation of the object.
		void setRotation(const double& rotation);

		/// @brief Indicates whether the object should be rendered, based on the visibility, renderer and parent.
		/// @returns The value.
		bool shouldRender() const;

		/// @brief Returns some of the information (is draggable, size, position, etc.)
		/// @returns The string that contains the specified information.
		std::string getEssentialInformation() const;

		/// @brief Dummy function for initializing the object.
		/// @param r The renderer.
		virtual void initialize(SDL_Renderer* r);
		
		/// @brief Destructor.
		virtual ~GuiObject();
	};
}
