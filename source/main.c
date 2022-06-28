#include "common.h"
#include "draw.h"
#include "fs.h"
#include "hid.h"
#include "platform.h"
#include "i2c.h"

//------------------------
#include "codec.h"
#include "spi.h"
//^ SPECIAL THANKS TO THE GodMode9 DISCORD FOR HELPING ME WITH THIS SHIT
//I COULD NOT HAVE DONE IT WITHOUT THEM!!!!    :D

#include "decryptor/keys.h"
#include "decryptor/game.h"
#include "decryptor/ak2i.h"
#include "decryptor/nand.h"
#include "decryptor/nandfat.h"
#include "decryptor/titlekey.h"
#include "decryptor/selftest.h"
#include "decryptor/xorpad.h"
#include "decryptor/transfer.h"

#include "csnd.h"


//my own code and stuff!!! :cirnosugoiwow:
#include "misc/mcu.h"
#include "misc/icons.h"



#define RAW_TX(t)   ((s32)(((t) / (1 << 16)) & 0xFFFF))
#define RAW_TY(t)   ((s32)((t) & 0xFFF))

#define VBLANK_INTERRUPT	(0x2A)

int touchX = 0;
int touchY = 0;
bool touchD = false;
bool rdr=false;
bool rdrupd = false;

//buncha status text variables
char* storagetext ="                                                                                 ";
char* proctext ="                                                                                 ";

void Reboot()
{
    i2cWriteRegister(I2C_DEV_MCU, 0x20, 1 << 2);
    while(true);
}

void Shutdown()
{
    i2cWriteRegister(I2C_DEV_MCU, 0x20, 1 << 0);
    while (true);
}

void screendelay()
{
	
}

//dont let sono see this^

























//amogus

struct colorbutton_t
{
	u32 offsetX;
	u32 offsetY;
	const char* label;
	u32 color;
	const char* dispname;	
};


struct colorbutton_t buttons[] =
{						
	{0,9,"Red", COLOR_RED		,"BASIC - Red"		},
	{1,9,"Org", COLOR_ORANGE	,"BASIC - Orange"	},
	{2,9,"Ylw", COLOR_YELLOW	,"BASIC - Yellow" 	},
	{3,9,"Gre", COLOR_GREEN		,"BASIC - Green"	},
	{4,9,"Blu", COLOR_BLUE		,"BASIC - Blue"		},
	{5,9,"Cyn", COLOR_CYAN		,"BASIC - Cyan"		},
	{6,9,"Wht", COLOR_WHITE		,"BASIC - White"	},
	{7,9,"Blk", COLOR_BLACK		,"BASIC - Black"	},
	{8,9,"Brn", COLOR_BROWN		,"BASIC - Brown"	},
	{9,9,"Baj", COLOR_BEIGE		,"BASIC - Beige"	},
	
	{0,18,"ADG", ASTH_DARKGREY	,"ASTHE - Dark Grey"},
	{1,18,"ALG", ASTH_LIGHTGREY	,"ASTHE - LightGrey"},
	{2,18,"ARe", ASTH_RED		,"ASTHE - Red"		},			
	{3,18,"AOr", ASTH_ORANGE	,"ASTHE - Orange"	},			
	{4,18,"AYe", ASTH_YELLOW	,"ASTHE - Yellow"	},			

	{5,18,"GoG", GOOG_GREEN		,"GOOGL - Green"	},
	{6,18,"GoB", GOOG_BLUE		,"GOOGL - Blue"		},			
	{7,18,"GoR", GOOG_RED		,"GOOGL - Red"		},			
	{8,18,"GoY", GOOG_YELLOW	,"GOOGL - Yellow"	},		

	{9,18,"Mnt", MISC_MINT		,"MISCC - Mint"		},		
	{10,18,"Pnk", MISC_PINK		,"MISCC - Pink"		},						
	{11,18,"Pch", MISC_PEACH 	,"MISCC - Peach"	},					
	{12,18,"Mdn", MISC_MIDNIGHT	,"MISCC - Midnight"	},						
	{13,18,"Pea", MISC_PEA     	,"MISCC - Pea"		},	
	{14,18,"Grs", MISC_GRASS    ,"MISCC - Grass"	}, //Touch grass, motherfuckers B)
	{0,0,0,0,0}
};

u8 *top_screen, *bottom_screen;

//
int scaledwny = 16.2;
int scaledwnx = 16.2;
int xofs = -16;
int yofs = -8;
int xresize = 90000; // aaaaugh.wav
//all these vars are for fixing the touchscreen coordinates
	
bool Button(int x,int y,int w,int h,char* text,bool drawgrph)
{
	if(drawgrph)
	{

		for (int bx = x ; bx < x+w ; bx++)	{
			for (int by = y ; by < y+h ; by++)	{
				if (bx == x || bx == x+w-1 || by == y || by == y+h-1 )
				{					
					DrawPixel(bottom_screen,bx,by,COLOR_BLACK);
				}
				else
				{
					DrawPixel(bottom_screen,bx,by,COLOR_WHITE);
				}
			}
		}
		DrawString(bottom_screen,text,x+2,y+2,COLOR_BLACK,COLOR_WHITE);
	}

	if (touchX < x+w && touchX > x &&  touchY < y+h && touchY > y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool AppButton(int y,int x, bool* icon,bool drawgrph)
{
	if(drawgrph)
	{
		int a = 0;
		for (int by = x ; by < x+32 ; by++)	{
			for (int bx = y ; bx < y+32 ; bx++)	{
				if (icon[a] == true)
					DrawPixel(bottom_screen,bx,by,EXTRA1_COAL);
				else
					DrawPixel(bottom_screen,bx,by,EXTRA1_CLOUD);

				a++;
			}
		}
	}
	if (touchX < y+32 && touchX >y &&  touchY < x+32 && touchY > x)
	{
		return true;
	}
	else
	{
		return false;
	}
}



bool ColorButton(int x,int y,int w,int h, int color,bool drawgrph)
{
	if(drawgrph)
	{

		for (int bx = x ; bx < x+w ; bx++)	{
			for (int by = y ; by < y+h ; by++)	{
				if (bx == x || bx == x+w-1 || by == y || by == y+h-1 )
				{					
					DrawPixel(bottom_screen,bx,by,COLOR_BLACK);
				}
				else
				{
					DrawPixel(bottom_screen,bx,by,color);
				}
			}
		}
	}

	if (touchX < x+w && touchX > x &&  touchY < y+h && touchY > y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void DisplayClr(unsigned char* a, int b, int c) //yeah i got lazy here .-.
{
	ClearScreen(a,b,c); // :v
	rdr=true;
}


void TouchProc()
{
		CODEC_Input t ;
		CODEC_Get(&t);
		touchX = (RAW_TX(t.ts_x*xresize)/scaledwnx)+xofs;
		touchY = (RAW_TY(t.ts_y)/scaledwny)+yofs;
		touchD = !(t.ts_y == 0xFFFF);
}
		
		
int main(int argc, char** argv)
{		
	




	
	u8 **fb = (u8 **)(void *)argv[1];
	top_screen = fb[0];
	bottom_screen = fb[2];
	ClearScreen(bottom_screen, 320, COLOR_WHITE);


	char* proc = "main";
	int 	fuct = 8; //(frame update clock lmao)
	DrawString(bottom_screen,"SuperScope OS! V0.0", 0,0, EXTRA1_COAL, COLOR_WHITE);
	SPI_Init();
	CODEC_Init();
	InitFS();
	MCU_powerLedSet(POWERLED_BLUE);
	bool screensoff=false;
	
	rdr = true;
	int pen = COLOR_BLACK;
	int pensize = 1;
	while(true)
	{
		TouchProc();
		
		if (touchD)
		{
		//Run code here when the screen is touched
		}


		


		//status display, top screen

		
		if (!screensoff && proc == "main")
		{
			if(AppButton(16,16,POWERICON,rdr))
			{
				proc = "powmen";
				ClearScreen(bottom_screen, 320, COLOR_WHITE);
				while(touchD){TouchProc();}
				touchD=true;
				rdr=false;
			}
			if(AppButton(64,16,DRAWICON,rdr))
			{
				proc = "draw";
				ClearScreen(bottom_screen, 320, COLOR_WHITE);
				while(touchD){TouchProc();}
				touchD=true;
				rdr=false;
			}
			
			if(AppButton(112,16,HELPICON,rdr))
			{
				proc = "4dummies";
				ClearScreen(bottom_screen, 320, COLOR_WHITE);
				while(touchD){TouchProc();}
				touchD=true;
				rdr=false;
			}
			
			rdrupd = true;
			DrawString(bottom_screen,"Pressing the SELECT button or closing the shell will lock the screen.", 0,240-12, COLOR_WHITE, EXTRA1_COAL);
			DrawString(bottom_screen,"Once this happens, Tap the screen, or press SELECT to unlock it.", 0,240-6, COLOR_WHITE, EXTRA1_COAL);
		}		
		
		
		
		else if (!screensoff && proc == "draw")
		{
			if(!touchD)
			{
			}
			if (touchD)
			{
				if(pensize == 0) {
				DrawPixel(bottom_screen,touchX,touchY,pen);
				}
				
				if(pensize == 1) {
				DrawPixel(bottom_screen,touchX,touchY,pen);
				DrawPixel(bottom_screen,touchX+1,touchY,pen);
				DrawPixel(bottom_screen,touchX-1,touchY,pen);
				DrawPixel(bottom_screen,touchX,touchY+1,pen);
				DrawPixel(bottom_screen,touchX,touchY-1,pen);
				}
				
				if(pensize == 2) {
				DrawPixel(bottom_screen,touchX,touchY,pen);
				
				DrawPixel(bottom_screen,touchX+1,touchY,pen);
				DrawPixel(bottom_screen,touchX+1,touchY+1,pen);
				DrawPixel(bottom_screen,touchX,touchY+1,pen);
				DrawPixel(bottom_screen,touchX-1,touchY+1,pen);
				DrawPixel(bottom_screen,touchX-1,touchY,pen);
				DrawPixel(bottom_screen,touchX-1,touchY-1,pen);
				DrawPixel(bottom_screen,touchX,touchY-1,pen);
				DrawPixel(bottom_screen,touchX+1,touchY-1,pen);
				
				DrawPixel(bottom_screen,touchX+2,touchY,pen);
				DrawPixel(bottom_screen,touchX,touchY+2,pen);
				DrawPixel(bottom_screen,touchX-2,touchY,pen);
				DrawPixel(bottom_screen,touchX,touchY-2,pen);

				}
			}	
			

			int div = (320/2)/16;
			int divs = (320/2)/3;

			// u32 count = sizeof(buttons) / sizeof(buttons[0]);
			// const struct colorbutton_t* button = buttons;
			// for(u32 i = 0; i < count; ++i, ++button)
			// {
				// if(ButtonColor(div*0,240-9,div,9,"Red",true)) {pen = COLOR_RED}
			// }
			
			
			
			if(ColorButton(div*0,240-9,div,9,COLOR_RED,true)) {pen = COLOR_RED;}
			if(ColorButton(div*1,240-9,div,9,COLOR_ORANGE,true)) {pen = COLOR_ORANGE;}
			if(ColorButton(div*2,240-9,div,9,COLOR_YELLOW,true)) {pen = COLOR_YELLOW;}
			if(ColorButton(div*3,240-9,div,9,COLOR_GREEN,true)) {pen = COLOR_GREEN;}
			if(ColorButton(div*4,240-9,div,9,COLOR_BLUE,true)) {pen = COLOR_BLUE;}
			if(ColorButton(div*5,240-9,div,9,COLOR_CYAN,true)) {pen = COLOR_CYAN;}
			if(ColorButton(div*6,240-9,div,9,COLOR_WHITE,true)) {pen = COLOR_WHITE;}
			if(ColorButton(div*7,240-9,div,9,COLOR_BLACK,true)) {pen = COLOR_BLACK;}
			if(ColorButton(div*8,240-9,div,9,COLOR_BROWN,true)) {pen = COLOR_BROWN;}
			if(ColorButton(div*9,240-9,div,9,COLOR_BEIGE,true)) {pen = COLOR_BEIGE;}

			if(ColorButton(div*14,240-9,div,9,MISC_MINT,true)) {pen = MISC_MINT;}
			if(ColorButton(div*15,240-9,div,9,MISC_PINK,true)) {pen = MISC_PINK;}

			if(ColorButton(div*10,240-9,div,9,ASTH_DARKGREY,true)) {pen = ASTH_DARKGREY;}
			if(ColorButton(div*11,240-9,div,9,ASTH_LIGHTGREY,true)) {pen = ASTH_LIGHTGREY;}
			if(ColorButton(div*12,240-9,div,9,ASTH_ORANGE,true)) {pen = ASTH_ORANGE;}
			if(ColorButton(div*13,240-9,div,9,ASTH_RED,true)) {pen = ASTH_RED;}
			
			
			
			
			if(ColorButton(div*0,240-18,div,9,EXTRA1_MAROON,true)) {pen = EXTRA1_MAROON;}
			if(ColorButton(div*1,240-18,div,9,EXTRA1_MANGO,true)) {pen = EXTRA1_MANGO;}
			if(ColorButton(div*2,240-18,div,9,EXTRA1_BANANA,true)) {pen = EXTRA1_BANANA;}
			if(ColorButton(div*3,240-18,div,9,EXTRA1_LIME,true)) {pen = EXTRA1_LIME;}
			if(ColorButton(div*4,240-18,div,9,EXTRA1_VIOLET,true)) {pen = EXTRA1_VIOLET;}
			if(ColorButton(div*5,240-18,div,9,EXTRA1_SKY,true)) {pen = EXTRA1_SKY;}
			if(ColorButton(div*6,240-18,div,9,EXTRA1_CLOUD,true)) {pen = EXTRA1_CLOUD;}
			if(ColorButton(div*7,240-18,div,9,EXTRA1_COAL,true)) {pen = EXTRA1_COAL;}
			if(ColorButton(div*8,240-18,div,9,EXTRA1_MOCHA,true)) {pen = EXTRA1_MOCHA;}
			if(ColorButton(div*9,240-18,div,9,EXTRA1_SAND,true)) {pen = EXTRA1_SAND;}

			if(ColorButton(div*14,240-18,div,9,TEAM_BLU,true)) {pen = TEAM_BLU;}
			if(ColorButton(div*15,240-18,div,9,TEAM_RED,true)) {pen = TEAM_RED;}

			if(ColorButton(div*10,240-18,div,9,DMG01_4,true)) {pen = DMG01_4;}
			if(ColorButton(div*11,240-18,div,9,DMG01_3,true)) {pen = DMG01_3;}
			if(ColorButton(div*12,240-18,div,9,DMG01_2,true)) {pen = DMG01_2;}
			if(ColorButton(div*13,240-18,div,9,DMG01_1,true)) {pen = DMG01_1;}
		
			if(Button(divs*0,240-27,divs,9,"Small",true)) {pensize = 0;}
			if(Button(divs*1,240-27,divs,9,"Medium",true)) {pensize = 1;}
			if(Button(divs*2,240-27,divs,9,"Large",true)) {pensize = 2;}
			
			
			

			i2cWriteRegister(3, 0x2D, pen);



			DrawString(bottom_screen,"@@@ Pen Color @@@", 10,0, pen, COLOR_WHITE);
			DrawString(bottom_screen,"PEN SIZE:", 120,0, COLOR_BLACK, COLOR_WHITE);
			switch(pensize) {
				case 0 :
					DrawString(bottom_screen,"Small", 200,0, COLOR_BLACK, COLOR_WHITE);

				case 1 :
					DrawString(bottom_screen,"Medium", 200,0, COLOR_BLACK, COLOR_WHITE);

				case 2 :
					DrawString(bottom_screen,"Large", 200,0, COLOR_BLACK, COLOR_WHITE);
			}

			if (Button(0,0,9,9,"X",true))
			{
				proc = "main";
				rdrupd = true;
			}
			
			// if (Button(320-(4*4),0,9,9,"Save",true))
			// {
				// while(touchD) {TouchProc();}
				// Screenshot(NULL);
				// ClearScreen(top_screen, 400, COLOR_GREEN);
				// sleep(1);
				


			// }
			//doesnt work properly for some reason, images come out corrupted
		}		

		
		else if (!screensoff && proc == "powmen")
		{
			DrawString(bottom_screen,"Shut down the system?",0,0,COLOR_BLACK,COLOR_TRANSPARENT);
			DrawString(bottom_screen,"WARN: Any unsaved data will be lost!!",0,6,COLOR_ORANGE,COLOR_TRANSPARENT);
			DrawString(bottom_screen,"INFO: Pressing yes will >>immediately<< shut the system down. (i)",0,12,COLOR_BLUE,COLOR_TRANSPARENT);
		

			if (Button(320/2,240-9,320/2,9,"Yes, shut down the system.",rdr))
			{
				Shutdown();
			}
			if (Button(0,240-9,320/2,9,"No, go back to the Main Menu.",rdr))
			{
				proc = "main";
				ClearScreen(bottom_screen, 320, COLOR_WHITE);
				while(CheckButton(BUTTON_A)){}
				rdr=false;
			}
			rdrupd = true;
		}
		
		else if (!screensoff && proc == "4dummies")
		{
			DrawString(bottom_screen,"Welcome to the SuperScope Help Menu! Select any topic.",0,0,COLOR_BLACK,COLOR_ORANGE);
			if (Button(0,240-9,320/12,9,"Exit",true))
			{
				proc = "main";
				ClearScreen(bottom_screen, 320, COLOR_WHITE);
				while(CheckButton(BUTTON_A)){}
				rdr=false;
			}
			if (Button(0,6,320/12,9,"Why?",true))
			{
				ClearScreen(bottom_screen, 320, COLOR_WHITE);
				DrawString(bottom_screen,"TLDR: I've always been obsessed with the idea.",320/12+2,8,COLOR_BLUE,COLOR_WHITE);
				DrawString(bottom_screen,"\nComputers have always fascinated me. Especially their software. that and, for as long as i can remember, i've always wanted to make my own operating system for well, whatever the hell i could find a way to do it on first. First it was Scratch, yeah, the block-based ''coding'' website, then i moved on to writing Batch programs, then to BASIC with my first TI-92 Plus, and then i started doing discord bot programming in python, and then after discovering C through 3DS homebrew, i thought, hey, is there a way to make this shit BOOTABLE? \n...And now?\nWell, you're viewing this text on *something*, right?\n\nIf you have any ideas or suggestions, or just want to yell at me for the code that makes up the color picker in Draw, Contact me on discord!\nAliceCE#2770\n\nThx for using my little OS, it means a lot to me as someone who usually doesnt finish, or much less get to the point i've gotten to with projects like these! <3\n\nSpecial thanks to...\nThe GodMode9 Discord for helping me with a LOT of shit lol\nEveryone who made Decrypt9WIP, the original codebase for this OS!",320/12+2,14,COLOR_BLACK,COLOR_WHITE);
			}
			if (Button(0,14,320/12,9,"Draw",true))
			{
				ClearScreen(bottom_screen, 320, COLOR_WHITE);
				DrawString(bottom_screen,"Draw - A simple drawing app for SuperScope OS!",320/12+2,8,COLOR_BLUE,COLOR_WHITE);
				DrawString(bottom_screen,"The first actual application i made for SuperScope OS. It's the simple things that matter, right?\nSelect a color, a pen size, and then start drawing whatever you feel like drawing!\nPress the X in the top left corner or press start to exit the drawing application. \nThe EXTRA1 color pallete (the 16 colors from the top left of the pallete) were all colors from https://coolors.co/. Go check them out for... colors, i guess? :P",320/12+2,14,COLOR_BLACK,COLOR_WHITE);
			}
			
			if (Button(0,22,320/12,9,"Main",true))
			{
				ClearScreen(bottom_screen, 320, COLOR_WHITE);
				DrawString(bottom_screen,"Main - The HOME Menu's analogue for SuperScope OS.",320/12+2,8,COLOR_BLUE,COLOR_WHITE);
				DrawString(bottom_screen,"Take the stylus, point at an icon and tap the screen, bam, you're in the application you just clicked, Magic~!\nThe screen will lock if you close the shell or press SELECT. you can tap the screen or press SELECT again to exit sleep mode. The LCD will not turn off on it's own yet, but i plan on adding this when i make the Config app.",320/12+2,14,COLOR_BLACK,COLOR_WHITE);
			}
			rdrupd = false;
		}
			//t
				
		while(!MCU_getShellClose() && !CheckButton(BUTTON_SELECT) && !touchD && !(MCU_getPowerButton()||CheckButton(BTNA)||(CheckButton(BTNSTART)||CheckButton(BTNSELECT)||CheckButton(BTNB)||CheckButton(BTNARROW))))
		{	
			TouchProc();
			if(fuct < 0)
			{
				fuct = 2000;
				ClearScreen(top_screen,400, EXTRA1_MANGO);
				sprintf(proctext,"The currently active process is %s                    ",proc);
				DrawString(top_screen,proctext, 1,1, COLOR_WHITE, EXTRA1_MANGO);
				DrawString(top_screen,MCU_getBatteryText(), 1,7, COLOR_WHITE, EXTRA1_MANGO);
				DrawString(top_screen,MCU_getSlidersText(), 1,13, COLOR_WHITE, EXTRA1_MANGO);
				
				sprintf(storagetext,"%llx Free Blocks are left on the SD Card.                    ",RemainingStorageSpace()/128);
				DrawString(top_screen,storagetext, 1,19, COLOR_WHITE, EXTRA1_MANGO);

				sprintf(storagetext,"there are %llx Total Blocks on the SD Card                    ",TotalStorageSpace()/128);
				DrawString(top_screen,storagetext, 1,25, COLOR_WHITE, EXTRA1_MANGO);

				}
				fuct--;


				if (CheckButton(BUTTON_B) && CheckButton(BUTTON_L1) && CheckButton(BUTTON_R1))
			{
				Reboot(); //failsafe
			}
		}
		
					if (CheckButton(BUTTON_START))
			{
				proc = "main";
				rdrupd = true;
			}
			
			else if (CheckButton(BUTTON_SELECT) || MCU_getShellClose() || (touchD && screensoff==true))
			{
				while( CheckButton(BUTTON_SELECT)){}
				if(screensoff==true && !MCU_getShellClose())
				{
					MCU_powerLedSet(POWERLED_BLUE);
					MCU_setBacklight(2,true);
					screensoff=false;
				}
				else if(screensoff==false || MCU_getShellClose())
				{
					MCU_powerLedSet(POWERLED_RED);
					MCU_setBacklight(2,false);
					screensoff=true;
				}
			}
		
		if (rdrupd==true)
		{
			//...what the fuck am i doing??
			rdrupd=false;
			rdr=true;
			ClearScreen(bottom_screen, 320, COLOR_WHITE);
			//tell the program to erase the screen... something something button drawing...
			//todo: scrap this shit whenever possible and replace with a less gritty method

		}
		else
		{
			rdr=false;
		}
			
	}
}


//amogus sus
// morbed all over, mobius
// I FUCKING LOVE LEAN!!!!!!!!!!!!!!!!!!!!!!!!!!
// CBT
// Sono, on the wall of colors:
// "oh my fucking god"
// "i want to choke something"
// "w h y"
// end quote
// bwuh


//TODO
//add config app
//switch color picker from Vertical placement to Horizontial, and add RTL <-> LTR setting in config app
//when? probably tommorow lol i have programmed enough for the amount of sleep i've gotten
//FIXED - fix EXTRA_SKY to make it not the same color as EXTRA_VIOLET (visible issue pictured in the above photos of the editor)
//add a primitive saving function, likely using code from screenshot.h
//dont let sono see the ifwall color selection code ever again
