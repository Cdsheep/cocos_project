#ifndef __BOTTOM_LAYOUT_CALCULATOR_H__
#define __BOTTOM_LAYOUT_CALCULATOR_H__

#include "model/BottomLayoutData.h"
#include "model/GameState.h"

namespace smallgame
{
    class BottomLayoutCalculator
    {
    public:
        static void applyBottomLayout(GameState& state, const BottomLayoutData& layoutData);
    };
}

#endif
