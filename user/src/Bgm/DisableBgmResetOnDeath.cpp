#include <exl/lib.hpp>
#include <mallow/mallow.hpp>
#include "Bgm/DisableBgmResetOnDeath.h"
namespace al {
class IUseAudioKeeper;
}
class StageScene;
 
extern bool deathbgmtrigger;  // defined in StageSceneStateModConfig.cpp

namespace ca {
extern bool gInMiss;  // defined in CollectBgmHooks.cpp
extern bool gCollectActive;
extern bool gStaleCollectBgm;
}

namespace {
HOOK_DEFINE_TRAMPOLINE(StopAllBgmHook) {
    static void Callback(const al::IUseAudioKeeper* k, int fade) {
        if (!deathbgmtrigger) {
            Orig(k, fade);
            return;
        }
        mallow::log::logLine("[BGM2] al::stopAllBgm fade=%d (inMiss=%d)", fade, ca::gInMiss);
        // Only keep the music alive when a collect-BGM song is playing; normal stage music
        // still fades out on death like in the vanilla game.
        if (ca::gInMiss && ca::gCollectActive) {
            ca::gStaleCollectBgm = true;
            return;
        }
        Orig(k, fade);
    }
};
 
HOOK_DEFINE_TRAMPOLINE(StopBgmNameHook) {
    static void Callback(const al::IUseAudioKeeper* k, const char* name, int fade) {
        mallow::log::logLine("[BGM2] al::stopBgm name=%s fade=%d", name ? name : "(null)", fade);
        Orig(k, name, fade);
    }
};
 
HOOK_DEFINE_TRAMPOLINE(StartBgmNameHook) {
    static void Callback(const al::IUseAudioKeeper* k, const char* name, int a, int b) {
        mallow::log::logLine("[BGM2] al::startBgm name=%s %d %d", name ? name : "(null)", a, b);
        Orig(k, name, a, b);
    }
};
 
HOOK_DEFINE_TRAMPOLINE(StartStageBgmHook) {
    static void Callback(StageScene* scene) {
        mallow::log::logLine("[BGM2] StageScene::exeStartStageBgm");
        Orig(scene);
    }
};
}  // namespace
 
namespace ca {
void installBgmTraceHooks() {
    StopAllBgmHook::InstallAtSymbol("_ZN2al10stopAllBgmEPKNS_15IUseAudioKeeperEi");
    StopBgmNameHook::InstallAtSymbol("_ZN2al7stopBgmEPKNS_15IUseAudioKeeperEPKci");
    StartBgmNameHook::InstallAtSymbol("_ZN2al8startBgmEPKNS_15IUseAudioKeeperEPKcii");
    StartStageBgmHook::InstallAtSymbol("_ZN10StageScene16exeStartStageBgmEv");
}
}  // namespace ca

class StageScene;
class CollectBgmPlayer;
namespace al {
class Scene;
class LiveActor;
}  // namespace al
 
namespace ca {
bool gInMiss = false;  // true while StageScene::exeMiss() is executing (shared with BgmTraceHooks.cpp)
bool gCollectActive = false;  // a collect-BGM song was started and not stopped by the game normally
bool gStaleCollectBgm = false;  // we swallowed a stop on death: audio plays, new scene doesn't know
}
 
class StageSceneStateCollectBgm;
 
namespace {
bool& sInMiss = ca::gInMiss;
 
// StageScene::exeMiss() runs every frame while the scene is in the Miss nerve.
// Anything that calls a "stop collect bgm" function from inside it gets caught by sInMiss.
HOOK_DEFINE_TRAMPOLINE(MissHook) {
    static void Callback(StageScene* scene) {
        if (!deathbgmtrigger) {
            Orig(scene);
            return;
        }
        sInMiss = true;
        Orig(scene);
        sInMiss = false;
    }
};

// Every stop path ends in CollectBgmPlayer::stop(s32 fadeFrames).
HOOK_DEFINE_TRAMPOLINE(PlayerStopHook) {
    static void Callback(CollectBgmPlayer* player, int fade) {
        if (!deathbgmtrigger) {
            Orig(player, fade);
            return;
        }
        mallow::log::logLine("[BGM] CollectBgmPlayer::stop fade=%d (inMiss=%d)", fade, sInMiss);
        if (sInMiss) {
            ca::gStaleCollectBgm = true;
            return;  // swallow the stop during death
        }
        ca::gCollectActive = false;
        ca::gStaleCollectBgm = false;
        Orig(player, fade);
    }
};

// Tracks that a collect-BGM song is playing (so death only keeps *your* song alive).
HOOK_DEFINE_TRAMPOLINE(PlayerStartHook) {
    static void Callback(CollectBgmPlayer* player, const char* name, const char* situation) {
        if (!deathbgmtrigger) {
            Orig(player, name, situation);
            return;
        }
        mallow::log::logLine("[BGM] CollectBgmPlayer::start name=%s", name ? name : "(null)");
        ca::gCollectActive = true;
        ca::gStaleCollectBgm = false;
        Orig(player, name, situation);
    }
};

// Crash fix: after respawn the new scene's collect-BGM state is empty while the old song still
// plays, and the list's "highlight the playing song" code compares against a null string.
HOOK_DEFINE_TRAMPOLINE(SelectPlayingHook) {
    static void Callback(StageSceneStateCollectBgm* state) {
        if (!deathbgmtrigger) {
            Orig(state);
            return;
        }
        if (ca::gStaleCollectBgm) {
            mallow::log::logLine("[BGM] skipping setSelectedIdxPlayingBgm (stale)");
            return;
        }
        Orig(state);
    }
};

// rs::stopCollectBgm(Scene*, int) / rs::stopCollectBgm(LiveActor*, int) are the wrappers
// other code uses. Logged only, so you can see which caller fires.
HOOK_DEFINE_TRAMPOLINE(RsStopSceneHook) {
    static void Callback(al::Scene* scene, int fade) {
        if (!deathbgmtrigger) {
            Orig(scene, fade);
            return;
        }
        mallow::log::logLine("[BGM] rs::stopCollectBgm(Scene) fade=%d (inMiss=%d)", fade, sInMiss);
        if (sInMiss) {
            ca::gStaleCollectBgm = true;
            return;
        }
        Orig(scene, fade);
    }
};

HOOK_DEFINE_TRAMPOLINE(RsStopActorHook) {
    static void Callback(al::LiveActor* actor, int fade) {
        if (!deathbgmtrigger) {
            Orig(actor, fade);
            return;
        }
        mallow::log::logLine("[BGM] rs::stopCollectBgm(LiveActor) fade=%d (inMiss=%d)", fade, sInMiss);
        if (sInMiss)
            return;
        Orig(actor, fade);
    }
};
}  // namespace

namespace ca {
void installDisableBgmResetOnDeath() {
    if (!deathbgmtrigger)
        return;

    MissHook::InstallAtSymbol("_ZN10StageScene7exeMissEv");
    PlayerStopHook::InstallAtSymbol("_ZN16CollectBgmPlayer4stopEi");
    PlayerStartHook::InstallAtSymbol("_ZN16CollectBgmPlayer5startEPKcS1_");
    SelectPlayingHook::InstallAtSymbol("_ZN25StageSceneStateCollectBgm24setSelectedIdxPlayingBgmEv");
    RsStopSceneHook::InstallAtSymbol("_ZN2rs14stopCollectBgmEPN2al5SceneEi");
    RsStopActorHook::InstallAtSymbol("_ZN2rs14stopCollectBgmEPN2al9LiveActorEi");
}
}  // namespace ca