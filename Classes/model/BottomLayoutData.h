#ifndef __BOTTOM_LAYOUT_DATA_H__
#define __BOTTOM_LAYOUT_DATA_H__

#include "cocos2d.h"

namespace smallgame
{
    struct BottomLayoutData
    {
        float baselineY = 0.0f;
        cocos2d::Vec2 handStartPosition = cocos2d::Vec2::ZERO;
        cocos2d::Vec2 handCardOffset = cocos2d::Vec2::ZERO;
        cocos2d::Vec2 topCardPosition = cocos2d::Vec2::ZERO;
        cocos2d::Vec2 undoButtonPosition = cocos2d::Vec2::ZERO;
    };
}

#endif
