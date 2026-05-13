#ifndef __TABLE_LAYOUT_DATA_H__
#define __TABLE_LAYOUT_DATA_H__

#include "cocos2d.h"
#include <vector>

namespace smallgame
{
    enum class TableLayoutType
    {
        FanLine
    };

    struct TableFanGroupLayout
    {
        int groupId = 0;
        TableLayoutType layoutType = TableLayoutType::FanLine;
        cocos2d::Vec2 startPosition = cocos2d::Vec2::ZERO;
        cocos2d::Vec2 cardOffset = cocos2d::Vec2::ZERO;
        std::vector<int> cardIds;
    };
}

#endif
