#ifndef __GFXINFO_H__
#define __GFXINFO_H__

#define NUMBER_OF_TEXTURES 37

#include "circle_bin.h"
#include "circleoverlay_bin.h"
#include "circleapproach_bin.h"
#include "spinner_bin.h"
#include "spinnerbg_bin.h"
#include "spinnerbars_bin.h"
#include "disc_bin.h"
#include "sliderb0_bin.h"
#include "sliderb1_bin.h"
#include "sliderb2_bin.h"
#include "sliderb3_bin.h"
#include "sliderb4_bin.h"
#include "sliderb5_bin.h"
#include "sliderb6_bin.h"
#include "sliderb7_bin.h"
#include "sliderb8_bin.h"
#include "sliderb9_bin.h"
#include "sliderfollow_bin.h"
#include "slidertick_bin.h"
#include "sliderreverse_bin.h"

#include "hit0_bin.h"
#include "hit50_bin.h"
#include "hit100_bin.h"
#include "hit100k_bin.h"
#include "hit300_bin.h"
#include "hit300k_bin.h"
#include "hit300g_bin.h"
#include "slider10_bin.h"
#include "slider30_bin.h"

#include "scorebar_bin.h"
#include "scorebar_colour_bin.h"
#include "scorebar_ki_bin.h"
#include "scorebar_kidanger_bin.h"
#include "scorebar_kidanger2_bin.h"

#include "white_bin.h"

#include <nds/arm9/video.h>
#include <nds/arm9/videoGL.h>

//#include "songbg_osu_bin.h"

typedef enum {
	TX_PLAY_CIRCLE,
	TX_PLAY_CIRCLEOVERLAY,
	TX_PLAY_CIRCLEAPPROACH,
	TX_PLAY_SPINNER,
	TX_PLAY_SPINNERBG,
	TX_PLAY_SPINNERBARS,
	TX_PLAY_SLIDERB0,
	TX_PLAY_SLIDERB1,
	TX_PLAY_SLIDERB2,
	TX_PLAY_SLIDERB3,
	TX_PLAY_SLIDERB4,
	TX_PLAY_SLIDERB5,
	TX_PLAY_SLIDERB6,
	TX_PLAY_SLIDERB7,
	TX_PLAY_SLIDERB8,
	TX_PLAY_SLIDERB9,
	TX_PLAY_DISC,
	TX_PLAY_SLIDERFOLLOW,
	TX_PLAY_SLIDERTICK,
	TX_PLAY_SLIDERREVERSE,
	
	TX_PLAY_HIT0,
	TX_PLAY_HIT50,
	TX_PLAY_HIT100,
	TX_PLAY_HIT100K,
	TX_PLAY_HIT300,
	TX_PLAY_HIT300K,
	TX_PLAY_HIT300G,
	TX_PLAY_SLIDER10,
	TX_PLAY_SLIDER30,
	
	TX_PLAY_SCOREBAR,
	TX_PLAY_SCOREBAR_BAR,
	TX_PLAY_SCOREBAR_KI,
	TX_PLAY_SCOREBAR_KIDANGER,
	TX_PLAY_SCOREBAR_KIDANGER2,
	
	TX_WHITE,
	
	TX_SONGSELECT_SONGBG
} TextureType;

// arrays

//extern const float zvalue[];

extern const u16 palette0[];
extern const u16 palette1[];
extern const u16 palette2[];
extern const u16 palette3[];
extern const u16 palette4[];
extern const u16 palette5[];
extern const u16 palette6[];
extern const u16 palette7[];
extern const u16 palette8[];

extern const u32 uv8x8[];
extern const u32 uv16x16[];
extern const u32 uv32x32[];
extern const u32 uv64x64[];
extern const u32 uv128x128[];
extern const u32 uv256x16[];
extern const u32 uv256x192[];

extern const v16 plu[];

#endif

