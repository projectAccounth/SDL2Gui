#pragma once

#include "GuiObject.h"

namespace GUILib {
    enum class TweenType {
        SIZE,
        POSITION
    };

	struct TweenInfo {
        bool active = false;
        double elapsed = 0.0;
        double duration = 0.0;
        GUILib::UIUnit start;
        GUILib::UIUnit end;
        TweenType type;
	};

	class TweenService {
    protected:
        static inline bool validateSizeScale(const UIUnit& u1, const UIUnit& u2) {
            return u1.isUsingScale == u2.isUsingScale;
        }
	public:
        static inline void startTween(const std::shared_ptr<GuiObject>& obj, TweenInfo& state, const TweenType& type, const GUILib::UIUnit& to, double duration) {
            switch (type) {
            case TweenType::SIZE: state.start = obj->getSize(); break;
            case TweenType::POSITION: state.start = obj->getPosition(); break;
            }
            if (!validateSizeScale(state.start, to)) {
                std::cout << "Tween position type mismatch. Please use 2 UIUnits with the same isUsingScale property.\n";
                return;
            }
            state.end = to;
            state.duration = duration;
            state.elapsed = 0.0;
            state.active = true;
            state.type = type;
        }

        // Call each frame!
        static inline void updateTween(const std::shared_ptr<GuiObject>& obj, TweenInfo& state, double deltaTime) {
            if (!state.active) return;
            state.elapsed += deltaTime;
            double t = std::min(state.elapsed / state.duration, 1.0);

            GUILib::UIUnit newPos;
            newPos.sizeX = state.start.sizeX + (state.end.sizeX - state.start.sizeX) * t;
            newPos.sizeY = state.start.sizeY + (state.end.sizeY - state.start.sizeY) * t;
            newPos.isUsingScale = state.start.isUsingScale;

            switch (state.type) {
            case TweenType::POSITION: obj->move(newPos); break;
            case TweenType::SIZE: obj->resize(newPos); break;
            }

            if (t >= 1.0) {
                state.active = false;
            }
        }
	};
}