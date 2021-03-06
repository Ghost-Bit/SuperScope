// Copyright 2013 Normmatt
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "font.h"
#include "draw.h"
#include "fs.h"
#ifdef USE_THEME
#include "theme.h"
#endif

static char debugstr[DBG_N_CHARS_X * DBG_N_CHARS_Y] = { 0 };
static u32 debugcol[DBG_N_CHARS_Y] = { DBG_COLOR_FONT };

void ClearScreen(u8* screen, int width, int color)
{
    if (color == COLOR_TRANSPARENT) color = COLOR_BLACK;
    for (int i = 0; i < (width * SCREEN_HEIGHT); i++) {
        *(screen++) = color >> 16;  // B
        *(screen++) = color >> 8;   // G
        *(screen++) = color & 0xFF; // R
    }
}

void ClearScreenR(u8* screen, int start,int width, int color)
{
    if (color == COLOR_TRANSPARENT) color = COLOR_BLACK;
    for (int i = start; i < (width * SCREEN_HEIGHT); i++) {
        *(screen++) = color >> 16;  // B
        *(screen++) = color >> 8;   // G
        *(screen++) = color & 0xFF; // R
    }
}

void ClearScreenFull(bool clear_top, bool clear_bottom)
{
    if (clear_top)
        ClearScreen(TOP_SCREEN, SCREEN_WIDTH_TOP, STD_COLOR_BG);
    if (clear_bottom)
        ClearScreen(BOT_SCREEN, SCREEN_WIDTH_BOT, STD_COLOR_BG);
}

void DrawCharacter(u8* screen, int character, int x, int y, int color, int bgcolor)
{
    for (int yy = 0; yy < FONT_HEIGHT; yy++) {
        int xDisplacement = (x * BYTES_PER_PIXEL * SCREEN_HEIGHT);
        int yDisplacement = ((SCREEN_HEIGHT - (y + yy) - 1) * BYTES_PER_PIXEL);
        u8* screenPos = screen + xDisplacement + yDisplacement;

        u8 charPos = font[character * FONT_HEIGHT + yy];
        for (int xx = 7; xx >= (8 - FONT_WIDTH); xx--) {
            if ((charPos >> xx) & 1) {
                *(screenPos + 0) = color >> 16;  // B
                *(screenPos + 1) = color >> 8;   // G
                *(screenPos + 2) = color & 0xFF; // R
            } else if (bgcolor != COLOR_TRANSPARENT) {
                *(screenPos + 0) = bgcolor >> 16;  // B
                *(screenPos + 1) = bgcolor >> 8;   // G
                *(screenPos + 2) = bgcolor & 0xFF; // R
            }
            screenPos += BYTES_PER_PIXEL * SCREEN_HEIGHT;
        }
    }
}



void DrawPixel(u8* screen, int x, int y, int color)
{
        int xDisplacement = (x * BYTES_PER_PIXEL * SCREEN_HEIGHT);
        int yDisplacement = ((SCREEN_HEIGHT - (y + 0) - 1) * BYTES_PER_PIXEL);
        u8* screenPos = screen + xDisplacement + yDisplacement;
                *(screenPos + 0) = color >> 16;  // B
                *(screenPos + 1) = color >> 8;   // G
                *(screenPos + 2) = color & 0xFF; // R
}

int DrawString(u8* screen, const char *str, int x, int y, int color, int bgcolor)
{
    int _x = x, _y = y, width = ScreenWidth(screen);
    if (x < 0 || x >= width || y < 0 || y >= 240) return y;

    int i = 0, len;
    while(str[i])
    {
        len = 0;
        while(IsCharPartOfWord(str[i+len])) len++;
        // Get length of word

        if ((len*FONT_WIDTH) > width)
        {
            len = (width/FONT_WIDTH)-1;
        }
        else if ((_x + (len*FONT_WIDTH)) > width)
        {
            _x = x;
            _y+= FONT_HEIGHT;
        }

        if (_y >= 240)
            break;

        for (int j = 0; j < len; j++)
        {
            DrawCharacter(screen, str[i+j], _x, _y, color, bgcolor);
            _x+= FONT_WIDTH;
        }

        if (str[i+len] == '\n')
        {
            _x = x;
            _y+= FONT_HEIGHT;
        }
        else if (str[i+len] == '\r')
        {
            _x = x;
        }
        else
        {
            DrawCharacter(screen, str[i+len], _x, _y, color, bgcolor);
            _x+= FONT_WIDTH;
        }

        i+= len+1;
    }

    return _y;
}

void DrawStringF(int x, int y, bool use_top, const char *format, ...)
{
    char str[512] = { 0 }; // 512 should be more than enough
    va_list va;

    va_start(va, format);
    vsnprintf(str, 512, format, va);
    va_end(va);

    DrawString((use_top) ? TOP_SCREEN : BOT_SCREEN, str, x, y, STD_COLOR_FONT, STD_COLOR_BG);
}

void DrawStringFC(int x, int y, bool use_top, u32 color, const char *format, ...)
{
    char str[512] = { 0 }; // 512 should be more than enough
    va_list va;

    va_start(va, format);
    vsnprintf(str, 512, format, va);
    va_end(va);

    DrawString((use_top) ? TOP_SCREEN : BOT_SCREEN, str, x, y, color, STD_COLOR_BG);
}

void Screenshot(const char* path)
{
    u8* buffer = (u8*) 0x21000000; // careful, this area is used by other functions in Decrypt9
    u8* buffer_t = buffer + (400 * 240 * 3);
    u8 bmp_header[54] = {
        0x42, 0x4D, 0x36, 0xCA, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
        0x00, 0x00, 0x90, 0x01, 0x00, 0x00, 0xE0, 0x01, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xCA, 0x08, 0x00, 0x12, 0x0B, 0x00, 0x00, 0x12, 0x0B, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    
    if (path == NULL) {
        static u32 n = 0;
        for (; n < 1000; n++) {
            char filename[16];
            snprintf(filename, 16, "Draw_%03i.bmp", (int) n);
            if (!FileOpen(filename)) {
                FileCreate(filename, true);
                break;
            }
            FileClose();
        }
        if (n >= 1000)
            return;
    } else {
        FileCreate(path, true);
    }
    
    memset(buffer, 0x1F, 400 * 240 * 3 * 2);
    for (u32 x = 0; x < 400; x++)
        for (u32 y = 0; y < 240; y++)
            memcpy(buffer_t + (y*400 + x) * 3, TOP_SCREEN + (x*240 + y) * 3, 3);
    for (u32 x = 0; x < 320; x++)
        for (u32 y = 0; y < 240; y++)
            memcpy(buffer + (y*400 + x + 40) * 3, BOT_SCREEN + (x*240 + y) * 3, 3);
    FileWrite(bmp_header, 54, 0);
    FileWrite(buffer, 400 * 240 * 3 * 2, 54);
    FileClose();
}

void DebugClear()
{
    memset(debugstr, 0x00, DBG_N_CHARS_X * DBG_N_CHARS_Y);
    for (u32 y = 0; y < DBG_N_CHARS_Y; y++)
        debugcol[y] = DBG_COLOR_FONT;
    ClearScreen(TOP_SCREEN, SCREEN_WIDTH_TOP, DBG_COLOR_BG);
    #if defined USE_THEME && defined GFX_DEBUG_BG
    LoadThemeGfx(GFX_DEBUG_BG, true);
    #endif
    LogWrite("");
    LogWrite(NULL);
}

void DebugSet(const char **strs)
{
    if (strs != NULL) for (int y = 0; y < DBG_N_CHARS_Y; y++) {
        int pos_dbgstr = DBG_N_CHARS_X * (DBG_N_CHARS_Y - 1 - y);
        snprintf(debugstr + pos_dbgstr, DBG_N_CHARS_X, "%-*.*s", DBG_N_CHARS_X - 1, DBG_N_CHARS_X - 1, strs[y]);
        debugcol[y] = DBG_COLOR_FONT;
    }
    
    int pos_y = DBG_START_Y;
    u32* col = debugcol + (DBG_N_CHARS_Y - 1);

    for (char* str = debugstr + (DBG_N_CHARS_X * (DBG_N_CHARS_Y - 1)); str >= debugstr; str -= DBG_N_CHARS_X, col--) {
        if (*str != '\0') {
            pos_y =  DrawString(TOP_SCREEN, str, DBG_START_X, pos_y, *col, DBG_COLOR_BG);
            pos_y += DBG_STEP_Y;
        }
    }
}

void DebugColor(u32 color, const char *format, ...)
{
    static bool adv_output = true;
    char tempstr[128] = { 0 }; // 128 instead of DBG_N_CHARS_X for log file 
    va_list va;
    
    va_start(va, format);
    vsnprintf(tempstr, 128, format, va);
    va_end(va);
    
    if (adv_output) {
        memmove(debugstr + DBG_N_CHARS_X, debugstr, DBG_N_CHARS_X * (DBG_N_CHARS_Y - 1));
        memmove(debugcol + 1, debugcol, (DBG_N_CHARS_Y - 1) * sizeof(u32));
    } else {
        adv_output = true;
    }
    
    *debugcol = color;
    if (*tempstr != '\r') { // not a good way of doing this - improve this later
        snprintf(debugstr, DBG_N_CHARS_X, "%-*.*s", DBG_N_CHARS_X - 1, DBG_N_CHARS_X - 1, tempstr);
        LogWrite(tempstr);
    } else {
        snprintf(debugstr, DBG_N_CHARS_X, "%-*.*s", DBG_N_CHARS_X - 1, DBG_N_CHARS_X - 1, tempstr + 1);
        adv_output = false;
    }
    
    DebugSet(NULL);
}

void Debug(const char *format, ...)
{
    char tempstr[128] = { 0 }; // 128 instead of DBG_N_CHARS_X for log file 
    va_list va;
    
    va_start(va, format);
    vsnprintf(tempstr, 128, format, va);
    DebugColor(DBG_COLOR_FONT, tempstr);
    va_end(va);
}

#if !defined(USE_THEME) || !defined(ALT_PROGRESS)
void ShowProgress(u64 current, u64 total)
{
    const u32 progX = SCREEN_WIDTH_TOP - 40;
    const u32 progY = SCREEN_HEIGHT - 20;
    
    if (total > 0) {
        char progStr[8];
        snprintf(progStr, 8, "%3llu%%", (current * 100) / total);
        DrawString(TOP_SCREEN, progStr, progX, progY, DBG_COLOR_FONT, DBG_COLOR_BG);
    } else {
        DrawString(TOP_SCREEN, "    ", progX, progY, DBG_COLOR_FONT, DBG_COLOR_BG);
    }
}
#endif
