/*
 *   This file is part of SuperScope OS
 *	I fucking love OSDev!!! -Alice
 *	I dont like legal shit, so use this however the hell you want lol ^_^
 */

#include <common.h>

#include <i2c.h>


void MCU_powerLedSet(u8 status) 		{i2cWriteRegister(3, 0x29, status);	}
void MCU_setBacklight(int display, bool state)	{
	if(display==0) {
		if(state) {
			i2cWriteRegister(3, 0x22, 0 << 3);	
			i2cWriteRegister(3, 0x22, 1 << 2);	
		}
		else {
			i2cWriteRegister(3, 0x22, 1 << 3);	
			i2cWriteRegister(3, 0x22, 0 << 2);	
		}
	}
	else if(display==1) {
		if(state) {
			i2cWriteRegister(3, 0x22, 0 << 4);	
			i2cWriteRegister(3, 0x22, 1 << 5);	
		}
		else {
			i2cWriteRegister(3, 0x22, 1 << 4);	
			i2cWriteRegister(3, 0x22, 0 << 5);	
		}
	}
	else if(display==2) {
		if(state) {
			i2cWriteRegister(3, 0x22, 0 << 4);	
			i2cWriteRegister(3, 0x22, 1 << 5);	
			i2cWriteRegister(3, 0x22, 1 << 3);	
			i2cWriteRegister(3, 0x22, 0 << 2);		
		}
		else {
			i2cWriteRegister(3, 0x22, 1<< 4);	
			i2cWriteRegister(3, 0x22, 0 << 5);	
			i2cWriteRegister(3, 0x22, 0 << 3);	
			i2cWriteRegister(3, 0x22, 1 << 2);	
		}
	}
}

u8 MCU_getVol()											{return i2cReadRegister(3, 0x27);		}
u8 MCU_get3d()												{return i2cReadRegister(3, 0x08);		}
u8 MCU_getBatteryPerc()							{return i2cReadRegister(3, 0x0B);		}
u8 MCU_getBatteryTemp()						{return i2cReadRegister(3, 0x0A);		}

bool MCU_getShellClose()							{return (i2cReadRegister(3, 0x0F) >> 1 & 1) == 0;}
bool MCU_getShellOpen()							{return (i2cReadRegister(3, 0x0F) >> 1 & 1) == 1;}

bool MCU_getPowerButton()						{return i2cReadRegister(3, 0x10)==0x01;}
bool MCU_getHomeButton()						{return i2cReadRegister(3, 0x10)==0x04;}


char* MCU_getBatteryText() { 	
	char* battstr = "";
	sprintf(battstr, "%d%% @ %d Degrees Celcius.",  MCU_getBatteryPerc(), MCU_getBatteryTemp());
	return battstr;
}

char* MCU_getSlidersText() { 	
	char* battstr = "";
	sprintf(battstr, "Left Slider: %d, Right slider: %d",  MCU_getVol(),MCU_get3d());
	return battstr;
}


