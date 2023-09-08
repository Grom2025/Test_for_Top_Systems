#include "dynamic_object.h"

using namespace util;
using namespace media;
using namespace object;

IDynamicObject::IDynamicObject(Fraction fraction, const media::AnimationPtr animation, float speed)
    : IStaticObject(animation)
    , _fraction(fraction)
    , _beginPosition({0, 0})
    , _endPosition({ 0, 0 })
    , _cartesianCoord({ 0.0f, 0.0f })
    , _speed(speed)
    , _alive(true)
    , _moveFlag(false)
{
    if (_animation != nullptr) {
        _animation->play();
    }
}

Fraction IDynamicObject::getFraction() const noexcept {
    return _fraction;
}

Position IDynamicObject::getBeginPosition() const noexcept {
    return _beginPosition;
}

Position IDynamicObject::getEndPosition() const noexcept {
    return _endPosition;
}

void IDynamicObject::setPosition(const Position& position) noexcept {
    _beginPosition = _endPosition = position;
}

Coordinate IDynamicObject::getCartesianCoord() const noexcept {
    return _cartesianCoord;
}

void IDynamicObject::setCartesianCoord(const Coordinate& cartesianPosition) noexcept {
    _cartesianCoord = cartesianPosition;
}

float IDynamicObject::getSpeed() const noexcept {
    return _speed;
}

void IDynamicObject::setSpeed(float speed) noexcept {
    _speed = speed;
}

bool IDynamicObject::isAlive() const noexcept {
    return _alive;
}

void IDynamicObject::setAlive(bool alive) noexcept {
    _alive = alive;
}

bool IDynamicObject::isMove() const noexcept {
    return _moveFlag;
}

void IDynamicObject::setMoveFlag(bool moveFlag) {
    _moveFlag = moveFlag;
}

void IDynamicObject::startMotion(Direction direction) {
    setDirection(direction);
    _endPosition = nextPosition(_beginPosition, direction);
    _moveFlag = true;
}

void IDynamicObject::finishMotion() {
    _beginPosition = _endPosition;
    _moveFlag = false;
}

void IDynamicObject::resetMotion() {
    _endPosition = _beginPosition;
    _moveFlag = false;
}

void IDynamicObject::onHitWithObject(Fraction) {
}
