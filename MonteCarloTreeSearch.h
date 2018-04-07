#pragma once

#include "TicTacToe.h"
#include <unordered_map>
#include <map>

const bool verbose_output = false;
#define DEBUG_PRINT(Str, ...)			if(verbose_output) printf(Str, ##__VA_ARGS__)

class MonteCarloTreeSearch: public TicTacToe {
private:
	struct Node {
		Node(bool debug=true) {
			if(debug)	printf("\n\n--WARNING--\nDEFAULT CONSTRUCTOR CALLED\n\n");
			for (int i = 0; i < rows; i++) {
				state.push_back(std::vector<int>(cols, 0));
			}
			n = n;
			w = 0;
			uct = 0.0f;
			terminal = false;
			player = 0;
			fully_explored = false;
		}

		Node(TicTacToe::game_board state, int player, bool terminal=false, int n=0, int w=0) {
			this->state = state;
			this->n = n;
			this->w = 0;
			uct = 0.0f;
			this->terminal = terminal;
			this->player = player;
			fully_explored = false;
		}


		TicTacToe::game_board state;
		int n;
		int w;
		int* np = NULL;	// This should be linked to the parent node's n value
		float uct;
		bool terminal;
		int player;
		bool fully_explored;
	};

	enum SimType {
		random=1,
		add,
		others,
		later
	};

	TicTacToe::game_board selection(MonteCarloTreeSearch::Node root);
	TicTacToe::game_board expansion(MonteCarloTreeSearch::Node& node, int player_number);
	bool simulation(MonteCarloTreeSearch::Node& node, int player_number);
	void update();

	int next_player(TicTacToe::game_board board, int player1 = 1, int player2 = 2);
	TicTacToe::game_board get_child(MonteCarloTreeSearch::Node node, bool greedy, bool debug=false);
	TicTacToe::game_board get_child(TicTacToe::game_board parent, bool greedy, bool debug = false);
	void get_nodes(TicTacToe::game_board root);
	bool is_visited(TicTacToe::game_board node);
	float get_uct(MonteCarloTreeSearch::Node node, bool greedy, bool debug=false);

	std::vector<TicTacToe::game_board> tmp_visited_nodes;
	std::map<TicTacToe::game_board, Node> visited_nodes;

	int breadth;
	int player;
	int opponent;
	float lose_reward;
	float win_reward;
	float c;	// Parameter for UCT
	SimType opponent_simulation;

public:
	MonteCarloTreeSearch(int player, int opponent, int breadth=3);
	void play(TicTacToe::game_board root);
	std::vector<int> get_move(TicTacToe::game_board board, bool debug=false);
	std::vector<int> compare_boards(TicTacToe::game_board first, TicTacToe::game_board second);
};