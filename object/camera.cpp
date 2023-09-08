#include "camera.h"

using namespace object;

Camera::Camera(int width, int height, const SDL_Rect& border) noexcept
    : _visibleRect({ 0, 0, width, height })
    , _border(border)
{
    correctBorder();
    correctVisibleRect();
}

SDL_Rect Camera::getBorder() const noexcept {
    return _border;
}

void Camera::setBorder(const SDL_Rect& rect) noexcept {
    _border = rect;
    correctBorder();
}

SDL_Rect Camera::getVisibleRect() const noexcept {
    return _visibleRect;
}

void Camera::setVisibleRect(const SDL_Rect& rect) noexcept {
    _visibleRect = rect;
    correctBorder();
}

SDL_Point Camera::getPosition() const noexcept {
    return SDL_Point{
        _visibleRect.x,
        _visibleRect.y
    };
}

void Camera::setPosition(const SDL_Point& position) noexcept {
    _visibleRect.x = position.x;
    _visibleRect.y = position.y;
}

void Camera::move(int deltaX, int deltaY) noexcept {
    _visibleRect.x -= deltaX;
    _visibleRect.y -= deltaY;
    correctVisibleRect();
}

void Camera::correctBorder() noexcept {
    if (_border.w < _visibleRect.w) {
        _border.w = _visibleRect.w;
    }
    if (_border.h < _visibleRect.h) {
        _border.h = _visibleRect.h;
    }
}

void Camera::correctVisibleRect() noexcept {
    if (_visibleRect.x < _border.x) {
        _visibleRect.x = _border.x;
    }

    if (_visibleRect.y < _border.y) {
        _visibleRect.y = _border.y;
    }

    int borderRight = _border.x + _border.w;
    if ((_visibleRect.x + _visibleRect.w) > borderRight) {
        _visibleRect.x = borderRight - _visibleRect.w;
    }

    int borderDown = _border.y + _border.h;
    if ((_visibleRect.y + _visibleRect.h) > borderDown) {
        _visibleRect.y = borderDown - _visibleRect.h;
    }
}
