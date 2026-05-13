#ifndef __TABLE_LAYOUT_CALCULATOR_H__
#define __TABLE_LAYOUT_CALCULATOR_H__

#include "model/GameState.h"

namespace smallgame
{
    class TableLayoutCalculator
    {
    public:
        static void applyTableFanLayouts(GameState& state);
    };
}

#endif
