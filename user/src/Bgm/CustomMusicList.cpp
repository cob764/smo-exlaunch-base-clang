//#include <exl/hook/nx64/inline_impl.hpp>
#include <mallow/mallow.hpp>
#include <exl/lib.hpp>
#include "Scene/StageScene.h"
#include "Bgm/CustomMusicList.h"

#include <vector>

uintptr_t OriginalBgmListAddr = 0x7101d98700;
struct BgmEntry {
    const char* field_0;
    const char* field_1;
    const char* field_2;
    const char* field_3;
    const char* field_4;
};
static std::vector<BgmEntry> CombinedBgmList;
// ---------------------------------------------------------
// 3️⃣ Funktion zur Initialisierung
// ---------------------------------------------------------

#define NEW_MUSIC_LIST_NUM 120
void InitCustomBgmList() {
        // Originale Liste kopieren (82 Einträge)
        /*
        const char** original = reinterpret_cast<const char**>(OriginalBgmListAddr);
        for (int i = 0; i < 82 * 5; ++i) {
            CombinedBgmList.push_back(original[i]);
        }
        */
        // Originale Einträge
        // ** 1 to 10 **
        CombinedBgmList.push_back({"StmRsBgmHat", nullptr, nullptr, nullptr, "CapWorld"});
        CombinedBgmList.push_back({"StmRsBgmFall", nullptr, nullptr, nullptr, "WaterfallWorld"});
        CombinedBgmList.push_back({"StmRsBgmFall", "In2DArea", "ピコピコ", nullptr, "WaterfallWorld"});
        CombinedBgmList.push_back({"StmRsBgmDinosaur", nullptr, "キャプチャ", nullptr, "WaterfallWorld"});
        CombinedBgmList.push_back({"StmRsBgmDesertMain", nullptr, nullptr, nullptr, "SandWorld"});
        CombinedBgmList.push_back({"StmRsBgmDesertMain", "In2DArea", "ピコピコ", nullptr, "SandWorld"});
        CombinedBgmList.push_back({"StmRsBgmDesertNight", nullptr, nullptr, nullptr, "SandWorld"});
        CombinedBgmList.push_back({"StmRsBgmDesertNight", "In2DArea", "ピコピコ", nullptr, "SandWorld"});
        CombinedBgmList.push_back({"StmRsBgmDesertTownClct", nullptr, nullptr, nullptr, "SandWorld"});
        CombinedBgmList.push_back({"StmRsBgmJaguar", nullptr, nullptr, nullptr, "SandWorld"});

        // ** 11 to 20 **
        CombinedBgmList.push_back({"StmRsBgmForest", nullptr, nullptr, nullptr, "ForestWorld"});
        CombinedBgmList.push_back({"StmRsBgmForest", "In2DArea", "ピコピコ", nullptr, "ForestWorld"});
        CombinedBgmList.push_back({"StmRsBgmForest", "TankZone", nullptr, nullptr, "ForestWorld"});
        CombinedBgmList.push_back({"StmRsBgmLakeScenario1", nullptr, nullptr, nullptr, "LakeWorld"});
        CombinedBgmList.push_back({"StmRsBgmLakeScenario1", "In2DArea", "ピコピコ", nullptr, "LakeWorld"});
        CombinedBgmList.push_back({"StmRsBgmLakeScenario2", nullptr, nullptr, nullptr, "LakeWorld"});
        CombinedBgmList.push_back({"StmRsBgmLakeTunnel", nullptr, nullptr, nullptr, "LakeWorld"});
        CombinedBgmList.push_back({"StmRsBgmClashFirst", nullptr, nullptr, nullptr, "ClashWorld"});
        CombinedBgmList.push_back({"StmRsBgmClashSecond", nullptr, nullptr, nullptr, "ClashWorld"});
        CombinedBgmList.push_back({"StmRsBgmClashSecond", "In2DArea", "ピコピコ", nullptr, "ClashWorld"});

        // ** 21 to 30 **
        CombinedBgmList.push_back({"StmRsBgmCityScenario01", nullptr, nullptr, nullptr, "CityWorld"});
        CombinedBgmList.push_back({"StmRsBgmCityScenario01Strain", nullptr, nullptr, nullptr, "CityWorld"});
        CombinedBgmList.push_back({"StmRsBgmCityScenario04", nullptr, nullptr, nullptr, "CityWorld"});
        CombinedBgmList.push_back({"StmRsBgmCityCafe01", nullptr, nullptr, nullptr, "CityWorld"});
        CombinedBgmList.push_back({"StmRsBgmCitySessionCheck", nullptr, nullptr, nullptr, "CityWorld"});
        CombinedBgmList.push_back({"StmRsBgmCityScenario03Clct", nullptr, nullptr, nullptr, "CityWorld"});
        CombinedBgmList.push_back({"StmRsBgmCityScenario03Jp", nullptr, nullptr, nullptr, "CityWorld"});
        CombinedBgmList.push_back({"StmRsBgmCityScenario03Special", "MuteOffPaulineVocal", "ピコピコ", nullptr, "CityWorld"});
        CombinedBgmList.push_back({"StmRsBgmSea02", nullptr, nullptr, nullptr, "SeaWorld"});
        CombinedBgmList.push_back({"StmRsBgmSea02", "InWater", nullptr, nullptr, "SeaWorld"});

        // ** 31 to 40 **
        CombinedBgmList.push_back({"StmRsBgmSea02", "In2DArea", "ピコピコ", nullptr, "SeaWorld"});
        CombinedBgmList.push_back({"StmRsBgmSnowTown", nullptr, nullptr, nullptr, "SnowWorld"});
        CombinedBgmList.push_back({"StmRsBgmSnowRaceEntrance", nullptr, nullptr, nullptr, "SnowWorld"});
        CombinedBgmList.push_back({"StmRsBgmLava", nullptr, nullptr, nullptr, "LavaWorld"});
        CombinedBgmList.push_back({"StmRsBgmLava", "In2DArea", "ピコピコ", nullptr, "LavaWorld"});
        CombinedBgmList.push_back({"StmRsBgmLavaTown", nullptr, nullptr, nullptr, "LavaWorld"});
        CombinedBgmList.push_back({"StmRsBgmSky01", nullptr, nullptr, nullptr, "SkyWorld"});
        CombinedBgmList.push_back({"StmRsBgmSky01", "In2DArea", "ピコピコ", nullptr, "SkyWorld"});
        CombinedBgmList.push_back({"StmRsBgmSky02", nullptr, nullptr, nullptr, "SkyWorld"});
        CombinedBgmList.push_back({"StmRsBgmMoon", nullptr, nullptr, nullptr, "MoonWorld"});

        // ** 41 to 50 **
        CombinedBgmList.push_back({"StmRsBgmMoon", "In2DArea", "ピコピコ", nullptr, "MoonWorld"});
        CombinedBgmList.push_back({"StmRsBgmMoonDungeon", nullptr, nullptr, nullptr, "MoonWorld"});
        CombinedBgmList.push_back({"StmRsBgmMoonChurch", nullptr, nullptr, nullptr, "MoonWorld"});
        CombinedBgmList.push_back({"StmRsBgmMoonEscape", nullptr, "キャプチャ", nullptr, "MoonWorld"});
        CombinedBgmList.push_back({"StmRsBgmMoonEscape", "In2DArea", "キャプチャ", "ピコピコ", "MoonWorld"});
        CombinedBgmList.push_back({"StmRsBgmEndRockClct", nullptr, "キャプチャ", nullptr, "MoonWorld"});
        CombinedBgmList.push_back({"StmRsBgmEndRockJp", nullptr, nullptr, nullptr, "MoonWorld"});
        CombinedBgmList.push_back({"StmRsBgmEndRockSpecial", nullptr, "ピコピコ", nullptr, "MoonWorld"});
        CombinedBgmList.push_back({"StmRsBgmPeachCastle", nullptr, nullptr, nullptr, "PeachWorld"});
        CombinedBgmList.push_back({"StmRsBgmBossGateKeeper", nullptr, "ボス", nullptr, "BossBattle"});

        // ** 51 to 60 **
        CombinedBgmList.push_back({"StmRsBgmBossBreeda", nullptr, "ボス", nullptr, "BossBattle"});
        CombinedBgmList.push_back({"StmRsBgmBossKnuckle", nullptr, "ボス", nullptr, "BossBattle"});
        CombinedBgmList.push_back({"StmRsBgmBossForest", "BossForestBattle3rd", "ボス", nullptr, "BossBattle"});
        CombinedBgmList.push_back({"StmRsBgmBossMofumofu", nullptr, "ボス", nullptr, "BossBattle"});
        CombinedBgmList.push_back({"StmRsBgmBossHaikai_B", "CollectBgmGiantWanderBoss", "ボス", nullptr, "BossBattle"});
        CombinedBgmList.push_back({"StmRsBgmBossHaikai_B", "CollectBgmGiantWanderBoss8Bit", "ボス","ピコピコ", "BossBattle"});
        CombinedBgmList.push_back({"StmRsBgmBossMagma", nullptr, "ボス", nullptr, "BossBattle"});
        CombinedBgmList.push_back({"StmRsBgmBossDragon", nullptr, "ボス", nullptr, "BossBattle"});
        CombinedBgmList.push_back({"StmRsBgmBossGolem", "BossGolemBattle3rd", "ボス", nullptr, "BossBattle"});
        CombinedBgmList.push_back({"StmRsBgmBossKoopa01", nullptr, "ボス", nullptr, "BossBattle"});

        // ** 61 to 70 **
        CombinedBgmList.push_back({"StmRsBgmBossKoopa02", nullptr, "ボス", nullptr, "BossBattle"});
        CombinedBgmList.push_back({"StmRsBgmExAthletic01", nullptr, nullptr, nullptr, "ExCourse"});
        CombinedBgmList.push_back({"StmRsBgmExAthletic02", nullptr, nullptr, nullptr, "ExCourse"});
        CombinedBgmList.push_back({"StmRsBgmExAthletic02", "In2DArea", "ピコピコ", nullptr, "ExCourse"});
        CombinedBgmList.push_back({"StmRsBgmExChika", nullptr, nullptr, nullptr, "ExCourse"});
        CombinedBgmList.push_back({"StmRsBgmExChika", "In2DArea", "ピコピコ", nullptr, "ExCourse"});
        CombinedBgmList.push_back({"StmRsBgmExChika2", nullptr, nullptr, nullptr, "ExCourse"});
        CombinedBgmList.push_back({"StmRsBgmExHat", nullptr, nullptr, nullptr, "ExCourse"});
        CombinedBgmList.push_back({"StmRsBgmExCold", nullptr, nullptr, nullptr, "ExCourse"});
        CombinedBgmList.push_back({"StmRsBgmExPsyche", nullptr, nullptr, nullptr, "ExCourse"});

        // ** 71 to 80 **
        CombinedBgmList.push_back({"StmRsBgmExRuin", nullptr, nullptr, nullptr, "ExCourse"});
        CombinedBgmList.push_back({"StmRsBgmExRuin", "In2DArea", "ピコピコ", nullptr, "ExCourse"});
        CombinedBgmList.push_back({"StmRsBgmM1PL", nullptr, "ピコピコ", nullptr, "ExCourse"});
        CombinedBgmList.push_back({"StmRsBgmM1UG", nullptr, "ピコピコ", nullptr, "ExCourse"});
        CombinedBgmList.push_back({"StmRsBgmExBonus", nullptr, "雲の上", nullptr, "ExCourse"});
        CombinedBgmList.push_back({"StmRsBgmKinopioBrigade", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"StmRsBgmDemoWorldMap", "CollectBgmWorldMap", nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"StmRsBgmShopClct", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"StmRsBgmRace", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"StmRsBgmRadioControlCar", nullptr, "ラジコン", nullptr, "EtcBgm"});

        // ** 81 to 90 **
        CombinedBgmList.push_back({"StmRsBgmSlotRoom", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"StmRsBgmMiniGame01", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_133", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_134", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_135", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_136", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_137", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_138", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_139", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_140", nullptr, nullptr, nullptr, "EtcBgm"});

        // ** 91 to 100 **
        CombinedBgmList.push_back({"STM_141", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_142", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_143", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_144", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_145", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_146", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_147", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_148", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_149", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_150", nullptr, nullptr, nullptr, "EtcBgm"});

        // ** 101 to 110 **
        CombinedBgmList.push_back({"STM_151", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_152", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_153", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_154", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_155", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_156", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_157", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_158", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_159", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_160", nullptr, nullptr, nullptr, "EtcBgm"});

        // ** 111 to 120 **
        CombinedBgmList.push_back({"STM_161", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_162", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_163", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_164", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_165", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_166", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_167", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_168", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_169", nullptr, nullptr, nullptr, "EtcBgm"});
        CombinedBgmList.push_back({"STM_170", nullptr, nullptr, nullptr, "EtcBgm"});

        // Eigene zusätzliche Einträge
        /*
        CombinedBgmList.push_back("StmRsBgmCloudKingdom");
        CombinedBgmList.push_back("StmRsBgmRabbitChase");
        CombinedBgmList.push_back("StmRsBgmSecretCourse");
        */
        // beliebig viele weitere hinzufügen
        
        // Optional: Null-Abschluss, falls Code das erwartet
        //CombinedBgmList.push_back(nullptr);
}

struct BgmListRedirectHook1 : public mallow::hook::Inline<BgmListRedirectHook1> {
    static void Callback(exl::hook::InlineCtx* ctx) {
        // x22 zeigt auf unsere vorbereitete Liste
        ctx->X[22] = reinterpret_cast<uintptr_t>(&CombinedBgmList[2]);
    }
};

struct BgmListRedirectHook2 : public mallow::hook::Inline<BgmListRedirectHook2> {
    static void Callback(exl::hook::InlineCtx* ctx) {
        // x22 zeigt auf unsere vorbereitete Liste
        ctx->X[22] = reinterpret_cast<uintptr_t>(&CombinedBgmList[0]);
    }
};
struct BgmListRedirectHook3 : public mallow::hook::Inline<BgmListRedirectHook3> {
    static void Callback(exl::hook::InlineCtx* ctx) {
        // x22 zeigt auf unsere vorbereitete Liste
        ctx->X[24] = reinterpret_cast<uintptr_t>(&CombinedBgmList[0]);    
    }
};
struct BgmListRedirectHook4 : public mallow::hook::Inline<BgmListRedirectHook4> {
    static void Callback(exl::hook::InlineCtx* ctx) {
        // x22 zeigt auf unsere vorbereitete Liste
        ctx->X[24] = reinterpret_cast<uintptr_t>(&CombinedBgmList[1]);
    }
};
struct BgmListRedirectHook5 : public mallow::hook::Inline<BgmListRedirectHook5> {
    static void Callback(exl::hook::InlineCtx* ctx) {
        // x22 zeigt auf unsere vorbereitete Liste
        ctx->X[22] = reinterpret_cast<uintptr_t>(&CombinedBgmList[1]);
    }
};
struct BgmListRedirectHook6 : public mallow::hook::Inline<BgmListRedirectHook6> {
    static void Callback(exl::hook::InlineCtx* ctx) {
        // x22 zeigt auf unsere vorbereitete Liste
        ctx->X[24] = reinterpret_cast<uintptr_t>(&CombinedBgmList[2]);
    }
};
struct BgmListRedirectHook7 : public mallow::hook::Inline<BgmListRedirectHook7> {
    static void Callback(exl::hook::InlineCtx* ctx) {
        // x22 zeigt auf unsere vorbereitete Liste
        ctx->X[20] = reinterpret_cast<uintptr_t>(&CombinedBgmList[1]);
    }
};
struct BgmListRedirectHook8 : public mallow::hook::Inline<BgmListRedirectHook8> {
    static void Callback(exl::hook::InlineCtx* ctx) {
        // x22 zeigt auf unsere vorbereitete Liste
        ctx->X[8] = reinterpret_cast<uintptr_t>(&CombinedBgmList[0]);
    }
};

HOOK_DEFINE_INLINE(CheckString){
    static void Callback(exl::hook::InlineCtx * ctx){
       u64* test = reinterpret_cast<u64*>(ctx->X[24]);
       mallow::log::logLine("MusicA Adresse ist ist: %x!", test);
       mallow::log::logLine("MusicB Adresse ist: %s!", (u64*)*test);

       u64* test2 = reinterpret_cast<u64*>(&CombinedBgmList[0]);
       mallow::log::logLine("Neue Liste Adresse ist: %s!", (u64*)*test2);
       
       //mallow::log::logLine("MusicB ist: %s!", ctx->X[24]);
       //mallow::log::logLine("MusicC ist: %s!", &ctx->X[24]);

    }
};

struct setBgmCounter : public mallow::hook::Inline<setBgmCounter> {
    static void Callback(exl::hook::InlineCtx* ctx) {
        ctx->W[2] = NEW_MUSIC_LIST_NUM;
    }
};

struct getCollectedBgmMaxNum : public mallow::hook::Inline<getCollectedBgmMaxNum> {
    static void Callback(exl::hook::InlineCtx* ctx) {
        ctx->W[0] = NEW_MUSIC_LIST_NUM;
    }
};

struct checkHigherThanDefault1 : public mallow::hook::Inline<checkHigherThanDefault1> {
    static void Callback(exl::hook::InlineCtx* ctx) {
        if (ctx->W[0] == 0) {
            if (ctx->W[20] > 81) {
                ctx->W[0] = 1;
            }        
        }
    }
};

struct checkHigherThanDefault2 : public mallow::hook::Inline<checkHigherThanDefault2> {
    static void Callback(exl::hook::InlineCtx* ctx) {
        if (ctx->W[0] == 0) {
            if (ctx->W[22] > 81) {
                ctx->W[0] = 1;
            }
        }
    }
};
namespace ca {
void addMusicList() {
    InitCustomBgmList();
    exl::patch::CodePatcher p{0x0073c4};
    p.WriteInst(exl::patch::inst::CmpImmediate(exl::armv8::reg::X21, NEW_MUSIC_LIST_NUM));
    p.Seek(0x0075a0);
    p.WriteInst(exl::patch::inst::CmpImmediate(exl::armv8::reg::X22, NEW_MUSIC_LIST_NUM));
    p.Seek(0x4d8d70);
    p.WriteInst(exl::patch::inst::CmpImmediate(exl::armv8::reg::X20, NEW_MUSIC_LIST_NUM));
    p.Seek(0x4d8d34);
    p.WriteInst(exl::patch::inst::CmpImmediate(exl::armv8::reg::X20, NEW_MUSIC_LIST_NUM));
    p.Seek(0x5165a0);
    p.WriteInst(exl::patch::inst::CmpImmediate(exl::armv8::reg::X21, NEW_MUSIC_LIST_NUM));
    p.Seek(0x516514);
    p.WriteInst(exl::patch::inst::CmpImmediate(exl::armv8::reg::X23, NEW_MUSIC_LIST_NUM));
    p.Seek(0x4d86c0);
    p.WriteInst(exl::patch::inst::CmpImmediate(exl::armv8::reg::X22, NEW_MUSIC_LIST_NUM));
    p.Seek(0x4d86ec);
    p.WriteInst(exl::patch::inst::CmpImmediate(exl::armv8::reg::W21, NEW_MUSIC_LIST_NUM));

    getCollectedBgmMaxNum::InstallAtOffset(0x526904);  // getCollectedBgmMaxNum
    setBgmCounter::InstallAtOffset(0x4d8720);  // update
    BgmListRedirectHook2::InstallAtOffset(0x0073ac); // tryRegistBgmResourceNameToBgmRhythmSyncDirector
    BgmListRedirectHook3::InstallAtOffset(0x00757c); // update
    BgmListRedirectHook4::InstallAtOffset(0x4d8d08); // setSelectedIdxPlayingBg
    BgmListRedirectHook5::InstallAtOffset(0x4d8d50); // setSelectedIdxPlayingBg
    BgmListRedirectHook6::InstallAtOffset(0x516490); // isEqualRequest
    BgmListRedirectHook1::InstallAtOffset(0x51652C); // isEqualRequest
    //BgmListRedirectHook7::InstallAtOffset(0x5183c0); // GameDataFile
    BgmListRedirectHook8::InstallAtOffset(0x52649c); // getCollectBgmByIndex
    checkHigherThanDefault1::InstallAtOffset(0x4d9158);  // 
    checkHigherThanDefault2::InstallAtOffset(0x4d85dc);  // 
}
}  // namespace ca