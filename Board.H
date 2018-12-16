#ifndef _BOARD_H_
#define _BOARD_H_
#include "Cell.H"
#include <iostream>
#include <cmath>

class Board
{
public:
  /// trivial constructor
  Board();
  /// constructor for nontrivial Board
  Board(const int a_dim);
  /// initializer for new game
  void initializeBoard(std::vector<int> a_input);
  /// get dimension
  const int getDim() const {return m_dim;};
  /// get length
  const int getLen() const {return m_len;};
  /// convert 2 index to 1
  int twoToOne(int a_i, int a_j);
  /// convert 1 index to 2
  void oneToTwo(int a_k, int* a_i, int* a_j);
  /// create note for each cell
  void createNote(int a_i);
  /// print out the current board
  void printBoard();



private:
  int m_dim, m_len;
  std::vector<Cell> m_cells;
};
#endif
