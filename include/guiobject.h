#pragma once

#include "types.h"
#include "event.h"

namespace GUILib {
	typedef struct UIUnit {
		double sizeX, sizeY;
		bool isUsingScale;
	} UIUnit;

	class GuiObject {
	protected:
		SDL_Rect objRect;
		SDL_Renderer*& ref;
		GuiObject* parent;

		int dragOffsetX;
		int dragOffsetY;

		UIUnit position;
		UIUnit size;

		bool isDragging;
		bool visible, active;
		bool canBeDragged;

		EventEmitter events;

		void update(SDL_Renderer* renderer);
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

		SDL_Rect getRect() const;

		UIUnit getSize() const;

		UIUnit getPosition() const;

		// Moves the object to the specified location.
		void move(const UIUnit& newPos);

		// Resizes the object.
		void resize(const UIUnit& newSize);

		bool isActive() const;
		inline void setActive(bool value) { 
			active = value;
			trigger("onActiveChange");
		}

		bool isVisible() const;
		inline void toggleVisiblility(bool value) {
			visible = value;
			trigger("onVisibilityChange");
		}

		void handleEvent(const SDL_Event& event);

		inline const GuiObject* getParent() const { return parent; }
		inline void setParent(GuiObject* newParent) { 
			parent = newParent;
			trigger("onParentChange");
		}
		inline bool hasParent() const { return parent != nullptr; }

		inline bool isDraggable() const { return canBeDragged; }
		inline void setDraggable(bool val) { 
			canBeDragged = val;
			trigger("onDraggableChange");
		}

		virtual void render() = 0;

		template <typename... Args>
		void on(
			const std::string& eventName,
			std::function<void(Args...)> callback
		) {
			events.connect(eventName, std::move(callback));
		}

		template <typename... Args>
		void trigger(
			const std::string& eventName,
			Args&&... args
		) {
			events.fire(eventName, std::forward<Args>(args)...); // Extra O(n)
		}

		inline void updateRenderer(SDL_Renderer*& renderer) {
			if (renderer == ref) return;
			ref = renderer;
			trigger("onRendererUpdate");
		}
		
		GuiObject& operator=(const GuiObject& other);

		virtual ~GuiObject() = default;
	};

	class Frame : public GuiObject {
	private:
		SDL_Color frameColor;
	public:
		Frame();
		Frame(
			GuiObject* parent,
			SDL_Renderer*& renderer,
			UIUnit size = UIUnit(),
			UIUnit position = UIUnit(),
			SDL_Color frameColor = SDL_Color(),
			bool isVisible = true,
			bool isActive = true
		);

		inline void setFrameColor(const SDL_Color& color) { frameColor = color; }
		inline SDL_Color getFrameColor() const { return frameColor; }

		void render() override;
	};
}
