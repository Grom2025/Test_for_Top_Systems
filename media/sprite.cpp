#include "sprite.h"

using namespace media;

Sprite::Sprite() noexcept
    : _texture(nullptr)
    , _rectangle({0, 0, 0, 0})
{
}

Sprite::Sprite(SDL_Texture* texture) noexcept
    : _texture(texture)
{
    _rectangle = { 0, 0, 0, 0 };
    SDL_QueryTexture(_texture, nullptr, nullptr, &_rectangle.w, &_rectangle.h);
}

Sprite::Sprite(SDL_Texture* texture, const SDL_Rect& rectangle) noexcept
    : _texture(texture)
    , _rectangle(rectangle)
{
}

SDL_Texture* Sprite::getTexture() const noexcept {
    return _texture;
}

SDL_Rect Sprite::getRectangle() const noexcept {
    return _rectangle;
}

void Sprite::copyTo(SDL_Renderer* renderer, int x, int y) const {
    if (renderer != nullptr) {
        SDL_Rect dstRect = { x, y, _rectangle.w, _rectangle.h };
        SDL_RenderCopy(renderer, _texture, &_rectangle, &dstRect);
    }
}

void Sprite::copyTo(SDL_Renderer* renderer, const SDL_Point& point) const {
    copyTo(renderer, point.x, point.y);
}
