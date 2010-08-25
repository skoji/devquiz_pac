#ifndef BASETESTCASE_H
#define BASETESTCASE_H

#include <cppunit/extensions/HelperMacros.h>

class PacTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE( PacTest );
  CPPUNIT_TEST( testDirRelativeDirection );
  CPPUNIT_TEST( testPacBoardConstructorOnlyOneDot );
  CPPUNIT_TEST( testPacBoardConstructorTwoCells );
  CPPUNIT_TEST( testPacBoardConstructorSomeCells );
  CPPUNIT_TEST( testPacBoardConstructorWithPlayer );
  CPPUNIT_TEST( testPacBoardConstructorWithEnemyV );
  CPPUNIT_TEST( testPacBoardConstructorWithEnemies );
  CPPUNIT_TEST( testMoveEnemyV );
  CPPUNIT_TEST( testMoveEnemyH );
  CPPUNIT_TEST( testCellCopyConstructor );
  CPPUNIT_TEST( testSolverOnlyPlayer );
  CPPUNIT_TEST( testSolverWithOneEnemy );
  CPPUNIT_TEST( testSolverProcess );
  CPPUNIT_TEST_SUITE_END();

public:
  PacTest();
  virtual ~PacTest();

  virtual void setUp();
  virtual void tearDown();

  void testDirRelativeDirection();
  void testPacBoardConstructorOnlyOneDot();
  void testPacBoardConstructorTwoCells();
  void testPacBoardConstructorSomeCells();
  void testPacBoardConstructorWithPlayer();
  void testPacBoardConstructorWithEnemyV();
  void testPacBoardConstructorWithEnemies();

  void testMoveEnemyV();
  void testMoveEnemyH();

  void testCellCopyConstructor();

  void testSolverOnlyPlayer();
  void testSolverWithOneEnemy();
  void testSolverProcess();
protected:

};



#endif  // BASETESTCASE_H
