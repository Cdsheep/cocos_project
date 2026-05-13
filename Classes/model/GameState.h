#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include "model/BottomLayoutData.h"
#include "model/CardTypes.h"
#include "model/TableLayoutData.h"
#include <unordered_map>
#include <vector>

namespace smallgame
{
    enum class GameActionType
    {
        PromoteToTop
    };

    struct MoveRecord
    {
        GameActionType actionType = GameActionType::PromoteToTop;

        int movedCardId = 0;
        CardArea movedFromArea = CardArea::Table;
        cocos2d::Vec2 movedFromPosition = cocos2d::Vec2::ZERO;
        cocos2d::Vec2 movedToPosition = cocos2d::Vec2::ZERO;

        int consumedTopCardId = 0;
        cocos2d::Vec2 consumedTopCardPosition = cocos2d::Vec2::ZERO;
        CardArea consumedTopCardOriginalArea = CardArea::Table;
    };

    struct GameState
    {
        std::unordered_map<int, CardData> cards;
        std::vector<int> handCardIds;
        std::vector<int> tableCardIds;
        std::vector<TableFanGroupLayout> tableGroupLayouts;
        BottomLayoutData bottomLayoutData;
        std::vector<MoveRecord> history;
        int currentTopCardId = 0;
        int nextCardId = 1;
    };
}

#endif
