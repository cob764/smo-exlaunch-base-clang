#include <exl/lib.hpp>

#include "Kingdom-18/sead/seadNinSDFileDeviceNin.h"
#include <filedevice/seadFileDevice.h>
#include <nn/fs.h>

#include "Library/Base/StringUtil.h"

HOOK_DEFINE_TRAMPOLINE(SaveWriteHook) {
    static s32 Callback(void* thisPtr, sead::FileDevice* _, const char* name) {
        nn::fs::DirectoryEntryType type;
        if (nn::fs::GetEntryType(&type, "sd:/smo/GhostDlc/save").IsFailure() ||
            type != nn::fs::DirectoryEntryType_Directory) {
            nn::fs::CreateDirectory("sd:/smo/");
            nn::fs::CreateDirectory("sd:/smo/GhostDlc/");
            nn::fs::CreateDirectory("sd:/smo/GhostDlc/save/");
        }

        al::StringTmp<50> newPath = {"GhostDlc/save/%s", name};
        sead::NinSDFileDevice newDevice = sead::NinSDFileDevice();

        return Orig(thisPtr, &newDevice, newPath.cstr());
    }
};

HOOK_DEFINE_TRAMPOLINE(SaveReadHook) {
    static s32 Callback(void* thisPtr, sead::FileDevice* _, const char* name, void* flags) {
        al::StringTmp<50> newPath = {"GhostDlc/save/%s", name};
        sead::NinSDFileDevice newDevice = sead::NinSDFileDevice();

        return Orig(thisPtr, &newDevice, newPath.cstr(), flags);
    }
};

void allowSaveOnSDCard() {
    SaveReadHook::InstallAtSymbol("_ZN2al20SaveDataSequenceRead4readEPN4sead10FileDeviceEPKcPj");
    SaveWriteHook::InstallAtSymbol("_ZN2al21SaveDataSequenceWrite5writeEPN4sead10FileDeviceEPKc");
}
