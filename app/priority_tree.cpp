#include "priority_tree.h"

using namespace std;
using namespace app;
using namespace media;
using namespace object;

PriorityTree::PriorityTree(uint32_t width, uint32_t height) {
    _nodes.assign(height, vector<SpriteNodePtr>());

    SpriteNodePtr lastRowNode = nullptr;

    for (vector<SpriteNodePtr>& row : _nodes) {
        row.resize(width);

        SpriteNodePtr lastColNode = nullptr;

        for (SpriteNodePtr& node : row) {
            node = make_shared<SpriteNode>();

            if (lastColNode != nullptr) {
                lastColNode->children.push_back(node);
            }
            lastColNode = node;
        }

        if (lastRowNode != nullptr) {
            lastRowNode->children.push_back( row.front() );
        }
        lastRowNode = row.front();
    }
}

PriorityTree::~PriorityTree() {
    for (vector<SpriteNodePtr>& row : _nodes) {
        for (SpriteNodePtr& node : row) {
            node->children.clear();
            node->spriteInfo.sprite.reset();
        }
    }
}

void PriorityTree::addSprite(const RenderData& spriteInfo, const Position& pos) {
    SpriteNodePtr& node = _nodes[pos.row][pos.column];
    
    if (node->spriteInfo.sprite == nullptr) {
        node->spriteInfo = spriteInfo;
    }
    else {
        SpriteNodePtr newNode = make_shared<SpriteNode>();
        newNode->spriteInfo = spriteInfo;
        newNode->children = move(node->children);

        node->children.push_back(newNode);
        node = newNode;
    }
}

void PriorityTree::addSprite(const RenderData& spriteInfo, const Position& pos, Direction moveDir) {
    /*if (isVerticalDirection(moveDir)) {
        Position delPos = (moveDir == Direction::UP) ?
            nextPosition(pos, moveDir) : pos;
        SpriteNodePtr delNode = _nodes[delPos.row][delPos.column];

        Position addPos = (moveDir == Direction::UP) ?
            pos : nextPosition(pos, moveDir);
        addSprite(spriteInfo, addPos);

        SpriteNodePtr addNode = _nodes[addPos.row][addPos.column];

        addNode->children.insert(
            addNode->children.begin(),
            delNode->children.begin(),
            delNode->children.end()
        );

        delNode->children.clear();
    }
    else { // LEFT or RIGHT
        Position addPos = (moveDir == Direction::RIGHT) ?
            nextPosition(pos, moveDir) : pos;
        addSprite(spriteInfo, addPos);
    }*/
}

SortedSprites PriorityTree::getSortedSprites() const {
    SortedSprites sprites;

    if (!_nodes.empty() && !_nodes.front().empty()) {
        traverseNodes(_nodes.front().front(), sprites);
    }

    return sprites;
}

void PriorityTree::traverseNodes(SpriteNodePtr node, SortedSprites& sprites) const {
    sprites.push_back(node->spriteInfo);

    for (SpriteNodePtr child : node->children) {
        traverseNodes(child, sprites);
    }
}
