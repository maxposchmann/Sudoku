#ifndef _BOARD_H_
#define _BOARD_H_
#include "Cell.H"
#include <iostream>
#include <cmath>
#include <stdexcept>

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
  /// create note for each cell and return note length
  int createNote(int a_k);
  /// print out the current board
  void printBoard();
  /// check if cell is set
  bool isCellSet(int a_k) {return m_cells[a_k].isSet();};
  /// get cell value
  int getCellVal(int a_k) {return m_cells[a_k].getVal();};
  /// set cell in board and modify related notes
  void setCell(int a_k, int a_val);
  /// when there is only one option in note, get its value
  int getSoleNote(int a_k);
  /// get note from a cell
  std::vector<bool> getCellNote(int a_k) {return m_cells[a_k].getNote();};
  /// get note length for cell
  int getCellNoteLength(int a_k) {return m_cells[a_k].getNoteLength();};
  /// get number of cells set
  int getNset();
  /// get row for cell
  const int getCellRow(int a_k) const {return m_cells[a_k].getRow();};
  /// get column for cell
  const int getCellCol(int a_k) const {return m_cells[a_k].getCol();};
  /// get box for cell
  const int getCellBox(int a_k) const {return m_cells[a_k].getBox();};
  /// set a note value false for cell
  void setCellNoteFalse(int a_k, int a_val) {m_cells[a_k].noteSetFalse(a_val);};


private:
  int m_dim, m_len;
  std::vector<Cell> m_cells;
};
#endif
