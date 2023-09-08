#include "static_object.h"

using namespace media;
using namespace object;

IStaticObject::IStaticObject(AnimationPtr animation)
    : _animation(animation)
    , _direction(Direction::LEFT)
{
}

void IStaticObject::setDirection(Direction direction) {
    _direction = direction;
}

Direction IStaticObject::getDirection() const noexcept {
    return _direction;
}

const AnimationPtr IStaticObject::getAnimation() const noexcept {
    return _animation;
}
