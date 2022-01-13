#include "GraphicsManager.h"
#include <string>

GraphicsManager GraphicsManager::sGraphicsManager;

int nextPBlock;

//---------------------------------------------------------------------------------
inline uint32 alignVal( uint32 val, uint32 to ) {
    return (val & (to-1))? (val & ~(to-1)) + to : val;
}

//---------------------------------------------------------------------------------
int getNextPaletteSlot(u16 count, uint8 format) {
//---------------------------------------------------------------------------------
    // ensure the result aligns on a palette block for this format
    uint32 result = alignVal(nextPBlock, 1<<(4-(format==GL_RGB4)));

    // convert count to bytes and align to next (smallest format) palette block
    count = alignVal( count<<1, 1<<3 );

    // ensure that end is within palette video mem
    if( result+count > 0x10000 )   // VRAM_F - VRAM_E
        return -1;

    nextPBlock = result+count;
    return (int)result;
}

//---------------------------------------------------------------------------------
void glTexLoadPal(const u16* pal, u16 count, u32 addr) {
//---------------------------------------------------------------------------------
    vramSetBankE(VRAM_E_LCD);
    swiCopy( pal, &VRAM_E[addr>>1] , count / 2 | COPY_MODE_WORD);
    vramSetBankE(VRAM_E_TEX_PALETTE);
}

//---------------------------------------------------------------------------------
int gluTexLoadPal(const u16* pal, u16 count, uint8 format) {
//---------------------------------------------------------------------------------
    int addr = getNextPaletteSlot(count, format);
    if( addr>=0 )
        glTexLoadPal(pal, count, (u32) addr);

    return addr;
}

GraphicsManager::GraphicsManager()
{
    videoSetMode(MODE_5_3D);
    videoSetModeSub(MODE_5_2D);

    vramSetBankA(VRAM_A_TEXTURE);
	vramSetBankB(VRAM_B_MAIN_BG_0x06020000);
    vramSetBankC(VRAM_C_SUB_BG);
    vramSetBankD(VRAM_D_MAIN_BG_0x06000000);
    vramSetBankE(VRAM_E_TEX_PALETTE);

	REG_BG0CNT = 1;

    glInit();
    glEnable(GL_BLEND | GL_TEXTURE_2D | GL_ANTIALIAS);

    // setup the rear plane
    glClearColor(20,20,31,0);
    glClearPolyID(63);
    glClearDepth(0x7FFF);

    glViewport(0,0,255,191);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(100.3, 4.0/3.0, 0.1, 100); //fovy, aspect(width/height), zNear, zFar
    glOrtho(0.f, 6.40f, 0.f, 4.80f, 0.1f, 100.f);

    // camera is flipped around a bit - x increases left to right, y increases top to bottom (0,0) to (640,480)
    gluLookAt(	0, 4.8, -50.0,		//camera position
                  0, 4.8, 0.0,		//look at
                  0.0, -1.0, 0.0);	//up

    glMatrixMode(GL_MODELVIEW);

    mPolygonId = 0;

    //// LOAD TEXTURES ////

    glGenTextures(NUMBER_OF_TEXTURES, textures);

    int pal0 = gluTexLoadPal(palette0, 4, GL_RGB4);

    LoadGLTexture(TX_PLAY_CIRCLE, 			GL_RGB4, TEXTURE_SIZE_64, TEXTURE_SIZE_64, pal0, uv64x64, circle_bin);
    LoadGLTexture(TX_PLAY_CIRCLEOVERLAY, 	GL_RGB4, TEXTURE_SIZE_64, TEXTURE_SIZE_64, pal0, uv64x64, circleoverlay_bin);
    LoadGLTexture(TX_PLAY_CIRCLEAPPROACH, 	GL_RGB4, TEXTURE_SIZE_64, TEXTURE_SIZE_64, pal0, uv64x64, circleapproach_bin);

    LoadGLTexture(TX_PLAY_DISC,				GL_RGB4, TEXTURE_SIZE_32, TEXTURE_SIZE_32, pal0, uv32x32, disc_bin);
    LoadGLTexture(TX_PLAY_SLIDERTICK, 		GL_RGB4, TEXTURE_SIZE_16, TEXTURE_SIZE_16, pal0, uv16x16, slidertick_bin);
    LoadGLTexture(TX_PLAY_SLIDERREVERSE,	GL_RGB4, TEXTURE_SIZE_32, TEXTURE_SIZE_32, pal0, uv32x32, sliderreverse_bin);

    LoadGLTexture(TX_WHITE,					GL_RGB4, TEXTURE_SIZE_8, TEXTURE_SIZE_8, pal0, uv8x8, white_bin);

    int pal1 = gluTexLoadPal(palette1, 16, GL_RGB16);

    LoadGLTexture(TX_PLAY_SPINNER,			GL_RGB16, TEXTURE_SIZE_128, TEXTURE_SIZE_128, pal1, uv128x128, spinner_bin);
    LoadGLTexture(TX_PLAY_SPINNERBARS, 		GL_RGB16, TEXTURE_SIZE_256, TEXTURE_SIZE_256, pal1, uv256x192, spinnerbars_bin);
    LoadGLTexture(TX_PLAY_SCOREBAR_BAR,		GL_RGB16, TEXTURE_SIZE_256, TEXTURE_SIZE_16, pal1, uv256x16, scorebar_colour_bin);

    int pal2 = gluTexLoadPal(palette2, 16, GL_RGB4);

    LoadGLTexture(TX_PLAY_SPINNERBG, 		GL_RGB16, TEXTURE_SIZE_256, TEXTURE_SIZE_256, pal2, uv256x192, spinnerbg_bin);

    int pal3 = gluTexLoadPal(palette3, 4, GL_RGB4);

    LoadGLTexture(TX_PLAY_SLIDERB0, 		GL_RGB4, TEXTURE_SIZE_64, TEXTURE_SIZE_64, pal3, uv64x64, sliderb0_bin);
    LoadGLTexture(TX_PLAY_SLIDERB1, 		GL_RGB4, TEXTURE_SIZE_64, TEXTURE_SIZE_64, pal3, uv64x64, sliderb1_bin);
    LoadGLTexture(TX_PLAY_SLIDERB2, 		GL_RGB4, TEXTURE_SIZE_64, TEXTURE_SIZE_64, pal3, uv64x64, sliderb2_bin);
    LoadGLTexture(TX_PLAY_SLIDERB3, 		GL_RGB4, TEXTURE_SIZE_64, TEXTURE_SIZE_64, pal3, uv64x64, sliderb3_bin);
    LoadGLTexture(TX_PLAY_SLIDERB4, 		GL_RGB4, TEXTURE_SIZE_64, TEXTURE_SIZE_64, pal3, uv64x64, sliderb4_bin);
    LoadGLTexture(TX_PLAY_SLIDERB5, 		GL_RGB4, TEXTURE_SIZE_64, TEXTURE_SIZE_64, pal3, uv64x64, sliderb5_bin);
    LoadGLTexture(TX_PLAY_SLIDERB6, 		GL_RGB4, TEXTURE_SIZE_64, TEXTURE_SIZE_64, pal3, uv64x64, sliderb6_bin);
    LoadGLTexture(TX_PLAY_SLIDERB7, 		GL_RGB4, TEXTURE_SIZE_64, TEXTURE_SIZE_64, pal3, uv64x64, sliderb7_bin);
    LoadGLTexture(TX_PLAY_SLIDERB8, 		GL_RGB4, TEXTURE_SIZE_64, TEXTURE_SIZE_64, pal3, uv64x64, sliderb8_bin);
    LoadGLTexture(TX_PLAY_SLIDERB9, 		GL_RGB4, TEXTURE_SIZE_64, TEXTURE_SIZE_64, pal3, uv64x64, sliderb9_bin);
    LoadGLTexture(TX_PLAY_SLIDERFOLLOW, 	GL_RGB4, TEXTURE_SIZE_64, TEXTURE_SIZE_64, pal3, uv64x64, sliderfollow_bin);

    int pal4 = gluTexLoadPal(palette4, 16, GL_RGB16);

    LoadGLTexture(TX_PLAY_HIT0,				GL_RGB16, TEXTURE_SIZE_64, TEXTURE_SIZE_64, pal4, uv64x64, hit0_bin);
    LoadGLTexture(TX_PLAY_HIT300,			GL_RGB16, TEXTURE_SIZE_64, TEXTURE_SIZE_64, pal4, uv64x64, hit300_bin);
    LoadGLTexture(TX_PLAY_HIT300K,			GL_RGB16, TEXTURE_SIZE_64, TEXTURE_SIZE_64, pal4, uv64x64, hit300k_bin);
    LoadGLTexture(TX_PLAY_HIT300G,			GL_RGB16, TEXTURE_SIZE_64, TEXTURE_SIZE_64, pal4, uv64x64, hit300g_bin);

    int pal5 = gluTexLoadPal(palette5, 16, GL_RGB16);

    LoadGLTexture(TX_PLAY_HIT50,			GL_RGB16, TEXTURE_SIZE_64, TEXTURE_SIZE_64, pal5, uv64x64, hit50_bin);
    LoadGLTexture(TX_PLAY_HIT100,			GL_RGB16, TEXTURE_SIZE_64, TEXTURE_SIZE_64, pal5, uv64x64, hit100_bin);
    LoadGLTexture(TX_PLAY_HIT100K,			GL_RGB16, TEXTURE_SIZE_64, TEXTURE_SIZE_64, pal5, uv64x64, hit100k_bin);

    int pal6 = gluTexLoadPal(palette6, 4, GL_RGB4);

    LoadGLTexture(TX_PLAY_SLIDER30,			GL_RGB4, TEXTURE_SIZE_16, TEXTURE_SIZE_16, pal6, uv16x16, slider30_bin);
    LoadGLTexture(TX_PLAY_SLIDER10,			GL_RGB4, TEXTURE_SIZE_16, TEXTURE_SIZE_16, pal6, uv16x16, slider10_bin);

    int pal7 = gluTexLoadPal(palette7, 16, GL_RGB16);

    LoadGLTexture(TX_PLAY_SCOREBAR_KI,		GL_RGB16, TEXTURE_SIZE_32, TEXTURE_SIZE_32, pal7, uv32x32, scorebar_ki_bin);
    LoadGLTexture(TX_PLAY_SCOREBAR_KIDANGER, GL_RGB16, TEXTURE_SIZE_32, TEXTURE_SIZE_32, pal7, uv32x32, scorebar_kidanger_bin);
    LoadGLTexture(TX_PLAY_SCOREBAR_KIDANGER2, GL_RGB16, TEXTURE_SIZE_32, TEXTURE_SIZE_32, pal7, uv32x32, scorebar_kidanger2_bin);

    int pal8 = gluTexLoadPal(palette8, 16, GL_RGB16);

    LoadGLTexture(TX_PLAY_SCOREBAR, 		GL_RGB16, TEXTURE_SIZE_256, TEXTURE_SIZE_16, pal8, uv256x16, scorebar_bin);

    // 16 bit textures

    //LoadGLTexture(TX_SONGSELECT_SONGBG,	GL_RGB, TEXTURE_SIZE_64, TEXTURE_SIZE_512, -1, NULL, songbg_osu_bin);
}

void GraphicsManager::LoadGLTextureEx(TextureType tex, GL_TEXTURE_TYPE_ENUM type, int sizeX, int sizeY, int palette, const u32* uv, const u8* texture, u32 size)
{
    /* DON'T TOUCH THIS FUNCTION
     * there seems to be some sort of memory problem somewhere, but it's completely eluding me where it is
     * the game only loads the textures if this function is the way it is >.>
     */

    void* temp = malloc(size);

    glBindTexture(0, textures[tex]);
    glTexImage2D(0, 0, type, sizeX, sizeY, 0, GL_TEXTURE_COLOR0_TRANSPARENT, (u8*)texture);

    free(temp);

    textureInfo[tex].palette = palette;
    textureInfo[tex].format = type;
    textureInfo[tex].uv = uv;
}

void GraphicsManager::Draw(TextureType tex, s32 x, s32 y, u32 width, u32 height, DrawOrigin origin, FieldType fieldtype, rgb color, u32 alpha, s32 angle, float z, const u32* uv)
{
    if (uv == NULL)
        uv = textureInfo[tex].uv;

    s32 x1 = 270, x2 = 370, y1 = 190, y2 = 290;
    //float z = zvalue[tex] + deltaz;

    if (fieldtype == FIELD_PLAY)
    {
        x += PlayXOffset;
        y += PlayYOffset;
    }

    switch (origin)
    {
        case ORIGIN_TOPLEFT:
            x1 = ForceBounds(x);
            x2 = ForceBounds(x + width);
            y1 = ForceBounds(y);
            y2 = ForceBounds(y + height);
            break;

        case ORIGIN_CENTER:
            width >>= 1;
            height >>= 1;

            x1 = ForceBounds(x - width);
            x2 = ForceBounds(x + width);
            y1 = ForceBounds(y - height);
            y2 = ForceBounds(y + height);
            break;

        case ORIGIN_BOTTOMLEFT:
            x1 = ForceBounds(x);
            x2 = ForceBounds(x + width);
            y1 = ForceBounds(y - height);
            y2 = ForceBounds(y);
            break;

        case ORIGIN_TOPRIGHT:
            x1 = ForceBounds(x - width);
            x2 = ForceBounds(x);
            y1 = ForceBounds(y);
            y2 = ForceBounds(y + height);
    }

    //need to keep rotating polygonid
    if (++mPolygonId > 63)
        mPolygonId = 0;

    //don't draw things out of the screen
    if (x1 > 640 || x2 < 0 || y1 > 480 || y2 < 0)
        return;

    glPushMatrix();

    glPolyFmt(POLY_ALPHA(alpha&31) | POLY_ID(mPolygonId) | POLY_CULL_NONE);
    glColor(color);

    if (angle != 0)
    {
        glTranslatef(x/100.0, y/100.0, 0);
        glRotateZi(angle);
        glTranslatef(-x/100.0, -y/100.0, 0);
    }

    glBindTexture(0, textures[tex]);

    if (textureInfo[tex].palette >= 0)
        GFX_PAL_FORMAT = textureInfo[tex].palette >> (4 - (textureInfo[tex].format == GL_RGB4));

    glBegin(GL_QUADS);

    GFX_TEX_COORD = uv[0];
    glVertex2lu1f(x1, y1, z);

    GFX_TEX_COORD = uv[1];
    glVertex2lu1f(x2, y1, z);

    GFX_TEX_COORD = uv[2];
    glVertex2lu1f(x2, y2, z);

    GFX_TEX_COORD = uv[3];
    glVertex2lu1f(x1, y2, z);

    glEnd();
//	glClearColor()

    glPopMatrix(1);
}

s32 GraphicsManager::ForceBounds(s32 value)
{
    if (value < -200)
        return -200;

    if (value > 799)
        return 799;

    return value;
}

