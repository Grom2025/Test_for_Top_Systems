#pragma once
#include <cstdio>

namespace object {

    enum class Direction {
        DOWN                        = 0,
        RIGHT                       = 1,
        UP                          = 2,
        LEFT                        = 3,
    };

    bool                            isHorizontalDirection(Direction dir);
    bool                            isVerticalDirection(Direction dir);
    Direction                       invertDirection(Direction dir);

    struct Position {
        int                         row                 = -1;
        int                         column              = -1;
    };

    bool operator==(const Position& lhs, const Position& rhs);
    bool operator!=(const Position& lhs, const Position& rhs);
    bool operator<(const Position& lhs, const Position& rhs);

    Position nextPosition(Position position, Direction direction);

}
