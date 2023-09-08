#include <algorithm>

#include "render_manager.h"

using namespace std;
using namespace media;
using namespace object;

RenderManager::RenderManager() noexcept
    : _renderer(nullptr)
    , _color({ 255, 255, 255, 255 })
{
}

RenderManager::RenderManager(SDL_Window* window) noexcept
    : _color({255, 255, 255, 255})
{
    _renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED |
        SDL_RENDERER_PRESENTVSYNC |
        SDL_RENDERER_TARGETTEXTURE
    );
}

RenderManager::~RenderManager() {
    SDL_DestroyRenderer(_renderer);
}

RenderManager::RenderManager(RenderManager&& otherManager) noexcept
    : _renderer(otherManager._renderer)
    , _queue(otherManager._queue)
    , _color(otherManager._color)
{
    otherManager._renderer = nullptr;
}

void RenderManager::operator=(RenderManager&& otherManager) {
    _renderer = otherManager._renderer;
    _queue = otherManager._queue;
    _color = otherManager._color;
    
    otherManager._renderer = nullptr;
}

void RenderManager::setColor(const SDL_Color& color) {
    _color = color;
}

SDL_Renderer* RenderManager::getRenderer() noexcept {
    return _renderer;
}

void RenderManager::addToQueue(const RenderData& renderData) {
    _queue.push_back(renderData);

}

void RenderManager::setQueue(vector<RenderData>&& spritesInfo) {
    _queue = move(spritesInfo);
}

void RenderManager::renderAll(const object::CameraPtr camera) {
    SDL_SetRenderDrawColor(_renderer, _color.r, _color.g, _color.b, _color.a);
    SDL_RenderClear(_renderer);

    SDL_Point cameraPoint = camera->getPosition();

    for (RenderData renderData: _queue) {
        renderData.point.x -= cameraPoint.x;
        renderData.point.y -= cameraPoint.y;
        //renderData.sprite->copyTo(_renderer, renderData.point);
    }

    SDL_RenderPresent(_renderer);
}

void RenderManager::clearQueue() {
    _queue.clear();
}
