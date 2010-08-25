/*
 *  pacboard.h
 *  pac-cpp-xcode
 *
 *  Created by KOJIMA Satoshi on 10/08/16.
 *  Copyright 2010 KOJIMA Satoshi All rights reserved.
 *
 */

#ifndef PACBOARD_H
#define PACBOARD_H

#include <vector>
#include <string>
#include <map>


class Position {
public:
  enum Dir {up, right, down, left,  same,  other};	
  int x, y;

 Position():x(-1),y(-1) { }
 Position(int x, int y) :x(x),y(y) {}

  Position(const Position &another) {
	x = another.x;
	y = another.y;
  }

  Dir is_adjacent_from(const Position &another) {
	if (x == another.x) {
	  if (y == another.y) return same;
	  if (y == another.y - 1) return up;
	  if (y == another.y + 1) return down;
	  return other;
	}
	if (y == another.y) {
	  if ( x == another.x - 1) return left;
	  if ( x == another.x + 1) return right;
	}
	return other;
  }

  bool operator==(const Position &another) const {
	return x == another.x && y == another.y;
  }

  bool operator!=(const Position &another) const {
	return !(*this == another);
  }
  
  Position to_dir(Dir dir) {
	switch (dir) {
	case up:
	  return Position(x, y - 1);
	case down:
	  return Position(x, y + 1 );
	case left:
	  return Position(x - 1, y);
	case right:
	  return Position(x + 1, y);
	case same:
	  return Position(x,y);
	}
	return Position();
  }

 static Dir relative_direction(Dir direction, Dir relative) {
	return (Dir)(((int)relative + (int)direction) % 4);
 }
 
};

std::ostream& operator<<(std::ostream& os, const Position& pos);

class Item {
public:
  Position current_pos;
  Position previous_pos;
  class PacBoard *the_board;
  char type;

  Item(Position initial_pos, class PacBoard *board, char type);

  virtual ~Item() {}

  bool can_move_to(Position::Dir dir);
  bool can_move_to(Position pos);  

  virtual bool move_to(Position::Dir dir) ;	
  virtual bool move_to(Position pos) ;	

 protected:
  Item(const Item &another) {
	type = another.type;
	current_pos = another.current_pos;
	  previous_pos = another.previous_pos;
	the_board = another.the_board;
  }

};

class Enemy : public Item {
public:
 Enemy(Position pos, PacBoard *board, char type)
   :Item(pos, board, type) {}
  virtual ~Enemy() {};
  void move();
  Position will_move();

  virtual Enemy * clone() const = 0;

 protected:
 Enemy(const Enemy &another)
  :Item(another) {}
  virtual Position determine_next_pos() = 0;

};

class EnemyV : public Enemy {
 public:
 EnemyV(Position initial_pos, PacBoard *board)
   :Enemy(initial_pos, board, 'V') {};

  virtual EnemyV * clone() const { return new EnemyV(*this); }

 protected:
  EnemyV(const EnemyV &another)
	: Enemy(another) {  }
  Position determine_next_pos();
};

class EnemyH : public Enemy {
 public:
 EnemyH(Position initial_pos, PacBoard *board)
   :Enemy(initial_pos, board,'H') {}

  virtual EnemyH * clone() const { return new EnemyH(*this); }
 protected:
  EnemyH(const EnemyH &another)
	: Enemy(another) {
  }
  Position determine_next_pos();
};

class EnemyLR : public Enemy {
 public:
 EnemyLR(Position initial_pos, PacBoard *board, char type)
   :Enemy(initial_pos, board, type) { }

  virtual EnemyLR * clone() const = 0;
 protected:
  EnemyLR(const EnemyLR &another)
	:Enemy(another) {}
  Position determine_next_pos_l();  
  Position determine_next_pos_r();  
};

class EnemyL : public EnemyLR {
 public:
 EnemyL(Position initial_pos, PacBoard *board)
   :EnemyLR(initial_pos, board, 'L') {}

  virtual EnemyL * clone() const { return new EnemyL(*this); }
 protected:
  EnemyL(const EnemyL &another)
	: EnemyLR(another) {}
  Position determine_next_pos() {
	return determine_next_pos_l();
  }
};

class EnemyR : public EnemyLR {
 public:
 EnemyR(Position initial_pos, PacBoard *board)
   :EnemyLR(initial_pos, board, 'R') { }

  virtual EnemyR * clone() const { return new EnemyR(*this); }
 protected:  
  EnemyR(const EnemyR &another)
	: EnemyLR(another) {}
  Position determine_next_pos() {
	return determine_next_pos_r();	
  }
};

class EnemyJ : public EnemyLR {
 private:
  bool toggle;

 public:
 EnemyJ(Position initial_pos, PacBoard *board)
   :EnemyLR(initial_pos, board, 'J') {
	toggle = false;
  }

  virtual EnemyJ * clone() const { return new EnemyJ(*this); }
 protected:
  EnemyJ(const EnemyJ &another)
	: EnemyLR(another) {
	toggle = another.toggle;
  }

  Position determine_next_pos() {
	toggle = !toggle;
	if (toggle) {
	  return determine_next_pos_l();		  	  
	} else {
	  return determine_next_pos_r();		  
	}
  }
};

class Player : public Item {
public:
 Player(Position initial_pos, PacBoard *board)
   : Item(initial_pos, board, '@') {}
 Player(const Player &another)
   : Item(another) {}
  ~Player() {}
  bool move_to(Position pos);
  bool move_to(Position::Dir dir);
};


class Cell {
 public:
  bool dot;
  bool high_priority_dot;
  Position pos;
  std::vector<int> adjacent_cells_index;

  Cell(bool dot, int x, int y, bool high_priority_dot = false) {
	this->dot = dot;
	pos.x = x;
	pos.y = y;
	this->high_priority_dot = high_priority_dot;
  }

  Cell(const Cell &another) {
	pos = another.pos;
	dot = another.dot;
	high_priority_dot = another.high_priority_dot;	
	adjacent_cells_index = another.adjacent_cells_index;
  }

  ~Cell() {}

  Position::Dir is_adjacent_from(const Cell &another) {
	return pos.is_adjacent_from(another.pos);
  }
  class PacBoard *the_board;
  static const int high_priority_dot_score = 5;

 private:
  Cell &operator=(const Cell &another);
};

class PacBoard {
 public:
	std::vector< std::vector<int> > board_index;
	std::vector< std::vector<char> > item_index;
	std::vector<Cell *> cells;
	Position::Dir entered_dir;
	Position h,v;
	PacBoard *parent;
	int rest_time_counter;
	std::vector<PacBoard *> children; // will not copied using copy constructor.
	std::vector<Enemy *> enemies;
	Player *player;
	int dots;
	
 public:
	std::vector<Position &> next_possible_direction(Position &current);	
	PacBoard(std::vector<std::string> &boardstring, int x, int y, int rest_time);
	~PacBoard();

	PacBoard *create_child_seed()  {
	  PacBoard *ret =  new PacBoard(*this);
	  ret->parent = this;
	  ret->move_enemies();
	  ret->rest_time_counter --;
	  return ret;
	}

	PacBoard *create_and_register_child(Position player_pos) {
	  PacBoard *child = create_child_seed();
	  child->player->move_to(player_pos);
	  children.push_back(child);	  
		return child;
	}

	PacBoard *find_and_cut_last_branch() {
	  PacBoard *board = this;
		while (board->parent != NULL) {
		std::vector<PacBoard *> &parent_c = board->parent->children;
		if (parent_c.size() > 1) {
		  parent_c.erase(std::remove(parent_c.begin(), parent_c.end(), board), parent_c.end());
		  return board;
		}
		board = board->parent;
	  }
	  return NULL;
	}

	PacBoard(const PacBoard &another);
	
	int index_of_cell_at(Position pos) const;
	Cell *cell_at(int x, int y) const;
	Cell *cell_at(Position pos) const;
	Cell *cell_adjacent_to(const Cell &cell, int index);
	void display_to(std::ostream &os);
	void display_all_history_to(std::ostream &os);
	std::string path_string() const;
	void move_enemies();

 protected:
	void read_player_or_enemy(char c, int x, int y);

 private:
	PacBoard &operator=(const PacBoard &another);
};

#endif
