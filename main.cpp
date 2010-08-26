#include <iostream>
#include <iterator>
#include <sstream>

#include "pacboard.h"
#include "pacsolver.h"

int main (int argc, char * const argv[]) {

  int count;
  int width, height;

  int threshold;
  std::string go_back_policy_string;
  bool go_back_policy;

  std::cin >> go_back_policy_string;
  if (go_back_policy_string == "go_back") {
	go_back_policy = true;
  } else {
	go_back_policy = false;
  }

  std::cin >>  threshold;
  std::cin >> count;
  std::cin >> width;
  std::cin >> height;

  std::vector<std::string> boardstring;
  std::string line;
  while (getline(std::cin, line)) {
	if (line.length() > 0) boardstring.push_back(line);
  }

  PacBoard *board = new PacBoard(boardstring, width, height, count);
  PacSolver solver(board, go_back_policy, threshold, threshold);
  PacSolver::Result result = PacSolver::Solving;
  while (result == PacSolver::Solving) {
	std::cout << "dots(best):" << solver.remaining_dots;
	std::cout << " leaves:" << solver.this_generation.size() ;
	if (solver.this_generation.size() > 0) {
	  std::cout << " rest_time: " << solver.this_generation[0]->rest_time_counter;
	}
	std::cout << std::endl << std::flush;
	result = solver.process();
  }

  std::cout << "solved?:" << result << std::endl;
  if (result == PacSolver::Solved) {
	solver.end_board->display_all_history_to(std::cout);
	std::cout << solver.end_board->path_string() << std::endl;
	std::cout << solver.end_board->dots << std::endl;
  } else {
	solver.highest_score_now->display_to(std::cout);
  }
  return 0;
}
