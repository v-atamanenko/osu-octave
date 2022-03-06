cmake_minimum_required(VERSION 3.0)

if(NOT DEFINED BUILDTARGET)
    #message(WARNING "Please define BUILDTARGET to choose your target platform: [desktop, vita]. Defaulting to vita.")
    set(BUILDTARGET "desktop")
endif()

if (BUILDTARGET STREQUAL "vita")
    if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        if(DEFINED ENV{VITASDK})
            set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
        else()
            message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
        endif()
    endif()
endif()

project(osu-ds)

if (BUILDTARGET STREQUAL "vita")
    include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)
    set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1 -d ATTRIBUTE2=12")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17 -Wno-narrowing -Wno-psabi")
# DEBUG:
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fsanitize=address,undefined,integer")

add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCE_DIR}/source/System/SDLInitializer.cpp # Important to be on top!
        ${PROJECT_SOURCE_DIR}/source/Modes/Player.cpp
        ${PROJECT_SOURCE_DIR}/source/Modes/SongSelect.cpp
        ${PROJECT_SOURCE_DIR}/source/Modes/Welcome.cpp
        ${PROJECT_SOURCE_DIR}/source/Modes/Mode.cpp
        ${PROJECT_SOURCE_DIR}/source/Beatmaps/BeatmapElements.cpp
        ${PROJECT_SOURCE_DIR}/source/Beatmaps/Beatmap.cpp
        ${PROJECT_SOURCE_DIR}/source/Beatmaps/BeatmapManager.cpp
        ${PROJECT_SOURCE_DIR}/source/Beatmaps/DifficultyManager.cpp
        ${PROJECT_SOURCE_DIR}/source/System/TextManager.cpp
        ${PROJECT_SOURCE_DIR}/source/System/GameBase.cpp
        ${PROJECT_SOURCE_DIR}/source/System/GameClock.cpp
        ${PROJECT_SOURCE_DIR}/source/System/SDL_FontCache.c
        ${PROJECT_SOURCE_DIR}/source/GameplayElements/Lifebar.cpp
        ${PROJECT_SOURCE_DIR}/source/GameplayElements/Score.cpp
        ${PROJECT_SOURCE_DIR}/source/GameplayElements/HitObjectManager.cpp
        ${PROJECT_SOURCE_DIR}/source/GameplayElements/DarkenOverlay.cpp
        ${PROJECT_SOURCE_DIR}/source/Rulesets/RulesetOsu.cpp
        ${PROJECT_SOURCE_DIR}/source/Rulesets/Ruleset.cpp
        ${PROJECT_SOURCE_DIR}/source/Graphics/pText.cpp
        ${PROJECT_SOURCE_DIR}/source/Graphics/pSprite.cpp
        ${PROJECT_SOURCE_DIR}/source/Graphics/GraphicsManager.cpp
        ${PROJECT_SOURCE_DIR}/source/Graphics/SpriteContainer.cpp
        ${PROJECT_SOURCE_DIR}/source/Graphics/SpriteManager.cpp
        ${PROJECT_SOURCE_DIR}/source/Graphics/Transformation.cpp
        ${PROJECT_SOURCE_DIR}/source/Graphics/pDrawable.cpp
        ${PROJECT_SOURCE_DIR}/source/Graphics/pAnimation.cpp
        ${PROJECT_SOURCE_DIR}/source/HitObjects/HitSpinner.cpp
        ${PROJECT_SOURCE_DIR}/source/HitObjects/HitSlider.cpp
        ${PROJECT_SOURCE_DIR}/source/HitObjects/HitCircle.cpp
        ${PROJECT_SOURCE_DIR}/source/HitObjects/HitObject.cpp
        ${PROJECT_SOURCE_DIR}/source/Helpers/MD5.cpp
        ${PROJECT_SOURCE_DIR}/source/Helpers/MathHelper.cpp
        ${PROJECT_SOURCE_DIR}/source/Helpers/InputHelper.cpp
        ${PROJECT_SOURCE_DIR}/source/Helpers/AudioManager.cpp
        ${PROJECT_SOURCE_DIR}/source/Helpers/OsuSliderCurves.cpp
        ${PROJECT_SOURCE_DIR}/source/Helpers/PreviewBuffer.cpp

        ${PROJECT_SOURCE_DIR}/source/Parser/OsuParser.cpp
        ${PROJECT_SOURCE_DIR}/source/Parser/OsUtils.cpp

        ${PROJECT_SOURCE_DIR}/source/main.cpp)

include_directories(
        ${PROJECT_SOURCE_DIR}/source
)

if (BUILDTARGET STREQUAL "vita")
    add_definitions(-DVITA)
    find_package(SDL2 REQUIRED)
    target_link_libraries(${PROJECT_NAME}
            SDL2_mixer
            SDL2_ttf
            SDL2_image
            SDL2::SDL2-static
            FLAC
            vorbisfile
            vorbis
            mpg123
            mikmod
            webp
            jpeg
            freetype
            ogg
            png
            pthread
            ScePower_stub
            SceDisplay_stub
            SceCtrl_stub
            SceAudio_stub
            SceSysmodule_stub
            SceGxm_stub
            SceCommonDialog_stub
            SceAppMgr_stub
            SceTouch_stub
            SceHid_stub
            SceMotion_stub
            z
            m
            c
            )
else()
    add_definitions(-DDESKTOP)
    #find_package(SDL2 REQUIRED HINTS /usr/local/Cellar/sdl2/2.0.20/)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments -I/usr/local/Cellar/sdl2/2.0.20/include -I/usr/local/Cellar/sdl2_image/2.0.5/include -I/usr/local/Cellar/sdl2_ttf/2.0.18/include -I/usr/local/Cellar/sdl2_mixer/2.0.4_3/include -L/usr/local/Cellar/sdl2/2.0.20/lib -L/usr/local/Cellar/sdl2_image/2.0.5/lib -L/usr/local/Cellar/sdl2_ttf/2.0.18/lib -L/usr/local/Cellar/sdl2_mixer/2.0.4_3/lib")

    INCLUDE(FindPkgConfig)

    PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
    PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
    PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)
    PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)

    INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2MIXER_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS})

    TARGET_LINK_LIBRARIES(${PROJECT_NAME} SDL2_image SDL2_mixer SDL2_ttf SDL2)


    #find_package(SDL2_image REQUIRED HINTS /usr/local/Cellar/sdl2_image/2.0.5/lib/pkgconfig/)
    #include_directories(${SDL2_INCLUDE_DIRS})
    #target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
endif()

if (BUILDTARGET STREQUAL "vita")
    vita_create_self(eboot.bin ${PROJECT_NAME})
    vita_create_vpk(osu-ds.vpk "OSU000001" eboot.bin
            VERSION "01.00"
            NAME "osu-ds!"
            #FILE sce_sys/icon0.png sce_sys/icon0.png
            #FILE sce_sys/bg.png sce_sys/livearea/contents/bg.png
            #FILE sce_sys/startup.png sce_sys/livearea/contents/startup.png
            #FILE sce_sys/template.xml sce_sys/livearea/contents/template.xml
            )
endif()