/*
 *  pacsolver.cpp
 *  pac-cpp-xcode
 *
 *  Created by KOJIMA Satoshi on 10/08/20.
 *  Copyright 2010 KOJIMA Satoshi All rights reserved.
 *
 */

#include "pacsolver.h"
#include <iostream>
#include <iterator>

PacSolver::Result PacSolver::process()
{
  std::deque<PacBoard *>::iterator itr = this_generation.begin();
  std::vector<Position> next_pos;

  if (this_generation.empty()) {
	std::cout << "all generation extinct." << std::endl;
	return PacSolver::Failed;
  }

  if (this_generation[0]->rest_time_counter == 0) {
	std::cout << "all generation timed up." << std::endl;
	return PacSolver::Failed;	
  }
  
  while (itr != this_generation.end()) {
	next_pos = trunc_next_pos(*itr, next_move(*itr));

	if (next_pos.empty()) {
	  PacBoard *branch_top = (*itr)->find_and_cut_last_branch();
	  delete branch_top;
	}
	
	std::vector<Position>::iterator pi = next_pos.begin();
	while (pi != next_pos.end()) {
	  PacBoard *child = (*itr)->create_and_register_child(*pi);
	  if (child->dots == 0) {
		end_board = child;
		return PacSolver::Solved;
	  }
	  if (child->dots < remaining_dots) {
		remaining_dots = child->dots;
		delete highest_score_now;
		highest_score_now = new PacBoard(*child);
	  }
	  next_generation.push_back(child);
	  pi ++;
	}
	itr ++;
  }

  if (next_generation.size() > threshold_leaves) {
	std::sort(next_generation.begin(), next_generation.end(), Comparator());
	for (int i = trunc_leaves_to; i < next_generation.size(); i ++) {
	  PacBoard *b = next_generation[i]->find_and_cut_last_branch();
	  delete b;
	}
	next_generation.resize(trunc_leaves_to);
  }
  this_generation = next_generation;
  next_generation.clear();
  return PacSolver::Solving;
}



std::vector<Position> PacSolver::next_move(PacBoard *board) {
  PacBoard *child_seed = board->create_child_seed();
  Player *p = child_seed->player;
  Cell *p_cell = child_seed->cell_at(p->current_pos);
  std::vector<Position> next_pos_c;

  // collect adjacent cells
  std::vector<int>::iterator iter = p_cell->adjacent_cells_index.begin();
  while (iter != p_cell->adjacent_cells_index.end()) {
	Cell *cell = child_seed->cells[*iter];
	Position *pos = &(cell->pos);
	next_pos_c.push_back(*pos);
	iter ++;
  }
  next_pos_c.push_back(p->current_pos);

  std::vector<Position> ng_pos;

  // check enemy's position
  std::vector<Enemy *>::iterator ei = child_seed->enemies.begin();
  Position::Dir relative_dir;
  while (ei != child_seed->enemies.end()) {
	relative_dir = (*ei)->current_pos.is_adjacent_from(p->current_pos);
	if (relative_dir != Position::other) {
	  ng_pos.push_back((*ei)->current_pos);
	  if (relative_dir == Position::same) {
		ng_pos.push_back((*ei)->previous_pos);
	  } else {
		Position mp =(*ei)->will_move();
		if ((*ei)->will_move() == p->current_pos) {
		  ng_pos.push_back(p->current_pos);
		}
	  }
	}
	ei++;
  }

  std::vector<Position>::iterator c_i = next_pos_c.begin();
  std::vector<Position> move_to_list;
  while (c_i != next_pos_c.end()) {
	if (std::find(ng_pos.begin(), ng_pos.end(), *c_i) == ng_pos.end()) {
	  move_to_list.push_back(*c_i);
	}
	c_i ++;
  }
  delete child_seed;
  return move_to_list;
}

std::vector<Position> PacSolver::trunc_next_pos(const PacBoard *theboard, const std::vector<Position> &next_pos) {
  std::vector<Position> new_next_pos = next_pos;

  if (!go_back) {
	Player *p = theboard->player;
	if (new_next_pos.size() > 1) {
	  new_next_pos.erase(std::remove(new_next_pos.begin(), new_next_pos.end(), p->previous_pos), new_next_pos.end());
	}
	if (new_next_pos.size() > 1) {
	  new_next_pos.erase(std::remove(new_next_pos.begin(), new_next_pos.end(), p->current_pos), new_next_pos.end());
	}
  }
  return new_next_pos;
}
