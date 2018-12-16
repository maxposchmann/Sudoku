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
  int dim;
  std::string filename;
  if (argc > 1)
  {
    filename = argv[1];
    if (argc > 2)
    {
      dim = atoi(argv[2]);
    }
    else
    {
      dim = 3;
    }
  }
  else throw std::invalid_argument("Need input file");
  // define game dimension
  int len = pow(dim,2);
  int siz = pow(dim,4);
  // create the board
  Board board(dim);
  // set debug (notes) flag
  bool printNotes = false;
  // set flags for strategies to use
  // first strategy is to find cells with only one option in note
  // second strategy is to find rows/columns/boxes with a value in only one cell's note
  bool strat1 = true;
  bool strat2 = true;

  // read input
  std::vector<int> gameInput(siz);
  gameInput = readInput(filename,siz);

  board.initializeBoard(gameInput);
  board.printBoard();

  // setup initial set of notes for board
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

  // solving loop
  int maxIterations = 100;
  for (int i = 0; i < maxIterations; i++)
  {
    for (int k = 0; k < siz; k++)
    {
      if (not(board.isCellSet(k)))
      {
        // first strategy
        if (strat1)
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
            continue;
          }
        }

        // second strategy
        if (strat2)
        {
          std::vector<bool> note = board.getCellNote(k);
          int row = board.getCellRow(k);
          int col = board.getCellCol(k);
          int box = board.getCellBox(k);
          int z;

          for (int noteIndex = 0; noteIndex < len; noteIndex++)
          {
            // only check possibilities for current cell
            if (not(note[noteIndex])) continue;

            // check row
            bool isOnly = true;
            for (int j = 0; j < len; j++)
            {
              z = board.twoToOne(row,j);
              if (z == k) continue;
              if (board.isCellSet(z)) continue;
              if (board.getCellNote(z)[noteIndex]) isOnly = false;
            }
            // if it gets here with isOnly still true, that must be cell value
            if (isOnly)
            {
              int val = noteIndex;
              board.setCell(k,val);
              board.printBoard();
              if (board.getNset() == siz)
              {
                printf("Solved!\n");
                break;
              }
              continue;
            }

            // check column
            isOnly = true;
            for (int j = 0; j < len; j++)
            {
              z = board.twoToOne(j,col);
              if (z == k) continue;
              if (board.isCellSet(z)) continue;
              if (board.getCellNote(z)[noteIndex]) isOnly = false;
            }
            // if it gets here with isOnly still true, that must be cell value
            if (isOnly)
            {
              int val = noteIndex;
              board.setCell(k,val);
              board.printBoard();
              if (board.getNset() == siz)
              {
                printf("Solved!\n");
                break;
              }
              continue;
            }

            // check box
            isOnly = true;
            z = (len * dim * floor(box / dim)) + (dim * (box % dim)) - 1;
            for (int j = 0; j < dim; j++)
            {
              for (int jj = 0; jj < dim; jj++)
              {
                z++;
                if (z == k) continue;
                if (board.isCellSet(z)) continue;
                if (board.getCellNote(z)[noteIndex]) isOnly = false;
              }
               z += len - dim;
            }
            // if it gets here with isOnly still true, that must be cell value
            if (isOnly)
            {
              int val = noteIndex;
              board.setCell(k,val);
              board.printBoard();
              if (board.getNset() == siz)
              {
                printf("Solved!\n");
                break;
              }
              continue;
            }
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
