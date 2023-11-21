#include "sprites.h"
#include <fxcg/rtc.h>
#include <fxcg/misc.h>
#include <string.h>

#define GRAY 0x3186
#define MENUCOLOR 0x0000
#define WHITE 0xffff

const int introLevel[128] = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 4, 4, 4, 2, 4, 2, 4, 2, 4, 4, 4, 4, 4, 4, 4, 2, 4, 2, 4, 2, 4, 2, 4, 2, 4, 2, 4, 2, 2, 2, 4, 2, 4, 2, 4, 2, 4, 4, 2, 2, 4, 2, 4, 2, 2, 2, 4, 2, 4, 2, 4, 2, 4, 2, 4, 2, 4, 2, 4, 4, 4, 4, 4, 2, 4, 4, 4, 2, 4, 2, 4, 2, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };

int maxOut(int value, int max) {
	if (value >= max)
		return max;
	return value;
}

int key_down(int basic_keycode)
{
	const unsigned short* keyboard_register = (unsigned short*)0xA44B0000;
	int row, col, word, bit;
	row = basic_keycode % 10;
	col = basic_keycode / 10 - 1;
	word = row >> 1;
	bit = col + ((row & 1) << 3);
	return (0 != (keyboard_register[word] & 1 << bit));
}

void Intro() {
	int end = RTC_GetTicks() + 460;
	int tick = 0;

	while (end > RTC_GetTicks())
	{
		if (key_down(29)){
			return;
		}
		if (RTC_GetTicks() % 6 == 0) {
			Bdisp_Fill_VRAM(MENUCOLOR, 1);

			int x;
			int y;
			const int minY = 32;

			int boxPoses[128];

			int i;

			for (y = 0; y < 8; y = y + 1) {
				for (x = 0; x < 16; x = x + 1) {
					boxPoses[x + y * 16] = maxOut(-200 - 64 * (7 - y) - 32 * x + (int)(16.6 * tick), 0);
				}
			}

			for (y = 0; y < 8; y = y + 1) {
				for (x = 0; x < 16; x = x + 1) {
					switch (introLevel[x + y * 16]) {
					case 2:
						VRAM_CopySprite(sBox, 24 * x, minY + 24 * y - 8 + boxPoses[x + y * 16], 24, 32);
						break;
					case 4:
						VRAM_CopySprite(sSolved, 24 * x, minY + 24 * y - 8 + boxPoses[x + y * 16], 24, 32);
						break;
					default:
						break;
					}
				}
			}
			DisplayStatusArea();

			x = 270;
			y = 200;
			PrintMini(&x, &y, "F6 Skip", 0x40, 0xFFFFFFFF, 0, 0, COLOR_WHITE, COLOR_BLACK, 1, 0);

			Bdisp_PutDisp_DD();
			tick += 1;
		}
	}
}

void renderHelp() {
	Bdisp_Fill_VRAM(MENUCOLOR, 1);
	DisplayStatusArea();
	PrintXY(1, 1, "xxJoystick - Move", TEXT_MODE_INVERT, TEXT_COLOR_BLACK);
	PrintXY(1, 2, "xxF1 - Play, Undo", TEXT_MODE_INVERT, TEXT_COLOR_BLACK);
	PrintXY(1, 3, "xxF3 - Help", TEXT_MODE_INVERT, TEXT_COLOR_BLACK);
	PrintXY(1, 4, "xxF6 – Restart Level", TEXT_MODE_INVERT, TEXT_COLOR_BLACK);
	PrintXY(1, 5, "xxExit - Menu", TEXT_MODE_INVERT, TEXT_COLOR_BLACK);
	PrintXY(1, 6, "xxGameplay questions:", TEXT_MODE_INVERT, TEXT_COLOR_BLACK);
	PrintXY(1, 7, "xxceme.tech/DL885", TEXT_MODE_INVERT, TEXT_COLOR_BLACK);
	PrintXY(1, 8, "xxUse any key to quit", TEXT_MODE_INVERT, TEXT_COLOR_BLACK);
}

int makePos(int pos, int i) {
	if (pos != (i - i % 16)) {
		return (i - i % 16);
	}
	return pos;
}

void renderMenu(int selected, const int diff[], int goal) {
	Bdisp_Fill_VRAM(MENUCOLOR, 1);
	const int minX = 0;
	const int minY = 32;

	int x;
	int y;
	int xx;
	int xy;
	char numberText[] = "####";

	for (y = 0; y < 6; y = y + 1) {
		for (x = 0; x < 16; x = x + 1) {
			if (x + y * 16 > 9)
				xx = x * 24;
			else
				xx = x * 24 + 5;
			xy = 24 * y + 4;

			itoa(x + y * 16, numberText);
			if (x + y * 16 != selected) {
				VRAM_CopySprite(sBox, minX + 24 * x, minY + 24 * y - 8, 24, 32);
				switch (diff[x + y * 16])
				{
				case 0:
					PrintMini(&xx, &xy, numberText, 0x02, 0xFFFFFFFF, 0, 0, COLOR_LIME, COLOR_LIME, 1, 0);
					break;
				case 1:
					PrintMini(&xx, &xy, numberText, 0x02, 0xFFFFFFFF, 0, 0, COLOR_WHITE, COLOR_WHITE, 1, 0);
					break;
				case 2:
					PrintMini(&xx, &xy, numberText, 0x02, 0xFFFFFFFF, 0, 0, COLOR_YELLOW, COLOR_YELLOW, 1, 0);
					break;
				case 3:
					PrintMini(&xx, &xy, numberText, 0x02, 0xFFFFFFFF, 0, 0, COLOR_ORANGE, COLOR_ORANGE, 1, 0);
					break;
				default:
					PrintMini(&xx, &xy, numberText, 0x02, 0xFFFFFFFF, 0, 0, COLOR_RED, COLOR_RED, 1, 0);
					break;
				}
			}
			else
			{
				VRAM_CopySprite(sSolved, minX + 24 * x, minY + 24 * y - 8, 24, 32);
				PrintMini(&xx, &xy, numberText, 0x02, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_BLACK, 1, 0);
			}
		}
	}

	// Text and stuff
	DisplayStatusArea();
	
	x = 375;
	y = 3;
	int dx;
	dx = 0;
	char text[] = "Menu";
	PrintMini(&dx, &y, text, 0x40, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 0, 0);
	x -= dx;
	PrintMini(&x, &y, text, 0x40, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);

	char buffmoves[] = "xxLevel: ##";
	itoa(selected, buffmoves + 9);

	char goalMoves[] = "xxMoves: ###";
	itoa(goal, goalMoves + 8);
	PrintXY(11, 7, goalMoves, TEXT_MODE_INVERT, TEXT_COLOR_BLACK);
	switch (diff[selected])
	{
	case 0:
		PrintXY(1, 8, "xxEasy", TEXT_MODE_INVERT, TEXT_COLOR_BLACK);
		break;
	case 1:
		PrintXY(1, 8, "xxNormal", TEXT_MODE_INVERT, TEXT_COLOR_BLACK);
		break;
	case 2:
		PrintXY(1, 8, "xxHard", TEXT_MODE_INVERT, TEXT_COLOR_BLACK);
		break;
	case 3:
		PrintXY(1, 8, "xxExpert", TEXT_MODE_INVERT, TEXT_COLOR_BLACK);
		break;
	default:
		PrintXY(1, 8, "xxUnknown", TEXT_MODE_INVERT, TEXT_COLOR_BLACK);
		break;
	}

	PrintXY(1, 7, buffmoves, TEXT_MODE_INVERT, TEXT_COLOR_BLACK);
	x = 240;
	y = 200;
	PrintMini(&x, &y, "F3 for Help", 0x40, 0xFFFFFFFF, 0, 0, COLOR_WHITE, COLOR_BLACK, 1, 0);
}

int Win(int totMoves, int goalMoves) {
	// Text and stuff
	DisplayStatusArea();

	char buffmoves[] = "Moves: ######";
	char bestmoves[] = "xxPerfect Moves:XXX";
	itoa(totMoves, buffmoves + 7);
	itoa(goalMoves, bestmoves + 16);

	int x;
	int y;
	x = 375;
	y = 3;
	int dx;
	dx = 0;

	PrintMini(&dx, &y, buffmoves, 0x40, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 0, 0);
	x -= dx;
	PrintMini(&x, &y, buffmoves, 0x40, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);

	MsgBoxPush(5);
	PrintXY(3, 2, "xxYou win!", TEXT_MODE_NORMAL, TEXT_COLOR_BLACK);
	PrintXY(3, 3, bestmoves, TEXT_MODE_NORMAL, TEXT_COLOR_BLACK);
	PrintXY(3, 4, "xxPlay Again?", TEXT_MODE_NORMAL, TEXT_COLOR_BLACK);
	PrintXY_2(TEXT_MODE_NORMAL, 1, 5, 3, TEXT_COLOR_BLACK);
	PrintXY_2(TEXT_MODE_NORMAL, 1, 6, 4, TEXT_COLOR_BLACK);
	int key;
	int vissza;
	vissza = 2;
	while (vissza == 2){
		GetKey(&key);
		switch (key) {
		case KEY_CTRL_F1:
			vissza = 1;
			break;
		case KEY_CTRL_F6:
			vissza = 0;
			break;
		default:
			break;
		}
	}
	MsgBoxPop();
	return (vissza);
}

void RenderInnit() {
	Bdisp_EnableColor(1);
	DefineStatusAreaFlags(DSA_SETDEFAULT, 0, 0, 0);
	DefineStatusAreaFlags(3, SAF_BATTERY | SAF_TEXT | SAF_GLYPH | SAF_ALPHA_SHIFT, 0, 0);
	DefineStatusMessage("Sokoban by Vovencio", 1, 0, 0);
}

void Render(int inMap[], int inPos, int inMove, int totMoves) {
	Bdisp_Fill_VRAM(GRAY, 1);

	int x;
	int y;

	const int minX = 0;
	const int minY = 24;

	// Render Floor
	for (y = 0; y < 8; y = y + 1) {
		for (x = 0; x < 16; x = x + 1) {
			switch (inMap[x + y * 16]){
			case 1:
				VRAM_CopySprite(sFloor, minX + 24 * x, minY + 24 * y, 24, 24);
				break;
			case 3:
				VRAM_CopySprite(sPlate, minX + 24 * x, minY + 24 * y, 24, 24);
				break;

			default:
				break;
			}
		}
	}

	// Render the Player
	if (inMap[inPos] == 3) {
		switch (inMove){
		case 0:
			VRAM_CopySprite(sPPlayerRight, minX + 24 * (inPos % 16), minY + 24 * ((inPos - (inPos % 16)) / 16), 24, 24);
			break;
		case 1:
			VRAM_CopySprite(sPPlayerUp, minX + 24 * (inPos % 16), minY + 24 * ((inPos - (inPos % 16)) / 16), 24, 24);
			break;
		case 2:
			VRAM_CopySprite(sPPlayerLeft, minX + 24 * (inPos % 16), minY + 24 * ((inPos - (inPos % 16)) / 16), 24, 24);
			break;
		case 3:
			VRAM_CopySprite(sPPlayerDown, minX + 24 * (inPos % 16), minY + 24 * ((inPos - (inPos % 16)) / 16), 24, 24);
			break;
		default:
			VRAM_CopySprite(sPPlayerRight, minX + 24 * (inPos % 16), minY + 24 * ((inPos - (inPos % 16)) / 16), 24, 24);
			break;
		}
	}
	else
	{
		switch (inMove) {
		case 0:
			VRAM_CopySprite(sPlayerRight, minX + 24 * (inPos % 16), minY + 24 * ((inPos - (inPos % 16)) / 16), 24, 24);
			break;
		case 1:
			VRAM_CopySprite(sPlayerUp, minX + 24 * (inPos % 16), minY + 24 * ((inPos - (inPos % 16)) / 16), 24, 24);
			break;
		case 2:
			VRAM_CopySprite(sPlayerLeft, minX + 24 * (inPos % 16), minY + 24 * ((inPos - (inPos % 16)) / 16), 24, 24);
			break;
		case 3:
			VRAM_CopySprite(sPlayerDown, minX + 24 * (inPos % 16), minY + 24 * ((inPos - (inPos % 16)) / 16), 24, 24);
			break;
		default:
			VRAM_CopySprite(sPlayerRight, minX + 24 * (inPos % 16), minY + 24 * ((inPos - (inPos % 16)) / 16), 24, 24);
			break;
		}
	}

	// Render the Other Stuff
	for (y = 0; y < 8; y = y + 1) {
		for (x = 0; x < 16; x = x + 1) {
			switch (inMap[x + y * 16]) {
			case 2:
				VRAM_CopySprite(sBox, minX + 24 * x, minY + 24 * y - 8, 24, 32);
				break;
			case 4:
				VRAM_CopySprite(sSolved, minX + 24 * x, minY + 24 * y - 8, 24, 32);
				break;
			case 5:
				VRAM_CopySprite(sWall, minX + 24 * x, minY + 24 * y - 8, 24, 32);
				break;
			default:
				break;
			}
		}
	}

	// Text and stuff
	DisplayStatusArea();

	char buffmoves[] = "Moves: ######";
	itoa(totMoves, buffmoves + 7);

	x = 375;
	y = 3;
	int dx;
	dx = 0;

	PrintMini(&dx, &y, buffmoves, 0x40, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 0, 0);
	x -= dx;
	PrintMini(&x, &y, buffmoves, 0x40, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);


	x = 270;
	y = 200;
	PrintMini(&x, &y, "F3 for Help", 0x40, 0xFFFFFFFF, 0, 0, COLOR_WHITE, COLOR_BLACK, 1, 0);

}