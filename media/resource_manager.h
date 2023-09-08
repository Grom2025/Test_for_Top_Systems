#pragma once

#include <map>
#include <string>
#include <vector>
#include <functional>

#include "SDL.h"

#include "sprite.h"
#include "animation.h"

namespace media {

    class ResourceManager {
    public:
        void                        loadSpriteFromDescription(const std::string& xmlPath, SDL_Renderer* renderer);
        void                        loadAnimationFromDescription(const std::string& xmlPath, SDL_Renderer* renderer);
        SpritePtr                   getSprite(const std::string& name) const;
        AnimationPtr                getAnimation(const std::string& name) const;
        void                        cleanup();

    private:
        using Textures              = std::vector<SDL_Texture*>;
        using SpriteByName          = std::map<std::string, SpritePtr>;
        using SpritesByName         = std::map<std::string, Sprites>;
        using SpriteHandler         = std::function<void(const std::string&, SpritePtr)>;

        void                        loadSprites(const std::string& path, SDL_Renderer* renderer, SpriteHandler handler);

        Textures                    _textures;
        SpriteByName                _nameToSprite;
        SpritesByName               _nameToSprites;
    };
    
}
