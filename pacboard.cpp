/*
 *  pacboard.cpp
 *  pac-cpp-xcode
 *
 *  Created by KOJIMA Satoshi on 10/08/16.
 *  Copyright 2010 KOJIMA Satoshi All rights reserved.
 *
 */

#include "pacboard.h"
#include <iostream>
#include <sstream>
#include <iterator>
#include <assert.h>

// #include <dispatch/dispatch.h> when need to implement GCD

std::ostream& operator<<(std::ostream& os, const Position& pos)
{
  os << pos.x << "," << pos.y;
  return os;
}

PacBoard::PacBoard(std::vector<std::string> &boardstring, int x, int y, int rest_time)
{
  player = NULL;
  rest_time_counter = rest_time;
  dots = 0;
  for (int i = 0; i < y; i++) {
	board_index.push_back(*(new std::vector<int>(x)));
	item_index.push_back(*(new std::vector<char>(x)));
  }

  std::vector<std::string>::iterator i = boardstring.begin();
  int lineno = 0;

  // read board string
  while (i != boardstring.end()) {
	int columnno = 0;
	std::string::iterator str_iter = i->begin();
	bool dot;
	bool high_priority_dot;
	while (str_iter != i->end()) {
	  high_priority_dot = false;
	  dot = false;
	  item_index[lineno][columnno] = *str_iter;
	  if (*str_iter == '#' || *str_iter == ' ')  {
		board_index[lineno][columnno] = -1;
	  } else {
		if (*str_iter == '.') {
		  dot = true;
		  dots ++;
		} else if (*str_iter == '*') {
		  dot = true;
		  dots += Cell::high_priority_dot_score; // high priority dot.
		  high_priority_dot = true;
		} else {
		  read_player_or_enemy(*str_iter, columnno, lineno);
		}
		cells.push_back(new Cell(dot, columnno, lineno, high_priority_dot));
		board_index[lineno][columnno] = (int)cells.size() - 1;
	  }
	  str_iter ++;
	  columnno ++;
	}
	i++;
	lineno++;
  }

  // check adjacent cells
  std::vector<Cell *>::iterator itr = cells.begin();
  while (itr != cells.end()) {
	Cell *cell = *itr;
	int index;
	index = index_of_cell_at(cell->pos.to_dir(Position::up));
	if (index >= 0)  cell->adjacent_cells_index.push_back(index);
	index = index_of_cell_at(cell->pos.to_dir(Position::down));
	if (index >= 0)  cell->adjacent_cells_index.push_back(index);
	index = index_of_cell_at(cell->pos.to_dir(Position::left));
	if (index >= 0)  cell->adjacent_cells_index.push_back(index);
	index = index_of_cell_at(cell->pos.to_dir(Position::right));
	if (index >= 0)  cell->adjacent_cells_index.push_back(index);
	itr ++;
  }
  entered_dir = Position::other;
}

PacBoard::PacBoard(const PacBoard &another) {
  for (int i = 0; i < another.board_index.size(); i ++) {
	board_index.push_back(another.board_index[i]);
  }

  for (int i = 0; i < another.item_index.size(); i ++) {
	item_index.push_back(another.item_index[i]);
  }

  for (int i = 0; i < another.cells.size(); i ++) {
	Cell *cell = new Cell(*another.cells[i]);
	cell->the_board = this;
	cells.push_back(cell);
  }

  entered_dir = entered_dir;
  parent = another.parent;
  rest_time_counter = another.rest_time_counter;

  for (int i = 0; i < another.enemies.size(); i ++) {
	Enemy *e = another.enemies[i]->clone();
	e->the_board = this;
	enemies.push_back(e);
  }
  player = new Player(*another.player);
  player->the_board = this;
  dots = another.dots;
  
}

PacBoard::~PacBoard()
{
  std::vector<Cell *>::iterator cells_iter = cells.begin();
  while (cells_iter != cells.end()) {
	delete *cells_iter;
	cells_iter ++;
  }

  std::vector<PacBoard *>::iterator children_iter = children.begin();
  while (children_iter != children.end()) {
	delete *children_iter;
	children_iter ++;
  }
  
  std::vector<Enemy *>::iterator enemies_iter = enemies.begin();
  while (enemies_iter != enemies.end()) {
	delete *enemies_iter;
	enemies_iter ++;
  }
  delete player;
}

void PacBoard::read_player_or_enemy(char c, int x, int y)
{
  Enemy *e = NULL;
  switch (c) {
  case '@':
	player = new Player(Position(x,y), this);
	break;
  case 'V':
	e = new EnemyV(Position(x,y), this);
	break;
  case 'H':
	e = new EnemyH(Position(x,y), this);
	break;
  case 'L':
	e = new EnemyL(Position(x,y), this);
	break;
  case 'R':
	e = new EnemyR(Position(x,y), this);
	break;
  case 'J':
	e = new EnemyJ(Position(x,y), this);
	break;
  case ' ':
	break;
  default:
	assert(false);
  }
  if (e) {
	enemies.push_back(e);
  }
}

Cell *PacBoard::cell_at(Position pos) const {
  return cell_at(pos.x, pos.y);

}
 

int PacBoard::index_of_cell_at(Position pos) const
{
  return board_index[pos.y][pos.x];
}
 
Cell *PacBoard::cell_at(int x, int y) const {
  int index = board_index[y][x];
  if (index >= 0) {
	return cells[board_index[y][x]];
  } else {
	return NULL;
  }
}

Cell *PacBoard::cell_adjacent_to(const Cell &cell, int index)
{
  return cells[cell.adjacent_cells_index[index]];
}

void PacBoard::move_enemies() 
{
  std::vector<Enemy *>::iterator ei = enemies.begin();
  while (ei != enemies.end()) {
	(*ei)->move();
	ei ++;
  }
}

void PacBoard::display_to(std::ostream &os) {
  os << "remaining dots: " << dots ;
  os << " player position: " << player->current_pos << std::endl;
  std::vector< std::vector<char> >::iterator i = item_index.begin();
  while (i != item_index.end()) {
	std::vector<char>::iterator j = i->begin();
	while (j != i->end()) {
	  os << *j;
	  j++;
	}
	os << std::endl;
	i++;
  }
  
}

std::string PacBoard::path_string() const {
  std::string path;
  const PacBoard *board = this;
  while (board->parent != NULL) {
	switch (board->entered_dir) {
	case Position::same:
	  path = "." + path;
	  break;
	case Position::up:
	  path = "k" + path;
	  break;
	case Position::down:
	  path = "j" + path;
	  break;
	case Position::left:
	  path = "h" + path;
	  break;
	case Position::right:
	  path = "l" + path;
	  break;
	default:
	  assert(false);
	}
	board = board->parent;
  }
  return path;
}

void PacBoard::display_all_history_to(std::ostream &os) {
  PacBoard *board = this;
  while (board->parent != NULL) {
	board->display_to(os);
	board = board->parent;
  }
}

Item::Item(Position initial_pos, class PacBoard *board, char type) {
  current_pos = initial_pos;
  this->the_board = board;
  this->type = type;
}

bool Item::can_move_to(Position next) {
  return (the_board->cell_at(next) != NULL);
}

bool Item::can_move_to(Position::Dir dir) {
  return can_move_to(current_pos.to_dir(dir));
}

bool Item::move_to(Position next)
{
  if (the_board->cell_at(next) != NULL) {
	previous_pos = current_pos;
	current_pos = next;
	bool prev_dot = the_board->cell_at(previous_pos)->dot;
	the_board->item_index[previous_pos.y][previous_pos.x] = prev_dot? '.' : ' ';
	the_board->item_index[current_pos.y][current_pos.x] = type;
	return true;
  }
  return false;
}

bool Item::move_to(Position::Dir dir)
{
  return move_to(current_pos.to_dir(dir));
}


bool Player::move_to(Position::Dir dir)
{
  return move_to(current_pos.to_dir(dir));
}


bool Player::move_to(Position next) {
  if (Item::move_to(next)) {
	Cell *c = the_board->cell_at(next);
	if (c->dot) {
	  if (c->high_priority_dot) {
		the_board->dots = the_board->dots - 5;
	  } else {
		the_board->dots --;
	  }
	  c->dot = false;
	  c->high_priority_dot = false;	  
	}
	the_board->entered_dir = current_pos.is_adjacent_from(previous_pos);
	return true;
  }
  assert(false);
  return false;
}

Position Enemy::will_move() {
  if (previous_pos.x == -1 && previous_pos.y == -1) {
	if (can_move_to(Position::down))  return current_pos.to_dir(Position::down);
	if (can_move_to(Position::left)) return current_pos.to_dir(Position::left);
	if (can_move_to(Position::up)) return current_pos.to_dir(Position::up);
	if (can_move_to(Position::right)) return current_pos.to_dir(Position::right);
  }

  Cell *cell = the_board->cell_at(current_pos);
  // 行き止まり
  if (cell->adjacent_cells_index.size() == 1) {
	return the_board->cell_adjacent_to(*cell, 0)->pos;
  }

  // 通路... ほんとにこの動き? 
  if (cell->adjacent_cells_index.size() == 2) {
	Position next;
	for (int i = 0; i < 2; i++) {
	  next = the_board->cell_adjacent_to(*cell, i)->pos;
	  if (!(next == previous_pos)) break;
	}
	assert(next.x != -1 && next.y != -1);
	return next;
  }
  return determine_next_pos();
}

void Enemy::move() {
  Position next = will_move();
  move_to(next);
	if (type == 'H') {
	the_board->h = current_pos;
  }
	if (type == 'V') {
	the_board->v = current_pos;
  }
}

Position EnemyV::determine_next_pos() {
  Position player_pos = the_board->player->current_pos;
  if (current_pos.y < player_pos.y) {
	if (can_move_to(Position::down)) {
	  return current_pos.to_dir(Position::down);
	}
  }
  if (current_pos.y > player_pos.y) {
	if (can_move_to(Position::up)) {
	  return current_pos.to_dir(Position::up);
	}
  }
  if (current_pos.x < player_pos.x) {
	if (can_move_to(Position::right)) {
	  return current_pos.to_dir(Position::right);
	}
  }
  
  if (current_pos.x > player_pos.x) {
	if (can_move_to(Position::left)) {
	  return current_pos.to_dir(Position::left);
	}
  }

  if (can_move_to(Position::down))  return current_pos.to_dir(Position::down);
  if (can_move_to(Position::left)) return current_pos.to_dir(Position::left);
  if (can_move_to(Position::up)) return current_pos.to_dir(Position::up);
  if (can_move_to(Position::right)) return current_pos.to_dir(Position::right);
  return Position();

}

Position EnemyH::determine_next_pos() {
  Position player_pos = the_board->player->current_pos;

  if (current_pos.x < player_pos.x) {
	if (can_move_to(Position::right)) {
	  return current_pos.to_dir(Position::right);
	}
  }
  
  if (current_pos.x > player_pos.x) {
	if (can_move_to(Position::left)) {
	  return current_pos.to_dir(Position::left);
	}
  }

  if (current_pos.y < player_pos.y) {
	if (can_move_to(Position::down)) {
	  return current_pos.to_dir(Position::down);
	}
  }
  if (current_pos.y > player_pos.y) {
	if (can_move_to(Position::up)) {
	  return current_pos.to_dir(Position::up);
	}
  }

  if (can_move_to(Position::down))  return current_pos.to_dir(Position::down);
  if (can_move_to(Position::left)) return current_pos.to_dir(Position::left);
  if (can_move_to(Position::up)) return current_pos.to_dir(Position::up);
  if (can_move_to(Position::right)) return current_pos.to_dir(Position::right);
  return Position();
}

Position EnemyLR::determine_next_pos_l() {
  Position::Dir dir = current_pos.is_adjacent_from(previous_pos);
  Position::Dir to;

  to = Position::relative_direction(dir, Position::left);
  if (can_move_to(to))  return current_pos.to_dir(to);
  to = Position::relative_direction(dir, Position::up);
  if (can_move_to(to))  return current_pos.to_dir(to);
  to = Position::relative_direction(dir, Position::right);
  if (can_move_to(to))  return current_pos.to_dir(to);
  return Position();
}

Position EnemyLR::determine_next_pos_r() {
  Position::Dir dir = current_pos.is_adjacent_from(previous_pos);
  Position::Dir to;
	
  to = Position::relative_direction(dir, Position::right);
  if (can_move_to(to))  return current_pos.to_dir(to);
  to = Position::relative_direction(dir, Position::up);
  if (can_move_to(to))  return current_pos.to_dir(to);
  to = Position::relative_direction(dir, Position::left);
  if (can_move_to(to))  return current_pos.to_dir(to);
  return Position();
}
