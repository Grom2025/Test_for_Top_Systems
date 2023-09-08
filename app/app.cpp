
#include "../SDL2/SDL.h"
#include "../SDL2/SDL_ttf.h"
#include "../SDL2/SDL_image.h"
#include <string>
#include <ctime>
#include <iostream>
#include <algorithm>
#include "app.h"
#include "settings.h"
#include "priority_tree.h"

#define CHECK_SDL_RESULT(condition, functionName)                           \
if (condition) {                                                            \
    cout << "Error [" << functionName << "]: " << SDL_GetError() << endl;   \
    return false;                                                           \
}

#define MS_MAX_SIMULATE 250;

const char FONT[] = "consola.ttf";
const SDL_Color BLACK = { 0, 0, 0, 0 };

using namespace std;
using namespace app;
using namespace util;
using namespace media;
using namespace object;

const TileDescription Application::TILE_DESCRIPTION = {
        70, // spriteWidth
        81, // spriteHeight
        35, // halfHorizontalDiag
        20, // halfVerticalDiag
        35, // tileX
        60  // tileY
};

const SDL_Color Application::BACKGROUND_COLOR = { 155, 221, 255, 255 };

const uint32_t Application::FIELD_SIZE = 30;

Application::Application() noexcept
        : _running(false)
        , _mouseControl(false)
        , _state(LevelState::PLAYING)
        , _window(nullptr)
        , _camera(nullptr)
        , _field(nullptr)
        , _lastUpdateTime(clock())
{
}

Application::~Application() {
    cleanup();
}

bool Application::isRunning() const noexcept {
    return _running;
}

void Application::setRunning(bool running) noexcept {
    _running = running;
}

bool Application::initialize(const string& title, const Settings& settings) {
    int sdlInitResult = SDL_Init(SDL_INIT_VIDEO);
    CHECK_SDL_RESULT(sdlInitResult < 0, "SDL_Init");

    int imgInitResult = IMG_Init(IMG_INIT_PNG);
    CHECK_SDL_RESULT(imgInitResult < 0, "IMG_Init");

    int ttfInitResult = TTF_Init();
    CHECK_SDL_RESULT(ttfInitResult < 0, "TTF_Init");

    ttf_font = TTF_OpenFont(FONT, 20);
    CHECK_SDL_RESULT(ttf_font == nullptr, "SDL_CreateFont");

    _window = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            settings.displayWidth,
            settings.displayHeight,
            SDL_WINDOW_SHOWN
    );
    CHECK_SDL_RESULT(_window == nullptr, "SDL_CreateWindow");

    if (settings.fullscreen) {
        SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN);
    }

    _renderManager = RenderManager(_window);
    _renderManager.setColor(BACKGROUND_COLOR);

    SDL_Renderer* renderer = _renderManager.getRenderer();
    CHECK_SDL_RESULT(renderer == nullptr, "SDL_CreateRenderer");

    generateField(FIELD_SIZE, FIELD_SIZE);

    _camera = make_shared<Camera>(
            settings.displayWidth,
            settings.displayHeight,
            generateVisibleRect(_field, settings.displayWidth, settings.displayHeight)
    );

    srand( static_cast<unsigned int>( time(nullptr) ) );
    initializeObjects();

    SDL_Point cameraPosition;
    cameraPosition.x -= settings.displayWidth / 2;
    cameraPosition.y -= settings.displayHeight / 2;
    _camera->setPosition(cameraPosition);

    return true;
}

void Application::print_ttf(SDL_Surface* surface, std::string* message, int size, SDL_Color color, SDL_Rect* rect, Centered centered)
{
    SDL_Surface* text_surface = TTF_RenderUTF8_Blended(ttf_font, message->c_str(), color);
    if (centered == x_centered)  {
        rect->x = (surface->w - text_surface->w) / 2;
    }
    else if (centered == y_centered) {
        rect->y = (surface->h - text_surface->h) / 2;
    }
    else if (centered == xy_centered) {
        rect->x = (surface->w - text_surface->w) / 2;
        rect->y = (surface->h - text_surface->h) / 2;
    }
    SDL_BlitSurface(text_surface, NULL, surface, rect);
    SDL_FreeSurface(text_surface);
}

void Application::update() {
    clock_t currentTime = clock();
    clock_t deltaTimeMs = currentTime - _lastUpdateTime;
    deltaTimeMs = MS_MAX_SIMULATE;


    for (size_t i = 0; i < _objects.size(); ++i) {
        IDynamicObjectPtr object = _objects[i];

        if (object->isAlive()) {
            object->update();
            //moveObject(object, _field, deltaTimeMs);
        }

        if (!object->isAlive()) {
            Position p1 = object->getBeginPosition();
            if (_field->getObject(p1) == object) {
                _field->setObject(p1, nullptr);
            }

            Position p2 = object->getEndPosition();
            if (_field->getObject(p2) == object) {
                _field->setObject(p2, nullptr);
            }
            _objects.erase(_objects.begin() + i);
            --i;
        }
    }

    updateState();

    _lastUpdateTime = currentTime;
}

void Application::render() {
    auto width = static_cast<uint32_t>(_field->getWidth());
    auto height = static_cast<uint32_t>(_field->getHeight());
    PriorityTree priorityTree(width, height);

    addFieldToPriorityTree(priorityTree);
    addMarkersToPriorityTree(priorityTree);

    sort(_objects.begin(), _objects.end(), [](IDynamicObjectPtr lhs, IDynamicObjectPtr rhs) {
        Position leftPos = lhs->getBeginPosition();
        Position rightPos = rhs->getBeginPosition();
        return leftPos < rightPos;
    });
    addObjectsToPriorityTree(priorityTree);

    _renderManager.setQueue(priorityTree.getSortedSprites());

    if (_state == LevelState::COMPLETED) {
        SpritePtr levelCompletedSprite = _resourceManager.getSprite("level_completed");
        renderInCenter(levelCompletedSprite);
    }
    else if (_state == LevelState::GAME_OVER) {
        SpritePtr gameOverSprite = _resourceManager.getSprite("game_over");
        renderInCenter(gameOverSprite);
    }

    _renderManager.renderAll(_camera);
    _renderManager.clearQueue();
}

void Application::handleEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_QUIT:
            setRunning(false);
            break;

        case SDL_KEYUP:
            handleKeyUp(event);
            break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            handleMouseButton(event);
            break;

        case SDL_MOUSEMOTION:
            handleMouseMotion(event);
            break;
    }
}

void Application::cleanup() {
    _resourceManager.cleanup();
    TTF_CloseFont(ttf_font);
    SDL_DestroyWindow(_window);
    SDL_Quit();
    IMG_Quit();
}

void Application::renderInCenter(const SpritePtr sprite) {
    int w = 1024, h = 768;
    SDL_GetWindowSize(_window, &w, &h);

    SDL_Point cameraPoint = _camera->getPosition();
    SDL_Rect spriteRect = sprite->getRectangle();
    int x = (w - spriteRect.w) / 2 + cameraPoint.x;
    int y = (h - spriteRect.h) / 2 + cameraPoint.y;

    _renderManager.addToQueue(RenderData{ { x, y }, sprite });
}

void Application::handleKeyUp(const SDL_Event& event) noexcept {
    SDL_Scancode scancode = event.key.keysym.scancode;
    if (scancode == SDL_SCANCODE_ESCAPE) {
        setRunning(false);
    }
}

void Application::handleMouseMotion(const SDL_Event& event) noexcept {
    if (_mouseControl) {
        _camera->move(event.motion.xrel, event.motion.yrel);
    }

    SDL_Point mouseGlobalPoint = _camera->getPosition();
    mouseGlobalPoint.x += event.motion.x;
    mouseGlobalPoint.y += event.motion.y;
   //
}

void Application::handleMouseButton(const SDL_Event& event) noexcept {
    bool isButtonDown = (event.button.type == SDL_MOUSEBUTTONDOWN);

    if (event.button.button == SDL_BUTTON_MIDDLE) {
        _mouseControl = isButtonDown;
        return;
    }

    if (!isButtonDown || _state != LevelState::PLAYING) {
        return;
    }

    if (event.button.button == SDL_BUTTON_LEFT) {
        handleLeftMouseButton();
    }
    else if (event.button.button == SDL_BUTTON_RIGHT) {
        handleRightMouseButton();
    }
}

void Application::handleLeftMouseButton() noexcept {
    if (!_field->isCorrectPosition(_currentPos)) {
        return;
    }
}

void Application::handleRightMouseButton() noexcept {
    if (!_field->isCorrectPosition(_currentPos)) {
        return;
    }
}

void Application::generateField(uint32_t width, uint32_t height) {
    _field = make_shared<Field>(
            width,
            height,
            TILE_DESCRIPTION
    );

    _finishPos = { 1, 1 };
    _field->setState(_finishPos, TileState::FINISH);

    int w = static_cast<int>(width);
    int h = static_cast<int>(height);

    for (int column = 0; column < w; ++column) {
        _field->setState({ 0, column }, TileState::WALL_BORDER);
        _field->setState({ h - 1, column }, TileState::WALL_BORDER);
    }

    for (int row = 1; row < h - 1; ++row) {
        _field->setState({ row, 0 }, TileState::WALL_BORDER);
        _field->setState({ row, w - 1 }, TileState::WALL_BORDER);
    }
}

void Application::initializeObjects() {
    //
}

void Application::addFieldToPriorityTree(PriorityTree& tree) const {
   //
}

void Application::addMarkersToPriorityTree(PriorityTree& tree) const {
   //
}

void Application::addObjectsToPriorityTree(PriorityTree& tree) const {
    //
}

void Application::updateState() {
    //
}

void app::loop(Application& app) {
    while (app.isRunning()) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            app.handleEvent(event);
        }

        app.update();
        app.render();
    }
}
