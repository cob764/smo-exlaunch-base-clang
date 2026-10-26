#pragma once
#include <vector>

struct BgmEntry {
    const char* field_0;
    const char* field_1;
    const char* field_2;
    const char* field_3;
    const char* field_4;
};

extern std::vector<BgmEntry> CombinedBgmList;

namespace ca {
void addMusicList();
const std::vector<BgmEntry>& getCombinedBgmList();
}