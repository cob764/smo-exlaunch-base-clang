#include <exl/lib.hpp>

#include "Kingdom-18/sead/seadNinSDFileDeviceNin.h"
#include <filedevice/nin/seadNinSaveFileDeviceNin.h>
#include <filedevice/seadFileDevice.h>
#include <nn/fs.h>
#include <nn/os.h>

#include "Library/Base/StringUtil.h"
#include "Library/Controller/InputFunction.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Placement/PlacementFunction.h"

#include "Project/HitSensor/HitSensor.h"
#include "Scene/StageScene.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolder.h"
#include "System/GameDataHolderAccessor.h"

namespace nn::ui2d {
struct ResourceTextureInfo;
}
namespace al {
nn::ui2d::ResourceTextureInfo* createTextureInfo(const char*, const char*, const char*);
}


HOOK_DEFINE_INLINE(WorldMapMovieUpdaterCtor) {
    static void Callback(exl::hook::InlineCtx* ctx) {
        u64 thing = *((u64*) (ctx->X[19] + 0x30));
        *((const char**)(thing + 17*0x10 + 0)) = "content:/MovieData/World18.mp4";
        *((nn::ui2d::ResourceTextureInfo**)(thing + 17*0x10 + 8)) = al::createTextureInfo("ObjectData/TextureWorldMap", "TextureWorldMap", "PuzzlePicImage");
    }
};

HOOK_DEFINE_INLINE(WorldMapRouteShuffle) {
    static void Callback(exl::hook::InlineCtx* ctx) {
        if (ctx->X[23] == 0x11) {
            ctx->W[9] = (ctx->W[23] - 0x11);
        } else if (ctx->X[23] == 0x12) {
            ctx->W[9] = (ctx->W[23] - 0);
        }
    }
};

bool checkEnableUnlockWorldSpecial3(al::LiveActor const* actor) {
    bool foundShine[4];
    GameDataHolderAccessor gdha = GameDataHolderAccessor(actor);

    unsigned long unlockFlag = GameDataFunction::isUnlockedWorld(gdha, 0x10);
    if ((unlockFlag & 1) != 0) {
        unlockFlag = GameDataFunction::isUnlockedWorld(gdha, 0x11);
        if ((unlockFlag & 1) == 0) {
            foundShine[0] = false;
            int unlockShineNum = GameDataFunction::findUnlockShineNum(foundShine, gdha);
            if (!foundShine[0]) {
                return false;
            }
            int totalShines = GameDataFunction::getTotalPayShineNum(gdha);
            return unlockShineNum <= totalShines;
        }
    }
    return false;
}

HOOK_DEFINE_TRAMPOLINE(CheckEnableUnlockWorldSpecial2and3) {
    static bool Callback(al::LiveActor const* actor) {
        return checkEnableUnlockWorldSpecial3(actor) || Orig(actor);
    }
};

HOOK_DEFINE_TRAMPOLINE(MoonRockCheck) {
    static int Callback(GameDataFile* gdf, int index) {
        if (index == 0x11) {
            return 0;
        }
        return Orig(gdf, index);
    }
};

void kingdom18() {
    WorldMapMovieUpdaterCtor::InstallAtOffset(0x508DB4);
    WorldMapRouteShuffle::InstallAtOffset(0x50168C);
    CheckEnableUnlockWorldSpecial2and3::InstallAtOffset(0x52a794);
    MoonRockCheck::InstallAtOffset(0x526a24);
}
