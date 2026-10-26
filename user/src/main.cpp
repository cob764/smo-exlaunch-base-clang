#include <exl/lib.hpp>
#include <mallow/mallow.hpp>

#include "ActorFactory/actorPatches.h"
#include "Kingdom-18/Kingdom-18-patches.h"
#include "Bgm/DisableBgmResetOnDeath.h"
#include "Library/Controller/InputFunction.h"
#include "ModOptions.h"
#include "Scene/StageScene.h"
#include "Bgm/CustomMusicList.h"
#include "Scene/PauseMenuHooks.h"

extern bool deathbgmtrigger;  // defined in StageSceneStateModConfig.cpp


//#include "Library/LiveActor/LiveActor.h"

using mallow::log::logLine;

/*namespace ca {

HOOK_DEFINE_TRAMPOLINE(DeathHookA) {
    static void Callback(al::LiveActor* actor) {
        if (al::isPadTriggerL(-1)) {
            logLine("Death trigger A was called");
        }

        Orig(actor);
    }
};

HOOK_DEFINE_TRAMPOLINE(DeathHookB) {
    static void Callback(al::LiveActor* actor) {
        if (al::isPadTriggerL(-1)) {
            logLine("Death trigger B was called");
        }

        Orig(actor);
    }
};

void installDeathHook() {
    DeathHookA::InstallAtSymbol("_ZN2al9LiveActor4killEv");
    DeathHookB::InstallAtSymbol("_ZN2al9LiveActor13makeActorDeadEv");
}

}
*/
extern "C" void userMain() {
    mallow::init::installHooks();
    ca::actorPatches();
    installKingdom18Patches();
    ca::addMusicList();
    ca::installBgmTraceHooks(); //Reports all BGM start/stop calls to the log, useful for debugging its defined in the DisableBgmResetOnDeath.cpp file
    PauseMenuHooks::installPauseMenuHooks();
    if (deathbgmtrigger) {
        ca::installDisableBgmResetOnDeath();
    }
    
    //ca::installDeathHook();
}
