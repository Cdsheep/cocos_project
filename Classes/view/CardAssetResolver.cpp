#include "view/CardAssetResolver.h"

namespace smallgame
{
    CardViewAssets CardAssetResolver::resolve(const CardData& card)
    {
        CardViewAssets assets;
        std::string rankText = buildRankText(card.rank);
        std::string colorText = buildColorText(card.suit);

        assets.backgroundPath = "res/card_general.png";
        assets.bigNumberPath = "res/number/big_" + colorText + "_" + rankText + ".png";
        assets.smallNumberPath = "res/number/small_" + colorText + "_" + rankText + ".png";
        assets.suitPath = "res/suits/" + buildSuitFileName(card.suit) + ".png";
        return assets;
    }

    std::string CardAssetResolver::buildRankText(int rank)
    {
        switch (rank)
        {
        case 1:
            return "A";
        case 11:
            return "J";
        case 12:
            return "Q";
        case 13:
            return "K";
        default:
            return std::to_string(rank);
        }
    }

    std::string CardAssetResolver::buildColorText(CardSuit suit)
    {
        switch (suit)
        {
        case CardSuit::Heart:
        case CardSuit::Diamond:
            return "red";
        case CardSuit::Spade:
        case CardSuit::Club:
        default:
            return "black";
        }
    }

    std::string CardAssetResolver::buildSuitFileName(CardSuit suit)
    {
        switch (suit)
        {
        case CardSuit::Spade:
            return "spade";
        case CardSuit::Heart:
            return "heart";
        case CardSuit::Club:
            return "club";
        case CardSuit::Diamond:
            return "diamond";
        }

        return "spade";
    }
}
