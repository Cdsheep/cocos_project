#ifndef __GAME_SETUP_FACTORY_H__
#define __GAME_SETUP_FACTORY_H__

#include "model/GameState.h"

namespace smallgame
{
    class GameSetupFactory
    {
    public:
        static GameState createRandomInitialState(const BottomLayoutData& bottomLayoutData,
                                                  const std::vector<TableFanGroupLayout>& tableGroupLayouts,
                                                  int handCardCount,
                                                  int topCardCount);
    };
}

#endif
