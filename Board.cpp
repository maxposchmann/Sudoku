#include "Board.H"

Board::Board(const int a_dim)
{
  m_dim = a_dim;
  m_len = pow(a_dim,2);
  for (int i = 0; i < m_len; i++)
  {
    for (int j = 0; j < m_len; j++)
    {
      m_cells.push_back(Cell(m_dim, i, j));
    }
  }
}

void Board::initializeBoard(std::vector<int> a_input)
{
  for (int i = 0; i < m_len; i++)
  {
    for (int j = 0; j < m_len; j++)
    {
      int k = twoToOne(i,j);
      if (a_input[k] >= 0)
      {
        m_cells[k].set(a_input[k]);
      }
    }
  }
};

int Board::twoToOne(int a_i, int a_j)
{
  int k = m_len * a_i + a_j;
  return k;
}

void Board::oneToTwo(int a_k, int* a_i, int* a_j)
{
  *a_i = floor(a_k / m_len);
  *a_j = a_k % m_len;
}

int Board::createNote(int a_k)
{
  int row = m_cells[a_k].getRow();
  int col = m_cells[a_k].getCol();
  int box = m_cells[a_k].getBox();

  int k;
  // scan row
  for (int j = 0; j < m_len; j++)
  {
    if (j == col) continue;
    k = twoToOne(row,j);
    if (m_cells[k].isSet())
    {
      m_cells[a_k].noteSetFalse(m_cells[k].getVal());
    }
  }

  // scan column
  for (int i = 0; i < m_len; i++)
  {
    if (i == row) continue;
    k = twoToOne(i,col);
    if (m_cells[k].isSet())
    {
      m_cells[a_k].noteSetFalse(m_cells[k].getVal());
    }
  }

  // scan box
  k = (m_len * m_dim * floor(box / m_dim)) + (m_dim * (box % m_dim));
  for (int i = 0; i < m_dim; i++)
  {
    for (int j = 0; j < m_dim; j++)
    {
      if ((i == row) && (j == col))
      {
        k++;
        continue;
      }
      if (m_cells[k].isSet())
      {
        m_cells[a_k].noteSetFalse(m_cells[k].getVal());
      }
      k++;
    }
     k += m_len - m_dim;
  }
  return m_cells[k].getNoteLength();
}

void Board::printBoard()
{
  printf("%i cells set\n",getNset());
  int k = 0;
  for (int i = 0; i < m_dim; i++)
  {
    for (int j = 0; j < m_dim; j++)
    {
      for (int m = 0; m < m_dim; m++)
      {
        for (int n = 0; n < m_dim; n++)
        {
          if (m_cells[k].isSet())
          {
            printf("%i ", m_cells[k].getVal() + 1);
          }
          else
          {
            printf("_ ");
          }
          k++;
        }
        if (m < (m_dim-1)) printf("| ");
      }
      printf("\n");
    }
    if (i < (m_dim-1))
    {
      for (int p = 0; p < m_dim; p++)
      {
        for (int l = 0; l < (2 * m_dim); l++)
        {
          printf("-");
        }
        if (p < (m_dim-1)) printf("+-");
      }
      printf("\n");
    }
  }
  printf("\n\n");
}

void Board::setCell(int a_k, int a_val)
{
  m_cells[a_k].set(a_val);

  int row = m_cells[a_k].getRow();
  int col = m_cells[a_k].getCol();
  int box = m_cells[a_k].getBox();

  int k;
  // scan row
  for (int j = 0; j < m_len; j++)
  {
    if (j == col) continue;
    k = twoToOne(row,j);
    if (not(m_cells[k].isSet()))
    {
      m_cells[k].noteSetFalse(a_val);
    }
  }

  // scan column
  for (int i = 0; i < m_len; i++)
  {
    if (i == row) continue;
    k = twoToOne(i,col);
    if (not(m_cells[k].isSet()))
    {
      m_cells[k].noteSetFalse(a_val);
    }
  }

  // scan box
  k = (m_len * m_dim * floor(box / m_dim)) + (m_dim * (box % m_dim));
  for (int i = 0; i < m_dim; i++)
  {
    for (int j = 0; j < m_dim; j++)
    {
      if ((i == row) && (j == col))
      {
        k++;
        continue;
      }
      if (not(m_cells[k].isSet()))
      {
        m_cells[k].noteSetFalse(a_val);
      }
      k++;
    }
     k += m_len - m_dim;
  }
}

int Board::getSoleNote(int a_k)
{
  if (m_cells[a_k].getNoteLength() != 1) throw std::invalid_argument("Note length not 1");
  else
  {
    for (int i = 0; i < m_len; i++)
    {
      if (m_cells[a_k].getNoteItem(i)) return i;
    }
  }
}

int Board::getNset()
{
  int nSet = 0;
  for (int i = 0; i < pow(m_len,2); i++)
  {
    if (isCellSet(i)) nSet++;
  }
  return nSet;
};
