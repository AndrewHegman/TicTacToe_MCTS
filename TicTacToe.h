#pragma once

#include <string>
#include <vector>

class TicTacToe {
protected:
	typedef std::vector<std::vector<int>> game_board;
	void set_board(game_board board);

	static int rows;
	static int cols;

private:
	game_board board;
	std::vector<int> pos2tuple(int pos);


public:
	TicTacToe(int _rows=3, int _cols=3);

	void print();
	int win();
	int place(int pos, const int player);
	int place(std::vector<int> v_pos, int player);
	game_board get_board();
};

void print_board(std::vector<std::vector<int>>);
int count_player(std::vector<std::vector<int>> board, int player);