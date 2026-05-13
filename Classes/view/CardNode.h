#ifndef __CARD_NODE_H__
#define __CARD_NODE_H__

#include "model/CardTypes.h"

namespace smallgame
{
    class CardNode : public cocos2d::Node
    {
    public:
        static CardNode* create(const CardData& data);

        bool init(const CardData& data);
        void refresh(const CardData& data);
        int getCardId() const;
        cocos2d::Size getCardSize() const;

    private:
        void buildView();
        void applyTopCardState();

    private:
        CardData _data;
        cocos2d::Sprite* _backgroundSprite = nullptr;
        cocos2d::Sprite* _bigNumberSprite = nullptr;
        cocos2d::Sprite* _smallNumberSprite = nullptr;
        cocos2d::Sprite* _suitSprite = nullptr;
        cocos2d::DrawNode* _highlightFrame = nullptr;
    };
}

#endif
