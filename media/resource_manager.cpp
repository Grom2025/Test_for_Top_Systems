#include <iostream>
#include <filesystem>

#include "../SDL2/SDL_image.h"
#include "../util/pugixml.hpp"

#include "resource_manager.h"

#include "../util/xml_attribute.h"

using namespace std;
using namespace pugi;
using namespace util;
using namespace media;
using namespace filesystem;

SDL_Rect generateRectangle(const xml_node& node) {
    SDL_Rect rectangle = { 0, 0, 0, 0 };
    tryReadXmlAttribute(rectangle.x, node.attribute("x"));
    tryReadXmlAttribute(rectangle.y, node.attribute("y"));
    tryReadXmlAttribute(rectangle.w, node.attribute("w"));
    tryReadXmlAttribute(rectangle.h, node.attribute("h"));
    return rectangle;
}

void ResourceManager::loadSpriteFromDescription(const string& xmlPath, SDL_Renderer* renderer) {
    loadSprites(xmlPath, renderer, [&](const string& name, SpritePtr sprite) {
        _nameToSprite[name] = sprite;
    });
}

void ResourceManager::loadAnimationFromDescription(const string& xmlPath, SDL_Renderer* renderer) {
    path path = xmlPath;
    string animationName = path.stem().string();

    loadSprites(xmlPath, renderer, [&](const string&, SpritePtr sprite) {
        _nameToSprites[animationName].push_back(sprite);
    });
}

SpritePtr ResourceManager::getSprite(const string& name) const {
    return (_nameToSprite.count(name) == 0) ? nullptr : _nameToSprite.at(name);
}

AnimationPtr ResourceManager::getAnimation(const string& name) const {
    if (_nameToSprites.count(name) == 0) {
        return nullptr;
    }

    const Sprites& sprites = _nameToSprites.at(name);
    return make_shared<Animation>(sprites);
}

void ResourceManager::cleanup() {
    for (SDL_Texture* texture : _textures) {
        SDL_DestroyTexture(texture);
    }
}

void ResourceManager::loadSprites(const std::string& path, SDL_Renderer* renderer, SpriteHandler handler) {
    xml_document doc;

    auto loadSuccess = doc.load_file(path.c_str());
    if (!loadSuccess) {
        return;
    }

    xml_node textureAtlasNode = doc.child("TextureAtlas");
    string imagePath = textureAtlasNode.attribute("image-path").value();
    SDL_Surface* pngImage = IMG_Load(imagePath.c_str());
    if (pngImage == nullptr) {
        cout << "Error [IMG_Load]: " + imagePath << endl;
        return;
    }

    _textures.push_back(SDL_CreateTextureFromSurface(renderer, pngImage));
    SDL_FreeSurface(pngImage);

    SDL_Texture* texture = _textures.back();
    auto spriteNodes = textureAtlasNode.children();

    for (const xml_node& node : spriteNodes) {
        SDL_Rect rect = generateRectangle(node);
        string name = node.attribute("name").value();

        if (!name.empty()) {
            SpritePtr sprite = make_shared<Sprite>(texture, rect);;
            handler(name, sprite);
        }
    }
}
