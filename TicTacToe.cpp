#include "TicTacToe.h"

int TicTacToe::rows;
int TicTacToe::cols;

TicTacToe::TicTacToe(int _rows, int _cols) {
	for (int i = 0; i < _rows; i++) {
		board.push_back(std::vector<int>(_cols, 0));
	}

	rows = _rows;
	cols = _cols;
}

void TicTacToe::print() {
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			if(c == 0) printf("%s | ", (board[r][c] == 0) ? " " : (board[r][c] == 1 ? "X" : "O"));
			else if (c == cols - 1) printf(" %s", (board[r][c] == 0) ? " " : (board[r][c] == 1 ? "X" : "O"));
			else printf("%s |", (board[r][c] == 0) ? " " : (board[r][c] == 1 ? "X" : "O"));
		}
		if(r < rows - 1) printf("\n--+---+--\n");
	}
	printf("\n");
}

// Returns -1 if no player has won
// Returns 0 if tie game
// Returns player number if a player has won
int TicTacToe::win() {
	// Vertical
	bool win = true;
	for (int c = 0; c < this->cols; c++) {
		if (board[0][c] != 0) {
			for (int r = 1; r < this->rows; r++) {
				if (board[r][c] != board[0][c]) {
					win = false;
				}
			}
			if (win) return board[0][c];
		}
		win = true;
	}


	// Horizontal
	win = true;
	for (int r = 0; r < this->rows; r++) {
		if (board[r][0] != 0) {
			for (int c = 1; c < this->cols; c++) {
				if (board[r][c] != board[r][0]) {
					win = false;
				}
			}
			if (win) return board[r][0];
		}
		win = true;
	}

	// Diagonal (Top Left to Bottom Right)
	win = true;
	if (board[0][0] != 0) {
		for (int i = 1; i < 3; i++) {
			if (board[i][i] != board[0][0]) {
				win = false;
				break;
			}
		}
		if (win) return board[0][0];
	}

	// Diagonal (Top Right to Bottom Left)
	win = true;
	if (board[0][cols - 1] != 0) {
		for (int i = 1; i < 3; i++) {
			if (board[i][(cols - 1) - i] != board[0][cols - 1]) {
				win = false;
				break;
			}
		}
		if (win) return board[0][cols - 1];
	}

	// Tie
	win = true;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (board[i][j] == 0) {
				win = false;
				break;
			}
		}
	}
	if (win) return 0;	// Tie game
	return -1;	// No player has won and its not a tie game
}

// Returns -1 on failure, 0 on success
int TicTacToe::place(int pos, int player) {
	std::vector<int> v_pos = pos2tuple(pos);
	return place(v_pos, player);
}

// Returns -1 or -2 on failure, 0 on success
int TicTacToe::place(std::vector<int> v_pos, const int player) {
	if ((v_pos[0] < 0 || v_pos[0] > rows) ||
		(v_pos[1] < 0 || v_pos[0] > cols))	return -2;	//Invalid position

	if (board[v_pos[0]][v_pos[1]] == 0) {
		board[v_pos[0]][v_pos[1]] = player;
		return 0;
	}
	return -1;	// Requested position is not empty
}

std::vector<int> TicTacToe::pos2tuple(int pos) {
	if (pos >= rows * cols || pos < 0) return{ -1, -1 };
	return { int(pos / rows), pos % cols };
}

TicTacToe::game_board TicTacToe::get_board() {
	return board;
}

void TicTacToe::set_board(TicTacToe::game_board board) {
	this->board = board;
}

void print_board(std::vector<std::vector<int>> board) {
	for (int r = 0; r < board.size(); r++) {
		for (int c = 0; c < board[0].size(); c++) {
			if (c == 0) printf("%s | ", (board[r][c] == 0) ? " " : (board[r][c] == 1 ? "X" : "O"));
			else if (c == board[0].size() - 1) printf(" %s", (board[r][c] == 0) ? " " : (board[r][c] == 1 ? "X" : "O"));
			else printf("%s |", (board[r][c] == 0) ? " " : (board[r][c] == 1 ? "X" : "O"));
		}
		if (r < board.size() - 1) printf("\n--+---+--\n");
	}
	printf("\n");
}

int count_player(std::vector<std::vector<int>> board, int player) {
	int c = 0;
	for (int i = 0; i < board.size(); i++) {
		c += std::count(board[i].begin(), board[i].end(), player);
	}
	return c;
}