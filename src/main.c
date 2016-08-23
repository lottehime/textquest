#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <psp2/ctrl.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/moduleinfo.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/io/dirent.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/sysmodule.h>
#include <psp2/moduleinfo.h>
#include <psp2/types.h>

#include <sys/time.h>

#include "graphics.h"

/*
* Map:
*                                        ^
*                                      NORTH
*
*
*                                 +------------+
*                                 |            |
*                                 |     11     |
*                                 | Princess!  |
*                                 +----+  +----+
*                                      |  |
*                                 +----+  +----+
*                                 |            |
*                                 |     10     |
*                                 |   Dragon!  |
*                                 +----+  +----+
*                                      |  |
*             +--------+ +------+ +----+  +----+ +---------+
*             |        | |      | |            | |         |
*             |   6    +-+  7   +-+      8     +-+   9     |
*             | Sword     Spider                   Flower  |
*             |        +-+      +-+            +-+         |
*             |        | |      | |            | |         |
*             +--------+ +------+ -----+  +----+ +---------+
*                                      |  |
*                 +--------+      +----+  +----+ +---------+
*                 |        |      |            | |         |
*                 |   3    +------+     4      +-+   5     |
*                 |  Dark     2     Spider Webs    Potion  |
*                 |        +-+  +-+            +-+         |
*                 |        | |  | |            | |         |
*                 +--------+ |  | +------------+ +---------+
*                            |  |
*                            |  |
*                             1
*                          Entrance
*/

#define DELAY 3000000

int delay = DELAY;
int location = 1;
int haveFlowers = 0;
int haveSword = 0;
int drankPotion = 0;
int dead = 0;
int killedSlug = 0;
int killedDragon = 0;
int savedPrincess = 0;
int coward = 0;
int action;
int buttonpress;
int startgame;

void doLocation1();
void doLocation2();
void doLocation3();
void doLocation4();
void doLocation5();
void doLocation6();
void doLocation7();
void doLocation8();
void doLocation9();
void doLocation10();
void doLocation11();

int get_key(void) {
	static unsigned buttons[] = {
		SCE_CTRL_SELECT,
		SCE_CTRL_START,
		SCE_CTRL_UP,
		SCE_CTRL_RIGHT,
		SCE_CTRL_DOWN,
		SCE_CTRL_LEFT,
		SCE_CTRL_LTRIGGER,
		SCE_CTRL_RTRIGGER,
		SCE_CTRL_TRIANGLE,
		SCE_CTRL_CIRCLE,
		SCE_CTRL_CROSS,
		SCE_CTRL_SQUARE,
	};

	static unsigned prev = 0;
	SceCtrlData pad;
	while (1) {
		memset(&pad, 0, sizeof(pad));
		sceCtrlPeekBufferPositive(0, &pad, 1);
		unsigned newbut = prev ^ (pad.buttons & prev);
		prev = pad.buttons;
		for (int i = 0; i < sizeof(buttons) / sizeof(*buttons); ++i)
			if (newbut & buttons[i])
				return buttons[i];

		sceKernelDelayThread(1000); // 1ms
	}
}

void doError()
{
	psvDebugScreenPrintf("What!?\n");
	sceKernelDelayThread(delay);
}

int getAction()
{
	
	switch (get_key()) {
	case SCE_CTRL_CROSS:
		buttonpress = 1;
		break;
	case SCE_CTRL_CIRCLE:
		buttonpress = 2;
		break;
	case SCE_CTRL_TRIANGLE:
		buttonpress = 3;
		break;
	case SCE_CTRL_SQUARE:
		buttonpress = 4;
		break;
	default:
		buttonpress = 0;
	}

	return buttonpress;;
}

void doMove()
{
	psvDebugScreenPrintf(".");
	fflush(stdout);
	sceKernelDelayThread(delay);
	psvDebugScreenPrintf(".");
	fflush(stdout);
	sceKernelDelayThread(delay);
	psvDebugScreenPrintf(".");
	fflush(stdout);
	sceKernelDelayThread(delay);
	psvDebugScreenPrintf("\n\n\n");
	
	while (!dead && !savedPrincess && !coward)
	{
		switch (location)
		{
		case 1:
			doLocation1();
			break;
		case 2:
			doLocation2();
			break;
		case 3:
			doLocation3();
			break;
		case 4:
			doLocation4();
			break;
		case 5:
			doLocation5();
			break;
		case 6:
			doLocation6();
			break;
		case 7:
			doLocation7();
			break;
		case 8:
			doLocation8();
			break;
		case 9:
			doLocation9();
			break;
		case 10:
			doLocation10();
			break;
		case 11:
			doLocation11();
			break;
		}
	}
}

void doLocation1()
{
	psvDebugScreenClear(0);
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("\n");
	psvDebugScreenSetFgColor(COLOR_GREY);
 	psvDebugScreenPrintf("                                    /   \\              /'\\       _                               \n");
 	psvDebugScreenPrintf("                \\_..           /'.,/     \\_         .,'   \\     / \\_                             \n");
 	psvDebugScreenPrintf("                    \\         /            \\      _/       \\_  /    \\     _                      \n");
 	psvDebugScreenPrintf("                     \\__,.   /              \\    /           \\/.,   _|  _/ \\                     \n");
 	psvDebugScreenPrintf("                          \\_/                \\  /',.,''\\      \\_ \\_/  \\/    \\                  \n");
 	psvDebugScreenPrintf("                                           _  \\/   /    ',../',.\\    _/      \\                   \n");
 	psvDebugScreenPrintf("                             /           _/m\\  \\  /    |         \\  /.,/'\\   _\\                  \n");
 	psvDebugScreenPrintf("                           _/           /MMmm\\  \\_     |          \\/      \\_/  \\                 \n");
 	psvDebugScreenPrintf("                          /      \\     |MMMMmm|   \\__   \\          \\_       \\   \\_              \n");
 	psvDebugScreenPrintf("                                  \\   /MMMMMMm|      \\   \\           \\       \\    \\             \n");
 	psvDebugScreenPrintf("                                   \\  |MMMMMMmm\\      \\___            \\_      \\_   \\            \n");
 	psvDebugScreenPrintf("                                    \\|MMMMMMMMmm|____.'  /\\_            \\       \\   \\_           \n");
 	psvDebugScreenPrintf("                                    /'.,___________...,,'   \\            \\   \\        \\          \n");
 	psvDebugScreenPrintf("                                   /       \\          |      \\    |__     \\   \\_       \\         \n");
 	psvDebugScreenPrintf("                                 _/        |           \\      \\_     \\     \\    \\       \\_      \n");
 	psvDebugScreenPrintf("                                /                               \\     \\     \\_   \\        \\      \n");
 	psvDebugScreenPrintf("                                                                 \\     \\      \\   \\__      \\     \n");
 	psvDebugScreenPrintf("                                                                  \\     \\_     \\     \\      \\    \n");
 	psvDebugScreenPrintf("                                                                   |      \\     \\     \\      \\   \n");
 	psvDebugScreenPrintf("                                                                    \\            |            \\  \n");
	psvDebugScreenSetFgColor(COLOR_WHITE);
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("You see the entrance to the dragon's lair to the north.\n");
	psvDebugScreenPrintf("You hear the princess yelling, \"Help me! Help me!\"\n");
	psvDebugScreenPrintf("You hear the bellow of the dragon.\n");
	psvDebugScreenPrintf("What do you do?\n");
	psvDebugScreenPrintf("X - Enter the lair.\n");
	psvDebugScreenPrintf("O - Run away.\n");
	psvDebugScreenPrintf("? ");
	action = getAction();
	switch (action)
	{
	case 1:
		location = 2;
		break;
	case 2:
		coward = 1;
		break;
	default:
		doError();
		break;
	}
	doMove();
}

void doLocation2()
{
	psvDebugScreenClear(0);
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("\n");
	psvDebugScreenSetFgColor(COLOR_GREY);
	psvDebugScreenPrintf("                             _______               ______________      ___      _______\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+-+-+-+-+-+-|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-+______+-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+I      |+-+|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-|      |-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+|      | ++|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-I      |-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+|      |+-+|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-|      |-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+I      |+-+|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|===--------===|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////                \\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|////////////                  \\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|///////////                    \\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|//////////                      \\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////                            |\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|////////                             |\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|///////                              |\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|//////                               |\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////                                 \\\\\\\\|_____|_|\n");
	psvDebugScreenSetFgColor(COLOR_WHITE);
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("You are inside the dragon's burned fortress lair.\n");
	psvDebugScreenPrintf("The heat is opressive and the stench of brimstone burns your nostrils.\n");
	psvDebugScreenPrintf("You see a door leading west, a door leading east, and a door leading south.\n");
	psvDebugScreenPrintf("What do you do?\n");
	psvDebugScreenPrintf("X - Go east.\n");
	psvDebugScreenPrintf("O - Go west.\n");
	psvDebugScreenPrintf("^ - Go south.\n");
	psvDebugScreenPrintf("? ");
	action = getAction();
	switch (action)
	{
	case 1:
		location = 4;
		break;
	case 2:
		location = 3;
		break;
	case 3:
		location = 1;
		break;
	default:
		doError();
		break;
	}
	doMove();
}

void doLocation3()
{
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("It's very dark in this room.\n");
	sceKernelDelayThread(2 * delay);
	psvDebugScreenPrintf("You cannot see a thing.\n");
	sceKernelDelayThread(2 * delay);
	psvDebugScreenPrintf("You feel your way around the room, finding nothing as a point of reference and nothing of interest.\n");
	sceKernelDelayThread(2 * delay);
	psvDebugScreenPrintf("Fumbling, you finnally find your way out of the room.\n");
	location = 2;
	doMove();
}

void doLocation4()
{
	psvDebugScreenClear(0);
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("\n");
	psvDebugScreenSetFgColor(COLOR_WHITE);
 	psvDebugScreenPrintf("                                           \\_______/        \n");
 	psvDebugScreenPrintf("                                       `.,-'\\_____/`-.,'      \n");
 	psvDebugScreenPrintf("                                        /`..'\\ _ /`.,'\\      \n");
 	psvDebugScreenPrintf("                                       /  /`.,' `.,'\\  \\    \n");
 	psvDebugScreenPrintf("                                      /__/__/     \\__\\__\\  \n");
 	psvDebugScreenPrintf("                                      \\  \\  \\     /  /  /   \n");
 	psvDebugScreenPrintf("                                       \\  \\,'`._,'`./  /    \n");
 	psvDebugScreenPrintf("                                        \\,'`./___\\,'`./          \n");
 	psvDebugScreenPrintf("                                       ,'`-./_____\\,-'`.          \n");
 	psvDebugScreenPrintf("                                           /       \\       \n");
 	psvDebugScreenSetFgColor(COLOR_WHITE);
 	psvDebugScreenSetFgColor(COLOR_GREY);
	psvDebugScreenPrintf("                             _______               ______________      ___      _______\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+-+-+-+-+-+-|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-+______+-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+I      |+-+|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-|      |-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+|      | ++|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-I      |-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+|      |+-+|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-|      |-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+I      |+-+|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|===--------===|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////                \\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|////////////                  \\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|///////////                    \\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|//////////                      \\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////                            |\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|////////                             |\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|///////                              |\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|//////                               |\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////                                 \\\\\\\\|_____|_|\n");
	psvDebugScreenSetFgColor(COLOR_WHITE);
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("You are standing in a room covered in spider webs.\n");
	psvDebugScreenPrintf("There are  doors leading west, north, and east.\n");
	psvDebugScreenPrintf("What do you do?\n");
	psvDebugScreenPrintf("X - Go east.\n");
	psvDebugScreenPrintf("O - Go north.\n");
	psvDebugScreenPrintf("^ - Go west.\n");
	psvDebugScreenPrintf("? ");
	action = getAction();
	switch (action)
	{
	case 1:
		location = 5;
		break;
	case 2:
		location = 8;
		break;
	case 3:
		location = 2;
		break;
	default:
		doError();
		break;
	}
	doMove();
}

void doLocation5()
{
	psvDebugScreenClear(0);
	if (!drankPotion)
	{
		psvDebugScreenSetFgColor(COLOR_GREEN);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("                                                       _____     \n");
		psvDebugScreenPrintf("                                                      `.___,'     \n");
		psvDebugScreenPrintf("                                                       (___)     \n");
		psvDebugScreenPrintf("                                                       <   >     \n");
		psvDebugScreenPrintf("                                                        ) (     \n");
		psvDebugScreenPrintf("                                                       /`-.\\     \n");
		psvDebugScreenPrintf("                                                      /     \\     \n");
		psvDebugScreenPrintf("                                                     / _    _\\     \n");
		psvDebugScreenPrintf("                                                    :,' `-.' `:     \n");
		psvDebugScreenPrintf("                                                    |         |     \n");
		psvDebugScreenPrintf("                                                    :         ;     \n");
		psvDebugScreenPrintf("                                                     \\       /     \n");
		psvDebugScreenPrintf("                                                      `.___.'     \n");
		psvDebugScreenSetFgColor(COLOR_WHITE);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("You are in a room with a table.\n");
		psvDebugScreenPrintf("On the table you can see a bottle that appears to be a potion.\n");
	}
	else
	{
		psvDebugScreenSetFgColor(COLOR_GREY);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("                                                        ___      \n");
		psvDebugScreenPrintf("                                                       <   >     \n");
		psvDebugScreenPrintf("                                                        ) (     \n");
		psvDebugScreenPrintf("                                                       /   \\     \n");
		psvDebugScreenPrintf("                                                      /     \\     \n");
		psvDebugScreenPrintf("                                                     /       \\     \n");
		psvDebugScreenPrintf("                                                    :         :     \n");
		psvDebugScreenPrintf("                                                    |         |     \n");
		psvDebugScreenPrintf("                                                    :         ;     \n");
		psvDebugScreenPrintf("                                                     \\       /     \n");
		psvDebugScreenPrintf("                                                      `.___.'     \n");
		psvDebugScreenSetFgColor(COLOR_WHITE);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("You are in a room with an empty potion bottle.\n");
	}
	psvDebugScreenSetFgColor(COLOR_GREY);
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("                             _______               ______________      ___      _______\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+-+-+-+-+-+-|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-+______+-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+I      |+-+|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-|      |-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+|      | ++|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-I      |-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+|      |+-+|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-|      |-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+I      |+-+|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|===--------===|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////                \\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|////////////                  \\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|///////////                    \\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|//////////                      \\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////                            |\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|////////                             |\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|///////                              |\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|//////                               |\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////                                 \\\\\\\\|_____|_|\n");
	psvDebugScreenSetFgColor(COLOR_WHITE);
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("There is a door to the west.\n");
	psvDebugScreenPrintf("What do you do?\n");
	psvDebugScreenPrintf("X - Go west.\n");
	if (!drankPotion)
	{
		psvDebugScreenPrintf("O - Drink the potion.\n");
	}
	psvDebugScreenPrintf("? ");
	action = getAction();
	switch (action)
	{
	case 1:
		location = 4;
		break;
	case 2:
		if (!drankPotion)
		{
			psvDebugScreenPrintf("\n");
			psvDebugScreenSetFgColor(COLOR_RED);
			psvDebugScreenPrintf("Yuck! It does not taste good. You wonder what will happen...\n");
			sceKernelDelayThread(2 * delay);
			psvDebugScreenSetFgColor(COLOR_GREEN);
			psvDebugScreenPrintf("Suddenly you feel much stronger!\n");
			psvDebugScreenSetFgColor(COLOR_WHITE);
			drankPotion = 1;
			break;
		}
	default:
		doError();
		break;
	}
	doMove();
}

void doLocation6()
{
	psvDebugScreenClear(0);
	if (!haveSword)
	{
		psvDebugScreenSetFgColor(COLOR_YELLOW);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
  		psvDebugScreenPrintf("                                  (O)               ");
  		psvDebugScreenPrintf("                                  <M                ");
  		psvDebugScreenPrintf("                       o          <M                ");
  		psvDebugScreenPrintf("                      /| ......  /:M\\------------------------------------------------,,,,,, ");
  		psvDebugScreenPrintf("                    (O)[]XXXXXX[]I:K+}=====<{H}>================================------------> ");
  		psvDebugScreenPrintf("                      \\| ^^^^^^  \\:W/------------------------------------------------'''''' ");
  		psvDebugScreenPrintf("                       o          <W                ");
  		psvDebugScreenPrintf("                                  <W                ");
  		psvDebugScreenPrintf("                                  (O)               ");
		psvDebugScreenSetFgColor(COLOR_WHITE);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("You are in a room with a golden sword laying across a purple cloth draped over a table.\n");
	}
	else
	{
		psvDebugScreenPrintf("You are in a room with a purple cloth draped over a table.\n");
	}
	psvDebugScreenSetFgColor(COLOR_GREY);
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("                             _______               ______________      ___      _______\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+-+-+-+-+-+-|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-+______+-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+I      |+-+|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-|      |-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+|      | ++|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-I      |-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+|      |+-+|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-|      |-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+I      |+-+|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|===--------===|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////                \\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|////////////                  \\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|///////////                    \\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|//////////                      \\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////                            |\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|////////                             |\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|///////                              |\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|//////                               |\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////                                 \\\\\\\\|_____|_|\n");
	psvDebugScreenSetFgColor(COLOR_WHITE);
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("There is a door to the east.\n");
	psvDebugScreenPrintf("What do you do?\n");
	psvDebugScreenPrintf("X - Go east.\n");
	if (!haveSword)
	{
		psvDebugScreenPrintf("O - Take the sword.\n");
	}
	psvDebugScreenPrintf("? ");
	action = getAction();
	switch (action)
	{
	case 1:
		location = 7;
		break;
	case 2:
		if (!haveSword)
		{
			psvDebugScreenSetFgColor(COLOR_GREEN);
			psvDebugScreenPrintf("You pick up the sword and buff it with the cloth, placing the cloth back onto the table.\n");
			psvDebugScreenSetFgColor(COLOR_WHITE);
			haveSword = 1;
			break;
		}
	default:
		doError();
		break;
	}
	doMove();
}

void doLocation7()
{
	psvDebugScreenClear(0);
	if (!killedSlug)
	{
		psvDebugScreenSetFgColor(COLOR_GREY);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("                                           ____                      ,         \n");
		psvDebugScreenPrintf("                                          /---.'.__             ____//         \n");
		psvDebugScreenPrintf("                                               '--.\\           /.---'         \n");
		psvDebugScreenPrintf("                                          _______  \\\\         //         \n");
		psvDebugScreenPrintf("                                        /.------.\\  \\|      .'/  ______         \n");
		psvDebugScreenPrintf("                                       //  ___  \\ \\ ||/|\\  //  _/_----.\\__         \n");
		psvDebugScreenPrintf("                                      |/  /.-.\\  \\ \\:|< >|// _/.'..\\   '--'         \n");
		psvDebugScreenPrintf("                                         //   \\'. | \\'.|.'/ /_/ /  \\\\         \n");
		psvDebugScreenPrintf("                                        //     \\ \\_\\/' ' ~\\-'.-'    \\\\         \n");
		psvDebugScreenPrintf("                                       //       '-._| :H: |'-.__     \\\\         \n");
		psvDebugScreenPrintf("                                      //           (/'==='\\)'-._\\     ||         \n");
		psvDebugScreenPrintf("                                      ||                        \\\\    \\|         \n");
		psvDebugScreenPrintf("                                      ||                         \\\\    '         \n");
		psvDebugScreenPrintf("                                      |/                          \\\\         \n");
		psvDebugScreenPrintf("                                                                   ||         \n");
		psvDebugScreenPrintf("                                                                   ||         \n");
		psvDebugScreenPrintf("                                                                   \\\\         \n");
		psvDebugScreenPrintf("                                                                    '         \n");
		psvDebugScreenSetFgColor(COLOR_WHITE);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("You are in a musty room.\n");
		psvDebugScreenPrintf("On the ground, you see an unusually large spider.\n");
		psvDebugScreenPrintf("What do you do?\n");
		psvDebugScreenPrintf("X - Run away.\n");
		psvDebugScreenPrintf("O - Crunch it under your heel.\n");
		psvDebugScreenPrintf("? ");
		action = getAction();
		switch (action)
		{
		case 1:
			psvDebugScreenSetFgColor(COLOR_RED);
			psvDebugScreenPrintf("You cry out in fear and run away.\n");
			psvDebugScreenPrintf("From the distance you hear the princess yell out, \"Do I have to save myself!?\"\n");
			psvDebugScreenSetFgColor(COLOR_WHITE);
			location = 8;
			break;
		case 2:
			psvDebugScreenSetFgColor(COLOR_GREEN);
			psvDebugScreenPrintf("You get bug guts on your boots as you crush the spider with a violent stomp.\n");
			psvDebugScreenSetFgColor(COLOR_WHITE);
			killedSlug = 1;
			break;
		default:
			doError();
			break;
		}
	}
	else
	{
		psvDebugScreenSetFgColor(COLOR_GREY);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("                             _______               ______________      ___      _______\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+-+-+-+-+-+-|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-+______+-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+I      |+-+|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-|      |-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+|      | ++|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-I      |-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+|      |+-+|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-|      |-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+I      |+-+|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|===--------===|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////                \\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|////////////                  \\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|///////////                    \\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|//////////                      \\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////                            |\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|////////                             |\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|///////                              |\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|//////                               |\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////                                 \\\\\\\\|_____|_|\n");
		psvDebugScreenSetFgColor(COLOR_WHITE);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("You are in a room with spider guts smeared on the floor.\n");
		psvDebugScreenPrintf("There is a door to the west and to the east.\n");
		psvDebugScreenPrintf("What do you do?\n");
		psvDebugScreenPrintf("X - Go west.\n");
		psvDebugScreenPrintf("O - Go east.\n");
		psvDebugScreenPrintf("? ");
		action = getAction();
		switch (action)
		{
		case 1:
			location = 6;
			break;
		case 2:
			location = 8;
			break;
		default:
			doError();
			break;
		}
	}
	doMove();
}

void doLocation8()
{
	psvDebugScreenClear(0);
	psvDebugScreenSetFgColor(COLOR_GREY);
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("                             _______               ______________      ___      _______\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+-+-+-+-+-+-|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-+______+-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+I      |+-+|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-|      |-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+|      | ++|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-I      |-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+|      |+-+|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-|      |-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+I      |+-+|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|===--------===|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////                \\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|////////////                  \\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|///////////                    \\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|//////////                      \\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////                            |\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|////////                             |\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|///////                              |\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|//////                               |\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////                                 \\\\\\\\|_____|_|\n");
	psvDebugScreenSetFgColor(COLOR_WHITE);
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("You are in an empty room. The walls feel as though they are emanating heat.\n");
	psvDebugScreenPrintf("There is a  door to the west, north, east, and south.\n");
	psvDebugScreenPrintf("What do you do?\n");
	psvDebugScreenPrintf("X - Go west.\n");
	psvDebugScreenPrintf("O - Go north.\n");
	psvDebugScreenPrintf("^ - Go east.\n");
	psvDebugScreenPrintf("[] - Go south.\n");
	psvDebugScreenPrintf("? ");
	action = getAction();
	switch (action)
	{
	case 1:
		location = 7;
		break;
	case 2:
		location = 10;
		break;
	case 3:
		location = 9;
		break;
	case 4:
		location = 4;
		break;
	default:
		doError();
		break;
	}
	doMove();
}

void doLocation9()
{
	psvDebugScreenClear(0);
	if (!haveFlowers)
	{
		psvDebugScreenSetFgColor(COLOR_YELLOW);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
  		psvDebugScreenPrintf("                                           .-'`'-.      _.---.     \n");
  		psvDebugScreenPrintf("                                         .'        \\   .'      '.    \n");
  		psvDebugScreenPrintf("                                        /           \\ /          \\   \n");
  		psvDebugScreenPrintf("                                       |      __     Y    __      |   \n");
  		psvDebugScreenPrintf("                                       |   .'`   '-./  .-'  `'.   |   \n");
  		psvDebugScreenPrintf("                                        \\ /        ',,'        \\ /-.   \n");
  		psvDebugScreenPrintf("                                         \\|      .-'()'-.      |/--.`\\  \n");
  		psvDebugScreenPrintf("                                          |    /`  /||\\  `\\    |    \\ \\ \n");
  		psvDebugScreenPrintf("                                           \\   |          |   /      ; ;  \n");
  		psvDebugScreenPrintf("                                            '--`\\        /`--'       | |  \n");
  		psvDebugScreenPrintf("                                                 '-.__.-'            ; ;  \n");
  		psvDebugScreenPrintf("                                                 __                  / /  \n");
  		psvDebugScreenPrintf("                                          .-'''-`  '.  .-.   _     /`/`  \n");
  		psvDebugScreenPrintf("                                         (_  .--..._ `-'  \\_| \\  /`/`  \n");
  		psvDebugScreenPrintf("                                          '.__.     `---._    |/`/`   \n");
  		psvDebugScreenPrintf("                                               `)    _..  '-./`/`    \n");
  		psvDebugScreenPrintf("                                               (__.-'  _)  /`/`    \n");
  		psvDebugScreenPrintf("                                                       `--`/`     \n");
  		psvDebugScreenPrintf("                                                          `              \n");
		psvDebugScreenSetFgColor(COLOR_WHITE);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("You are in a humid, musty room.\n");
		psvDebugScreenPrintf("On the ground, you see a single yellow flower growing.\n");
	}
	else
	{
		psvDebugScreenPrintf("You are in a humid, musty room. You see a patch of dirt where flowers once grew.\n");
	}
	psvDebugScreenSetFgColor(COLOR_GREY);
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("                             _______               ______________      ___      _______\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+-+-+-+-+-+-|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-+______+-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+I      |+-+|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-|      |-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+|      | ++|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-I      |-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+|      |+-+|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-|      |-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+I      |+-+|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|===--------===|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////                \\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|////////////                  \\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|///////////                    \\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|//////////                      \\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////                            |\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|////////                             |\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|///////                              |\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|//////                               |\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////                                 \\\\\\\\|_____|_|\n");
	psvDebugScreenSetFgColor(COLOR_WHITE);
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("There is a door to the west.\n");
	psvDebugScreenPrintf("What do you do?\n");
	psvDebugScreenPrintf("X - Go west.\n");
	if (!haveFlowers)
	{
		psvDebugScreenPrintf("O - Pick the flower.\n");
	}
	psvDebugScreenPrintf("? ");
	action = getAction();
	switch (action)
	{
	case 1:
		location = 8;
		break;
	case 2:
		if (!haveFlowers)
		{
			psvDebugScreenSetFgColor(COLOR_GREEN);
			psvDebugScreenPrintf("You pick the flower and place it in your chainmail.\n");
			psvDebugScreenPrintf("It smells subtle and sweet.\n");
			psvDebugScreenSetFgColor(COLOR_WHITE);
			haveFlowers = 1;
			break;
		}
	default:
		doError();
		break;
	}
	doMove();
}

void doLocation10()
{
	psvDebugScreenClear(0);
	if (!killedDragon)
	{
		psvDebugScreenSetFgColor(COLOR_RED);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
  		psvDebugScreenPrintf("                                                    .%%,       \n");
  		psvDebugScreenPrintf("                                                   X:-x\\',       \n");
  		psvDebugScreenPrintf("                                                  X:/%%;::\\:X      \n");
  		psvDebugScreenPrintf("                                                 X:l%%  ; :'\\:X     \n");
  		psvDebugScreenPrintf("                                                X:l%%   :  : '\\:X     \n");
  		psvDebugScreenPrintf("                                                X:l%%   :   :  '\\:X    \n");
  		psvDebugScreenPrintf("                             b,      b,        X:/l%%   :    :   \\:X    \n");
  		psvDebugScreenPrintf("                            JPQ,    JPQ,       X:l%%    :     :   '\\:X    \n");
  		psvDebugScreenPrintf("                          .dP'd|._,=dPQq\\     X:l%%'    :      :    '\\:X   \n");
  		psvDebugScreenPrintf("                         xdP  #P'_    _,:   .X:l%%     :       :    '\\:X   \n");
  		psvDebugScreenPrintf("                      .d/'p   '  'O \\  'O:;  X:l%%'     :       :      \\:X  \n");
  		psvDebugScreenPrintf("                    ,pP' q.          \\:  `#  X:ld       :       :      ':X   \n");
  		psvDebugScreenPrintf("                  ,d'   ,pq  .,-qx_,  '\\  `Q:  l%%       :       :       k:X  \n");
  		psvDebugScreenPrintf("                ./'     Jp       .      `  ` 3  %%       :        ;      k:X  \n");
  		psvDebugScreenPrintf("               dP       p            p    `  `q         :        :      k:X   \n");
  		psvDebugScreenPrintf("              d/       ; J,/';xpx'\\: '*q    ` `\\,       :         :     l:X.  \n");
  		psvDebugScreenPrintf("             dP      ;'    dP      '\\:_,`.q. /d b\\      :         :      k:X  \n");
  		psvDebugScreenPrintf("           .d'      ;    dP            .\\_j '- u-'      :         :      k:X  \n");
  		psvDebugScreenPrintf("X         ./'     ;'    /''            .'                :         ;      k:X  \n");
  		psvDebugScreenPrintf("\\X       .d'     ;    ,''             :      X:l%%        :        :       k:X  \n");
  		psvDebugScreenPrintf(":\\X      d'     ;   ./'             :       X:/%%        :        ;       k:X  \n");
  		psvDebugScreenPrintf("::lX    JP     ;    J              :      .X:/ %%        :       :        k;X  \n");
  		psvDebugScreenPrintf("k:lX    #'    :    j'             :    .X:/ %%           :       :       d:lX  \n");
  		psvDebugScreenPrintf("k:lX   |P    ;     |             :  .X:/ %%              :       ;       k;lX  \n");
  		psvDebugScreenPrintf("k:lX   ||    ;    |'            : X:/ %%                 ;       :      d:;X'  \n");
  		psvDebugScreenPrintf("k:lX   d|   ;     :l           :X:/ %%'                 :        :      k:lX  \n");
  		psvDebugScreenPrintf("k:iX   #|   ;     ||          X:/ %%                    ;        ;      k:lX  \n");
  		psvDebugScreenPrintf(" k:\\X  ||  ;       ||       X:/ %%                     :        :       k:lX  \n");
  		psvDebugScreenPrintf("  k:\\pQJb  ;        \\N.PQ XX/ %%                      ;         ;       k:lX  \n");
  		psvDebugScreenPrintf("   kJP.Ql\\;          XQ. J Q J                      :         ;        k:lX  \n");
  		psvDebugScreenPrintf("   6Q : Q%%           \\Q  Q   J              ;''''':.:;''::.  :         k:lX  \n");
		psvDebugScreenSetFgColor(COLOR_WHITE);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("You are in a room, the heat is opressive. Your armour is warming up.\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenPrintf("You see the dragon!\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenPrintf("Fear grips you.\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenPrintf("What do you do?\n");
		psvDebugScreenPrintf("X - Run away.\n");
		psvDebugScreenPrintf("O - Fight the dragon.\n");
		psvDebugScreenPrintf("? ");
		action = getAction();
		switch (action)
		{
		case 1:
			psvDebugScreenSetFgColor(COLOR_RED);
			psvDebugScreenPrintf("You run away wailing.\n");
			psvDebugScreenPrintf("You hear the princess call out, \"Where are you going, knight!? Help me!\"\n");
			psvDebugScreenSetFgColor(COLOR_WHITE);
			location = 8;
			break;
		case 2:
			if (!haveSword)
			{
				psvDebugScreenSetFgColor(COLOR_RED);
				psvDebugScreenPrintf("You lunge at the dragon.\n");
				sceKernelDelayThread(2 * delay);
				psvDebugScreenPrintf("It snorts amused at the image of you desperately clawing at him with your measely iron sword.\n");
				sceKernelDelayThread(2 * delay);
				psvDebugScreenPrintf("The dragon arches down and swallows you.\n");
				sceKernelDelayThread(2 * delay);
				psvDebugScreenPrintf("You are dead.\n");
				psvDebugScreenSetFgColor(COLOR_WHITE);
				dead = 1;
			}
			else if (!drankPotion)
			{
				psvDebugScreenSetFgColor(COLOR_RED);
				psvDebugScreenPrintf("You lunge at the dragon swinging the golden sword.\n");
				sceKernelDelayThread(2 * delay);
				psvDebugScreenPrintf("But you just don't have the strength to pierce his incredible scales.\n");
				sceKernelDelayThread(2 * delay);
				psvDebugScreenPrintf("It breathes a burst of fire and burns you to a crisp.\n");
				sceKernelDelayThread(2 * delay);
				psvDebugScreenPrintf("You are dead.\n");
				psvDebugScreenSetFgColor(COLOR_WHITE);
				dead = 1;
			}
			else
			{
				psvDebugScreenSetFgColor(COLOR_GREEN);
				psvDebugScreenPrintf("You lunge at the dragon swinging the golden sword with fervour.\n");
				sceKernelDelayThread(2 * delay);
				psvDebugScreenPrintf("You drive your sword deep into the dragon's chest, plunging it into it's dark heart.\n");
				sceKernelDelayThread(2 * delay);
				psvDebugScreenPrintf("The dragon roars in pain, clasping at it's chest.\n");
				sceKernelDelayThread(2 * delay);
				psvDebugScreenPrintf("It snorts it's final breath as life ebbs from it's nostrils.\n");
				sceKernelDelayThread(2 * delay);
				psvDebugScreenSetFgColor(COLOR_WHITE);
				killedDragon = 1;
			}
			break;
		default:
			doError();
			break;
		}
	}
	else
	{
  		psvDebugScreenSetFgColor(COLOR_GREY);
  		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("                             _______               ______________      ___      _______\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+-+-+-+-+-+-|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-+______+-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+I      |+-+|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-|      |-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+|      | ++|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-I      |-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+|      |+-+|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|-+-|      |-+-|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////|+-+I      |+-+|\\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|/////////////|===--------===|\\\\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////////                \\\\\\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|////////////                  \\\\\\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|///////////                    \\\\|___|\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|//////////                      \\|_|_|\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////////                            |\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|////////                             |\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|///////                              |\\\\\\\\|_____|_|\n");
	psvDebugScreenPrintf("                            |____|__|//////                               |\\\\\\\\|__|____|\n");
	psvDebugScreenPrintf("                            |_|_____|/////                                 \\\\\\\\|_____|_|\n");
		psvDebugScreenSetFgColor(COLOR_WHITE);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("You are in a room with the carcass of a dead dragon.\n");
		psvDebugScreenPrintf("There is a door to the south and north.\n");
		psvDebugScreenPrintf("What do you do?\n");
		psvDebugScreenPrintf("X - Go north.\n");
		psvDebugScreenPrintf("O - Go south.\n");
		psvDebugScreenPrintf("? ");
		action = getAction();
		switch (action)
		{
		case 1:
			location = 11;
			break;
		case 2:
			location = 8;
			break;
		default:
			doError();
			break;
		}
	}
	doMove();
}

void doLocation11()
{
	psvDebugScreenClear(0);
	if (!haveFlowers)
	{
		psvDebugScreenSetFgColor(COLOR_YELLOW);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
     	psvDebugScreenPrintf("                                                           %%%%%%%%     \n");
     	psvDebugScreenPrintf("                                                          %%%%%%%%-(    \n");
     	psvDebugScreenPrintf("                                                        _%%%%%%%%%%_/    \n");
     	psvDebugScreenPrintf("                                                      _%%%%%%%%%%%%%%%%    \n");
     	psvDebugScreenPrintf("                                                    _%%%%%%%%%%%%%%/ \\%%   \n");
     	psvDebugScreenPrintf("                                                   %%%%%%%%%%%%%%%%%%\\\\ \\_   \n");
     	psvDebugScreenPrintf("                                                     %%%%%%%%%%%%   \\ \\\\   \n");
     	psvDebugScreenPrintf("                                                         )    /\\_/     \n");
     	psvDebugScreenPrintf("                                                       /(___. \\     \n");
     	psvDebugScreenPrintf("                                                       '----' (      \n");
     	psvDebugScreenPrintf("                                                      /       )      \n");
     	psvDebugScreenPrintf("                                                     /        (      \n");
     	psvDebugScreenPrintf("                                                    /         )      \n");
     	psvDebugScreenPrintf("                                                  ,'          (      \n");
     	psvDebugScreenPrintf("                                               ,-'            )      \n");
     	psvDebugScreenPrintf("                                               '-._    '-..___(      \n");
     	psvDebugScreenPrintf("                                                   ``---....__)      \n");
     	psvDebugScreenPrintf("                                                         )|)|        \n");
     	psvDebugScreenPrintf("                                                        '-'-.\\      \n");
		psvDebugScreenSetFgColor(COLOR_WHITE);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("You enter the room.\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenPrintf("Before you is the most beautiful woman you have ever seen!\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenSetFgColor(COLOR_YELLOW);
		psvDebugScreenPrintf("She looks upset.\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenSetFgColor(COLOR_RED);
		psvDebugScreenPrintf("No flowers!? Not even a poem!?\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenPrintf("Annoyed, she pushes you out of the room and huffs at the barred window.\n");
		psvDebugScreenSetFgColor(COLOR_WHITE);
		location = 10;
	}
	else
	{
		psvDebugScreenSetFgColor(COLOR_CYAN);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
     	psvDebugScreenPrintf("                                                        ,-''.        \n");
     	psvDebugScreenPrintf("                                                       ,@@o. )        \n");
     	psvDebugScreenPrintf("                                                      (_      )        \n");
     	psvDebugScreenPrintf("                                                     s  S.    '        \n");
     	psvDebugScreenPrintf("                                                      )=  '? /        \n");
     	psvDebugScreenPrintf("                                                      \\     (`        \n");
     	psvDebugScreenPrintf("                                                       `-;   \\        \n");
     	psvDebugScreenPrintf("                                                        /,-.  )        \n");
     	psvDebugScreenPrintf("                                                      ,-''-.`-'|        \n");
     	psvDebugScreenPrintf("                                                     ( /\\/{_\\  !       \n");
     	psvDebugScreenPrintf("                                                    ,/\\/\\/ {_\\ ;       \n");
     	psvDebugScreenPrintf("                                                   /\\/\\/\\    {V       \n");
     	psvDebugScreenPrintf("                                                 ,/\\/\\// )    (_       \n");
     	psvDebugScreenPrintf("                                               ,\\/\\/\\//,-       `-.      \n");
     	psvDebugScreenPrintf("                                              /\\/\\/\\/-' \\_         `-.     \n");
     	psvDebugScreenPrintf("                                         __,--\\/\\/\\/( '.  `-.         `.     \n");
     	psvDebugScreenPrintf("                                          ///`-\\/\\/  \\  `.   `-.__      `.    \n");
     	psvDebugScreenPrintf("                                               ,\\/    `.  `.       `----''\\    \n");
     	psvDebugScreenPrintf("                                              /         `.  `-.            \\    \n");
     	psvDebugScreenPrintf("                                             /            `-.  `-.          \\    \n");
     	psvDebugScreenPrintf("                                            /                `-.  `----'--'' \\    \n");
     	psvDebugScreenPrintf("                                           /                    `-.____.__.-'-'    \n");
     	psvDebugScreenPrintf("                                          /                                    \\   \n");
     	psvDebugScreenPrintf("                                        /                                        \\   \n");
     	psvDebugScreenPrintf("                                      /                                            \\   \n");
     	psvDebugScreenPrintf("                                       `--___-_                            __-__--'   \n");
     	psvDebugScreenPrintf("                                               `-___-_                ___-'     \n");
     	psvDebugScreenPrintf("                                                      `---___-_____--'      \n");
		psvDebugScreenSetFgColor(COLOR_WHITE);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("You enter the room.\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenPrintf("Before you is the most beautiful woman you have ever seen!\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenSetFgColor(COLOR_GREEN);
		psvDebugScreenPrintf("She runs toward you with her arms open!\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenPrintf("\"My hero!!!\"\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenPrintf("\"What a beautfil flower! I love flowers!\"\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenPrintf("\"Get me out of here! I want to go home!\"\n");
		psvDebugScreenSetFgColor(COLOR_WHITE);
		savedPrincess = 1;
	}
	doMove();
}

int main()
{
	psvDebugScreenInit();
 	psvDebugScreenSetFgColor(COLOR_YELLOW);
 	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("\n");
	psvDebugScreenPrintf("\n");
 	psvDebugScreenPrintf("                                                 ,     \\    /      ,        \n");
 	psvDebugScreenPrintf("                                                / \\    )\\__/(     / \\          \n");
 	psvDebugScreenPrintf("                                               /   \\  (_\\  /_)   /   \\                   \n");
 	psvDebugScreenPrintf("                            __________________/_____\\__\\@  @/___/_____\\_________________   \n");
 	psvDebugScreenPrintf("                            |                          |\\../|                          |   \n");
 	psvDebugScreenPrintf("                            |                           \\VV/                           |   \n");
 	psvDebugScreenPrintf("                            |                 __ __|             |                     |    \n");
 	psvDebugScreenPrintf("                            |                    |   _ \\ \\ \\  /  __|                   |   \n");
 	psvDebugScreenPrintf("                            |                    |   __/  `  <   |                     |    \n");
 	psvDebugScreenPrintf("                            |          ________ _| \\___|  _/\\_\\ \\__|    __             |   \n");
 	psvDebugScreenPrintf("                            |          \\_____  \\  __ __   ____   _______/  |_          |   \n");
 	psvDebugScreenPrintf("                            |           /  / \\  \\|  |  \\_/ __ \\ /  ___/\\   __\\         |   \n");
 	psvDebugScreenPrintf("                            |          /   \\_/.  \\  |  /\\  ___/ \\___ \\  |  |           |  \n");
 	psvDebugScreenPrintf("                            |          \\_____\\ \\_/____/  \\___  >____  > |__|           |   \n");
 	psvDebugScreenPrintf("                            |                 \\__>           \\/     \\/                 |   \n");
 	psvDebugScreenPrintf("                            |__________________________________________________________|    \n");
 	psvDebugScreenPrintf("                                          |    /\\ /      \\\\       \\ /\\    |      \n");
 	psvDebugScreenPrintf("                                          |  /   V        ))       V   \\  |       \n");
 	psvDebugScreenPrintf("                                          |/     `       //        '     \\|       \n");
 	psvDebugScreenPrintf("                                          `              V                '       \n");
 	psvDebugScreenSetFgColor(COLOR_GREY);
 	psvDebugScreenPrintf("                                     A simple choose your own adventure text game                       \n");
 	psvDebugScreenPrintf("                                                     by josh_axey                          \n");
 	psvDebugScreenPrintf("                                                                                              \n");
 	psvDebugScreenPrintf("                                                                                              \n");
 	psvDebugScreenSetFgColor(COLOR_WHITE);
 	psvDebugScreenPrintf("                                          PRESS [START] TO BEGIN YOUR JOURNEY                         \n");

	switch (get_key()) {
	case SCE_CTRL_START:
		startgame = 1;
		break;
	default:
		startgame = 0;
	}

	if(startgame = 1){
		psvDebugScreenClear(0);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("A herald has come to deliver news from the King to all of his knights.\n");
		psvDebugScreenPrintf("\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenPrintf("The princess and future queen, April, has been kidnapped by a dragon.\n");
		psvDebugScreenPrintf("\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenPrintf("The dragon has stolen her away to his cave in the ruins of an old fortress.\n");
		psvDebugScreenPrintf("\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenPrintf("You have heard tales of her beauty and wit, and hope not only to save her but to ask her hand in marriage.\n");
		psvDebugScreenPrintf("\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenPrintf("You bravely accept the King's quest and head off into the dark forest, towards the dragon's lair.\n");
		psvDebugScreenPrintf("\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenSetFgColor(COLOR_GREY);
		psvDebugScreenPrintf("                                                                         .\n");
		psvDebugScreenPrintf("                                                              .         ;  \n");
		psvDebugScreenPrintf("                                 .              .              ;%%     ;;   \n");
		psvDebugScreenPrintf("                                   ,           ,                :;%%  %%;   \n");
		psvDebugScreenPrintf("                                    :         ;                   :;%%;'     ., \n");  
		psvDebugScreenPrintf("                           ,.        %%;     %%;            ;        %%;'    ,;\n");
		psvDebugScreenPrintf("                             ;       ;%%;  %%%%;        ,     %%;    ;%%;    ,%%'\n");
		psvDebugScreenPrintf("                              %%;       %%;%%;      ,  ;       %%;  ;%%;   ,%%;' \n");
		psvDebugScreenPrintf("                               ;%%;      %%;        ;%%;        %% ;%%;  ,%%;'\n");
		psvDebugScreenPrintf("                                `%%;.     ;%%;     %%;'         `;%%%%;.%%;'\n");
		psvDebugScreenPrintf("                                 `:;%%.    ;%%%%. %%@;        %%; ;@%%;%%'\n");
		psvDebugScreenPrintf("                                    `:%%;.  :;bd%%;          %%;@%%;'\n");
		psvDebugScreenPrintf("                                      `@%%:.  :;%%.         ;@@%%;'   \n");
		psvDebugScreenPrintf("                                        `@%%.  `;@%%.      ;@@%%;         \n");
		psvDebugScreenPrintf("                                          `@%%%%. `@%%%%    ;@@%%;        \n");
		psvDebugScreenPrintf("                                            ;@%%. :@%%%%  %%@@%%;       \n");
		psvDebugScreenPrintf("                                              %%@bd%%%%%%bd%%%%:;     \n");
		psvDebugScreenPrintf("                                                #@%%%%%%%%%%:;;\n");
		psvDebugScreenPrintf("                                                %%@@%%%%%%::;\n");
		psvDebugScreenPrintf("                                                %%@@@%%(o);  . '\n");         
		psvDebugScreenPrintf("                                                %%@@@o%%;:(.,'       \n");  
		psvDebugScreenPrintf("                                            `.. %%@@@o%%::;         \n");
		psvDebugScreenPrintf("                                               `)@@@o%%::;         \n");
		psvDebugScreenPrintf("                                                %%@@(o)::;        \n");
		psvDebugScreenPrintf("                                               .%%@@@@%%::;         \n");
		psvDebugScreenPrintf("                                               ;%%@@@@%%::;.          \n");
		psvDebugScreenPrintf("                                              ;%%@@@@%%%%:;;;. \n");
		psvDebugScreenPrintf("                                          ...;%%@@@@@%%%%:;;;;,..   \n");
		psvDebugScreenSetFgColor(COLOR_WHITE);
	
		doMove();
	}

	if (savedPrincess)
	{
		psvDebugScreenClear(0);
		psvDebugScreenSetFgColor(COLOR_WHITE);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
  		psvDebugScreenPrintf("                                    ///........./|\\.........\\\\\\  \n");
  		psvDebugScreenPrintf("                                   ////    :    |||    :    \\\\\\\\ \n");
  		psvDebugScreenPrintf("                                   \\\\'''''':''''|||'''':''''''//  \n");
  		psvDebugScreenPrintf("                                    \\\\`.    .   |||   .    .'//  \n");
  		psvDebugScreenPrintf("                                     \\\\\\`.   .  |||  .   .'///  \n");
  		psvDebugScreenPrintf("                                      \\\\\\\\`.  . ||| .  .'////  \n");
  		psvDebugScreenPrintf("                                       \\\\\\\\\\`. .|||. .'/////    \n");
  		psvDebugScreenPrintf("                                        '\\\\\\\\\\`.|||.'/////'     \n");
  		psvDebugScreenPrintf("                                         '\\\\\\\\\\\\|||//////'  \n");
  		psvDebugScreenPrintf("                                           '\\\\\\\\\\|/////'     \n");
  		psvDebugScreenSetFgColor(COLOR_YELLOW);
  		psvDebugScreenPrintf("                                        .:::::::::::::::::::::.   \n");
  		psvDebugScreenPrintf("                                      .::::::::'''''''''::::::::.   \n");
  		psvDebugScreenPrintf("                                    .::::::''              ''::::::.  \n");
  		psvDebugScreenPrintf("                                   ::::::''                  ''::::::  \n");
  		psvDebugScreenPrintf("                                  ::::::'                      '::::::  \n");
  		psvDebugScreenPrintf("                                 ::::::'                        '::::::  \n");
  		psvDebugScreenPrintf("                                ::::::'                          ':::::: \n");
  		psvDebugScreenPrintf("                                ::::::                            :::::: \n");
  		psvDebugScreenPrintf("                                ::::::                            ::::::  \n");
  		psvDebugScreenPrintf("                                ::::::                            :::::: \n");
  		psvDebugScreenPrintf("                                ::::::.                          .:::::: \n");
  		psvDebugScreenPrintf("                                 ::::::.                        .:::::: \n");
  		psvDebugScreenPrintf("                                  ::::::.                      .:::::: \n");
  		psvDebugScreenPrintf("                                   ::::::..                  ..::::::  \n");
  		psvDebugScreenPrintf("                                     '::::::..            ..::::::'  \n");
  		psvDebugScreenPrintf("                                       '::::::::........::::::::'    \n");
  		psvDebugScreenPrintf("                                         '::::::::::::::::::::'   \n");
  		psvDebugScreenPrintf("                                            ''::::::::::::''      \n");
		psvDebugScreenSetFgColor(COLOR_GREEN);
		psvDebugScreenPrintf("You have saved princess April.\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenPrintf("You marry her and live happily ever after.\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenPrintf("...\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenPrintf("THE END.\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenSetFgColor(COLOR_WHITE);
	}
	else if (dead)
	{
		psvDebugScreenClear(0);
		psvDebugScreenSetFgColor(COLOR_GREY);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("                                           /'       \n");
		psvDebugScreenPrintf("                                           ||       \n");
		psvDebugScreenPrintf("                                           ||      ***       \n");
		psvDebugScreenPrintf("                                           ||      __X_        \n");
		psvDebugScreenPrintf("                                           ||     ( ___\\       \n");
		psvDebugScreenPrintf("                                           ||     |:  \\\\         \n");
		psvDebugScreenPrintf("                                          ><><  ___)..:/_#__,       \n");
		psvDebugScreenPrintf("                                          (X|) (|+(____)+\\ _)       \n");
		psvDebugScreenPrintf("                                           o|_\\/>> + + + << \\       \n");
		psvDebugScreenPrintf("                                             |:\\/|+ + + +| \\_\\<       \n");
		psvDebugScreenPrintf("                                             \\./  XXXXXX.  (o_)_       \n");
		psvDebugScreenPrintf("                                                 /+ + + |   \\:|       \n");
		psvDebugScreenPrintf("                                                /+ +/+ +|  -/->>>----.       \n");
		psvDebugScreenPrintf("                                               /+ +|+ /XX /   _--,  _ \\       \n");
		psvDebugScreenPrintf("                                              \\+ + + /  |X   (,\\- \\/_ ,       \n");
		psvDebugScreenPrintf("                                              /\\+ + /\\  |X \\    /,//_/       \n");
		psvDebugScreenPrintf("                                             +_+_+_( )o_)X  \\  (( ///       \n");
		psvDebugScreenPrintf("                                              (_o(  /__/ X   \\  \\\\//       \n");
		psvDebugScreenPrintf("                                               \\_|  |_/  X    \\ ///       \n");
		psvDebugScreenPrintf("                                               \\_| >(_/        \\,/       \n");
		psvDebugScreenPrintf("                                            ,//__o\\ /__//,      V        \n");
		psvDebugScreenSetFgColor(COLOR_RED);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("You have died on your quest.\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenPrintf("For your bravery, the king erects a statue in the town centre.\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenPrintf("You will be remembered until a better knight comes along.\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenSetFgColor(COLOR_WHITE);
	}
	else if (coward)
	{
		psvDebugScreenClear(0);
		psvDebugScreenSetFgColor(COLOR_GREY);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
     	psvDebugScreenPrintf("                                                        ._--+.                         \n");
     	psvDebugScreenPrintf("                                                     -''      '.                         \n");
     	psvDebugScreenPrintf("                                                  +:'           '.                       \n");
     	psvDebugScreenPrintf("                                                  | '.        _.-'|                      \n");
     	psvDebugScreenPrintf("                                                  |   +    .-'   J                       \n");
     	psvDebugScreenPrintf("                               _.+        .....'.'|    '.-'      |                       \n");
     	psvDebugScreenPrintf("                          _.-''   '.   ..'        |     +       J''..                     \n");
     	psvDebugScreenPrintf("                       +:'          '.'           ;-+.  |    _+.|    :                    \n");
     	psvDebugScreenPrintf("                       | \\         _.-+         _.' _.F F +:'   '.    '....                 \n");
     	psvDebugScreenPrintf("                        L \\   _.-''   |    _.-'  _.' J J J  '.    +        :                \n");
     	psvDebugScreenPrintf("                        |  '+'        |_.-'  _.-'    | | |    +    '.      '._''.              \n");
     	psvDebugScreenPrintf("                      .' L  L         J  _.-'        | | |     '.    '.  _.-'    '.             \n");
     	psvDebugScreenPrintf("                     :   |  |         J-'            F F F       '.  _.-'          '.           \n");
     	psvDebugScreenPrintf("                    :     L  L     _+  L            J J J          '|.               ';           \n");
     	psvDebugScreenPrintf("                   :      J  |  +-'  \\ L          _.+.|.+.          F '.          _.-' F         \n");
     	psvDebugScreenPrintf("                   :       |  L L\\    \\|      _.-'    '   '.       J    '.     .-'    |         \n");
     	psvDebugScreenPrintf("                   :       .L | | \\    ', _.-'              '.     |      '..-'      J'.         \n");
     	psvDebugScreenPrintf("                   :      : |  L L '.    \\     _.-+.          '.   :+-.     |        F  '.       \n");
     	psvDebugScreenPrintf("                   :      :  L | |   \\    ;.-''     '.          :-'    ':+ J        |     :       \n");
     	psvDebugScreenPrintf("                   :      :  |  L L   +:''            '.    _.-'     .-' | |       J:      :       \n");
     	psvDebugScreenPrintf("                   :      :   L | |   J \\               '.-'     _.-'   J J        F :      :     \n");
     	psvDebugScreenPrintf("                    :     :    \\ L L   L \\             _.-+  _.-'       | |       |   :      :     \n");
     	psvDebugScreenPrintf("                    :      :    \\| |   |  '.       _.-'   |-'          J J       J     :      :     \n");
     	psvDebugScreenPrintf("                    :      '.    +'\\   J    \\  _.-'       F    ,-T'\\  | |     .-'      :      :   \n");
     	psvDebugScreenPrintf("                     :       '.     \\   L    +'          J    /  | J  J J  .-'        .'      :     \n");
     	psvDebugScreenPrintf("                      :        :     \\  |    |           |    F  '.|.-'+|-'         .'        :     \n");
     	psvDebugScreenPrintf("                       :        :     \\ J    |           F   J     '...           .'          :     \n");
     	psvDebugScreenPrintf("                        :        :     \\ L   |          J    |      \\  '.''.''''.'           :     \n");
     	psvDebugScreenPrintf("                         :        :     \\|   |          |  .-'\\      \\                      :     \n");
     	psvDebugScreenPrintf("                          :        '.    J   |          F-'  .'\\      \\                   .'     \n");
     	psvDebugScreenPrintf("                           :         :    L  |         J     :  \\      \\                .'       \n");
     	psvDebugScreenPrintf("                            :         :   |  |        .+  ...'   \\      \\          .''.'        \n");
     	psvDebugScreenPrintf("                             :         :  J  |     .-'  .'        \\      \\'.'''.'.'           \n");
     	psvDebugScreenPrintf("                              :         :  \\ |  .-'   .'          .\\    _-'                  \n");
        psvDebugScreenPrintf("                               ''''''''''''''''''''''''              '''''        \n");
		psvDebugScreenSetFgColor(COLOR_RED);
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("\n");
		psvDebugScreenPrintf("You run away from your quest like a coward.\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenPrintf("As you run away, you hear the sound of the princess being eaten by the dragon.\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenPrintf("When you arrive back at the town, the King arrests you for your cowardice.\n");
		sceKernelDelayThread(2 * delay);
		psvDebugScreenPrintf("The rest of your days are spent rotting in the dungeons.\n");
		psvDebugScreenSetFgColor(COLOR_WHITE);
	}
}
