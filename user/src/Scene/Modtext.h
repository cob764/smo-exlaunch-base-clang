#pragma once
 
#include <basis/seadTypes.h>
 
#include "Library/Message/IUseMessageSystem.h"
#include "Library/Message/MessageHolder.h"
 
namespace ca {
 
inline const char16* modText(const al::IUseMessageSystem* ms, const char* key,
                             const char16* fallback) {
    if (al::isExistSystemMessage(ms, "ModMenu") &&
        al::isExistLabelInSystemMessage(ms, "ModMenu", key))
        return al::getSystemMessageString(ms, "ModMenu", key);
    return fallback;
}
 
}  // namespace ca