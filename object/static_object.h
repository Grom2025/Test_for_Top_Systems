#pragma once

#include <memory>

#include "position.h"

#include "../media/animation.h"

namespace object {

    class IStaticObject {
    public:
        IStaticObject(const media::AnimationPtr animation);

        virtual void                setDirection(Direction direction);
        Direction                   getDirection() const noexcept;
        const media::AnimationPtr   getAnimation() const noexcept;
        virtual void                update() = 0;

    protected:
        Direction                   _direction;
        const media::AnimationPtr   _animation;
    };

    using IStaticObjectPtr          = std::shared_ptr<IStaticObject>;

}
