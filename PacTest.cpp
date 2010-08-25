#include <cppunit/config/SourcePrefix.h>
#include "pacboard.h"
#include "pacsolver.h"
#include "PacTest.h"


CPPUNIT_TEST_SUITE_REGISTRATION(PacTest);

PacTest::PacTest() {}
PacTest::~PacTest() {}

void 
PacTest::setUp()
{
}


void 
PacTest::tearDown()
{
}

void
PacTest::testDirRelativeDirection()
{
  Position::Dir dir;

  dir = Position::up;
  CPPUNIT_ASSERT_EQUAL(Position::up, Position::relative_direction(dir, Position::up));
  CPPUNIT_ASSERT_EQUAL(Position::right, Position::relative_direction(dir, Position::right));
  CPPUNIT_ASSERT_EQUAL(Position::down, Position::relative_direction(dir, Position::down));
  CPPUNIT_ASSERT_EQUAL(Position::left, Position::relative_direction(dir, Position::left));

  dir = Position::right;
  CPPUNIT_ASSERT_EQUAL(Position::right, Position::relative_direction(dir, Position::up));
  CPPUNIT_ASSERT_EQUAL(Position::down, Position::relative_direction(dir, Position::right));
  CPPUNIT_ASSERT_EQUAL(Position::left, Position::relative_direction(dir, Position::down));
  CPPUNIT_ASSERT_EQUAL(Position::up, Position::relative_direction(dir, Position::left));

  dir = Position::down;
  CPPUNIT_ASSERT_EQUAL(Position::down, Position::relative_direction(dir, Position::up));
  CPPUNIT_ASSERT_EQUAL(Position::left, Position::relative_direction(dir, Position::right));
  CPPUNIT_ASSERT_EQUAL(Position::up, Position::relative_direction(dir, Position::down));
  CPPUNIT_ASSERT_EQUAL(Position::right, Position::relative_direction(dir, Position::left));

  dir = Position::left;
  CPPUNIT_ASSERT_EQUAL(Position::left, Position::relative_direction(dir, Position::up));
  CPPUNIT_ASSERT_EQUAL(Position::up, Position::relative_direction(dir, Position::right));
  CPPUNIT_ASSERT_EQUAL(Position::right, Position::relative_direction(dir, Position::down));
  CPPUNIT_ASSERT_EQUAL(Position::down, Position::relative_direction(dir, Position::left));
}

void 
PacTest::testPacBoardConstructorOnlyOneDot()
{
  std::vector<std::string> boardstr;
  boardstr.push_back("###");  
  boardstr.push_back("#.#");
  boardstr.push_back("###");
  PacBoard board(boardstr, 3, 3, 100);

  CPPUNIT_ASSERT_EQUAL(1, board.dots);
  Cell *cell = board.cell_at(1, 1);
  CPPUNIT_ASSERT_EQUAL(cell->pos, Position(1,1));
	
}

void 
PacTest::testPacBoardConstructorTwoCells()
{
  std::vector<std::string> boardstr;
  boardstr.push_back("####");  
  boardstr.push_back("#..#");
  boardstr.push_back("####");
  PacBoard board(boardstr, 4, 3, 100);

  CPPUNIT_ASSERT_EQUAL(2, board.dots);
  Cell *cell = board.cell_at(1, 1);
  CPPUNIT_ASSERT_EQUAL(1, (int)cell->adjacent_cells_index.size());
  Cell *cell_adj = board.cell_adjacent_to(*cell, 0);
  CPPUNIT_ASSERT_EQUAL(2, cell_adj->pos.x);
  CPPUNIT_ASSERT_EQUAL(1, cell_adj->pos.y);

  Cell *cell2 = board.cell_at(2, 1);
  CPPUNIT_ASSERT_EQUAL(1, (int)cell2->adjacent_cells_index.size());
  cell_adj = board.cell_adjacent_to(*cell2, 0);
  CPPUNIT_ASSERT_EQUAL(1, cell_adj->pos.x);
  CPPUNIT_ASSERT_EQUAL(1, cell_adj->pos.y);
}


void 
PacTest::testPacBoardConstructorSomeCells()
{
  std::vector<std::string> boardstr;
  boardstr.push_back("#####");  
  boardstr.push_back("#...#");
  boardstr.push_back("##.##");
  boardstr.push_back("#...#");
  boardstr.push_back("##.##");
  boardstr.push_back("#####");

  PacBoard board(boardstr, 5, 6, 100);
  CPPUNIT_ASSERT_EQUAL(8, board.dots);
  Cell *cell = board.cell_at(2,3);
  CPPUNIT_ASSERT_EQUAL(4, (int)cell->adjacent_cells_index.size());
  
}

void 
PacTest::testPacBoardConstructorWithPlayer()
{
  std::vector<std::string> boardstr;
  boardstr.push_back("#####");  
  boardstr.push_back("#...#");
  boardstr.push_back("#.#.#");
  boardstr.push_back("#@..#");
  boardstr.push_back("##.##");
  boardstr.push_back("#####");

  PacBoard board(boardstr, 5, 6, 100);
  CPPUNIT_ASSERT_EQUAL(8, board.dots);
  CPPUNIT_ASSERT_EQUAL(board.player->previous_pos, Position(-1, -1));
  CPPUNIT_ASSERT_EQUAL(board.player->current_pos, Position(1, 3));
  
}

void 
PacTest::testPacBoardConstructorWithEnemyV()
{
  std::vector<std::string> boardstr;
  boardstr.push_back("#####");  
  boardstr.push_back("#. V#");
  boardstr.push_back("#.#.#");
  boardstr.push_back("#@..#");
  boardstr.push_back("##.##");
  boardstr.push_back("#####");

  PacBoard board(boardstr, 5, 6, 100);
  CPPUNIT_ASSERT_EQUAL(6, board.dots);
  CPPUNIT_ASSERT_EQUAL(board.player->previous_pos, Position(-1, -1));
  CPPUNIT_ASSERT_EQUAL(board.player->current_pos, Position(1, 3));

  CPPUNIT_ASSERT_EQUAL(1, (int)board.enemies.size());
  Enemy *e = board.enemies[0];
  CPPUNIT_ASSERT_EQUAL('V', e->type);
  CPPUNIT_ASSERT_EQUAL(e->previous_pos, Position(-1, -1));
  CPPUNIT_ASSERT_EQUAL(e->current_pos, Position(3, 1));

  
}

void 
PacTest::testPacBoardConstructorWithEnemies() {
  std::vector<std::string> boardstr;
  boardstr.push_back("#####");  
  boardstr.push_back("#@HV#");
  boardstr.push_back("# #L#");
  boardstr.push_back("# R.#");
  boardstr.push_back("##J##");
  boardstr.push_back("#####");

  PacBoard board(boardstr, 5, 6, 100);
  CPPUNIT_ASSERT_EQUAL(1, board.dots);
  CPPUNIT_ASSERT_EQUAL(board.player->previous_pos, Position(-1, -1));
  CPPUNIT_ASSERT_EQUAL(board.player->current_pos, Position(1, 1));

  CPPUNIT_ASSERT_EQUAL(5, (int)board.enemies.size());
  // 順序依存チェックしてる。よくない。
  Enemy *e = board.enemies[0];
  CPPUNIT_ASSERT_EQUAL('H', e->type);
  CPPUNIT_ASSERT_EQUAL(e->previous_pos, Position(-1, -1));
  CPPUNIT_ASSERT_EQUAL(e->current_pos, Position(2, 1));
  e = board.enemies[1];
  CPPUNIT_ASSERT_EQUAL('V', e->type);
  CPPUNIT_ASSERT_EQUAL(e->previous_pos, Position(-1, -1));
  CPPUNIT_ASSERT_EQUAL(e->current_pos, Position(3, 1));
  e = board.enemies[2];
  CPPUNIT_ASSERT_EQUAL('L', e->type);
  CPPUNIT_ASSERT_EQUAL(e->previous_pos, Position(-1, -1));
  CPPUNIT_ASSERT_EQUAL(e->current_pos, Position(3, 2));

  e = board.enemies[3];
  CPPUNIT_ASSERT_EQUAL('R', e->type);
  CPPUNIT_ASSERT_EQUAL(e->previous_pos, Position(-1, -1));
  CPPUNIT_ASSERT_EQUAL(e->current_pos, Position(2, 3));

  e = board.enemies[4];
  CPPUNIT_ASSERT_EQUAL('J', e->type);
  CPPUNIT_ASSERT_EQUAL(e->previous_pos, Position(-1, -1));
  CPPUNIT_ASSERT_EQUAL(e->current_pos, Position(2, 4));
  
}

void 
PacTest::testMoveEnemyV()
{

  {
  	std::vector<std::string> boardstr;
  	boardstr.push_back("#####");  
  	boardstr.push_back("###.#");  
  	boardstr.push_back("#..V#");
  	boardstr.push_back("#.#.#");
  	boardstr.push_back("#.@.#");
  	boardstr.push_back("##.##");
  	boardstr.push_back("#####");

  	PacBoard board(boardstr, 5, 7, 100);
  	Enemy *e = board.enemies[0];
  	e->previous_pos = e->current_pos;
  	e->move();
  	CPPUNIT_ASSERT_EQUAL(Position(3,2), e->previous_pos);
  	CPPUNIT_ASSERT_EQUAL(Position(3,3), e->current_pos);
  }
  {
  	std::vector<std::string> boardstr;
  	boardstr.push_back("#####");  
  	boardstr.push_back("#..@#");
  	boardstr.push_back("#.#.#");
  	boardstr.push_back("#V..#");
  	boardstr.push_back("#.###");
  	boardstr.push_back("#####");

  	PacBoard board(boardstr, 5, 6, 100);
  	Enemy *e = board.enemies[0];
  	e->previous_pos = e->current_pos;
  	e->move();
  	CPPUNIT_ASSERT_EQUAL(Position(1,2), e->current_pos);
  	CPPUNIT_ASSERT_EQUAL(Position(1,3), e->previous_pos);
  }

  {
  	std::vector<std::string> boardstr;
  	boardstr.push_back("#####");  
  	boardstr.push_back("#..@#");
  	boardstr.push_back("#.#.#");
  	boardstr.push_back("#.V.#");
  	boardstr.push_back("##.##");
  	boardstr.push_back("#####");

  	PacBoard board(boardstr, 5, 6, 100);
  	Enemy *e = board.enemies[0];
  	e->previous_pos = e->current_pos;
  	e->move();
  	CPPUNIT_ASSERT_EQUAL(Position(3,3), e->current_pos);
  	CPPUNIT_ASSERT_EQUAL(Position(2,3), e->previous_pos);
  }

  {
	std::vector<std::string> boardstr;
	boardstr.push_back("#####");  
	boardstr.push_back("##.##");  
	boardstr.push_back("#.V.#");
	boardstr.push_back("#.#.#");
	boardstr.push_back("#..@#");
	boardstr.push_back("##.##");
	boardstr.push_back("#####");

	PacBoard board(boardstr, 5, 7, 100);
	Enemy *e = board.enemies[0];
	e->previous_pos = e->current_pos;
	e->move();
	CPPUNIT_ASSERT_EQUAL(Position(3,2), e->current_pos);
	CPPUNIT_ASSERT_EQUAL(Position(2,2), e->previous_pos);
  }
  {
  	std::vector<std::string> boardstr;
  	boardstr.push_back("#####");  
  	boardstr.push_back("##.##");  
  	boardstr.push_back("#.V.#");
  	boardstr.push_back("#.#.#");
  	boardstr.push_back("#.@.#");
  	boardstr.push_back("##.##");
  	boardstr.push_back("#####");

  	PacBoard board(boardstr, 5, 7, 100);
  	Enemy *e = board.enemies[0];
  	e->previous_pos = e->current_pos;
  	e->move();
  	CPPUNIT_ASSERT_EQUAL(Position(1,2), e->current_pos);
  	CPPUNIT_ASSERT_EQUAL(Position(2,2), e->previous_pos);
  }

  {
  	std::vector<std::string> boardstr;
  	boardstr.push_back("#####");  
  	boardstr.push_back("##.##");  
  	boardstr.push_back("#...#");
  	boardstr.push_back("#.#V#");
  	boardstr.push_back("#..@#");
  	boardstr.push_back("##.##");
  	boardstr.push_back("#####");

  	PacBoard board(boardstr, 5, 7, 100);
  	Enemy *e = board.enemies[0];
  	e->previous_pos = Position(e->current_pos.x, e->current_pos.y + 1);
  	e->move();
  	CPPUNIT_ASSERT_EQUAL(Position(3,2), e->current_pos);
  	CPPUNIT_ASSERT_EQUAL(Position(3,3), e->previous_pos);
  }
   
}

void 
PacTest::testMoveEnemyH()
{
  std::vector<std::string> boardstr;
  boardstr.push_back("#####");  
  boardstr.push_back("###.#");  
  boardstr.push_back("#..H#");
  boardstr.push_back("#.#.#");
  boardstr.push_back("#.@.#");
  boardstr.push_back("##.##");
  boardstr.push_back("#####");

  PacBoard board(boardstr, 5, 7, 100);
  Enemy *e = board.enemies[0];
  e->previous_pos = e->current_pos;
  e->move();
  CPPUNIT_ASSERT_EQUAL(Position(2,2), e->current_pos);
  CPPUNIT_ASSERT_EQUAL(Position(3,2), e->previous_pos);
}

void
PacTest::testCellCopyConstructor()
{
  Cell *cell = new Cell(true, 193, -23);
  cell->adjacent_cells_index.push_back(3);
  cell->adjacent_cells_index.push_back(99);
  cell->adjacent_cells_index.push_back(5);

  Cell *newcell = new Cell(*cell);

  CPPUNIT_ASSERT(newcell != cell);
  CPPUNIT_ASSERT_EQUAL(cell->pos, newcell->pos);
  newcell->pos.x ++;
  newcell->pos.y --;
  CPPUNIT_ASSERT(!(cell->pos == newcell->pos));
  CPPUNIT_ASSERT_EQUAL(3, newcell->adjacent_cells_index[0]);
  CPPUNIT_ASSERT_EQUAL(99, newcell->adjacent_cells_index[1]);
  CPPUNIT_ASSERT_EQUAL(5, newcell->adjacent_cells_index[2]);

  newcell->adjacent_cells_index[1] = -12;
  CPPUNIT_ASSERT_EQUAL(99, cell->adjacent_cells_index[1]);


}

void
PacTest::testSolverOnlyPlayer()
{
  std::vector<std::string> boardstr;
  boardstr.push_back("#####");  
  boardstr.push_back("#...#");
  boardstr.push_back("#.#.#");
  boardstr.push_back("#.@.#");
  boardstr.push_back("##.##");
  boardstr.push_back("#####");
  PacBoard board(boardstr, 5, 6, 100);
  
  std::vector<Position> poss = PacSolver::next_move(&board);
  CPPUNIT_ASSERT_EQUAL(4, (int)poss.size()); 
  CPPUNIT_ASSERT(std::find(poss.begin(), poss.end(), Position(2,3)) != poss.end());
  CPPUNIT_ASSERT(std::find(poss.begin(), poss.end(), Position(1,3)) != poss.end());
  CPPUNIT_ASSERT(std::find(poss.begin(), poss.end(), Position(3,3)) != poss.end());
  CPPUNIT_ASSERT(std::find(poss.begin(), poss.end(), Position(2,4)) != poss.end());
}

void
PacTest::testSolverWithOneEnemy()
{
  std::vector<std::string> boardstr;
  boardstr.push_back("#####");  
  boardstr.push_back("#...#");
  boardstr.push_back("#.#.#");
  boardstr.push_back("#.@H#");
  boardstr.push_back("##.##");
  boardstr.push_back("#####");
  PacBoard board(boardstr, 5, 6, 100);
  board.enemies[0]->previous_pos = Position(3,2);

  PacBoard *next_next;

  std::vector<Position> poss = PacSolver::next_move(&board);
  CPPUNIT_ASSERT_EQUAL(2, (int)poss.size()); 
  CPPUNIT_ASSERT(std::find(poss.begin(), poss.end(), Position(1,3)) != poss.end());
  CPPUNIT_ASSERT(std::find(poss.begin(), poss.end(), Position(2,4)) != poss.end());

  PacBoard *next_board = board.create_child_seed();
  next_board->player->move_to(Position::down);
  next_board->move_enemies();
  poss = PacSolver::next_move(next_board);
  CPPUNIT_ASSERT_EQUAL(0, (int)poss.size()); 
  
  next_board = board.create_child_seed();
  next_board->move_enemies();
  next_board->player->move_to(Position::left);
  poss = PacSolver::next_move(next_board);
  CPPUNIT_ASSERT_EQUAL(1, (int)poss.size()); 
  CPPUNIT_ASSERT(std::find(poss.begin(), poss.end(), Position(1,2)) != poss.end());

}

void
PacTest::testSolverProcess() {
  std::vector<std::string> boardstr;
  boardstr.push_back("#####");  
  boardstr.push_back("#...#");
  boardstr.push_back("#.#.#");
  boardstr.push_back("#.@H#");
  boardstr.push_back("##.##");
  boardstr.push_back("#####");
  PacBoard board(boardstr, 5, 6, 100);
  board.enemies[0]->previous_pos = Position(3,2);
  
  PacSolver pacsolver(&board);
  CPPUNIT_ASSERT_EQUAL(PacSolver::Solving, pacsolver.process());
  CPPUNIT_ASSERT_EQUAL(2, (int)pacsolver.this_generation.size());

  CPPUNIT_ASSERT_EQUAL(PacSolver::Solving, pacsolver.process());  
  CPPUNIT_ASSERT_EQUAL(1, (int)pacsolver.this_generation.size());
}
