#pragma once

#include <memory>
#include <string>
#include <vector>
#include <cstdint>

#include "position.h"

#include "../media/sprite.h"

#include "../object/camera.h"
#include "../object/dynamic_object.h"

#include "../util/transformation.h"

namespace object {
    
    enum class TileState {
        DEFAULT,
        FINISH,
        WALL_DEFAULT,
        WALL_SMALL,
        WALL_BORDER,
    };
    
    struct Tile {
        TileState                   state               = TileState::DEFAULT;
        IDynamicObjectPtr           object              = nullptr;
        SDL_Point                   drawPoint;
    };

    struct TileDescription {
        int                         spriteWidth         = 0;
        int                         spriteHeight        = 0;
        int                         halfHorizontalDiag  = 0;
        int                         halfVerticalDiag    = 0;
        int                         tileX               = 0;
        int                         tileY               = 0;
    };

    struct TransformationParameters {
        float                       toIsometricScaleY   = 1.0f;
        float                       toCartesianScaleY   = 1.0f;
        float                       squareSize          = 1.0f;
    };

    using TileMatrix                = std::vector<std::vector<Tile>>;

    class Field {
    public:
        Field(uint32_t width, uint32_t height, const TileDescription& tileDescription);
        
        float                       getSquareSize() const noexcept;
        TileDescription             getTileDescription() const noexcept;
        int                         getWidth() const noexcept;
        int                         getHeight() const noexcept;
        util::Coordinate            getIsometricCoord(const util::Coordinate& cartesianCoord) const;
        util::Coordinate            getIsometricCoord(const Position& pos) const;
        util::Coordinate            getSpriteCoord(const util::Coordinate& cartesianCoord) const;
        util::Coordinate            getSpriteCoord(const Position& pos) const;
        util::Coordinate            getCartesianCoord(const util::Coordinate& isometricCoord) const;
        util::Coordinate            getCartesianCoord(const Position& pos) const;
        Position                    getRowCol(const util::Coordinate& isometricCoord) const noexcept;
        const TileMatrix&           getTiles() const noexcept;
        TileState                   getState(const Position& pos) const;
        void                        setState(const Position& pos, TileState newState);
        IDynamicObjectPtr           getObject(const Position& pos) const;
        void                        setObject(const Position& pos, IDynamicObjectPtr newObject = nullptr);
        bool                        isCorrectPosition(const Position& pos) const;
        bool                        isWalkable(const Position& pos) const;
        bool                        isWall(const Position& pos) const;

    private:
        TransformationParameters    _transformParams;
        TileDescription             _tileDescription;
        TileMatrix                  _tiles;
    };

    using FieldPtr                  = std::shared_ptr<Field>;

    std::string                     getTileName(TileState state);
    SDL_Rect                        generateVisibleRect(const FieldPtr field, int windowWidth, int windowHeight);
    void                            addObject(
                                        FieldPtr field,
                                        IDynamicObjectPtr object,
                                        const Position& pos,
                                        const util::Coordinate& cartOffset = { 0.0f, 0.0f }
                                    );

}
