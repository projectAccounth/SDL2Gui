#pragma once

#if __cplusplus < 201703L
#error "This library requires C++17 or above."
#endif

// Include guard
#ifndef TYPES_H
#define TYPES_H

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

enum TextAlign {
    LEFT,
    CENTER,
    RIGHT
};

#endif /* TYPES_H */
