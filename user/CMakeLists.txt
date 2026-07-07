# For those who want to use globbing, uncomment the `file` call below,
# and remove the manually entered sources from the `target_sources` call.

#file(GLOB_RECURSE SUBSDK9_SOURCES
#    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/*.s
#    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
#)

target_sources(subsdk9 PRIVATE
        ${SUBSDK9_SOURCES}
        src/main.cpp
        src/mallowConfig.cpp
        src/ActorFactory/actorPatches.cpp
        src/Kingdom-18/Kingdom-18-patches.cpp
        src/Kingdom-18/hooks/Kingdom.cpp
        src/Kingdom-18/hooks/SDFileRedirection.cpp
        src/Kingdom-18/hooks/SaveOnSDCard.cpp
        src/Kingdom-18/sead/seadNinSDFileDeviceNin.cpp
	src/Bgm/CustomMusicList.cpp
	src/Scene/PauseMenuHooks.cpp
	src/Scene/StageSceneStateModConfig.cpp
	src/Scene/StageSceneStatePauseMenu.cpp
)

target_include_directories(subsdk9 BEFORE PUBLIC src
        # add any extra include directories here...
        src
)
