#ifndef __CARD_TYPES_H__
#define __CARD_TYPES_H__

#include "cocos2d.h"
#include <string>

namespace smallgame
{
    enum class CardSuit
    {
        Spade,
        Heart,
        Club,
        Diamond
    };

    enum class CardType
    {
        Standard
    };

    enum class CardArea
    {
        Table,
        Hand,
        Top,
        Removed
    };

    struct CardData
    {
        int id = 0;
        CardSuit suit = CardSuit::Spade;
        CardType type = CardType::Standard;
        int rank = 1;
        CardArea originalArea = CardArea::Table;
        CardArea currentArea = CardArea::Table;
        cocos2d::Vec2 originalPosition = cocos2d::Vec2::ZERO;
        cocos2d::Vec2 currentPosition = cocos2d::Vec2::ZERO;
        bool isTopCard = false;
    };

    inline std::string getSuitText(CardSuit suit)
    {
        switch (suit)
        {
        case CardSuit::Spade:
            return "S";
        case CardSuit::Heart:
            return "H";
        case CardSuit::Club:
            return "C";
        case CardSuit::Diamond:
            return "D";
        }

        return "?";
    }

    inline std::string getCardText(const CardData& card)
    {
        std::string rankText;
        switch (card.rank)
        {
        case 1:
            rankText = "A";
            break;
        case 11:
            rankText = "J";
            break;
        case 12:
            rankText = "Q";
            break;
        case 13:
            rankText = "K";
            break;
        default:
            rankText = std::to_string(card.rank);
            break;
        }

        return getSuitText(card.suit) + rankText;
    }
}

#endif
