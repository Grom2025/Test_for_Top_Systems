#include <cmath>

#include "field.h"

using namespace std;
using namespace util;
using namespace object;

Field::Field(uint32_t width, uint32_t height, const TileDescription& tileDescription)
    : _tileDescription(tileDescription)
{
    _transformParams.toCartesianScaleY = 1.0f * _tileDescription.halfHorizontalDiag
        / _tileDescription.halfVerticalDiag;

    _transformParams.toIsometricScaleY = 1.0f * _tileDescription.halfVerticalDiag
        / _tileDescription.halfHorizontalDiag;

    _transformParams.squareSize = sqrtf(
        0.5f * powf(static_cast<float>(2.0f * _tileDescription.halfHorizontalDiag), 2.0f)
    );

    _tiles.assign(height, vector<Tile>(width));

    for (size_t i = 0; i < _tiles.size(); ++i) {
        vector<Tile>& row = _tiles[i];
        for (size_t j = 0; j < row.size(); ++j) {
            Coordinate spriteCoord = getSpriteCoord(
                Position{ static_cast<int>(i), static_cast<int>(j) }
            );
            row[j].drawPoint = convertToSdlPoint(spriteCoord);
        }
    }
}

float Field::getSquareSize() const noexcept {
    return _transformParams.squareSize;
}

TileDescription Field::getTileDescription() const noexcept {
    return _tileDescription;
}

int Field::getWidth()const noexcept {
    return (_tiles.size() == 0) ? 0 : static_cast<int>(_tiles[0].size());
}

int Field::getHeight()const noexcept {
    return static_cast<int>(_tiles.size());
}

Coordinate Field::getIsometricCoord(const Coordinate& cartesianCoord) const {
    Coordinate rotatedCoord = rotate(cartesianCoord, QUATER_PI);
    return scale(
        rotatedCoord, 1.0f, _transformParams.toIsometricScaleY
    );
}

Coordinate Field::getIsometricCoord(const Position& pos) const {
    float isometricX = static_cast<float>(pos.column - pos.row) * _tileDescription.halfHorizontalDiag;
    float isometricY = static_cast<float>(pos.column + pos.row) * _tileDescription.halfVerticalDiag;
    return {
        isometricX,
        isometricY + _tileDescription.halfVerticalDiag
    };
}

Coordinate Field::getSpriteCoord(const Coordinate& cartesianCoord) const {
    Coordinate isometricCoord = getIsometricCoord(cartesianCoord);
    isometricCoord.x -= _tileDescription.tileX;
    isometricCoord.y -= _tileDescription.tileY;
    return isometricCoord;
}

Coordinate Field::getSpriteCoord(const Position& pos) const {
    float isometricX = static_cast<float>(pos.column - pos.row) * _tileDescription.halfHorizontalDiag;
    float isometricY = static_cast<float>(pos.column + pos.row) * _tileDescription.halfVerticalDiag;
    return {
        isometricX - _tileDescription.tileX,
        isometricY - _tileDescription.tileY + _tileDescription.halfVerticalDiag
    };
}

Coordinate Field::getCartesianCoord(const Coordinate& isometricCoord) const {
    Coordinate scaledCoord = scale(isometricCoord, 1.0f, _transformParams.toCartesianScaleY);
    return rotate(scaledCoord, -QUATER_PI);
}

Coordinate Field::getCartesianCoord(const Position& pos) const {
    float x = (static_cast<float>(pos.column) + 0.5f) * _transformParams.squareSize;
    float y = (static_cast<float>(pos.row) + 0.5f) * _transformParams.squareSize;
    return { x, y };
}

Position Field::getRowCol(const Coordinate& isometricCoord) const noexcept {
    SDL_Point clickPoint = convertToSdlPoint(
        getCartesianCoord(isometricCoord)
    );
    return {
        static_cast<int>(clickPoint.y / _transformParams.squareSize),
        static_cast<int>(clickPoint.x / _transformParams.squareSize)
    };
}

const TileMatrix& Field::getTiles() const noexcept {
    return _tiles;
}

TileState Field::getState(const Position& pos) const {
    return _tiles[pos.row][pos.column].state;
}

void Field::setState(const Position& pos, TileState newState) {
    _tiles[pos.row][pos.column].state = newState;

}

IDynamicObjectPtr Field::getObject(const Position& pos) const {
    return _tiles[pos.row][pos.column].object;
}

void Field::setObject(const Position& pos, IDynamicObjectPtr newObject) {
    _tiles[pos.row][pos.column].object = newObject;
}

bool Field::isCorrectPosition(const Position& pos) const {
    return (pos.row >= 0) && (pos.row < getHeight())
        && (pos.column >= 0) && (pos.column < getWidth());
}

bool Field::isWalkable(const Position& pos) const {
    TileState state = _tiles[pos.row][pos.column].state;
    return state == TileState::DEFAULT || state == TileState::FINISH;
}

bool Field::isWall(const Position& pos) const {
    TileState state = _tiles[pos.row][pos.column].state;
    return state == TileState::WALL_BORDER
        || state == TileState::WALL_DEFAULT
        || state == TileState::WALL_SMALL;
}

string object::getTileName(TileState state) {
    switch (state) {
    case TileState::FINISH:
        return "tile_finish";

    case TileState::DEFAULT:
        return "tile_default";

    case TileState::WALL_SMALL:
        return "wall_small";

    case TileState::WALL_DEFAULT:
        return "wall_default";

    case TileState::WALL_BORDER:
        return "wall_border";
    }
    return string{};
}

SDL_Rect object::generateVisibleRect(const FieldPtr field, int windowWidth, int windowHeight) {
    TileDescription tileDescr = field->getTileDescription();
    int fieldLeftX = tileDescr.halfHorizontalDiag * (field->getHeight() - 1);

    return {
        -fieldLeftX - windowWidth / 2,
        -windowHeight / 2 + tileDescr.halfVerticalDiag,
        fieldLeftX + tileDescr.halfHorizontalDiag * (field->getWidth() - 1) + windowWidth,
        tileDescr.halfVerticalDiag * (field->getHeight() - 1)
        + tileDescr.halfVerticalDiag * (field->getWidth() - 1) + windowHeight
    };
}

void object::addObject(FieldPtr field, IDynamicObjectPtr object, const Position& pos, const Coordinate& cartOffset) {
    object->setPosition(pos);

    Coordinate cartCoord = field->getCartesianCoord(pos);
    cartCoord.x += cartOffset.x;
    cartCoord.y += cartOffset.y;

    object->setCartesianCoord(cartCoord);
    field->setObject(pos, object);
}
