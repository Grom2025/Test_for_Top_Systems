#pragma once

#include "SDL.h"

#define ROUND_TO_INT(x) static_cast<int>( round(x) )

namespace util {
    
    struct Coordinate {
        float                       x                   = 0.0f;
        float                       y                   = 0.0f;
    };

    const float                     PI                  = 3.14159f;
    const float                     HALF_PI             = 0.5f * PI;
    const float                     QUATER_PI           = 0.25f * PI;

    SDL_Point                       convertToSdlPoint(const Coordinate& coord);
    Coordinate                      convertToCoordinate(const SDL_Point& point);
    Coordinate                      rotate(const Coordinate& coord, float angle);
    Coordinate                      scale(const Coordinate& coord, float xCoef, float yCoef);

}
