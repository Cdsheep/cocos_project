#include "view/CardNode.h"
#include "view/CardAssetResolver.h"

USING_NS_CC;

namespace smallgame
{
    namespace
    {
        const float CARD_WIDTH = 182.0f;
        const float CARD_HEIGHT = 282.0f;
    }

    CardNode* CardNode::create(const CardData& data)
    {
        CardNode* ret = new (std::nothrow) CardNode();
        if (ret != nullptr && ret->init(data))
        {
            ret->autorelease();
            return ret;
        }

        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool CardNode::init(const CardData& data)
    {
        if (!Node::init())
        {
            return false;
        }

        _data = data;
        setContentSize(Size(CARD_WIDTH, CARD_HEIGHT));
        setAnchorPoint(Vec2(0.5f, 0.5f));

        buildView();
        refresh(data);
        return true;
    }

    void CardNode::refresh(const CardData& data)
    {
        _data = data;

        CardViewAssets assets = CardAssetResolver::resolve(data);
        _backgroundSprite->setTexture(assets.backgroundPath);
        _bigNumberSprite->setTexture(assets.bigNumberPath);
        _smallNumberSprite->setTexture(assets.smallNumberPath);
        _suitSprite->setTexture(assets.suitPath);

        _bigNumberSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
        _bigNumberSprite->setPosition(Vec2(CARD_WIDTH * 0.50f, CARD_HEIGHT * 0.43f));

        _smallNumberSprite->setAnchorPoint(Vec2(0.0f, 1.0f));
        _smallNumberSprite->setPosition(Vec2(16.0f, CARD_HEIGHT - 16.0f));

        _suitSprite->setAnchorPoint(Vec2(1.0f, 1.0f));
        _suitSprite->setPosition(Vec2(CARD_WIDTH - 16.0f, CARD_HEIGHT - 18.0f));

        applyTopCardState();
    }

    int CardNode::getCardId() const
    {
        return _data.id;
    }

    Size CardNode::getCardSize() const
    {
        return getContentSize();
    }

    void CardNode::buildView()
    {
        _backgroundSprite = Sprite::create("res/card_general.png");
        _backgroundSprite->setAnchorPoint(Vec2(0.0f, 0.0f));
        _backgroundSprite->setPosition(Vec2::ZERO);
        addChild(_backgroundSprite, 0);

        _bigNumberSprite = Sprite::create();
        addChild(_bigNumberSprite, 1);

        _smallNumberSprite = Sprite::create();
        addChild(_smallNumberSprite, 2);

        _suitSprite = Sprite::create();
        addChild(_suitSprite, 2);

        _highlightFrame = DrawNode::create();
        addChild(_highlightFrame, 3);
    }

    void CardNode::applyTopCardState()
    {
        _highlightFrame->clear();
        if (!_data.isTopCard)
        {
            return;
        }

        Vec2 rect[4] = {
            Vec2(4.0f, 4.0f),
            Vec2(CARD_WIDTH - 4.0f, 4.0f),
            Vec2(CARD_WIDTH - 4.0f, CARD_HEIGHT - 4.0f),
            Vec2(4.0f, CARD_HEIGHT - 4.0f)
        };
        _highlightFrame->drawPoly(rect, 4, true, Color4F(1.0f, 0.84f, 0.0f, 1.0f));
    }
}
