#include <queue>

#include "path_searching.h"

using namespace std;
using namespace app;
using namespace object;

PathMatrix app::generatePathMatrix(object::FieldPtr field) {
    int width = field->getWidth();
    int height = field->getHeight();
    TileMatrix matrix = field->getTiles();
    PathMatrix pathMatrix(height);

    for (int i = 0; i < height; ++i) {
        pathMatrix[i].assign(width, STATE_EMPTY);
        
        for (int j = 0; j < width; ++j) {
            if (field->isWall({i, j})) {
                pathMatrix[i][j] = STATE_BARRIER;
            }
        }
    }

    return pathMatrix;
}

void app::fillDistance(PathMatrix& matrix, const object::Position& fromPos, const object::Position& toPos) {
    matrix[fromPos.row][fromPos.column] = 0;

    if (fromPos == toPos) {
        return;
    }

    int distance = 0;
    queue<Position> currentPositions;
    currentPositions.push(fromPos);
    
    do {
        if (currentPositions.empty()) {
            return;
        }

        Position pos = currentPositions.front();
        currentPositions.pop();

        distance = matrix[pos.row][pos.column] + 1;

        if (matrix[pos.row - 1][pos.column] == STATE_EMPTY) {
            matrix[pos.row - 1][pos.column] = distance;
            currentPositions.push( nextPosition(pos, Direction::UP) );
        }

        if (matrix[pos.row + 1][pos.column] == STATE_EMPTY) {
            matrix[pos.row + 1][pos.column] = distance;
            currentPositions.push( nextPosition(pos, Direction::DOWN) );
        }

        if (matrix[pos.row][pos.column - 1] == STATE_EMPTY) {
            matrix[pos.row][pos.column - 1] = distance;
            currentPositions.push( nextPosition(pos, Direction::LEFT) );
        }

        if (matrix[pos.row][pos.column + 1] == STATE_EMPTY) {
            matrix[pos.row][pos.column + 1] = distance;
            currentPositions.push( nextPosition(pos, Direction::RIGHT) );
        }
    } while (matrix[toPos.row][toPos.column] != distance);
}

std::deque<object::Direction> app::searchPath(const PathMatrix& matrix, const object::Position& endPos) {
    std::deque<object::Direction> path;

    if (matrix[endPos.row][endPos.column] <= 0) {
        return path;
    }
    
    Position currentPos = endPos;

    for (int dist = matrix[currentPos.row][currentPos.column]; dist > 0; dist--) {
        if (matrix[currentPos.row - 1][currentPos.column] == dist - 1) {
            --currentPos.row;
            path.push_front(Direction::DOWN);
            continue;
        }

        if (matrix[currentPos.row + 1][currentPos.column] == dist - 1) {
            ++currentPos.row;
            path.push_front(Direction::UP);
            continue;
        }

        if (matrix[currentPos.row][currentPos.column - 1] == dist - 1) {
            --currentPos.column;
            path.push_front(Direction::RIGHT);
            continue;
        }

        if (matrix[currentPos.row][currentPos.column + 1] == dist - 1) {
            ++currentPos.column;
            path.push_front(Direction::LEFT);
            continue;
        }

        // путь не найден, возможно матрица не правильная
        break;
    }

    return path;
}
