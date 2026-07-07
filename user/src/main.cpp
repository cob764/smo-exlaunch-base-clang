#include <exl/lib.hpp>
#include <mallow/mallow.hpp>

#include "ActorFactory/actorPatches.h"
#include "Kingdom-18/Kingdom-18-patches.h"

#include "Library/Controller/InputFunction.h"
#include "ModOptions.h"
#include "Scene/StageScene.h"
#include "Bgm/CustomMusicList.h"
#include "Scene/PauseMenuHooks.h"
using mallow::log::logLine;

struct ScenePlayHook : public mallow::hook::Trampoline<ScenePlayHook> {
    static void Callback(StageScene* thisPtr) {
        if (al::isPadTriggerL(-1) && mallow::config::getConfg<ModOptions>()->myModOption)
            logLine("You pressed L while the game was unpaused and myModOption was enabled");
        Orig(thisPtr);
    }
};

extern "C" void userMain() {
    mallow::init::installHooks();
    ca::actorPatches();
    installKingdom18Patches();
    ca::addMusicList();
    PauseMenuHooks::installPauseMenuHooks();
}
