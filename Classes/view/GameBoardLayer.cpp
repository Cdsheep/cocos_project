#include "view/GameBoardLayer.h"

USING_NS_CC;

namespace smallgame
{
    namespace
    {
        const float MOVE_DURATION = 0.25f;
        const int CARD_Z_BASE = 10;
        const float DESIGN_WIDTH = 1080.0f;
        const float DESIGN_HEIGHT = 2080.0f;
        const float STOCK_AREA_HEIGHT = 580.0f;
        const float MAIN_AREA_HEIGHT = 1500.0f;
    }

    bool GameBoardLayer::init()
    {
        if (!Layer::init())
        {
            return false;
        }

        _controller.setupGame();

        buildStaticBoard();
        buildCards();
        buildButtons();
        return true;
    }

    void GameBoardLayer::buildStaticBoard()
    {
        auto background = LayerColor::create(Color4B(18, 96, 52, 255), static_cast<int>(DESIGN_WIDTH), static_cast<int>(DESIGN_HEIGHT));
        addChild(background);

        auto mainArea = DrawNode::create();
        Vec2 mainRect[4] = {
            Vec2(0.0f, STOCK_AREA_HEIGHT),
            Vec2(DESIGN_WIDTH, STOCK_AREA_HEIGHT),
            Vec2(DESIGN_WIDTH, STOCK_AREA_HEIGHT + MAIN_AREA_HEIGHT),
            Vec2(0.0f, STOCK_AREA_HEIGHT + MAIN_AREA_HEIGHT)
        };
        mainArea->drawSolidPoly(mainRect, 4, Color4F(0.70f, 0.50f, 0.26f, 1.0f));
        addChild(mainArea);

        auto stockArea = DrawNode::create();
        Vec2 stockRect[4] = {
            Vec2(0.0f, 0.0f),
            Vec2(DESIGN_WIDTH, 0.0f),
            Vec2(DESIGN_WIDTH, STOCK_AREA_HEIGHT),
            Vec2(0.0f, STOCK_AREA_HEIGHT)
        };
        stockArea->drawSolidPoly(stockRect, 4, Color4F(0.60f, 0.15f, 0.65f, 1.0f));
        addChild(stockArea);

        auto divider = DrawNode::create();
        divider->drawLine(Vec2(0.0f, STOCK_AREA_HEIGHT), Vec2(DESIGN_WIDTH, STOCK_AREA_HEIGHT), Color4F(1.0f, 1.0f, 1.0f, 0.18f));
        addChild(divider, 1);

        _cardRoot = Node::create();
        addChild(_cardRoot, CARD_Z_BASE);
    }

    void GameBoardLayer::buildCards()
    {
        rebuildAllCards();
    }

    void GameBoardLayer::buildButtons()
    {
        const BottomLayoutData& bottomLayout = _controller.getBottomLayoutData();

        auto buttonBackground = DrawNode::create();
        Vec2 rect[4] = {
            Vec2(-78.0f, -36.0f),
            Vec2(78.0f, -36.0f),
            Vec2(78.0f, 36.0f),
            Vec2(-78.0f, 36.0f)
        };
        buttonBackground->drawSolidPoly(rect, 4, Color4F(0.45f, 0.08f, 0.50f, 1.0f));
        buttonBackground->drawPoly(rect, 4, true, Color4F(1.0f, 1.0f, 1.0f, 0.85f));
        buttonBackground->setPosition(bottomLayout.undoButtonPosition);
        addChild(buttonBackground, 2);

        auto undoLabel = Label::createWithSystemFont(u8"回退", "Arial", 46);
        undoLabel->setTextColor(Color4B(255, 248, 210, 255));
        undoLabel->enableOutline(Color4B(72, 18, 78, 255), 2);
        undoLabel->enableShadow(Color4B(0, 0, 0, 140), Size(0.0f, -2.0f), 2);

        _undoMenuItem = MenuItemLabel::create(undoLabel, CC_CALLBACK_1(GameBoardLayer::onUndoClicked, this));
        _undoMenuItem->setPosition(bottomLayout.undoButtonPosition);

        auto menu = Menu::create(_undoMenuItem, nullptr);
        menu->setPosition(Vec2::ZERO);
        addChild(menu, 3);
    }

    void GameBoardLayer::rebuildAllCards()
    {
        _cardRoot->removeAllChildren();
        _cardNodes.clear();

        const GameState& state = _controller.getState();
        for (const auto& pair : state.cards)
        {
            const CardData& card = pair.second;

            if (card.currentArea == CardArea::Removed)
            {
                continue;
            }

            auto cardNode = CardNode::create(card);
            cardNode->setPosition(card.currentPosition);
            cardNode->setLocalZOrder(card.isTopCard ? 100 : card.id);
            _cardRoot->addChild(cardNode);
            _cardNodes[card.id] = cardNode;

            auto listener = EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(true);
            listener->onTouchBegan = [this, cardNode](Touch* touch, Event* event)
            {
                CC_UNUSED_PARAM(event);
                if (_isAnimating)
                {
                    return false;
                }

                Vec2 localPoint = cardNode->convertToNodeSpace(touch->getLocation());
                Rect rect(0.0f, 0.0f, cardNode->getCardSize().width, cardNode->getCardSize().height);
                return rect.containsPoint(localPoint);
            };
            listener->onTouchEnded = [this, card](Touch* touch, Event* event)
            {
                CC_UNUSED_PARAM(touch);
                CC_UNUSED_PARAM(event);
                onCardClicked(card.id);
            };

            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, cardNode);
        }
    }

    void GameBoardLayer::onCardClicked(int cardId)
    {
        if (_isAnimating)
        {
            return;
        }

        MoveResult result = _controller.trySelectCard(cardId);
        if (!result.success)
        {
            return;
        }

        playMoveAnimation(result.record, [this]()
        {
            rebuildAllCards();
        });
    }

    void GameBoardLayer::onUndoClicked(Ref* sender)
    {
        CC_UNUSED_PARAM(sender);
        if (_isAnimating)
        {
            return;
        }

        MoveResult result = _controller.undoLastMove();
        if (!result.success)
        {
            return;
        }

        const CardData* consumedCard = _controller.getCard(result.record.consumedTopCardId);
        if (consumedCard != nullptr && _cardNodes.find(result.record.consumedTopCardId) == _cardNodes.end())
        {
            auto restoredTopNode = CardNode::create(*consumedCard);
            restoredTopNode->setPosition(result.record.movedToPosition);
            restoredTopNode->setLocalZOrder(201);
            _cardRoot->addChild(restoredTopNode);
            _cardNodes[result.record.consumedTopCardId] = restoredTopNode;
        }

        auto movedNodeIt = _cardNodes.find(result.record.movedCardId);
        auto restoredTopIt = _cardNodes.find(result.record.consumedTopCardId);
        if (movedNodeIt == _cardNodes.end() || restoredTopIt == _cardNodes.end())
        {
            rebuildAllCards();
            return;
        }

        _isAnimating = true;
        auto movedNode = movedNodeIt->second;
        auto restoredTopNode = restoredTopIt->second;
        movedNode->setLocalZOrder(200);
        restoredTopNode->setLocalZOrder(201);
        movedNode->stopAllActions();
        restoredTopNode->stopAllActions();

        auto onFinished = CallFunc::create([this]()
        {
            _isAnimating = false;
            rebuildAllCards();
        });

        movedNode->runAction(MoveTo::create(MOVE_DURATION, result.record.movedFromPosition));
        restoredTopNode->runAction(Sequence::create(
            MoveTo::create(MOVE_DURATION, result.record.consumedTopCardPosition),
            onFinished,
            nullptr));
    }

    void GameBoardLayer::playMoveAnimation(const MoveRecord& record, const std::function<void()>& onComplete)
    {
        auto it = _cardNodes.find(record.movedCardId);
        if (it == _cardNodes.end())
        {
            if (onComplete)
            {
                onComplete();
            }
            return;
        }

        _isAnimating = true;
        auto node = it->second;
        node->setLocalZOrder(200);
        node->stopAllActions();
        node->runAction(Sequence::create(
            MoveTo::create(MOVE_DURATION, record.movedToPosition),
            CallFunc::create([this, onComplete]()
            {
                _isAnimating = false;
                if (onComplete)
                {
                    onComplete();
                }
            }),
            nullptr));
    }

    void GameBoardLayer::refreshStatusText(const std::string& message)
    {
        CC_UNUSED_PARAM(message);
    }

    std::string GameBoardLayer::getAreaText(CardArea area) const
    {
        switch (area)
        {
        case CardArea::Table:
            return "TABLE";
        case CardArea::Hand:
            return "HAND";
        case CardArea::Top:
            return "TOP";
        case CardArea::Removed:
            return "REMOVED";
        }

        return "UNKNOWN";
    }
}
