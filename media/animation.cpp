#include "animation.h"

using namespace std;
using namespace media;

Animation::Animation(const Sprites& sprites, uint32_t delayMs)
    : _updateTimeMs(clock())
    , _passedTimeMs(0)
    , _frame(0)
    , _running(false)
    , _sprites(sprites)
{
    setDelay(delayMs);
}

SpritePtr Animation::getSprite() const {
    return _sprites.at(_frame);
}

void Animation::setDelay(uint32_t delayMs) noexcept {
    if (delayMs < MIN_DELAY_MS) {
        _delayMs = MIN_DELAY_MS;
    }
    else {
        _delayMs = delayMs;
    }
}

uint32_t Animation::getDelay() const noexcept {
    return _delayMs;
}

uint32_t Animation::getFrame() const noexcept {
    return _frame;
}

uint32_t Animation::getFramesCount() const noexcept {
    return static_cast<uint32_t>( _sprites.size() );
}

bool Animation::isRunning() const noexcept {
    return _running;
}

void Animation::play() noexcept {
    _updateTimeMs = clock();
    _running = true;
}

void Animation::stop() noexcept {
    _running = false;
}

void Animation::goToAndStop(uint32_t frame) noexcept {
    changeFrame(frame);
    stop();
}

void Animation::goToAndPlay(uint32_t frame) noexcept {
    changeFrame(frame);
    play();
}

void Animation::update() noexcept {
    if (!_running) {
        return;
    }

    clock_t currentTime = clock();
    clock_t deltaTime = currentTime - _updateTimeMs;

    _passedTimeMs += static_cast<uint32_t>(deltaTime);
    _updateTimeMs = currentTime;

    if (_passedTimeMs >= _delayMs) {
        uint32_t frameCount = _passedTimeMs / _delayMs;
        _frame += frameCount;
        _frame %= static_cast<uint32_t>(_sprites.size());

        _passedTimeMs %= _delayMs;
    }
}

void Animation::changeFrame(uint32_t frame) {
    if (frame < _sprites.size()) {
        _frame = frame;
    }
}
