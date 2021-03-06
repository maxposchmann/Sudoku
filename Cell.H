#ifndef _CELL_H_
#define _CELL_H_
#include <vector>
#include <cmath>

class Cell
{
public:
  /// trivial constructor
  Cell();
  /// constructor for nontrivial Board
  Cell(const int a_dim, const int a_row, const int a_col);
  /// set cell value
  void set(const int a_val);
  /// get row
  const int getRow() const {return m_row;};
  /// get column
  const int getCol() const {return m_col;};
  /// get box
  const int getBox() const {return m_box;};
  /// check if set
  bool isSet() {return m_set;};
  /// get value
  int getVal() {return m_val;};
  /// get note
  std::vector<bool> getNote() {return m_note;};
  /// get note length
  int getNoteLength() {return m_noteLength;};
  /// set item in note to false
  void noteSetFalse(int a_val);
  /// get a single item (bool) from note
  bool getNoteItem(int a_val) {return m_note[a_val];};




private:
  int m_val;
  bool m_set;
  int m_row, m_col, m_box;
  int m_noteLength;
  std::vector<bool> m_note;
};
#endif
