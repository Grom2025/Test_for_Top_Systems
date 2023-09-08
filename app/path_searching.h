#pragma once

#include <deque>
#include <vector>

#include "../object/field.h"
#include "../object/position.h"

namespace app {

    const int                       STATE_EMPTY         = -1;
    const int                       STATE_BARRIER       = -2;

    using PathMatrix                = std::vector<std::vector<int>>;

    PathMatrix                      generatePathMatrix(object::FieldPtr field);
    void                            fillDistance(PathMatrix& matrix, const object::Position& fromPos, const object::Position& toPos);
    std::deque<object::Direction>   searchPath(const PathMatrix& matrix, const object::Position& endPos);

}
