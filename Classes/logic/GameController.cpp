#include "logic/GameController.h"
#include "logic/BottomLayoutCalculator.h"
#include "logic/GameRules.h"
#include "logic/GameSetupFactory.h"
#include "logic/TableLayoutCalculator.h"

namespace smallgame
{
    namespace
    {
        const float BOTTOM_BASELINE_Y = 270.0f;

        BottomLayoutData makeBottomLayout()
        {
            BottomLayoutData layout;
            layout.baselineY = BOTTOM_BASELINE_Y;
            layout.handStartPosition = cocos2d::Vec2(330.0f, BOTTOM_BASELINE_Y);
            layout.handCardOffset = cocos2d::Vec2(120.0f, 0.0f);
            layout.topCardPosition = cocos2d::Vec2(730.0f, BOTTOM_BASELINE_Y);
            layout.undoButtonPosition = cocos2d::Vec2(970.0f, BOTTOM_BASELINE_Y);
            return layout;
        }

        std::vector<TableFanGroupLayout> makeTableGroupTemplates()
        {
            std::vector<TableFanGroupLayout> groups;

            TableFanGroupLayout leftGroup;
            leftGroup.groupId = 0;
            leftGroup.layoutType = TableLayoutType::FanLine;
            leftGroup.startPosition = cocos2d::Vec2(340.0f, 1380.0f);
            leftGroup.cardOffset = cocos2d::Vec2(45.0f, -195.0f);
            leftGroup.cardIds = {0, 0, 0};
            groups.push_back(leftGroup);

            TableFanGroupLayout rightGroup;
            rightGroup.groupId = 1;
            rightGroup.layoutType = TableLayoutType::FanLine;
            rightGroup.startPosition = cocos2d::Vec2(760.0f, 1380.0f);
            rightGroup.cardOffset = cocos2d::Vec2(-35.0f, -195.0f);
            rightGroup.cardIds = {0, 0, 0};
            groups.push_back(rightGroup);

            return groups;
        }
    }

    void GameController::setupGame()
    {
        BottomLayoutData bottomLayout = makeBottomLayout();
        std::vector<TableFanGroupLayout> groupTemplates = makeTableGroupTemplates();

        _state = GameSetupFactory::createRandomInitialState(bottomLayout, groupTemplates, 2, 1);

        TableLayoutCalculator::applyTableFanLayouts(_state);
        BottomLayoutCalculator::applyBottomLayout(_state, _state.bottomLayoutData);
    }

    MoveResult GameController::trySelectCard(int cardId)
    {
        CardData* selectedCard = getMutableCard(cardId);
        CardData* topCard = getMutableCard(_state.currentTopCardId);
        MoveResult result;

        if (selectedCard == nullptr || topCard == nullptr)
        {
            result.message = "card not found";
            return result;
        }

        if (selectedCard->id == topCard->id)
        {
            result.message = "top card selected";
            return result;
        }

        if (selectedCard->currentArea == CardArea::Removed)
        {
            result.message = "card already removed";
            return result;
        }

        if (GameRules::canPlayCard(*selectedCard, *topCard))
        {
            return promoteToTop(cardId);
        }

        result.message = "card does not match top card";
        return result;
    }

    MoveResult GameController::undoLastMove()
    {
        MoveResult result;
        if (_state.history.empty())
        {
            result.message = "no undo history";
            return result;
        }

        MoveRecord record = _state.history.back();
        _state.history.pop_back();

        CardData* movedCard = getMutableCard(record.movedCardId);
        CardData* consumedCard = getMutableCard(record.consumedTopCardId);
        if (movedCard == nullptr || consumedCard == nullptr)
        {
            result.message = "undo failed";
            return result;
        }

        movedCard->currentArea = record.movedFromArea;
        movedCard->currentPosition = record.movedFromPosition;
        movedCard->isTopCard = false;

        consumedCard->currentArea = CardArea::Top;
        consumedCard->currentPosition = record.consumedTopCardPosition;
        consumedCard->isTopCard = true;

        _state.currentTopCardId = consumedCard->id;

        TableLayoutCalculator::applyTableFanLayouts(_state);
        BottomLayoutCalculator::applyBottomLayout(_state, _state.bottomLayoutData);

        result.success = true;
        result.message = "undo success";
        result.record = record;
        return result;
    }

    const GameState& GameController::getState() const
    {
        return _state;
    }

    const std::vector<TableFanGroupLayout>& GameController::getTableGroupLayouts() const
    {
        return _state.tableGroupLayouts;
    }

    const BottomLayoutData& GameController::getBottomLayoutData() const
    {
        return _state.bottomLayoutData;
    }

    const CardData* GameController::getCard(int cardId) const
    {
        auto it = _state.cards.find(cardId);
        if (it == _state.cards.end())
        {
            return nullptr;
        }

        return &it->second;
    }

    CardData* GameController::getMutableCard(int cardId)
    {
        auto it = _state.cards.find(cardId);
        if (it == _state.cards.end())
        {
            return nullptr;
        }

        return &it->second;
    }

    MoveResult GameController::promoteToTop(int cardId)
    {
        MoveResult result;

        CardData* selectedCard = getMutableCard(cardId);
        CardData* topCard = getMutableCard(_state.currentTopCardId);
        if (selectedCard == nullptr || topCard == nullptr)
        {
            result.message = "promote failed";
            return result;
        }

        MoveRecord record;
        record.actionType = GameActionType::PromoteToTop;
        record.movedCardId = selectedCard->id;
        record.movedFromArea = selectedCard->currentArea;
        record.movedFromPosition = selectedCard->currentPosition;
        record.movedToPosition = topCard->currentPosition;
        record.consumedTopCardId = topCard->id;
        record.consumedTopCardPosition = topCard->currentPosition;
        record.consumedTopCardOriginalArea = topCard->originalArea;

        topCard->isTopCard = false;
        topCard->currentArea = CardArea::Removed;
        topCard->currentPosition = cocos2d::Vec2::ZERO;

        selectedCard->currentArea = CardArea::Top;
        selectedCard->currentPosition = record.movedToPosition;
        selectedCard->isTopCard = true;

        _state.currentTopCardId = selectedCard->id;
        _state.history.push_back(record);

        TableLayoutCalculator::applyTableFanLayouts(_state);
        BottomLayoutCalculator::applyBottomLayout(_state, _state.bottomLayoutData);

        result.success = true;
        result.message = "move success";
        result.record = record;
        return result;
    }
}
