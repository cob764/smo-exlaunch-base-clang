#include <exl/lib.hpp>

#include "Scene/PauseMenuHooks.h"
#include "Scene/StageSceneStateModConfig.hpp"

#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Scene/Scene.h"
#include "Layout/FooterParts.h"
#include "Layout/MenuSelectParts.h"
#include "Layout/StageSceneLayout.h"
#include "Scene/StageSceneStateOption.h"
#include "Scene/StageSceneStatePauseMenu.h"
#include "System/GameDataHolder.h"

class SceneAudioSystemPauseController;

namespace patch = exl::patch;
namespace inst  = exl::armv8::inst;
namespace reg   = exl::armv8::reg;

static StageSceneStateModConfig* sceneStateModConfig = nullptr;

// Offset: 0x4EA1F0
HOOK_DEFINE_TRAMPOLINE(InitNerveStateHook) {
    static void Callback(StageSceneStatePauseMenu* state, const char* name, al::Scene* host, al::SimpleLayoutAppearWaitEnd* menuLayout, GameDataHolder* gameDataHolder, const al::SceneInitInfo& sceneInitInfo, const al::ActorInitInfo& actorInitInfo, const al::LayoutInitInfo& layoutInitInfo, al::WindowConfirm* windowConfirm, StageSceneLayout* stageSceneLayout, bool isTitle, SceneAudioSystemPauseController* sceneAudioSystemPauseController) {
        Orig(state, name, host, menuLayout, gameDataHolder, sceneInitInfo, actorInitInfo,
             layoutInitInfo, windowConfirm, stageSceneLayout, isTitle, sceneAudioSystemPauseController);
        al::initNerveState(state, sceneStateModConfig,
                           &NrvStageSceneStatePauseMenu.ModConfig, "ModConfigNerveState");
    }
};

HOOK_DEFINE_TRAMPOLINE(InitStateHook) {
    static void Callback(StageSceneStateOption* thisPtr, const char* stateName, al::Scene* host, const al::LayoutInitInfo& initInfo, FooterParts* footer, GameDataHolder* data, bool unkBool) {
        Orig(thisPtr, stateName, host, initInfo, footer, data, unkBool);
        sceneStateModConfig = new StageSceneStateModConfig(
            "ModConfig", host, initInfo, footer, data, unkBool);
    }
};

HOOK_DEFINE_REPLACE(OverrideHelpFadeNerve) {
    static void Callback(StageSceneStatePauseMenu* state) {
        state->exeModConfig();
        al::setNerve(state, &NrvStageSceneStatePauseMenu.ModConfig);
    }
};

namespace PauseMenuHooks {
    void installPauseMenuHooks() {
        patch::CodePatcher(0x4EA104).WriteInst(inst::Movz(reg::W2, 15));
        InitNerveStateHook::InstallAtSymbol("_ZN24StageSceneStatePauseMenuC1EPKcPN2al5SceneEPNS2_25SimpleLayoutAppearWaitEndEP14GameDataHolderRKNS2_13SceneInitInfoERKNS2_13ActorInitInfoERKNS2_14LayoutInitInfoEPNS2_13WindowConfirmEP16StageSceneLayoutbP31SceneAudioSystemPauseController");
        InitStateHook::InstallAtSymbol("_ZN21StageSceneStateOptionC1EPKcPN2al5SceneERKNS2_14LayoutInitInfoEP11FooterPartsP14GameDataHolderb");
        OverrideHelpFadeNerve::InstallAtSymbol("_ZN24StageSceneStatePauseMenu17exeFadeBeforeHelpEv");
    }
}