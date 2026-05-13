#include "logic/TableLayoutCalculator.h"

namespace smallgame
{
    void TableLayoutCalculator::applyTableFanLayouts(GameState& state)
    {
        for (const auto& groupLayout : state.tableGroupLayouts)
        {
            for (size_t index = 0; index < groupLayout.cardIds.size(); ++index)
            {
                int cardId = groupLayout.cardIds[index];
                auto cardIt = state.cards.find(cardId);
                if (cardIt == state.cards.end())
                {
                    continue;
                }

                CardData& card = cardIt->second;
                if (card.currentArea != CardArea::Table)
                {
                    continue;
                }

                cocos2d::Vec2 position = groupLayout.startPosition + groupLayout.cardOffset * static_cast<float>(index);
                card.originalPosition = position;
                card.currentPosition = position;
            }
        }
    }
}
