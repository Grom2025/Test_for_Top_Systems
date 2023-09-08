//
// Created by Grom2025 on 07.09.2023.
//

#ifndef TESTTOPSYSTEMS_APP_H
#define TESTTOPSYSTEMS_APP_H

#include "../SDL2/SDL.h"
#include "../SDL2/SDL_ttf.h"
#include <vector>
#include <string>
#include "priority_tree.h"
#include "../media/render_manager.h"
#include "../media/resource_manager.h"
#include "settings.h"
#include "../object/field.h"
#include "../object/camera.h"
#include "../object/dynamic_object.h"
#include "../media/render_manager.h"
#include "../media/resource_manager.h"


namespace app {

    enum Centered { not_centered, x_centered, y_centered, xy_centered };

    enum class LevelState {
        PLAYING,
        COMPLETED,
        GAME_OVER,
    };

    class Application {
    public:
        Application() noexcept;
        ~Application();
        bool initialize(const std::string &title, const app::Settings &settings);

        bool                        isRunning() const noexcept;
        void                        setRunning(bool running) noexcept;

        void                        update();
        void                        render();
        void                        handleEvent(const SDL_Event& event);
        void                        cleanup();
        void                        print_ttf(SDL_Surface* surface, std::string* message, int size, SDL_Color color, SDL_Rect* rect, Centered centered);

    private:
        void                        renderInCenter(const media::SpritePtr sprite);
        void                        handleKeyUp(const SDL_Event& event) noexcept;
        void                        handleMouseMotion(const SDL_Event& event) noexcept;
        void                        handleMouseButton(const SDL_Event& event) noexcept;
        void                        handleLeftMouseButton() noexcept;
        void                        handleRightMouseButton() noexcept;
        void                        generateField(uint32_t width, uint32_t height);
        void                        initializeObjects();
        void                        addFieldToPriorityTree(PriorityTree& tree) const;
        void                        addMarkersToPriorityTree(PriorityTree& tree) const;
        void                        addObjectsToPriorityTree(PriorityTree& tree) const;
        void                        updateState();

        using DynamicObjects        = std::vector<object::IDynamicObjectPtr>;

        bool                        _running;
        bool                        _mouseControl;
        LevelState                  _state;
        SDL_Window*                 _window;
        media::ResourceManager      _resourceManager;
        media::RenderManager        _renderManager;
        object::CameraPtr           _camera;
        object::FieldPtr            _field;
        DynamicObjects              _objects;
        clock_t                     _lastUpdateTime;
        object::Position            _currentPos;
        object::Position            _finishPos;

        TTF_Font*                   ttf_font;

        static const object::TileDescription    TILE_DESCRIPTION;
        static const SDL_Color                  BACKGROUND_COLOR;
        static const uint32_t                   FIELD_SIZE;
    };

    void                            loop(Application& application);

}


#endif //TESTTOPSYSTEMS_APP_H
