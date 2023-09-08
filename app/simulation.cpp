#include "simulation.h"

using namespace app;
using namespace util;
using namespace object;

void changeCoord(Coordinate& coord, Direction dir, float deltaDist) {
    switch (dir) {
    case Direction::LEFT:
        coord.x -= deltaDist;
        break;
    case Direction::RIGHT:
        coord.x += deltaDist;
        break;
    case Direction::UP:
        coord.y -= deltaDist;
        break;
    case Direction::DOWN:
        coord.y += deltaDist;
        break;
    }
}

bool isCartesianCoordForward(const Coordinate& testCoord, const Coordinate& coord, Direction dir) {
    const float EPS = 0.01f;

    switch (dir) {
    case Direction::LEFT:
        return testCoord.x + EPS < coord.x;
    case Direction::RIGHT:
        return testCoord.x - EPS> coord.x;
    case Direction::UP:
        return testCoord.y + EPS < coord.y;
    case Direction::DOWN:
        return testCoord.y - EPS> coord.y;
    }

    return false;
}

void app::moveObject(IDynamicObjectPtr object, FieldPtr field, clock_t deltaTimeMs) {
    if (object == nullptr || field == nullptr || !object->isMove()) {
        return;
    }

    if (!field->isWalkable( object->getEndPosition() )) {
        object->onHitWithBarrier();
        return;
    }

    Coordinate coord = object->getCartesianCoord();
    Coordinate oldCoord = coord;
    Position oldPos = field->getRowCol( field->getIsometricCoord(coord) );
    Direction dir = object->getDirection();
    Position nextPos = nextPosition(oldPos, dir);
    float deltaDist = object->getSpeed() * static_cast<float>(deltaTimeMs) / MS_IN_SECOND;
    changeCoord(coord, dir, deltaDist);

    Position newPos = field->getRowCol( field->getIsometricCoord(coord) );
    bool endOfStep = false;

    if (newPos == oldPos) {
        Coordinate tileCoord = field->getCartesianCoord(newPos);
        endOfStep = newPos == object->getEndPosition()
            && !isCartesianCoordForward(tileCoord, coord, dir);
    }
    else {
        IDynamicObjectPtr neighbour = field->getObject(newPos);
        if (neighbour != nullptr) {
            object->onHitWithObject(neighbour->getFraction());
            neighbour->onHitWithObject(object->getFraction());
        }

        IDynamicObjectPtr obj = field->getObject(oldPos);
        if (obj == object) {
            field->setObject(oldPos, nullptr);
        }

        if (object->isAlive()) {
            field->setObject(newPos, object);
        }
    }

    if (endOfStep) {
        object->setCartesianCoord( field->getCartesianCoord(newPos) );
        object->finishMotion();
    }
    else {
        object->setCartesianCoord(coord);
    }
}
