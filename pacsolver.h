/*
 *  pacsolver.h
 *  pac-cpp-xcode
 *
 *  Created by KOJIMA Satoshi on 10/08/20.
 *  Copyright 2010 KOJIMA Satoshi All rights reserved.
 *
 */

#include "pacboard.h"
#include <deque>
#include <iterator>

class PacSolver {
 public:

  enum Result {
	Solved,
	Solving,
	Failed
  };

  PacBoard *root_board;
  PacBoard *end_board;
  int remaining_dots;
  PacBoard *highest_score_now;
  std::deque<PacBoard *> this_generation;
  std::deque<PacBoard *> next_generation;
  bool go_back;
  

  int threshold_leaves;
  int trunc_leaves_to;
  
  PacSolver(PacBoard *board, bool go_back = true, int threshold_leaves = 1500, int trunc_leaves_to = 1500) {
	this->remaining_dots = board->dots;
	this->root_board = board;
	this->go_back = go_back;
	this_generation.push_back(board);
	this->threshold_leaves = threshold_leaves;
	this->trunc_leaves_to = trunc_leaves_to;
	highest_score_now = NULL;
  }

  virtual ~PacSolver() {};
  Result process();
  static std::vector<Position> next_move(PacBoard *board) ;

  virtual std::vector<Position> trunc_next_pos(const PacBoard *theboard, const std::vector<Position> &next_pos) ;
};

class Comparator {
 public:
  Comparator() {};

  static int min_distance_to_dot(const PacBoard *board) {
	Player *p = board->player;
	Cell *c = board->cell_at(p->current_pos);
	int d = 0;
	std::deque<Cell *> cell_to_see;
	std::vector<Cell *> cell_already_saw;
	cell_to_see.push_back(c);
	cell_already_saw.push_back(c);

	while (cell_to_see.size() > 0) {
	  c = cell_to_see.front();
	  cell_to_see.pop_front();
	  if (c->dot) return d;
	  d ++;
	  Cell *next;
	  for (int i = 0; i < c->adjacent_cells_index.size(); i++) {
		next = board->cells[c->adjacent_cells_index[i]];
		if (std::find(cell_already_saw.begin(),cell_already_saw.end(), next) == cell_already_saw.end()) {
		  cell_to_see.push_back(next);
		  cell_already_saw.push_back(next);
		}
	  }
	}
	return 0;
  }

  static int more_far_distance_to_hv(const PacBoard *board) {
	Position h = board->h;
	Position v = board->v;
	bool found_near = false;
	if (h == v) found_near = true;

	Player *p = board->player;
	Cell *c = board->cell_at(p->current_pos);
	int d = 0;
	std::deque<Cell *> cell_to_see;
	std::vector<Cell *> cell_already_saw;
	cell_to_see.push_back(c);
	cell_already_saw.push_back(c);

	while (cell_to_see.size() > 0) {
	  c = cell_to_see.front();
	  cell_to_see.pop_front();
	  if (c->pos == h || c->pos == v) {
		if (found_near)
		  return d;
		found_near = true;
	  }
	  d ++;
	  Cell *next;
	  for (int i = 0; i < c->adjacent_cells_index.size(); i++) {
		next = board->cells[c->adjacent_cells_index[i]];
		if (std::find(cell_already_saw.begin(),cell_already_saw.end(), next) == cell_already_saw.end()) {
		  cell_to_see.push_back(next);
		  cell_already_saw.push_back(next);
		}
	  }
	}
	return 0;

  }
  
  bool operator ()(const PacBoard *a, const PacBoard *b) {
	if (a->dots == b->dots) {
	  // 
	}
	return a->dots < b->dots;
  }
};

