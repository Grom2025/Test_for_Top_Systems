#include "position.h"

using namespace object;

bool object::isHorizontalDirection(Direction dir) {
    return dir == Direction::LEFT || dir == Direction::RIGHT;
}

bool object::isVerticalDirection(Direction dir) {
    return dir == Direction::UP || dir == Direction::DOWN;
}

Direction object::invertDirection(Direction dir) {
    switch (dir) {
    case Direction::DOWN:
        return Direction::UP;
    case Direction::UP:
        return Direction::DOWN;
    case Direction::LEFT:
        return Direction::RIGHT;
    case Direction::RIGHT:
        return Direction::LEFT;
    }
    return dir;
}

bool object::operator==(const Position& lhs, const Position& rhs) {
    return lhs.row == rhs.row && lhs.column == rhs.column;
}

bool object::operator!=(const Position& lhs, const Position& rhs) {
    return lhs.row != rhs.row || lhs.column != rhs.column;
}

bool object::operator<(const Position& lhs, const Position& rhs) {
    if (lhs.row == rhs.row) {
        return lhs.column < rhs.column;
    }
    return lhs.row < rhs.row;
}

Position object::nextPosition(Position position, Direction direction) {
    switch (direction) {
    case Direction::DOWN:
        ++position.row;
        break;
    case Direction::UP:
        --position.row;
        break;
    case Direction::LEFT:
        --position.column;
        break;
    case Direction::RIGHT:
        ++position.column;
        break;
    }
    return position;
}
