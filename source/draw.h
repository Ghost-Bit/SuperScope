// Copyright 2013 Normmatt
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include "common.h"

#define BYTES_PER_PIXEL 3
#define SCREEN_HEIGHT 240
#define SCREEN_WIDTH_TOP 400
#define SCREEN_WIDTH_BOT 320
#ifdef FONT_6X10 // special font width
#define FONT_WIDTH_EXT 6
#elif defined FONT_GB // special font width
#define FONT_WIDTH_EXT 7
#else
#define FONT_WIDTH_EXT 8
#endif

#define RGB(r,g,b) (b<<16|g<<8|r)

#define COLOR_BLACK         RGB(0x00, 0x00, 0x00)
#define COLOR_WHITE         RGB(0xFF, 0xFF, 0xFF)
#define COLOR_RED           RGB(0xFF, 0x00, 0x00)
#define COLOR_ORANGE        RGB(0xFF, 0xA5, 0x00)
#define COLOR_YELLOW        RGB(0xFF, 0xFF, 0x00)
#define COLOR_GREEN         RGB(0x00, 0xFF, 0x00)
#define COLOR_BLUE          RGB(0x00, 0x00, 0xFF)
#define COLOR_CYAN          RGB(0x00, 0xFF, 0xFF)
#define COLOR_MAGENTA       RGB(0xFF, 0x00, 0xFF)
#define COLOR_GREY          RGB(0x77, 0x77, 0x77)
#define COLOR_BROWN         RGB(0x96, 0x4B, 0x00) // shit color lmao!!!!!!
#define COLOR_BEIGE         RGB(0xE8, 0xBE, 0xAC)
#define COLOR_TRANSPARENT   RGB(0xFF, 0x00, 0xEF) // otherwise known as 'super fuchsia'

//asth colors
#define ASTH_DARKGREY       RGB(0x66, 0x54, 0x5e) // ADG
#define ASTH_LIGHTGREY      RGB(0xA3, 0x91, 0x93) // ALG
#define ASTH_RED      	    RGB(0xAA, 0x6F, 0x73) // ARD
#define ASTH_ORANGE         RGB(0xEE, 0xA9, 0x90) // AOG
#define ASTH_YELLOW         RGB(0xF6, 0xE0, 0xB5) // AYL

//goog colors
#define GOOG_GREEN         	RGB(0x00, 0x87, 44) // GOG
#define GOOG_BLUE          	RGB(0x00, 0x57, 0xE7) // GOB
#define GOOG_RED           	RGB(0xD6, 0x2D, 0x20) // GOR
#define GOOG_YELLOW        	RGB(0xFF, 0xA7, 0x00) // GOY

// misc colors
#define MISC_MINT           RGB(0x1D, 0xB9, 0x54) //Mnt
#define MISC_PINK           RGB(0xE1, 0x28, 0x85) //Pnk
#define MISC_PEACH          RGB(0xFF, 0x66, 0x66) //Pch
#define MISC_MIDNIGHT       RGB(0x52, 0x52, 0x66) //Mdn
#define MISC_PEA 	        RGB(0x7D, 0xDC, 0x1F) //Pea
#define MISC_GRASS 	        RGB(0x0B, 0x42, 0x1A) //Grs

//EXTRA1
#define EXTRA1_COAL			RGB(0x40, 0x45, 0x52)
#define EXTRA1_CLOUD		RGB(0xD6, 0xD7, 0xD7)
#define EXTRA1_MAROON		RGB(0x8F, 0x10, 0x10)
#define EXTRA1_MANGO		RGB(0xE8, 0x70, 0x2A)
#define EXTRA1_BANANA		RGB(0xFF, 0xBE, 0x4F)
#define EXTRA1_LIME			RGB(0x52, 0xD0, 0x53)
#define EXTRA1_VIOLET		RGB(0x76, 0x58, 0x98)
#define EXTRA1_SKY			RGB(0xA1, 0xCF, 0xD7)
#define EXTRA1_NEONPINK		RGB(0x76, 0x58, 0x98)
#define EXTRA1_SAKUYA		RGB(0x83, 0x89, 0x96)
#define EXTRA1_MOCHA		RGB(0x63, 0x39, 0x0F)
#define EXTRA1_SAND			RGB(0xEC, 0xCC, 0xA2)

//DMG01
#define DMG01_4		RGB(0x84, 0xD0, 0x7D)
#define DMG01_3		RGB(0x5E, 0x78, 0x5D)
#define DMG01_2		RGB(0x3E, 0x49, 0x43)
#define DMG01_1		RGB(0x25, 0x2B, 0x25)

//TEAM

#define TEAM_BLU	RGB(0x49, 0x47, 0x86)
#define TEAM_RED	RGB(0x9A, 0x22, 0x57)



#define COLOR_GREYBLUE      RGB(0xA0, 0xA0, 0xFF)
#define COLOR_GREYGREEN     RGB(0xA0, 0xFF, 0xA0)
#define COLOR_GREYRED       RGB(0xFF, 0xA0, 0xA0)
#define COLOR_GREYCYAN      RGB(0xA0, 0xFF, 0xFF)
#define COLOR_TINTEDRED     RGB(0xFF, 0x60, 0x60)
#define COLOR_LIGHTGREY     RGB(0xA0, 0xA0, 0xA0)

#define COLOR_ASK           COLOR_GREYGREEN
#define COLOR_SELECT        COLOR_LIGHTGREY
#define COLOR_ACCENT        COLOR_GREEN

#ifndef USE_THEME
#define STD_COLOR_BG   COLOR_BLACK
#define STD_COLOR_FONT COLOR_WHITE

#define DBG_COLOR_BG   COLOR_BLACK
#define DBG_COLOR_FONT COLOR_WHITE

#define DBG_START_Y 10
#define DBG_END_Y   (SCREEN_HEIGHT - 10)
#define DBG_START_X 10
#define DBG_END_X   (SCREEN_WIDTH_TOP - 10)
#define DBG_STEP_Y  10
#endif

#define DBG_N_CHARS_Y ((DBG_END_Y - DBG_START_Y) / DBG_STEP_Y)
#define DBG_N_CHARS_X (((DBG_END_X - DBG_START_X) / FONT_WIDTH) + 1)

#define TOP_SCREEN top_screen
#define BOT_SCREEN bottom_screen

#define ScreenWidth(x)       (((x) == (TOP_SCREEN) ? 400 : 320))
#define IsCharPartOfWord(x)  (((x) >= 'a' && (x) <= 'z') || ((x) >= '0' && (x) <= '9') || ((x) >= 'A' && (x) <= 'Z'))

extern u8 *top_screen, *bottom_screen;

void ClearScreen(unsigned char *screen, int width, int color);
void ClearScreenR(unsigned char *screen,int start, int width, int color);
void ClearScreenFull(bool clear_top, bool clear_bottom);

void DrawCharacter(unsigned char *screen, int character, int x, int y, int color, int bgcolor);
void  DrawPixel(unsigned char *screen, int x, int y, int color);
int  DrawString(unsigned char *screen, const char *str, int x, int y, int color, int bgcolor);
void DrawStringF(int x, int y, bool use_top, const char *format, ...);
void DrawStringFC(int x, int y, bool use_top, u32 color, const char *format, ...);

void Screenshot(const char* path);
void DebugClear();
void DebugSet(const char **strs);
void DebugColor(u32 color, const char *format, ...);
void Debug(const char *format, ...);

void ShowProgress(u64 current, u64 total);
