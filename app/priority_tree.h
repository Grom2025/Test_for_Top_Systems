#pragma once

#include <deque>
#include <vector>
#include <cstdint>

#include "../media/sprite.h"
#include "../media/render_manager.h"

#include "../object/field.h"

namespace app {

    struct SpriteNode;

    using SpriteNodePtr             = std::shared_ptr<SpriteNode>;
    using SortedSprites             = std::vector<media::RenderData>;

    struct SpriteNode {
        media::RenderData           spriteInfo;
        std::deque<SpriteNodePtr>   children;
    };

    class PriorityTree {
    public:
        PriorityTree(uint32_t width, uint32_t height);
        ~PriorityTree();

        void                        addSprite(const media::RenderData& spriteInfo, const object::Position& pos);
        void                        addSprite(const media::RenderData& spriteInfo, const object::Position& pos, object::Direction moveDir);
        SortedSprites               getSortedSprites() const;

    private:
        void                        traverseNodes(SpriteNodePtr node, SortedSprites& sprites) const;

        using NodeMatrix            = std::vector<std::vector<SpriteNodePtr>>;

        NodeMatrix                  _nodes;
    };

}
