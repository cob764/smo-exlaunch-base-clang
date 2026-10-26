#include <mallow/mallow.hpp>
#include "Scene/StageSceneStateModConfig.hpp"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "game/Layout/CommonVerticalList.h"
#include "Layout/FooterParts.h"
#include "Layout/SimpleLayoutMenu.h"
#include "Util/StageInputFunction.h"

#include "Library/Bgm/BgmLineFunction.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderWriter.h"

// Not present in this project's OdysseyHeaders copy, so declared locally.
// Matches the real struct layout confirmed via decompiled source.
// (BgmLineFunction.h already forward-declares this class and the startBgm
// overload that takes it -- we only need to provide the actual definition.)
namespace al {
class BgmPlayingRequest {
public:
    BgmPlayingRequest(const char* name) : bgmName(name) {}
    const char* bgmName = nullptr;
    const char* _8 = nullptr;
    const char* _10 = nullptr;
    s32 _18 = -1;
    s32 _1c = 15;
    s32 _20;
    s32 _24;
    s32 _28;
    bool _2c = false;
    bool _2d = false;
    bool _2e = false;
    bool _2f = false;
};
}  // namespace al

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

    const auto& list = ca::getCombinedBgmList();
    const char* name = list[mCurrentSongIndex].field_0;

    char16 label[80];
    int idx = 0;
    label[idx++] = u'<';
    label[idx++] = u' ';
    for (int j = 0; name[j] != '\0' && idx < 77; ++j)
        label[idx++] = (char16)name[j];
    label[idx++] = u' ';
    label[idx++] = u'>';
    label[idx] = u'\0';

    msgList[MENU_MAIN]->mBuffer[MAIN_PLAY_CUSTOM_SONG].copy(label);

    optionsList[MENU_MAIN]->updateParts();
}
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
        mallow::log::logLine("decide end reached, mCurSelected=%d", mCurrentList->mCurSelected);
        switch (mCurrentList->mCurSelected) {
        case MAIN_DUMMY_ACTION:
            // TODO: replace with real action
            break;
        case MAIN_PLAY_CUSTOM_SONG: {
            const auto& list = ca::getCombinedBgmList();
            const char* songName = list[mCurrentSongIndex].field_0;
            bool disabled = al::isDisableBgmSituationChange(getHost());
            mallow::log::logLine("isDisableBgmSituationChange=%d", disabled);
            if (disabled)
                al::enableBgmSituationChange(getHost(), true);
            bool registed = al::tryRegistTargetBgmResourceName(getHost(), songName);
            mallow::log::logLine("regist=%d for name=%s", registed, songName);
            al::BgmPlayingRequest request(songName);
            al::startBgm(getHost(), request, false, false);
            bool playing = al::isPlayingBgm(getHost(), songName);
            mallow::log::logLine("isPlayingBgm(%s)=%d", songName, playing);
            break;
        }
        }
        activateInput();
    }
}

void StageSceneStateModConfig::handleMenuInput() {
    mInput->update();
    mCurrentList->update();

    mallow::log::logLine("mCurSelected = %d", mCurrentList->mCurSelected);

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

    if (mCurrentList->mCurSelected == MAIN_PLAY_CUSTOM_SONG) {
        const auto& list = ca::getCombinedBgmList();
        s32 count = (s32)list.size();

        bool right = mInput->isTriggerUiRight();
        bool left = mInput->isTriggerUiLeft();
        if (right || left)
            mallow::log::logLine("song row: right=%d left=%d", right, left);

        if (right) {
            mCurrentSongIndex = (mCurrentSongIndex + 1) % count;
            updateMainMenuOptions();
        } else if (left) {
            mCurrentSongIndex = (mCurrentSongIndex - 1 + count) % count;
            updateMainMenuOptions();
        }
    }

    if (rs::isTriggerUiDecide(mHost)) {
        mallow::log::logLine("decide triggered, mCurSelected=%d", mCurrentList->mCurSelected);
        deactivateInput();
    }
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