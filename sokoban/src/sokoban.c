#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include "render.h"
#include "moves.h"
#include "levels.h"

int checkWin(int infoMap[]) {
	int loop;
	for (loop = 0; loop < 128; loop++) {
		if (infoMap[loop] == 2) {
			return (0);
		}
	}
	return (1);
}

void playGame(const int level[], const int stPos, const int ID) {
	int infoMap[128];
	int backMap[2064];
	int backMoves = 0;
	int pos;
	int lastMove = 0;
	int moves = 0;
	int loopMoves = 0;
	int key;

	pos = stPos;

	int loop;
	for (loop = 0; loop < 128; loop++) {
		infoMap[loop] = level[loop];
		backMap[loop] = level[loop];
	}
	backMap[128] = stPos;

	RenderInnit();
	Render(infoMap, pos, lastMove, moves);

	for (;;) {
		GetKey(&key);

		// Movement
		int newPos;
		int newBoxPos;
		newPos = move(pos, codeToIndex(key));
		if (codeToIndex(key) != 4) {
			lastMove = codeToIndex(key);
			switch (infoMap[newPos]) {
			case 0:
				pos = newPos;
				moves += 1;

				loopMoves = moves % 16;
				for (loop = 129 * loopMoves; loop < 129 * (loopMoves)+128; loop += 1) {
					backMap[loop] = infoMap[loop % 129];
				}
				backMap[128 + loopMoves * 129] = pos;
				if (backMoves < 15) {
					backMoves += 1;
				}
				break;
			case 1:
				pos = newPos;
				moves += 1;
				loopMoves = moves % 16;
				for (loop = 129 * loopMoves; loop < 129 * (loopMoves)+128; loop += 1) {
					backMap[loop] = infoMap[loop % 129];
				}
				backMap[128 + loopMoves * 129] = pos;
				if (backMoves < 15) {
					backMoves += 1;
				}
				break;
			case 2:
				newBoxPos = move(newPos, codeToIndex(key));
				switch (infoMap[newBoxPos])
				{
				case 0:
					pos = newPos;
					infoMap[newPos] = 1;
					infoMap[newBoxPos] = 2;
					moves += 1;
					loopMoves = moves % 16;
					for (loop = 129 * loopMoves; loop < 129 * (loopMoves)+128; loop += 1) {
						backMap[loop] = infoMap[loop % 129];
					}
					backMap[128 + loopMoves * 129] = pos;
					if (backMoves < 15) {
						backMoves += 1;
					}
					break;
				case 1:
					pos = newPos;
					infoMap[newPos] = 1;
					infoMap[newBoxPos] = 2;
					moves += 1;
					loopMoves = moves % 16;
					for (loop = 129 * loopMoves; loop < 129 * (loopMoves)+128; loop += 1) {
						backMap[loop] = infoMap[loop % 129];
					}
					backMap[128 + loopMoves * 129] = pos;
					if (backMoves < 15) {
						backMoves += 1;
					}
					break;
				case 3:
					pos = newPos;
					infoMap[newPos] = 1;
					infoMap[newBoxPos] = 4;
					moves += 1;
					loopMoves = moves % 16;
					for (loop = 129 * loopMoves; loop < 129 * (loopMoves)+128; loop += 1) {
						backMap[loop] = infoMap[loop % 129];
					}
					backMap[128 + loopMoves * 129] = pos;
					if (backMoves < 15) {
						backMoves += 1;
					}
					break;
				}
				break;
			case 3:
				pos = newPos;
				moves += 1;
				loopMoves = moves % 16;
				for (loop = 129 * loopMoves; loop < 129 * (loopMoves)+128; loop += 1) {
					backMap[loop] = infoMap[loop % 129];
				}
				backMap[128 + loopMoves * 129] = pos;
				if (backMoves < 15) {
					backMoves += 1;
				}
				break;
			case 4:
				newBoxPos = move(newPos, codeToIndex(key));
				switch (infoMap[newBoxPos])
				{
				case 0:
					pos = newPos;
					infoMap[newPos] = 3;
					infoMap[newBoxPos] = 2;
					moves += 1;
					loopMoves = moves % 16;
					for (loop = 129 * loopMoves; loop < 129 * (loopMoves)+128; loop += 1) {
						backMap[loop] = infoMap[loop % 129];
					}
					backMap[128 + loopMoves * 129] = pos;
					if (backMoves < 15) {
						backMoves += 1;
					}
					break;
				case 1:
					pos = newPos;
					infoMap[newPos] = 3;
					infoMap[newBoxPos] = 2;
					moves += 1;
					loopMoves = moves % 16;
					for (loop = 129 * loopMoves; loop < 129 * (loopMoves)+128; loop += 1) {
						backMap[loop] = infoMap[loop % 129];
					}
					backMap[128 + loopMoves * 129] = pos;
					if (backMoves < 15) {
						backMoves += 1;
					}
					break;
				case 3:
					pos = newPos;
					infoMap[newPos] = 3;
					infoMap[newBoxPos] = 4;
					moves += 1;
					loopMoves = moves % 16;
					for (loop = 129 * loopMoves; loop < 129 * (loopMoves)+128; loop += 1) {
						backMap[loop] = infoMap[loop % 129];
					}
					backMap[128 + loopMoves * 129] = pos;
					if (backMoves < 15) {
						backMoves += 1;
					}
					break;
				}
				break;
			default:
				break;
			}
		}
		else
		{
			if (key == KEY_CTRL_F6) {
				pos = stPos;
				backMoves = 0;
				moves = 0;
				int loop;
				for (loop = 0; loop < 2064; loop++) {
					backMap[loop] = 0;
				}
				for (loop = 0; loop < 128; loop++) {
					infoMap[loop] = level[loop];
					backMap[loop] = level[loop];
				}
				backMap[128] = pos;
				loopMoves = 0;
				Render(infoMap, pos, lastMove, moves);
			}
			else if (key == KEY_CTRL_F1) {
				if (backMoves > 0) {
					lastMove = 3;
					backMoves -= 1;
					moves -= 1;
					loopMoves = moves % 16;
					int loop;
					for (loop = 129 * loopMoves; loop < 129 * (loopMoves)+128; loop += 1) {
						infoMap[loop % 129] = backMap[loop];
					}
					pos = backMap[128 + loopMoves * 129];
					Render(infoMap, pos, lastMove, moves);
				}
			}
			else if (key == KEY_CTRL_F3)
			{
				renderHelp();
				GetKey(&key);
			}
			else if (key == KEY_CTRL_EXIT) {
				break;
			}
		}



		// Rendering
		if (checkWin(infoMap) == 0) {
			Render(infoMap, pos, lastMove, moves);
		}
		else
		{
			Render(infoMap, pos, lastMove, moves);
			int answ;
			answ = Win(moves, lGoalMoves[ID]);

			if (answ == 1) {
				pos = stPos;
				backMoves = 0;
				moves = 0;
				int loop;
				for (loop = 0; loop < 2064; loop++) {
					backMap[loop] = 0;
				}
				for (loop = 0; loop < 128; loop++) {
					infoMap[loop] = level[loop];
					backMap[loop] = level[loop];
				}
				backMap[128] = pos;
				loopMoves = 0;
				Render(infoMap, pos, lastMove, moves);
			}
			else
			{
				break;
			}
		}
	}
}

int Menu() {
	int selected = 0;
	int key;
	for (;;)
	{
		renderMenu(selected, lDiffucult, lGoalMoves[selected]);
		GetKey(&key);
		if (codeToIndex(key) != 4)
		{
			selected = menuMove(selected, codeToIndex(key));
		}
		else if (key == KEY_CTRL_F1)
		{
			return (selected);
		}
		else if (key == KEY_CTRL_F3)
		{
			renderHelp();
			GetKey(&key);
			return (96);
		}
	}
	return (0);
}

int main(void) {
	RenderInnit();
	Intro();
	int ret;
	for (;;) {
		ret = Menu();
		switch (ret)
		{
		case 0:
			playGame(level0, level0PlayerPos, 0);
			break;
		case 1:
			playGame(level1, level1PlayerPos, 1);
			break;
		case 2:
			playGame(level2, level2PlayerPos, 2);
			break;
		case 3:
			playGame(level3, level3PlayerPos, 3);
			break;
		case 4:
			playGame(level4, level4PlayerPos, 4);
			break;
		case 5:
			playGame(level5, level5PlayerPos, 5);
			break;
		case 6:
			playGame(level6, level6PlayerPos, 6);
			break;
		case 7:
			playGame(level7, level7PlayerPos, 7);
			break;
		case 8:
			playGame(level8, level8PlayerPos, 8);
			break;
		default:
			break;
		}
	}
}