#include "MonteCarloTreeSearch.h"
#include <time.h>

MonteCarloTreeSearch::MonteCarloTreeSearch(int player, int opponent, int breadth) {
	this->breadth = breadth;
	this->player = player;
	this->opponent = opponent;
	srand(unsigned int(time(NULL)));
	opponent_simulation = random;
	c = 4.0f;//sqrtf(2.0f);
}

void MonteCarloTreeSearch::play(TicTacToe::game_board root) {
	printf("Hmmmm, I've seen this before.\n");
	visited_nodes.clear();

	// Make sure that root node has been added to list of visited nodes (root = very first node--empty board)
	if (!is_visited(root)) {
		visited_nodes.insert(std::pair<TicTacToe::game_board, MonteCarloTreeSearch::Node>(
			root, MonteCarloTreeSearch::Node{ root, player, win() >= 0 })
		);
	}
	
	for (int iters = 0; iters < 10000; iters++) {
		// Ensure that we are at root node
		set_board(root);

		//Clear visited nodes from previous run
		tmp_visited_nodes.clear();

		// Step 1: Selection
		DEBUG_PRINT("Child: \n");
		TicTacToe::game_board child = selection(visited_nodes[root]);
		if(verbose_output) print_board(child);

		// Step 2: Expansion
		DEBUG_PRINT("\nExpansion: \n");
		child = expansion(visited_nodes[child], next_player(get_board(), opponent, player));
		if(verbose_output) print_board(child);

		// Step 3: Simulation
		DEBUG_PRINT("\nSimulation: \n");
		if(!visited_nodes[tmp_visited_nodes[tmp_visited_nodes.size() - 1]].terminal)
			while (!simulation(visited_nodes[tmp_visited_nodes[tmp_visited_nodes.size() - 1]], next_player(tmp_visited_nodes[tmp_visited_nodes.size() - 1], opponent, player)));

		if(verbose_output) print_board(child);

		// Step 4: Update
		DEBUG_PRINT("\nUpdate: \n");
		update();
		for (int i = 0; i < tmp_visited_nodes.size(); i++) {
			if(verbose_output) print_board(tmp_visited_nodes[i]);
			DEBUG_PRINT("\tVisited: %d\n", visited_nodes[tmp_visited_nodes[i]].n);
			DEBUG_PRINT("\tWins: %d\n", visited_nodes[tmp_visited_nodes[i]].w);
			DEBUG_PRINT("\tPlayer: %d\n", visited_nodes[tmp_visited_nodes[i]].player);
			DEBUG_PRINT("\n\n");
		}
		DEBUG_PRINT("\n******************************************************\n");
	}
}

TicTacToe::game_board MonteCarloTreeSearch::selection(MonteCarloTreeSearch::Node root) {
	if (root.terminal || win() > -1) {
		return root.state;
	}
	tmp_visited_nodes.push_back(root.state);
	TicTacToe::game_board child = get_child(root, false);
	MonteCarloTreeSearch::Node parent(false);

	if (child == root.state)	return child;

	do {
		tmp_visited_nodes.push_back(child);
		if (is_visited(child)){
			parent = visited_nodes[child];	// They grow up so fast *sniff*
			child = get_child(parent, false);
		}
	} while (child != parent.state);

	return child;
}

TicTacToe::game_board MonteCarloTreeSearch::expansion(MonteCarloTreeSearch::Node& node, int player_number) {
	// Play in a RANDOM place. Keep trying until successful. Ensure that that place has never been played before
	int success = 0;
	set_board(node.state);

	// If node is NOT fully explored, we need to ENSURE that an un-visited node is expanded to
	/*
	if (!node.fully_explored) {
		do {
			set_board(node.state);
			success = place({ rand() % rows, rand() % cols }, player_number);
		} while ((success != 0 || is_visited(get_board())) && win() == -1);
	}
	else {
	*/
		do {
			set_board(node.state);
			success = place({ rand() % rows, rand() % cols }, player_number);
		} while ((success != 0) && win() == -1);
	//}


	tmp_visited_nodes.push_back(get_board());	// Add the current board to visited nodes of this run

	// If node hasn't been visited before, add it to the list
	if (!is_visited(tmp_visited_nodes[tmp_visited_nodes.size() - 1])) {

		visited_nodes.insert(std::pair<TicTacToe::game_board, MonteCarloTreeSearch::Node>(
			tmp_visited_nodes[tmp_visited_nodes.size() - 1], MonteCarloTreeSearch::Node{ tmp_visited_nodes[tmp_visited_nodes.size() - 1], player_number, win() >= 0})
		);

		// Point the newly created node's np to the parent's n
		visited_nodes[tmp_visited_nodes[tmp_visited_nodes.size() - 1]].np = &node.n;
	}

	return tmp_visited_nodes[tmp_visited_nodes.size() - 1];
}

bool MonteCarloTreeSearch::simulation(MonteCarloTreeSearch::Node& node, int player_number) {
	MonteCarloTreeSearch::Node parent = node;
	set_board(node.state);

	if (win() >= 0) {
		printf("Why am I here...\n");
	}

	switch (this->opponent_simulation) {
	case random:
		while ((place({ rand() % rows, rand() % cols }, player_number) < 0));	// Play in a RANDOM place. Keep trying until successful
		
		//compare_boards(node.state, get_board());
		tmp_visited_nodes.push_back(get_board());

		// If node hasn't been visited before, add it to the list
		if (!is_visited(tmp_visited_nodes[tmp_visited_nodes.size() - 1])) {
				
			visited_nodes.insert(std::pair<TicTacToe::game_board, MonteCarloTreeSearch::Node>(
				tmp_visited_nodes[tmp_visited_nodes.size() - 1], MonteCarloTreeSearch::Node{ tmp_visited_nodes[tmp_visited_nodes.size() - 1], player_number, win() >= 0 })
			);

			// Point the newly created node's np to the parent's n
			visited_nodes[tmp_visited_nodes[tmp_visited_nodes.size() - 1]].np = &node.n;
		}
		break;

	default:
		while ((place({ rand() % rows, rand() % cols }, player_number) < 0));
		tmp_visited_nodes.push_back(get_board());

		// If node hasn't been visited before, add it to the list
		if (!is_visited(tmp_visited_nodes[tmp_visited_nodes.size() - 1])) {

			visited_nodes.insert(std::pair<TicTacToe::game_board, MonteCarloTreeSearch::Node>(
				tmp_visited_nodes[tmp_visited_nodes.size() - 1], MonteCarloTreeSearch::Node{ tmp_visited_nodes[tmp_visited_nodes.size() - 1], player_number, win() >= 0 })
			);

			// Point the newly created node's np to the parent's n
			visited_nodes[tmp_visited_nodes[tmp_visited_nodes.size() - 1]].np = &node.n;
		}
		break;
	}

	return visited_nodes[tmp_visited_nodes[tmp_visited_nodes.size() - 1]].terminal;
}

void MonteCarloTreeSearch::update() {
	// If this node has been visited before, then we don't need to create it
	for (int i = 0; i < tmp_visited_nodes.size(); i++) {
		if (!is_visited(tmp_visited_nodes[i])) {
			visited_nodes.insert(std::pair<TicTacToe::game_board, MonteCarloTreeSearch::Node>(tmp_visited_nodes[i], MonteCarloTreeSearch::Node{ tmp_visited_nodes[tmp_visited_nodes.size() - 1], player, win() >= 0 }));
			printf("Having to add another node..?\n");
		}

		// Update statistics for this node
		visited_nodes[tmp_visited_nodes[i]].n++;	// Increment visit count
		visited_nodes[tmp_visited_nodes[i]].w += ((win() == player));	// Only increment win counter if the specified player won
		if (win() > -1)	visited_nodes[tmp_visited_nodes[i]].fully_explored = true;
	}
}

TicTacToe::game_board MonteCarloTreeSearch::get_child(TicTacToe::game_board parent, bool greedy, bool debug) {
	if (!is_visited(parent)) {
		printf("Uhhh...Houston, we have a problem...\n");
	}
	else {
		return get_child(visited_nodes[parent], greedy, debug);
	}
}

TicTacToe::game_board MonteCarloTreeSearch::get_child(MonteCarloTreeSearch::Node parent, bool greedy, bool debug) {
	// Ensure board is at current state
	set_board(parent.state);

	std::vector<float> ucb1_vec;
	std::vector<TicTacToe::game_board> board_vec;

	// If the board is a TIE, just return parent, no child nodes can be found
	if (win() > 0)	return parent.state;

	// Run UCB1 to get child
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			if (place({ r, c }, next_player(get_board(), opponent, player)) == 0) {	// If successful
				//if(win() < 0) {
					board_vec.push_back(get_board());
					if (is_visited(get_board())) {	// If the board has been visited
						ucb1_vec.push_back(get_uct(visited_nodes[get_board()], greedy, debug));
						if (debug) {
							print_board(board_vec[board_vec.size() - 1]);
							printf("UCB1: %f\n", ucb1_vec[ucb1_vec.size() - 1]);
							printf("\n");
						}
					}
					else {
						ucb1_vec.push_back(INFINITY);
					}
				//}
				set_board(parent.state);
			}
		}
	}
	for (int i = 0; i < ucb1_vec.size(); i++) {
		DEBUG_PRINT("%f\n", ucb1_vec[i]);
	}

	// Find best UCB1 value
	float b = -INFINITY;
	int idx = 0;
	bool all_inf = true;
	for (int i = 0; i < ucb1_vec.size(); i++) {
		if (ucb1_vec[i] > b) {
			b = ucb1_vec[i];
			idx = i;

		}
		if (ucb1_vec[i] != INFINITY) all_inf = false;
	}

	// If best was not INFINITY, then all nodes have been visited
	if (b != INFINITY)	
		parent.fully_explored = true;

	// If the best node is still INFINITY, we need to go to EXPANSION!
	if (b == INFINITY)	return parent.state;
	else if (ucb1_vec.size() == 0)	return parent.state;
	else	return board_vec[idx];
}

// Returns TRUE if node has been visited
bool MonteCarloTreeSearch::is_visited(TicTacToe::game_board node) {
	if (visited_nodes.find(node) == visited_nodes.end()) return false;
	return true;
}

float MonteCarloTreeSearch::get_uct(MonteCarloTreeSearch::Node node, bool greedy, bool debug) {
	if (node.np == NULL) {
		printf("");
	}
	if (debug) {
		printf("Wins: %d\n", node.w);
		printf("Visited: %d\n", node.n);
		printf("Parent visited: %d\n", *node.np);
		printf("Win rate: %f\n", float(node.w) / float(node.n));
	}
	if (greedy) return (float(node.w) / float(node.n));
	else return (float(node.w) / float(node.n)) + c*sqrtf(log(float(*node.np)) / float(node.n));
}

std::vector<int> MonteCarloTreeSearch::get_move(TicTacToe::game_board board, bool debug) {
	if (is_visited(board)) {
		Node tmp = visited_nodes[board];
		printf("Ah! I know where to play..\n");
		return compare_boards(board, get_child(board, true));
	}
	else {
		printf("Never seen this board before!!\n");
		return{ 0, 0 };
	}
}

std::vector<int> MonteCarloTreeSearch::compare_boards(TicTacToe::game_board first, TicTacToe::game_board second) {
	
	for (int i = 0; i < first.size(); i++) {
		for (int j = 0; j < first[0].size(); j++) {
			if (first[i][j] != second[i][j])	return{ i, j };
		}
	}
	printf("--WARNING--\nNo Differences found between the boards!!\n");
	printf("\n************\n");
	printf("FIRST");
	printf("\n************\n");
	print_board(first);
	printf("\n");
	printf("\n************\n");
	printf("SECOND");
	printf("\n************\n");
	print_board(second);
	printf("\n");
	return{ 0, 0 };
}

int MonteCarloTreeSearch::next_player(TicTacToe::game_board board, int player1, int player2) {
	if (count_player(board, player1) > count_player(board, player2)) return player2;
	else return player1;
}