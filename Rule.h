#pragma once
#include<iostream>
#include<stdio.h>
#include<string>
#include<math.h>

enum piece { wrook = -1, brook = 1, wknight = -2, bknight = 2, wbishop = -3, bbishop = 3, wqueen = -4, bqueen = 4, wking = -5, bking = 5, wpawn = -6, bpawn = 6 };

// update broad after a move
void updateBoard(std::string str, int** board) {
	int x1 = str[0] - 97;
	int y1 = str[1] - 49;
	int x2 = str[2] - 97;
	int y2 = str[3] - 49;
	board[y2][x2] = board[y1][x1];
	board[y1][x1] = 0;
}

//check a move
int check(std::string str, int** board, int turn,int color) {
	if (turn != color) return 0;
	int x1 = str[0] - 97;
	int y1 = str[1] - 49;
	int x2 = str[2] - 97;
	int y2 = str[3] - 49;

	if (turn*board[y1][x1] <= 0) return 0;
	if (x1 == x2&&y1 == y2) return 0;
	if (x1 < 0 || x1>7 || y1 < 0 || y1>7 || x2 < 0 || x2>7 || y2 < 0 || y2>7) return 0;

	switch (board[y1][x1])
	{
	case wrook:
		if (x1 == x2) {
			if (y2 - y1 > 0) {
				for (int i = 1; i < y2 - y1; i++) {
					if (board[y1 + i][x1] != 0) return 0;
				}
			}
			else if (y1 - y2 > 0) {
				for (int i = 1; i < y1 - y2; i++) {
					if (board[y1 - i][x1] != 0) return 0;
				}
			}
		}
		else if (y1 == y2) {
			if (x2 - x1 > 0) {
				for (int i = 1; i < x2 - x1; i++) {
					if (board[y1][x1 + i] != 0) return 0;
				}
			}
			else if (x1 - x2 > 0) {
				for (int i = 1; i < x1 - x2; i++) {
					if (board[y1][x1 - i] != 0) return 0;
				}
			}
		}
		else return 0;
		if (board[y2][x2] * board[y1][x1]>0) return 0;
		else return 1;
		break;
	case brook:
		if (x1 == x2) {
			if (y2 - y1 > 0) {
				for (int i = 1; i < y2 - y1; i++) {
					if (board[y1 + i][x1] != 0) return 0;
				}
			}
			else if (y1 - y2 > 0) {
				for (int i = 1; i < y1 - y2; i++) {
					if (board[y1 - i][x1] != 0) return 0;
				}
			}
		}
		else if (y1 == y2) {
			if (x2 - x1 > 0) {
				for (int i = 1; i < x2 - x1; i++) {
					if (board[y1][x1 + i] != 0) return 0;
				}
			}
			else if (x1 - x2 > 0) {
				for (int i = 1; i < x1 - x2; i++) {
					if (board[y1][x1 - i] != 0) return 0;
				}
			}
		}
		else return 0;
		if (board[y2][x2] * board[y1][x1]>0) return 0;
		else return 1;
		break;
	case wknight:
		if (abs(y1 - y2) == 1 && abs(x1 - x2) != 2) return 0;
		else if (abs(y1 - y2) == 2 && abs(x1 - x2) != 1) return 0;
		else if (abs(y1 - y2) != 1 && abs(y1 - y2) != 2) return 0;
		if (board[y2][x2] * board[y1][x1] > 0) return 0;
		else return 1;
		break;
	case bknight:
		if (abs(y1 - y2) == 1 && abs(x1 - x2) != 2) return 0;
		else if (abs(y1 - y2) == 2 && abs(x1 - x2) != 1) return 0;
		else if (abs(y1 - y2) != 1 && abs(y1 - y2) != 2) return 0;
		if (board[y2][x2] * board[y1][x1] > 0) return 0;
		else return 1;
		break;
	case wbishop:
		if (abs(y1 - y2) != abs(x1 - x2)) return 0;
		if (y1 - y2 == x1 - x2) {
			if (y1 - y2 > 0) {
				for (int i = 1; i < y1 - y2; i++) {
					if (board[y1 - i][x1 - i] != 0) return 0;
				}
			}
			else if (y2 - y1 > 0) {
				for (int i = 1; i < y2 - y1; i++) {
					if (board[y1 + i][x1 + i] != 0) return 0;
				}
			}
		}
		else if (y1 - y2 == x2 - x1) {
			if (y1 - y2 > 0) {
				for (int i = 1; i < y1 - y2; i++) {
					if (board[y1 - i][x1 + i] != 0) return 0;
				}
			}
			else if (y2 - y1 > 0) {
				for (int i = 1; i < y2 - y1; i++) {
					if (board[y1 + i][x1 - i] != 0) return 0;
				}
			}
		}
		if (board[y2][x2] * board[y1][x1]>0) return 0;
		else return 1;
		break;
	case bbishop:
		if (abs(y1 - y2) != abs(x1 - x2)) return 0;
		if (y1 - y2 == x1 - x2) {
			if (y1 - y2 > 0) {
				for (int i = 1; i < y1 - y2; i++) {
					if (board[y1 - i][x1 - i] != 0) return 0;
				}
			}
			else if (y2 - y1 > 0) {
				for (int i = 1; i < y2 - y1; i++) {
					if (board[y1 + i][x1 + i] != 0) return 0;
				}
			}
		}
		else if (y1 - y2 == x2 - x1) {
			if (y1 - y2 > 0) {
				for (int i = 1; i < y1 - y2; i++) {
					if (board[y1 - i][x1 + i] != 0) return 0;
				}
			}
			else if (y2 - y1 > 0) {
				for (int i = 1; i < y2 - y1; i++) {
					if (board[y1 + i][x1 - i] != 0) return 0;
				}
			}
		}
		if (board[y2][x2] * board[y1][x1]>0) return 0;
		else return 1;
		break;
	case wqueen:
		if (x1 == x2) {
			if (y2 - y1 > 0) {
				for (int i = 1; i < y2 - y1; i++) {
					if (board[y1 + i][x1] != 0) return 0;
				}
			}
			else if (y1 - y2 > 0) {
				for (int i = 1; i < y1 - y2; i++) {
					if (board[y1 - i][x1] != 0) return 0;
				}
			}
		}
		else if (y1 == y2) {
			if (x2 - x1 > 0) {
				for (int i = 1; i < x2 - x1; i++) {
					if (board[y1][x1 + i] != 0) return 0;
				}
			}
			else if (x1 - x2 > 0) {
				for (int i = 1; i < x1 - x2; i++) {
					if (board[y1][x1 - i] != 0) return 0;
				}
			}
		}
		else if (y1 - y2 == x1 - x2) {
			if (y1 - y2 > 0) {
				for (int i = 1; i < y1 - y2; i++) {
					if (board[y1 - i][x1 - i] != 0) return 0;
				}
			}
			else if (y2 - y1 > 0) {
				for (int i = 1; i < y2 - y1; i++) {
					if (board[y1 + i][x1 + i] != 0) return 0;
				}
			}
		}
		else if (y1 - y2 == x2 - x1) {
			if (y1 - y2 > 0) {
				for (int i = 1; i < y1 - y2; i++) {
					if (board[y1 - i][x1 + i] != 0) return 0;
				}
			}
			else if (y2 - y1 > 0) {
				for (int i = 1; i < y2 - y1; i++) {
					if (board[y1 + i][x1 - i] != 0) return 0;
				}
			}
		}
		else return 0;
		if (board[y2][x2] * board[y1][x1]>0) return 0;
		else return 1;
		break;
	case bqueen:
		if (x1 == x2) {
			if (y2 - y1 > 0) {
				for (int i = 1; i < y2 - y1; i++) {
					if (board[y1 + i][x1] != 0) return 0;
				}
			}
			else if (y1 - y2 > 0) {
				for (int i = 1; i < y1 - y2; i++) {
					if (board[y1 - i][x1] != 0) return 0;
				}
			}
		}
		else if (y1 == y2) {
			if (x2 - x1 > 0) {
				for (int i = 1; i < x2 - x1; i++) {
					if (board[y1][x1 + i] != 0) return 0;
				}
			}
			else if (x1 - x2 > 0) {
				for (int i = 1; i < x1 - x2; i++) {
					if (board[y1][x1 - i] != 0) return 0;
				}
			}
		}
		else if (y1 - y2 == x1 - x2) {
			if (y1 - y2 > 0) {
				for (int i = 1; i < y1 - y2; i++) {
					if (board[y1 - i][x1 - i] != 0) return 0;
				}
			}
			else if (y2 - y1 > 0) {
				for (int i = 1; i < y2 - y1; i++) {
					if (board[y1 + i][x1 + i] != 0) return 0;
				}
			}
		}
		else if (y1 - y2 == x2 - x1) {
			if (y1 - y2 > 0) {
				for (int i = 1; i < y1 - y2; i++) {
					if (board[y1 - i][x1 + i] != 0) return 0;
				}
			}
			else if (y2 - y1 > 0) {
				for (int i = 1; i < y2 - y1; i++) {
					if (board[y1 + i][x1 - i] != 0) return 0;
				}
			}
		}
		else return 0;
		if (board[y2][x2] * board[y1][x1]>0) return 0;
		else return 1;
		break;
	case wking:
		if ((abs(x1 - x2) > 1) || abs(y1 - y2) > 1) return 0;
		if (board[y2][x2] * board[y1][x1] > 0) return 0;
		else return 1;
		break;
	case bking:
		if ((abs(x1 - x2) > 1) || abs(y1 - y2) > 1) return 0;
		if (board[y2][x2] * board[y1][x1] > 0) return 0;
		else return 1;
		break;
	case wpawn:
		if (y1 == 1) {
			if (y2 == 3 && x1 == x2&&board[2][x1] == 0 && board[3][x1] == 0) return 1;
		}
		if (y2 - y1 == 1) {
			if (abs(x1 - x2) == 1 && board[y2][x2] > 0) return 1;
			if (x1 - x2 == 0 && board[y2][x2] == 0) return 1;
			return 0;
		}
		return 0;
		break;
	case bpawn:
		if (y1 == 6) {
			if (y2 == 4 && x1 == x2&&board[5][x1] == 0 && board[4][x1] == 0) return 1;
		}
		if (y1 - y2 == 1) {
			if (abs(x1 - x2) == 1 && board[y2][x2] < 0) return 1;
			if (x1 - x2 == 0 && board[y2][x2] == 0) return 1;
			return 0;
		}
		return 0;
		break;
	default:
		break;
	}

	return 0;
}
//print board to the console
void printBoard(int** board) {
	std::cout << std::endl;
	for (int i = 7; i >= 0; i--) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j] >= 0)
				printf(" %d ", board[i][j]);
			else printf("%d ", board[i][j]);
		}
		printf("\n");
	}
}

//check end game
int checkEndGame(int **board) {
	int b = 0, w = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j] == -5) w = 1;
			if (board[i][j] == 5) b = 1;
		}
	}
	if (w == 0) return 1;
	if (b == 0) return -1;
	return 0;
}