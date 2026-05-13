#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "model/GameState.h"
#include <functional>
#include <string>

namespace smallgame
{
    struct MoveResult
    {
        bool success = false;
        std::string message;
        MoveRecord record;
    };

    class GameController
    {
    public:
        void setupGame();

        MoveResult trySelectCard(int cardId);
        MoveResult undoLastMove();

        const GameState& getState() const;
        const std::vector<TableFanGroupLayout>& getTableGroupLayouts() const;
        const BottomLayoutData& getBottomLayoutData() const;
        const CardData* getCard(int cardId) const;
        CardData* getMutableCard(int cardId);

    private:
        MoveResult promoteToTop(int cardId);

    private:
        GameState _state;
    };
}

#endif
