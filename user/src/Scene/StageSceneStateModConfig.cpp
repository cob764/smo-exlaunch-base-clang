#include <mallow/mallow.hpp>
#include "Scene/StageSceneStateModConfig.hpp"

#include <cstdio>

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/File/FileUtil.h"
#include "Library/Message/IUseMessageSystem.h"
#include "Library/Message/LanguageUtil.h"
#include "Library/Message/MessageHolder.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Scene/SceneObjUtil.h"

#include "game/Layout/CommonVerticalList.h"
#include "Layout/FooterParts.h"
#include "Layout/SimpleLayoutMenu.h"
#include "Util/StageInputFunction.h"

#include "Audio/CollectBgmPlayer.h"
#include "Bgm/CustomMusicList.h"
#include "ModText.h"  // user/src/Scene/ModText.h (same folder as this file)

bool deathbgmtrigger = true;  // global variable to trigger death BGM in the mod config menu

namespace {
const char* kBgmDbArchive = "SoundData/BgmDataBase";  // checked with al::isExistArchive
const s32 kInfoPages = 3;

struct U16Buf {
    char16 data[96];
    int len = 0;
    U16Buf() { data[0] = u'\0'; }
    void add(const char16* s) {
        for (; s && *s && len < 94; ++s)
            data[len++] = *s;
        data[len] = u'\0';
    }
    void add(const char* s) {
        for (; s && *s && len < 94; ++s)
            data[len++] = (char16)(unsigned char)*s;
        data[len] = u'\0';
    }
    void addInt(int v) {
        char t[16];
        snprintf(t, sizeof(t), "%d", v);
        add(t);
    }
};

const char16* lookupSongName(const al::IUseMessageSystem* ms, const BgmEntry& e) {
    char key[128];
    if (e.field_1) {  // variants, e.g. the 2D version of the same song
        snprintf(key, sizeof(key), "%s_%s", e.field_0, e.field_1);
        if (al::isExistLabelInSystemMessage(ms, "CollectBgmList", key))
            return al::getSystemMessageString(ms, "CollectBgmList", key);
    }
    if (al::isExistLabelInSystemMessage(ms, "CollectBgmList", e.field_0))
        return al::getSystemMessageString(ms, "CollectBgmList", e.field_0);
    return nullptr;
}
}  // namespace

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

    al::setPaneString(menuList[MENU_MAIN], "TxtOption",
                      ca::modText(menuList[MENU_MAIN], "Title", u"Mod Configuration"), 0);
    optionsList[MENU_MAIN]->initDataNoResetSelected(mMainMenuOptionCount);
    updateMainMenuOptions();
    optionsList[MENU_MAIN]->addStringData(msgList[MENU_MAIN]->mBuffer, "TxtContent");

    for (int i = 0; i < mMainMenuOptionCount; i++)
        setMenuItemBase(optionsList[MENU_MAIN]->mListPartsArr[i + 1]);
}

// The title-screen menu has no CollectBgmPlayer scene object -> the BGM player is unavailable.
bool StageSceneStateModConfig::isBgmPlayerAvailable() const {
    return al::isExistSceneObj<CollectBgmPlayer>(mHost);
}

// Only clears the buffers; the real row texts are written every frame by refreshRows().
void StageSceneStateModConfig::updateMainMenuOptions() {
    for (int i = 0; i < maxMsgCount; i++)
        msgList[MENU_MAIN]->mBuffer[i].copy(u"");
    optionsList[MENU_MAIN]->updateParts();
}

// Writes the text of every row straight into the list (called every frame).
void StageSceneStateModConfig::refreshRows() {
    const al::IUseMessageSystem* ms = menuList[MENU_MAIN];
    CommonVerticalList* list = optionsList[MENU_MAIN];
    auto setRow = [&](int row, const char16* text) {
        al::setPaneString(list->mListPartsArr[row + 1], "TxtContent", text, 0);
    };

    // Row 0: song selector (or the "unavailable" message)
    if (isBgmPlayerAvailable()) {
        const BgmEntry& e = ca::getCombinedBgmList()[mCurrentSongIndex];
        U16Buf b;
        b.add(u"< ");
        if (const char16* t = lookupSongName(ms, e))
            b.add(t);
        else
            b.add(e.field_0);
        b.add(u" >");
        setRow(MAIN_SONG_SELECT, b.data);
    } else {
        setRow(MAIN_SONG_SELECT,
               ca::modText(ms, "BgmUnavailable", u"The BGM Player is unavailable right now"));
    }

    // Row 1: play button
    setRow(MAIN_PLAY_SONG, ca::modText(ms, "Play", u"Play"));

    // Row 2: info pages
    {
        U16Buf b;
        b.add(ca::modText(ms, "Info", u"Info"));
        b.add(u": < ");
        switch (mInfoPage) {
        case 0:
            b.add(ca::modText(ms, "InfoSongs", u"Songs registered"));
            b.add(u": ");
            b.addInt((int)ca::getCombinedBgmList().size());
            break;
        case 1:
            b.add(ca::modText(ms, "InfoLanguage", u"Language"));
            b.add(u": ");
            b.add(al::getLanguageString());
            break;
        default:
            b.add(ca::modText(ms, "InfoBgmDb", u"BgmDataBase.szs"));
            b.add(u": ");
            if (al::isExistArchive(kBgmDbArchive))
                b.add(ca::modText(ms, "Found", u"Found"));
            else
                b.add(ca::modText(ms, "NotFound", u"Not found"));
            break;
        }
        b.add(u" >");
        setRow(MAIN_INFO, b.data);
    }
    {
        U16Buf b;
        b.add(ca::modText(ms, "InfoDeathBgm", u"Death BGM Trigger"));
        b.add(u": ");
        if (deathbgmtrigger == true)
            b.add(ca::modText(ms, "InfoDeathBgmOn", u"On"));
        else
            b.add(ca::modText(ms, "InfoDeathBgmOff", u"Off"));
        setRow(MAIN_DEATH_BGM, b.data);
    }
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
        switch (mCurrentList->mCurSelected) {
        case MAIN_SONG_SELECT:
        {
            if (!isBgmPlayerAvailable()) {
                mallow::log::logLine("Play pressed but the BGM player is unavailable (title menu)");
            } else {
                const BgmEntry& e = ca::getCombinedBgmList()[mCurrentSongIndex];
                auto* player = al::getSceneObj<CollectBgmPlayer>(getHost());
                mallow::log::logLine("CollectBgmPlayer=%p song=%s", player, e.field_0);
                if (player) {
                    player->stop(0);
                    player->start(e.field_0, e.field_1);
                }
                activateInput();
            }
        }
            
        case MAIN_PLAY_SONG:
        {
            if (!isBgmPlayerAvailable()) {
                mallow::log::logLine("Play pressed but the BGM player is unavailable (title menu)");
            } else {
                const BgmEntry& e = ca::getCombinedBgmList()[mCurrentSongIndex];
                auto* player = al::getSceneObj<CollectBgmPlayer>(getHost());
                mallow::log::logLine("CollectBgmPlayer=%p song=%s", player, e.field_0);
                if (player) {
                    player->stop(0);
                    player->start(e.field_0, e.field_1);
                }
                activateInput();
            }
        }
        case MAIN_INFO:
            activateInput();
            break;
        case MAIN_DEATH_BGM:
            deathbgmtrigger = !deathbgmtrigger;
            activateInput();
            break;
        }
        
    }
}

void StageSceneStateModConfig::handleMenuInput() {
    mInput->update();
    mCurrentList->update();

    refreshRows();

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

    int dir = mInput->isTriggerUiRight() ? 1 : (mInput->isTriggerUiLeft() ? -1 : 0);
    if (dir != 0) {
        switch (mCurrentList->mCurSelected) {
        case MAIN_SONG_SELECT:
            if (isBgmPlayerAvailable()) {
                s32 count = (s32)ca::getCombinedBgmList().size();
                mCurrentSongIndex = (mCurrentSongIndex + dir + count) % count;
            }
            break;
        case MAIN_INFO:
            mInfoPage = (mInfoPage + dir + kInfoPages) % kInfoPages;
            break;
        }
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