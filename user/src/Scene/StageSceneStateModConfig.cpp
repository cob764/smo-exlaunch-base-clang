#include "Scene/StageSceneStateModConfig.hpp"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "game/Layout/CommonVerticalList.h"
#include "Layout/FooterParts.h"
#include "Layout/SimpleLayoutMenu.h"
#include "Util/StageInputFunction.h"
//static storage
bool StageSceneStateModConfig::sExampleToggle = false;

StageSceneStateModConfig::StageSceneStateModConfig(
    const char* name, al::Scene* scene,
    const al::LayoutInitInfo& initInfo,
    FooterParts* footerParts,
    GameDataHolder* dataHolder,
    bool)
    : al::HostStateBase<al::Scene>(name, scene) {
    mFooterParts = footerParts;
    mGameDataHolder = dataHolder;
    mMsgSystem = initInfo.getMessageSystem();
    mInput = new InputSeparator(mHost, true);

    for (int i = 0; i < menuCount; i++)
        msgList[i] = new sead::SafeArray<sead::WFixedSafeString<0x200>, maxMsgCount>();

    initMainMenu(initInfo);

    mCurrentList = optionsList[MENU_MAIN];
    mCurrentMenu = menuList[MENU_MAIN];
}

void StageSceneStateModConfig::init() {
    initNerve(&NrvStageSceneStateModConfig.MainMenu, 0);
}

void StageSceneStateModConfig::appear() {
    mCurrentMenu->startAppear("Appear");
    al::NerveStateBase::appear();
}

void StageSceneStateModConfig::kill() {
    mCurrentMenu->startEnd("End");
    al::NerveStateBase::kill();
}

void StageSceneStateModConfig::initMainMenu(const al::LayoutInitInfo& initInfo) {
    menuList[MENU_MAIN] = new SimpleLayoutMenu(
        "ModConfigMenu", "OptionModCheck", initInfo, 0, false);
    optionsList[MENU_MAIN] = new CommonVerticalList(
        menuList[MENU_MAIN], initInfo, true);

    al::setPaneString(menuList[MENU_MAIN], "TxtOption", u"Mod Configuration", 0);
    optionsList[MENU_MAIN]->initDataNoResetSelected(mMainMenuOptionCount);
    updateMainMenuOptions();
    optionsList[MENU_MAIN]->addStringData(msgList[MENU_MAIN]->mBuffer, "TxtContent");

    for (int i = 0; i < mMainMenuOptionCount; i++)
        setMenuItemBase(optionsList[MENU_MAIN]->mListPartsArr[i + 1]);
}
//Update this to reflect the current state of your menu options. For example, if you have a toggle option, update the text to show whether it's on or off.
void StageSceneStateModConfig::updateMainMenuOptions() {
    for (int i = 0; i < maxMsgCount; i++)
        msgList[MENU_MAIN]->mBuffer[i].copy(u"");

    msgList[MENU_MAIN]->mBuffer[MAIN_DUMMY_ACTION].copy(u"Dummy Action");
    msgList[MENU_MAIN]->mBuffer[MAIN_TOGGLE_EXAMPLE].copy(
    sExampleToggle ? u"Example Toggle: ON" : u"Example Toggle: OFF");
}
//Update exeMainMenu() to handle the input and actions for your menu options. For example, if you have a toggle option, flip its state when selected.
void StageSceneStateModConfig::exeMainMenu() {
    if (al::isFirstStep(this)) {
        mCurrentList = optionsList[MENU_MAIN];
        mCurrentMenu = menuList[MENU_MAIN];
        activateInput();
    }

    handleMenuInput();

    if (rs::isTriggerUiCancel(mHost))
        kill();

    if (mIsDecideConfig && mCurrentList->isDecideEnd()) {
        switch (mCurrentList->mCurSelected) {
        case MAIN_DUMMY_ACTION:
            // TODO: replace with real action
            break;
        case MAIN_TOGGLE_EXAMPLE:
        sExampleToggle = !sExampleToggle;
        updateMainMenuOptions();
        break;
        }
        activateInput();
    }
}

void StageSceneStateModConfig::handleMenuInput() {
    mInput->update();
    mCurrentList->update();

    if (mInput->isHoldUiUp() && mInput->isRepeatUiUp()) {
        if (mInput->isTriggerUiUp() && mCurrentList->mCurSelected == mCurrentList->mTopSelectableIdx)
            mCurrentList->jumpBottom();
        mCurrentList->up();
    }

    if (mInput->isHoldUiDown() && mInput->isRepeatUiDown()) {
        if (mInput->isTriggerUiDown() && mCurrentList->mCurSelected == (mCurrentList->mDataCount - 1))
            mCurrentList->jumpTop();
        mCurrentList->down();
    }

    if (rs::isTriggerUiDecide(mHost))
        deactivateInput();
}

void StageSceneStateModConfig::activateInput() {
    mInput->reset();
    mCurrentList->activate();
    mCurrentList->appearCursor();
    mIsDecideConfig = false;
}

void StageSceneStateModConfig::deactivateInput() {
    al::startHitReaction(mCurrentMenu, "決定", 0);
    mCurrentList->endCursor();
    mCurrentList->decide();
    mIsDecideConfig = true;
}