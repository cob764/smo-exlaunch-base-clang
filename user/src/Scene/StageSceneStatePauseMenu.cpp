#include "Scene/StageSceneStatePauseMenu.h"

#include "Library/Message/MessageHolder.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Scene/SceneUtil.h"

#include "Layout/FooterParts.h"
#include "Scene/StageScene.h"

void StageSceneStatePauseMenu::exeModConfig(void) {
    if (al::isFirstStep(this)) {
    }

    al::updateKitListPrev(mHost);
    rs::requestGraphicsPresetAndCubeMapPause(mHost);
    al::updateKitList(mHost, "２Ｄ（ポーズ無視）");
    al::updateKitListPost(mHost);

    if (al::updateNerveState(this)) {
        if (mStateOption->isChangeLanguage() || mStateOption->mIsLoadData) {
            kill();
        } else {
            mSelectParts->appearWait();
            mFooterParts->tryChangeTextFade(al::getSystemMessageString(mMenuGuide, "Footer", "MenuMessage_Footer"));

            al::setNerve(this, &NrvStageSceneStatePauseMenu.Wait);
        }
    }
}