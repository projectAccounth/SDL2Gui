#pragma once

#if __cplusplus < 201703L
#error "This library requires C++17 or above."
#endif

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <functional>
#include <variant>
#include <type_traits>
#include <unordered_map>
#include <memory>
#include <optional>
#include <any>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <cstdint>

namespace GUILib {

    enum class HorizontalTextAlign : uint8_t {
        CENTER,
        LEFT,
        RIGHT
    };


    enum class VerticalTextAlign : uint8_t {
        CENTER,
        UP,
        BOTTOM
    };

}
