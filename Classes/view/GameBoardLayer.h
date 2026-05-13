#ifndef __GAME_BOARD_LAYER_H__
#define __GAME_BOARD_LAYER_H__

#include "logic/GameController.h"
#include "view/CardNode.h"
#include <functional>
#include <unordered_map>

namespace smallgame
{
    class GameBoardLayer : public cocos2d::Layer
    {
    public:
        CREATE_FUNC(GameBoardLayer);

        virtual bool init() override;

    private:
        void buildStaticBoard();
        void buildCards();
        void buildButtons();
        void rebuildAllCards();
        void onCardClicked(int cardId);
        void onUndoClicked(cocos2d::Ref* sender);
        void playMoveAnimation(const MoveRecord& record, const std::function<void()>& onComplete);
        void refreshStatusText(const std::string& message);
        std::string getAreaText(CardArea area) const;

    private:
        GameController _controller;
        cocos2d::Node* _cardRoot = nullptr;
        cocos2d::Label* _statusLabel = nullptr;
        cocos2d::MenuItemLabel* _undoMenuItem = nullptr;
        std::unordered_map<int, CardNode*> _cardNodes;
        bool _isAnimating = false;
    };
}

#endif
