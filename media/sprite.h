#pragma once

#include <memory>

#include "../SDL2/SDL.h"

namespace media {
    
    class Sprite {
    public:
        Sprite() noexcept;
        Sprite(SDL_Texture* texture) noexcept;
        Sprite(SDL_Texture* texture, const SDL_Rect& rectangle) noexcept;
        
        SDL_Texture*                getTexture() const noexcept;
        SDL_Rect                    getRectangle() const noexcept;
        void                        copyTo(SDL_Renderer* renderer, int x = 0, int y = 0) const;
        void                        copyTo(SDL_Renderer* renderer, const SDL_Point& point) const;

    private:
        SDL_Texture*                _texture;
        SDL_Rect                    _rectangle;
    };

    using SpritePtr = std::shared_ptr<Sprite>;

}
