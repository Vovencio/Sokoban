#include <fxcg/keyboard.h>
int move(int pos, int index) {
	float flPos;
	flPos = pos;
	switch (index){
	case 0:
		return ((pos + 1) % 16 + ((int)(flPos / 16)) * 16);
	case 1:
		return ((pos + 112) % 128);
	case 2:
		return ((pos + 15) % 16 + ((int)(flPos / 16)) * 16);
	case 3:
		return ((pos + 16) % 128);
	default:
		break;
	}
	return pos;
}

int menuMove(int pos, int index) {
	float flPos;
	flPos = pos;
	switch (index) {
	case 0:
		return ((pos + 1) % 16 + ((int)(flPos / 16)) * 16);
	case 1:
		return ((pos + 80) % 96);
	case 2:
		return ((pos + 15) % 16 + ((int)(flPos / 16)) * 16);
	case 3:
		return ((pos + 16) % 96);
	default:
		break;
	}
	return pos;
}

int codeToIndex(int code) {
	switch (code) {
	case KEY_CTRL_RIGHT:
		return (0);
	case KEY_CTRL_UP:
		return (1);
	case KEY_CTRL_LEFT:
		return (2);
	case KEY_CTRL_DOWN:
		return (3);
	default:
		break;
	}
	return 4;
}