#include "pch.h"
#include "EngineFlags.h"
using namespace Spartan;

namespace {
    uint32_t flags = 0;
}

void EngineFlags::AddFlag(const EngineMode flag)
{
    flags |= static_cast<uint32_t>(flag);
}

void EngineFlags::RemoveFlag(const EngineMode flag)
{
    flags &= ~static_cast<uint32_t>(flag);
}


bool EngineFlags::IsFlagSet(const EngineMode flag)
{
    return flags & static_cast<uint32_t>(flag);
}

bool EngineFlags::IsEqualFlag(const EngineMode flag1, const EngineMode flag2)
{
    return static_cast<uint32_t>(flag1) & static_cast<uint32_t>(flag2);
}

void EngineFlags::ToggleFlag(const EngineMode flag)
{
    IsFlagSet(flag) ? RemoveFlag(flag) : AddFlag(flag);
}
