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
  std::vector<int> input(a_siz,-1);
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
  // define game dimension
  int dim = 3;
  int len = pow(dim,2);
  int siz = pow(dim,4);
  // create the board
  Board board(dim);
  // set debug (notes) flag
  bool printNotes = false;

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
          if (board.getNset() == siz)
          {
            printf("Solved!\n");
            break;
          }
        }
      }
    }
  }

  // debug: print notes if flag set
  if (printNotes)
  {
    for (int k = 0; k < siz; k++)
    {
      if (board.isCellSet(k))
      {
        printf("Set to %i\n", board.getCellVal(k) + 1);
      }
      else
      {
        printf("Not set, options: ");
        std::vector<bool> note = board.getCellNote(k);
        for (int i = 0; i < len; i++)
        {
          if (note[i]) printf("%i ", i + 1);
        }
        printf("\n");
      }
    }
  }
}
