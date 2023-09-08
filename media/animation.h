#pragma once

#include <ctime>
#include <vector>
#include <memory>
#include <cstdint>

#include "sprite.h"

namespace media {

    using Sprites                   = std::vector<SpritePtr>;

    class Animation {
    public:
        Animation(const Sprites& sprites, uint32_t delayMs = 25);

        SpritePtr                   getSprite() const;
        void                        setDelay(uint32_t delayMs) noexcept;
        uint32_t                    getDelay() const noexcept;
        uint32_t                    getFrame() const noexcept;
        uint32_t                    getFramesCount() const noexcept;
        bool                        isRunning() const noexcept;
        void                        play() noexcept;
        void                        stop() noexcept;
        void                        goToAndStop(uint32_t frame) noexcept;
        void                        goToAndPlay(uint32_t frame) noexcept;
        void                        update() noexcept;

    private:
        static const uint32_t       MIN_DELAY_MS = 10;

        void                        changeFrame(uint32_t frame);

        clock_t                     _updateTimeMs;
        uint32_t                    _passedTimeMs;
        uint32_t                    _delayMs;
        uint32_t                    _frame;
        bool                        _running;
        const Sprites&              _sprites;
    };

    using AnimationPtr              = std::shared_ptr<Animation>;

}
