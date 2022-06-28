/*
 *   This file is part of SuperScope OS
 *	I fucking love OSDev!!! -Alice
 *	I dont like legal shit, so use this however the hell you want lol ^_^
 */


#pragma once

#include <common.h>
#include <misc/mcu.c>


#define POWERLED_BLUE 1
#define POWERLED_RED 4
#define POWERLED_DANGER 6
#define POWERLED_OFF 3

void MCU_powerLedSet(u8 status);
void MCU_setBacklight(int display, bool state);
u8 MCU_getVol();
u8 MCU_get3d();
u8 MCU_getBatteryPerc();
u8 MCU_getBatteryTemp();
bool MCU_getShellClose();
bool MCU_getShellOpen();

bool MCU_getPowerButton();
bool MCU_getHomeButton();

char* MCU_getBatteryText();