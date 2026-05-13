#include "logic/BottomLayoutCalculator.h"

namespace smallgame
{
    void BottomLayoutCalculator::applyBottomLayout(GameState& state, const BottomLayoutData& layoutData)
    {
        for (size_t index = 0; index < state.handCardIds.size(); ++index)
        {
            int cardId = state.handCardIds[index];
            auto cardIt = state.cards.find(cardId);
            if (cardIt == state.cards.end())
            {
                continue;
            }

            CardData& card = cardIt->second;
            if (card.currentArea != CardArea::Hand)
            {
                continue;
            }

            cocos2d::Vec2 position = layoutData.handStartPosition + layoutData.handCardOffset * static_cast<float>(index);
            card.originalPosition = position;
            card.currentPosition = position;
        }

        auto topCardIt = state.cards.find(state.currentTopCardId);
        if (topCardIt != state.cards.end())
        {
            CardData& topCard = topCardIt->second;
            if (topCard.currentArea == CardArea::Top)
            {
                topCard.originalPosition = layoutData.topCardPosition;
                topCard.currentPosition = layoutData.topCardPosition;
            }
        }
    }
}
