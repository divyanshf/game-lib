#ifndef TICBOARD_H
#define TICBOARD_H

#include <cstdlib>
#include <time.h>
#include <tuple>
#include <map>

typedef std::tuple<int, int> Cord;

class TicBoard
{
private:
	const char* board[3][3];
	int boardCount;
	const char* winner;
	int turn;
	const char* player1, * player2;
	Cord findMove();
	std::map<const char*, int> scoreMap;
	int minimax(int depth, bool isMaximizing);
public:
	TicBoard(const char* p1, const char* p2);
	int aiMove();
	int humanMove(int row, int col);
	int getTurn() { return turn; }
	const char* getResult();
	const char* getBoardChar(int row, int col) const { return board[row][col]; }
	const char* getPlayer1() { return player1; }
	const char* getPlayer2() { return player2; }
};


#endif // !TICBOARD_H