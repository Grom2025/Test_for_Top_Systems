#include <cmath>

#include "transformation.h"

using namespace std;
using namespace util;

SDL_Point util::convertToSdlPoint(const Coordinate& coord) {
    return {
        ROUND_TO_INT(coord.x),
        ROUND_TO_INT(coord.y)
    };
}

Coordinate util::convertToCoordinate(const SDL_Point& point) {
    return {
        static_cast<float>(point.x),
        static_cast<float>(point.y)
    };
}

Coordinate util::rotate(const Coordinate& point, float angle) {
    float x = point.x * cos(angle) - point.y * sin(angle);
    float y = point.x * sin(angle) + point.y * cos(angle);
    return { x, y };
}

Coordinate util::scale(const Coordinate& point, float xCoef, float yCoef) {
    return { xCoef * point.x, yCoef * point.y };
}
