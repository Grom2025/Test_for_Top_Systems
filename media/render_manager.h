#pragma once

#include <vector>

#include "SDL.h"

#include "sprite.h"

#include "../object/camera.h"

namespace media {

    struct RenderData {
        SDL_Point                   point                   = { 0, 0 };
        media::SpritePtr            sprite                  = nullptr;
    };

    class RenderManager {
    public:
        RenderManager() noexcept;
        explicit RenderManager(SDL_Window* window) noexcept;
        ~RenderManager();
        RenderManager(RenderManager&& otherManager) noexcept;

        void                        operator=(RenderManager&& otherManager);

        void                        setColor(const SDL_Color& color);
        SDL_Renderer*               getRenderer() noexcept;
        void                        setQueue(std::vector<RenderData>&& spritesInfo);
        void                        addToQueue(const RenderData& renderData);
        void                        renderAll(const object::CameraPtr camera);
        void                        clearQueue();

    private:
        SDL_Renderer*               _renderer;
        SDL_Color                   _color;
        std::vector<RenderData>     _queue;
    };

}
