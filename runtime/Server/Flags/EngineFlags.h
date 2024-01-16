#pragma once

//= INCLUDES ===========
#include "Definitions.h"
#include <stdint.h>
//======================

namespace Spartan
{
    enum class EngineMode : uint32_t
    {
        Editor = 1 << 0,
        Physics = 1 << 1,
        Game = 1 << 2
    };

    class SP_CLASS EngineFlags
    {
    public:
        static void AddFlag(const EngineMode flag);
        static void RemoveFlag(const EngineMode flag);
        static bool IsFlagSet(const EngineMode flag);
        static bool IsEqualFlag(const EngineMode flag1, const EngineMode flag2);
        static void ToggleFlag(const EngineMode flag);
    };
}
