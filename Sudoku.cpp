#include "Board.H"
#include "Cell.H"
#include <fstream>
#include <cctype>

std::vector<int> readInput(std::string a_filename, int a_siz)
{
  std::ifstream file(a_filename);
  std::string line;
  char c;
  int k = 0;
  std::vector<int> input(a_siz,0);
  if (file.is_open())
  {
    while (getline(file, line))
    {
      if (line.at(0) == '-') continue;
      for (int i = 0; i < line.length(); i++)
      {
        c = line.at(i);
        if (c == '_')
        {
          k++;
        }
        else if (std::isdigit(c))
        {
          input[k] = (int)c  - '1';
          k++;
        }
      }
    }
    file.close();
  }
  return input;
}

int main(int argc, char* argv[])
{
  int dim = 3;
  int len = pow(dim,2);
  int siz = pow(dim,4);
  // create the board
  Board board(dim);

  // read input
  std::vector<int> gameInput(siz);
  gameInput = readInput("game1",siz);

  board.initializeBoard(gameInput);
  board.printBoard();

  for (int k = 0; k < siz; k++)
  {
    if (not(board.isCellSet(k)))
    {
      int nl = board.createNote(k);
      if (nl == 1)
      {
        int val = board.getSoleNote(k);
        board.setCell(k,val);
        board.printBoard();
      }
    }
  }
  int maxIterations = 100;
  for (int i = 0; i < maxIterations; i++)
  {
    for (int k = 0; k < siz; k++)
    {
      if (not(board.isCellSet(k)))
      {
        int nl = board.getCellNoteLength(k);
        if (nl == 1)
        {
          int val = board.getSoleNote(k);
          board.setCell(k,val);
          board.printBoard();
        }
      }
    }
  }

}
