#ifndef __CARD_ASSET_RESOLVER_H__
#define __CARD_ASSET_RESOLVER_H__

#include "model/CardTypes.h"
#include "view/CardViewAssets.h"

namespace smallgame
{
    class CardAssetResolver
    {
    public:
        static CardViewAssets resolve(const CardData& card);

    private:
        static std::string buildRankText(int rank);
        static std::string buildColorText(CardSuit suit);
        static std::string buildSuitFileName(CardSuit suit);
    };
}

#endif
