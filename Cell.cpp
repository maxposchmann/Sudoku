#include "Cell.H"

Cell::Cell()
{
  m_row = -1;
  m_col = -1;
  m_box = -1;
  m_val = -1;
  m_set = false;
};

Cell::Cell(const int a_dim, const int a_row, const int a_col)
{
  m_row = a_row;
  m_col = a_col;
  m_box = a_dim * floor(a_row/a_dim) + floor(a_col/a_dim);
  // initialize note for cell
  for (int i = 0; i < pow(a_dim,2); i++)
  {
    m_note.push_back(true);
  }
  m_noteLength = pow(a_dim,2);
  m_val = -1;
  m_set = false;
}

void Cell::set(const int a_val)
{
  m_val = a_val;
  m_set = true;
  m_note.clear();
  m_noteLength = 0;
};

void Cell::noteSetFalse(int a_val)
{
  if (m_note[a_val]) m_noteLength--;
  m_note[a_val] = false;
}
