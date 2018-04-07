#include "TicTacToe.h"
#include "MonteCarloTreeSearch.h"
#include <iostream>
#include <time.h>

//TODO: 
// I think exploration and exploitation isn't working correctly

int main(void) {

	TicTacToe tic;
	MonteCarloTreeSearch mcts(2, 1);

	int pos;
	
	while (tic.win() < 0) {
		tic.print();
		std::cout << "Where do you want to play: ";
		std::cin >> pos;
		if (tic.place(pos, 1) < 0) {
			printf("Moron...\n");
		}
		if (tic.win() > -1) break;
		mcts.play(tic.get_board());
		tic.place(mcts.get_move(tic.get_board(), false), 2);
	}
	printf("Player %d won!\n", tic.win());
	print_board(tic.get_board());
	system("PAUSE");
}