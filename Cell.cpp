#include "Cell.H"

Cell::Cell()
{
  m_row = 0;
  m_col = 0;
  m_box = 0;
  m_val = 0;
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
  m_val = 0;
  m_set = false;
}

void Cell::set(const int a_val)
{
  m_val = a_val;
  m_set = true;
};

void Cell::noteSetFalse(int a_val)
{
  if (m_note[a_val]) m_noteLength--;
  m_note[a_val] = false;
}
