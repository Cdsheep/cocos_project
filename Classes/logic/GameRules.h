#ifndef __GAME_RULES_H__
#define __GAME_RULES_H__

#include "model/CardTypes.h"
#include <cstdlib>

namespace smallgame
{
    class GameRules
    {
    public:
        static bool canPlayCard(const CardData& card, const CardData& topCard)
        {
            if (card.currentArea == CardArea::Removed)
            {
                return false;
            }

            if (card.currentArea == CardArea::Hand)
            {
                return true;
            }

            if (card.currentArea == CardArea::Table)
            {
                return std::abs(card.rank - topCard.rank) == 1;
            }

            return false;
        }
    };
}

#endif
