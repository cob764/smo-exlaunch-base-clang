#pragma once

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Message/IUseMessageSystem.h"
#include "Library/Message/MessageSystem.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveStateBase.h"
#include "Library/Scene/Scene.h"
#include "container/seadSafeArray.h"

#include "Library/Controller/InputFunction.h"
#include "game/Layout/CommonVerticalList.h"
#include "game/Input/InputSeparator.h"
#include "Layout/SimpleLayoutMenu.h"
#include "System/GameDataHolder.h"

class FooterParts;

class StageSceneStateModConfig : public al::HostStateBase<al::Scene>,
                                 public al::IUseMessageSystem {
public:
    StageSceneStateModConfig(const char* name, al::Scene* scene,
        const al::LayoutInitInfo& initInfo,
        FooterParts* footerParts,
        GameDataHolder* dataHolder,
        bool unused);

    virtual void init() override;
    virtual void appear() override;
    virtual void kill() override;
    virtual al::MessageSystem* getMessageSystem() const override { return mMsgSystem; }

    void exeMainMenu();

    static void setMenuItemBase(al::LayoutActor* item) {
        if (!item)
            return;
        al::hidePane(item, "WinCheck");
        al::hidePane(item, "Roll");
        al::setPaneLocalSize(item, "TxtContent", { 420, 50 });
    }

    bool isDone() const { return mIsDone; }

private:
    static constexpr int menuCount = 1;
    static constexpr int maxMsgCount = 8;

    SimpleLayoutMenu* menuList[menuCount];
    CommonVerticalList* optionsList[menuCount];
    sead::SafeArray<sead::WFixedSafeString<0x200>, maxMsgCount>* msgList[menuCount];

    enum Menu
    {
        MENU_MAIN = 0
    };

    // ---- main menu ----------------------------------------------------------
    void initMainMenu(const al::LayoutInitInfo& initInfo);
    void updateMainMenuOptions();
    void handleMenuInput();
    void activateInput();
    void deactivateInput();
// add menu options here:
    enum MainMenuOption
    {
        MAIN_DUMMY_ACTION = 0,
        MAIN_TOGGLE_EXAMPLE = 1
    };
    static constexpr int mMainMenuOptionCount = 2; //change this to the number of options you add to the menu
// somewhere accessible — a public static getter so other files
    public:
    static bool isExampleToggleOn() { return sExampleToggle; }

private:
    static bool sExampleToggle;

    // ---- state --------------------------------------------------------------
    al::MessageSystem* mMsgSystem = nullptr;
    FooterParts* mFooterParts = nullptr;
    GameDataHolder* mGameDataHolder = nullptr;
    InputSeparator* mInput = nullptr;

    SimpleLayoutMenu* mCurrentMenu = nullptr;
    CommonVerticalList* mCurrentList = nullptr;

    bool mIsDecideConfig = false;
    bool mIsDone = false;
};

namespace {
    NERVE_IMPL(StageSceneStateModConfig, MainMenu)
    NERVES_MAKE_STRUCT(StageSceneStateModConfig, MainMenu)
}