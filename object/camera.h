#pragma once

#include <memory>

#include "SDL.h"

#include "dynamic_object.h"

namespace object {

    class Camera {
    public:
        Camera(int width, int height, const SDL_Rect& border) noexcept;

        SDL_Rect                    getBorder() const noexcept;
        void                        setBorder(const SDL_Rect& rect) noexcept;
        SDL_Rect                    getVisibleRect() const noexcept;
        void                        setVisibleRect(const SDL_Rect& rect) noexcept;
        SDL_Point                   getPosition() const noexcept;
        void                        setPosition(const SDL_Point& point) noexcept;
        void                        move(int deltaX, int deltaY) noexcept;

    private:
        void                        correctBorder() noexcept;
        void                        correctVisibleRect() noexcept;

        SDL_Rect                    _border;        // ограничение области просмотра
        SDL_Rect                    _visibleRect;   // видимая часть
    };

    using CameraPtr                 = std::shared_ptr<Camera>;

}
