/*---------------------------------------------------------------------------------

Copyright (C) 2009
Steven Harrison

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1.	The origin of this software must not be misrepresented; you
must not claim that you wrote the original software. If you use
this software in a product, an acknowledgment in the product
documentation would be appreciated but is not required.
2.	Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.
3.	This notice may not be removed or altered from any source
distribution.



---------------------------------------------------------------------------------*/

#include <nds/ndstypes.h>
#include <nds/memory.h>
#include <nds/arm9/console.h>
#include <nds/arm9/video.h>
#include <nds/arm9/background.h>
#include <nds/debug.h>

#include <stdio.h>
#include <stdarg.h>
#include <sys/iosupport.h>

#include "gfxconsole.h"
#include "4x6_a_bmf.h" /*!< Default monospaced font >*/


BMFont gfx_default_font_mono;
//BMFont gfx_default_font_mcol;


// default console palette.
const u16 palette[] = {
					RGB15( 0, 0, 0), //30 normal black
					RGB15(15, 0, 0), //31 normal red	 
					RGB15( 0,15, 0), //32 normal green	
					RGB15(15,15, 0), //33 normal yellow	

					RGB15( 0, 0,15), //34 normal blue
					RGB15(15, 0,15), //35 normal magenta
					RGB15( 0,15,15), //36 normal cyan
					RGB15(24,24,24), //37 normal white

					RGB15(15,15,15), //40 bright black
					RGB15(31, 0, 0), //41 bright red
					RGB15( 0,31, 0), //42 bright green
					RGB15(31,31, 0), //43 bright yellow

					RGB15( 0, 0,31), //44 bright blue
					RGB15(31, 0,31), //45 bright magenta
					RGB15( 0,31,31), //46 bright cyan
					RGB15(31,31,31)  //47 & 39 bright white
				};

gfxPrintConsole gfxdefaultConsole = 
{
	//Font:
	{
		(BMFont *) &gfx_default_font_mono	// Font information - Font 0
//		(BMFont *) &gfx_default_font_mcol	// Font information - Font 1
	},
	0,										// Current font
	2,										// Number of fonts
	(u16 *) 0,								// Background buffer
	RGB5( 31, 31, 31 ) /*| BIT(15)*/,			// Text color - warning here
	RGB5(  0,  0,  0 ) /*| BIT(15)*/,			// Background color
	-1,										// bg id
	false,									// Bacground mode
	1,										// Use line height for y offsets
	0,0,									// cursorX cursorY
	0,0,									// prevcursorX prevcursorY
	256,									// console width
	192,									// console height
	0,										// window x
	0,										// window y
	256,									// window width
	192,									// window height
	4,										// tab size
	0,										// print callback
	false,									// console initialized
	true,									// load graphics
};

gfxPrintConsole gfxcurrentCopy;

gfxPrintConsole* gfxcurrentConsole = &gfxcurrentCopy;

gfxPrintConsole* gfxConsoleGetDefault(void){return &gfxdefaultConsole;}

void gfxConsolePrintChar(char c);

//---------------------------------------------------------------------------------
static void gfxConsoleCls(char mode) {
	//---------------------------------------------------------------------------------
	u16 *buf, width, ypos;

	buf = gfxcurrentConsole->buffer;
	width = gfxcurrentConsole->windowWidth << 1;

	switch (mode)
	{
	case '0':
		{
			// Move the buffer to the line after the cursor, and then clear it
			buf += ( gfxcurrentConsole->windowY << 8 ) + gfxcurrentConsole->windowX;
			buf += ( gfxcurrentConsole->cursorY << 8 ) + gfxcurrentConsole->cursorX;
			buf += ( gfxcurrentConsole->font[ gfxcurrentConsole->curFont ]->Head->bLineHeight << 8 );
			
			for( ypos = 0; ypos <= ( ( gfxcurrentConsole->windowHeight - gfxcurrentConsole->cursorY ) + gfxcurrentConsole->font[ gfxcurrentConsole->curFont ]->Head->bLineHeight ); ypos ++ )
			{
				dmaFillHalfWords( gfxcurrentConsole->backcolor /*| BIT(15)*/, buf, width );
				buf += 256;
			}
			
			break;
		}	
	case '1':
		{
			// Move the buffer to the line after the cursor, and then clear it
			buf += ( gfxcurrentConsole->windowY << 8 ) + gfxcurrentConsole->windowX;
			
			for( ypos = 0; ypos <= gfxcurrentConsole->cursorY; ypos ++ )
			{
				dmaFillHalfWords( gfxcurrentConsole->backcolor /*| BIT(15)*/, buf, width );
				buf += 256;
			}
			
			break;
		}	
	case '2':
		{
			
			buf += ( gfxcurrentConsole->windowY << 8 ) + gfxcurrentConsole->windowX;
			
			for( ypos = 0; ypos <= gfxcurrentConsole->windowHeight; ypos ++ )
			{
				dmaFillHalfWords( gfxcurrentConsole->backcolor /*| BIT(15)*/, buf, width );
				buf += 256;
			}
			
			gfxcurrentConsole->cursorY  = 0;
			gfxcurrentConsole->cursorX  = 0;
			break;
		}	
	}
}
//---------------------------------------------------------------------------------
static void gfxConsoleClearLine(char mode) {
	//---------------------------------------------------------------------------------

	u16 *buf, width, i;

	buf = gfxcurrentConsole->buffer;
	width = gfxcurrentConsole->windowWidth << 1;
	buf += ( gfxcurrentConsole->windowY << 8 ) + gfxcurrentConsole->windowX;
	
	switch (mode)
	{
	case '0':
		{
			for( i = 0; i < gfxcurrentConsole->font[ gfxcurrentConsole->curFont ]->Head->bLineHeight; i ++ ) {
				dmaFillHalfWords( gfxcurrentConsole->backcolor /*| BIT(15)*/, buf, ( gfxcurrentConsole->cursorX ) << 1 );
			}
			break;
		}	
	case '1':
		{
			for( i = 0; i < gfxcurrentConsole->font[ gfxcurrentConsole->curFont ]->Head->bLineHeight; i ++ ) {
				dmaFillHalfWords( gfxcurrentConsole->backcolor, buf, ( gfxcurrentConsole->windowX - gfxcurrentConsole->cursorX ) << 1 );
			}

			break;
		}	
	case '2':
	default:
		{
			for( i = 0; i < gfxcurrentConsole->font[ gfxcurrentConsole->curFont ]->Head->bLineHeight; i ++ ) {
				dmaFillHalfWords( gfxcurrentConsole->backcolor, buf, width );
			}
			break;
		}
	}
}

//---------------------------------------------------------------------------------
ssize_t gfxnocash_write(struct _reent *r, int fd, const char *ptr, size_t len) {
	//---------------------------------------------------------------------------------
	int count = 0;

	if(!ptr || len <= 0) return -1;

	while(count < len)
	{
		count += (len - count) > 80 ? 80 : len - count;
	}

	return len;
}


//---------------------------------------------------------------------------------
ssize_t gfxcon_write(struct _reent *r,int fd,const char *ptr, size_t len) {
	//---------------------------------------------------------------------------------
	
	char chr;

	int i, count = 0;
	char *tmp = (char*)ptr;

	if(!tmp || len<=0) return -1;

	i = 0;

	while(*tmp!='\0' && i<len) {

		chr = *(tmp++);
		i++; count++;
		
		/////////////////////////////////////////
		// Font Set G0 (VT-100 escape sequence)
		/////////////////////////////////////////
		if ( chr == 0x1b && *tmp == '(' ) {
			gfxcurrentConsole->curFont = 0;
			chr = *(tmp++);
			i++; count++;
			continue;
		} else

		/////////////////////////////////////////
		// Font Set G1 (VT-100 escape sequence)
		/////////////////////////////////////////
		if ( chr == 0x1b && *tmp == ')' ) {
			gfxcurrentConsole->curFont = 1;
			chr = *(tmp++);
			i++; count++;
			continue;
		} else

		if ( chr == 0x1b && *tmp == '[' ) {
			bool escaping = true;
			char *escapeseq	= tmp;
			int escapelen = 0;

			do {
				chr = *(tmp++);
				i++; count++; escapelen++;
				int parameter, pos, color;

				switch (chr) {
					/////////////////////////////////////////
					// Cursor directional movement
					/////////////////////////////////////////
					case 'A':
						siscanf(escapeseq,"[%dA", &parameter);
						pos = gfxcurrentConsole->lineType ? ( parameter * gfxcurrentConsole->font[ gfxcurrentConsole->curFont ]->Head->bLineHeight ) : parameter;
						gfxcurrentConsole->cursorY  =  (gfxcurrentConsole->cursorY  - pos ) < 0 ? 0 : gfxcurrentConsole->cursorY - pos;
						escaping = false;
						break;
					case 'B':
						siscanf(escapeseq,"[%dB", &parameter);
						pos = gfxcurrentConsole->lineType ? ( parameter * gfxcurrentConsole->font[ gfxcurrentConsole->curFont ]->Head->bLineHeight ) : parameter;
						gfxcurrentConsole->cursorY  =  (gfxcurrentConsole->cursorY  + pos ) > gfxcurrentConsole->windowHeight - 1 ? ( gfxcurrentConsole->windowHeight - 1 ) / gfxcurrentConsole->font[ gfxcurrentConsole->curFont ]->Head->bLineHeight : gfxcurrentConsole->cursorY  + pos;
						escaping = false;
						break;
					case 'C':
						siscanf(escapeseq,"[%dC", &parameter);
						gfxcurrentConsole->cursorX  =  (gfxcurrentConsole->cursorX  + parameter) > gfxcurrentConsole->windowWidth - 1 ? gfxcurrentConsole->windowWidth - 1 : gfxcurrentConsole->cursorX  + parameter;
						escaping = false;
						break;
					case 'D':
						siscanf(escapeseq,"[%dD", &parameter);
						gfxcurrentConsole->cursorX  =  (gfxcurrentConsole->cursorX  - parameter) < 0 ? 0 : gfxcurrentConsole->cursorX  - parameter;
						escaping = false;
						break;
						/////////////////////////////////////////
						// Cursor position movement
						/////////////////////////////////////////
					case 'H':
					case 'f':
						siscanf(escapeseq,"[%d;%df", &gfxcurrentConsole->cursorY , &gfxcurrentConsole->cursorX );
						if( gfxcurrentConsole->lineType )
							gfxcurrentConsole->cursorY *= gfxcurrentConsole->font[ gfxcurrentConsole->curFont ]->Head->bLineHeight;
						escaping = false;
						break;
						
						// echo hack: absolute positioning
						
					case 'X':
						siscanf(escapeseq, "[%d;%dX", &gfxcurrentConsole->cursorY, &gfxcurrentConsole->cursorX);
						escaping = false;
						break;
						
						// echo hack: positioning only one coordinate
					
					case 'x':
						siscanf(escapeseq, "[%dx", &gfxcurrentConsole->cursorX);
						escaping = false;
						break;
					case 'y':
						siscanf(escapeseq, "[%dy", &gfxcurrentConsole->cursorY);
						escaping = false;
						break;
						
						
						/////////////////////////////////////////
						// Set Scale Unit "ESC[? I"
						/////////////////////////////////////////
					case 'I':
						if(escapeseq[escapelen-2] == ' ')
						{
							if(escapeseq[escapelen-3] == '0')
							{
								gfxcurrentConsole->lineType = 1;
							}
							else if(escapeseq[escapelen-3] == '7')
							{
								gfxcurrentConsole->lineType = 0;
							}
							else
							{
								gfxcurrentConsole->lineType = 1;
							}
						}
						escaping = false;
						break;
						/////////////////////////////////////////
						// Screen clear
						/////////////////////////////////////////
					case 'J':
						gfxConsoleCls(escapeseq[escapelen-2]);
						escaping = false;
						break;
						/////////////////////////////////////////
						// Line clear
						/////////////////////////////////////////
					case 'K':
						gfxConsoleClearLine(escapeseq[escapelen-2]);
						escaping = false;
						break;
						/////////////////////////////////////////
						// Save cursor position
						/////////////////////////////////////////
					case 's':
						gfxcurrentConsole->prevCursorX  = gfxcurrentConsole->cursorX ;
						gfxcurrentConsole->prevCursorY  = gfxcurrentConsole->cursorY ;
						escaping = false;
						break;
						/////////////////////////////////////////
						// Load cursor position
						/////////////////////////////////////////
					case 'u':
						gfxcurrentConsole->cursorX  = gfxcurrentConsole->prevCursorX ;
						gfxcurrentConsole->cursorY  = gfxcurrentConsole->prevCursorY ;
						escaping = false;
						break;
						/////////////////////////////////////////
						// Color scan codes
						/////////////////////////////////////////
					case 'm':
						pos = 0;
						parameter = 0;
						
						while( pos < escapelen ) {
							
							if( ( escapeseq[ pos ] >= '0' ) && ( escapeseq[ pos ] <= '9' ) ) {
								parameter *= 10;
								parameter += escapeseq[ pos ] - '0';
							}
							else if( ( escapeseq[ pos ] == ';' ) || ( escapeseq[ pos ] == 'm' ) ) {
								switch( parameter ) {
									/////////////////////////////////////////
									// Reset all values
									/////////////////////////////////////////
									case 0: // Reset all settings
										gfxcurrentConsole->textcolor = palette[ 15 ];
										gfxcurrentConsole->backcolor = palette[ 0 ];
										gfxcurrentConsole->backOpaque = false;
										break;
									case 1:
										gfxcurrentConsole->textcolor = bmfColorIntensityBold( gfxcurrentConsole->textcolor ) | BIT(15);
										break;
									case 2:
										gfxcurrentConsole->textcolor = bmfColorIntensityFaint( gfxcurrentConsole->textcolor ) | BIT(15);
										break;
									case 3: // Italic
										break;
									case 4: // Underline
										break;
									case 7:
									case 27:
										color = gfxcurrentConsole->textcolor;
										gfxcurrentConsole->textcolor = gfxcurrentConsole->backcolor;
										gfxcurrentConsole->backcolor = color;
										break;
									case 10: // Primary font
									case 11: // Font 1
									case 12: // Font 2
									case 13: // Font 3
									case 14: // Font 4
									case 15: // Font 5
									case 16: // Font 6
									case 17: // Font 7
									case 18: // Font 8
									case 19: // Font 9
										if( gfxcurrentConsole->font[ parameter - 10 ] )
											gfxcurrentConsole->curFont = parameter - 10;
										else
											gfxcurrentConsole->curFont = 0;
										break;
									case 21: // Double underline
										break;
									case 23: // Non Italic
										break;
									case 24: // Underline off
										break;
									case 30:
									case 31:
									case 32:
									case 33:
									case 34:
									case 35:
									case 36:
									case 37:
										gfxcurrentConsole->textcolor = palette[ parameter - 30 ] | BIT(15);
										break;
									case 39:
										gfxcurrentConsole->textcolor = palette[ 15 ] | BIT(15);
										break;
									case 40:
									case 41:
									case 42:
									case 43:
									case 44:
									case 45:
									case 46:
									case 47:
										gfxcurrentConsole->backcolor = palette[ parameter - 40 ] | BIT(15);
										gfxcurrentConsole->backOpaque = true;
										break;
									case 49:
										gfxcurrentConsole->backcolor = palette[ 0 ];
										gfxcurrentConsole->backOpaque = false;
										break;
								}
								parameter = 0;
							}
							
							pos ++;
						}

						escaping = false;
						break;
				}
			} while (escaping);
			continue;
		}
		
		gfxConsolePrintChar(chr);
	}

	return count;
}

static const devoptab_t dotab_gfxstdout = {
	"con",
	0,
	NULL,
	NULL,
	gfxcon_write,
	NULL,
	NULL,
	NULL
};


static const devoptab_t dotab_gfxstderr = {
	"nocash",
	0,
	NULL,
	NULL,
	gfxnocash_write,
	NULL,
	NULL,
	NULL
};

gfxPrintConsole* gfxConsoleInit(gfxPrintConsole* console, BgType type, BgSize size, bool mainDisplay, bool loadGraphics){

	static bool firstConsoleInit = true;

	if(firstConsoleInit)
	{
		LoadBMFont( &gfx_default_font_mono, _4x6_a_bmf );
//		LoadBMFont( &gfx_default_font_mcol, gc_def_c_bmf );
		
		devoptab_list[STD_OUT] = &dotab_gfxstdout;
	
		setvbuf(stdout, NULL , _IONBF, 0);
		
		firstConsoleInit = false;
	}
	
	if(console)
	{
		gfxcurrentConsole = console;
	}
	else
	{
		console = gfxcurrentConsole;	
	}

	*gfxcurrentConsole = gfxdefaultConsole;

	if(mainDisplay)
	{
		console->bgId = bgInit(3, type, size, 0, 0);
	}
	else
	{
		console->bgId = bgInitSub(3, type, size, 0, 0);
	}	
	
	console->buffer = (u16*)bgGetGfxPtr(console->bgId);

	console->consoleInitialised = 1;
	
	gfxConsoleCls('2');

	return gfxcurrentConsole;

}
//---------------------------------------------------------------------------------
void gfxConsoleSelect(gfxPrintConsole* console){
//---------------------------------------------------------------------------------

	gfxcurrentConsole = console;
}

//---------------------------------------------------------------------------------
void gfxConsoleSetFont(gfxPrintConsole* console, u8 fontNumber){
//---------------------------------------------------------------------------------
	console->curFont = fontNumber;

	gfxConsoleSelect(console);
}

//---------------------------------------------------------------------------------
int gfxConsoleAddFont(gfxPrintConsole* console, BMFont* font){
//---------------------------------------------------------------------------------
	u8 i, j = 11;
	
	for( i = 0; i < 10; i++ )
	{
		if( gfxcurrentConsole->font[ i ] == 0 )
		{
			if( j > i )
				j = i;
		}
	}
	
	if( j < 10 )
		gfxcurrentConsole->font[ j ] = font;
	else
		j = -1; 
	
	gfxConsoleSelect(console);
	
	return j;
}

//---------------------------------------------------------------------------------
void gfxConsoleDebugInit(DebugDevice device){
//---------------------------------------------------------------------------------

	if(device & DebugDevice_NOCASH)
	{
		devoptab_list[STD_ERR] = &dotab_gfxstderr;
		setvbuf(stderr, NULL , _IONBF, 0);
	}

	if(device & DebugDevice_CONSOLE)
	{
		devoptab_list[STD_ERR] = &dotab_gfxstdout;
		setvbuf(stderr, NULL , _IONBF, 0);
	}
}

////---------------------------------------------------------------------------------
//// Places the console in a default mode using bg0 of the sub display, and vram c for 
//// font and map..this is provided for rapid prototyping and nothing more
gfxPrintConsole* gfxConsoleDemoInit(void) {
//---------------------------------------------------------------------------------
	videoSetModeSub(MODE_5_2D);
	vramSetBankC(VRAM_C_SUB_BG); 

	return gfxConsoleInit(NULL, BgType_Bmp16, BgSize_B16_256x256, false, true);
}

//---------------------------------------------------------------------------------
static void gfxnewRow() {
//---------------------------------------------------------------------------------
	u16 line;
	u16 *buf, offset;
	
	gfxcurrentConsole->cursorY += gfxcurrentConsole->font[ gfxcurrentConsole->curFont ]->Head->bLineHeight;
	
	if(gfxcurrentConsole->cursorY  >= gfxcurrentConsole->windowHeight ) 
	{
		gfxcurrentConsole->cursorY -= gfxcurrentConsole->font[ gfxcurrentConsole->curFont ]->Head->bLineHeight;
		
		buf = gfxcurrentConsole->buffer;
		offset = gfxcurrentConsole->font[ gfxcurrentConsole->curFont ]->Head->bLineHeight << 8;
		
		buf += ( gfxcurrentConsole->windowY << 8 ) + gfxcurrentConsole->windowX;
		
		for( line = 0; line < gfxcurrentConsole->windowHeight - gfxcurrentConsole->font[ gfxcurrentConsole->curFont ]->Head->bLineHeight; line ++ )
		{
//			dmaFillHalfWords( 0x800F, buf, gfxcurrentConsole->windowWidth << 1 );
			dmaCopy( buf + offset, buf, gfxcurrentConsole->windowWidth << 1 );
			buf += 256;
		}
		
		for( line = 0; line < gfxcurrentConsole->font[ gfxcurrentConsole->curFont ]->Head->bLineHeight; line ++ )
		{
//			dmaFillHalfWords( 0x81E0, buf, gfxcurrentConsole->windowWidth << 1 );
			dmaFillHalfWords( gfxcurrentConsole->backcolor /*| BIT(15)*/, buf, gfxcurrentConsole->windowWidth << 1 );
			buf += 256;
		}

//		dmaCopy( gfxcurrentConsole->buffer + ( gfxcurrentConsole->font[ gfxcurrentConsole->curFont ]->Head->bLineHeight << 8 ), gfxcurrentConsole->buffer, ( 192 - gfxcurrentConsole->font[ gfxcurrentConsole->curFont ]->Head->bLineHeight ) << 9 );
		
//		dmaFillHalfWords( gfxcurrentConsole->backcolor, gfxcurrentConsole->buffer + ( gfxcurrentConsole->cursorY << 8 ), ( 192 - gfxcurrentConsole->cursorY ) << 9 );
	}
}


//---------------------------------------------------------------------------------
void gfxConsolePrintChar( char c ) {
	//---------------------------------------------------------------------------------

	if(gfxcurrentConsole->PrintChar) 
		if(gfxcurrentConsole->PrintChar(gfxcurrentConsole, c))
			return;

	if( ( gfxcurrentConsole->cursorX + gfxcurrentConsole->font[ gfxcurrentConsole->curFont ]->Ltrs[c]->bShift) > gfxcurrentConsole->windowWidth) {
		gfxcurrentConsole->cursorX  = 0;

		gfxnewRow();
	}

	switch(c) {
		/*
		The only special characters we will handle are tab (\t), carriage return (\r), line feed (\n)
		and backspace (\b).
		Carriage return & line feed will function the same: go to next line and put cursor at the beginning.
		For everything else, use VT sequences.

		Reason: VT sequences are more specific to the task of cursor placement.
		The special escape sequences \b \f & \v are archaic and non-portable.
		*/
		case 8:
			gfxcurrentConsole->cursorX--;
			
			if(gfxcurrentConsole->cursorX < 0)
			{			
				if(gfxcurrentConsole->cursorY > 0) 
				{	
					gfxcurrentConsole->cursorX = gfxcurrentConsole->windowX - 1;
					gfxcurrentConsole->cursorY -= gfxcurrentConsole->font[ gfxcurrentConsole->curFont ]->Head->bLineHeight;
				}
				else
				{
					gfxcurrentConsole->cursorX = 0;
				}
			}
			break;

		case 9:
			gfxcurrentConsole->cursorX  += gfxcurrentConsole->tabSize;
			break;
		case 10:
		case 13:
			gfxnewRow();
			gfxcurrentConsole->cursorX  = 0;
			break;
		default:
			if( gfxcurrentConsole->font[ gfxcurrentConsole->curFont ]->Ltrs[c] )
			{
				//if( gfxcurrentConsole->backOpaque )
					bmfPutCharOpaque( gfxcurrentConsole->font[ gfxcurrentConsole->curFont ], gfxcurrentConsole->cursorX + gfxcurrentConsole->windowX, gfxcurrentConsole->cursorY + gfxcurrentConsole->windowY, gfxcurrentConsole->textcolor, gfxcurrentConsole->backcolor, gfxcurrentConsole->buffer, c );
				//else
				//	bmfPutCharTransparent( gfxcurrentConsole->font[ gfxcurrentConsole->curFont ], gfxcurrentConsole->cursorX + gfxcurrentConsole->windowX, gfxcurrentConsole->cursorY + gfxcurrentConsole->windowY, gfxcurrentConsole->textcolor, gfxcurrentConsole->backcolor, gfxcurrentConsole->buffer, c );
				
				gfxcurrentConsole->cursorX += gfxcurrentConsole->font[ gfxcurrentConsole->curFont ]->Ltrs[c]->bShift + gfxcurrentConsole->font[ gfxcurrentConsole->curFont ]->Head->bAddSpace;
			}
			break;
	}
}

//---------------------------------------------------------------------------------
void gfxConsoleClear(void) {
//---------------------------------------------------------------------------------
	iprintf("\x1b[2J");
}

//---------------------------------------------------------------------------------
void gfxConsoleSetWindow(gfxPrintConsole* console, int x, int y, int width, int height) {
//---------------------------------------------------------------------------------
	
	if(!console) console = gfxcurrentConsole;

	console->windowWidth = width;
	console->windowHeight = height;
	console->windowX = x;
	console->windowY = y;
	
	console->cursorX = 0;
	console->cursorY = -gfxcurrentConsole->font[ gfxcurrentConsole->curFont ]->Head->bSizeOver;

}


