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
      if (a_input[k])
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

void Board::createNote(int a_k)
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
  k = (m_len * m_dim * floor(box / m_dim)) + (box % m_dim);
  for (int i = 0; i < m_dim; i++)
  {
    for (int j = 0; j < m_dim; j++)
    {
      if ((i == row) && (j == col)) continue;
      if (m_cells[k].isSet())
      {
        m_cells[a_k].noteSetFalse(m_cells[k].getVal());
      }
      k++;
    }
     k += (m_dim - 1) * m_len;
  }
}

void Board::printBoard()
{
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
            printf("%i ", m_cells[k].getVal());
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
