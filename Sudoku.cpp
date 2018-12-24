#include "Board.H"
#include "Cell.H"
#include <fstream>
#include <cctype>
#include <algorithm>

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
  bool strat1 = true;
  int  strat1threshold = 0;
  // second strategy is to find rows/columns/boxes with a value in only one cell's note
  bool strat2 = true;
  int  strat2threshold = 0;
  // third strategy is checking for notes of length n with only the same n items
  bool strat3 = true;
  int  strat3threshold = 0;
  // fourth strategy is to reduce notes by using cells that are in same row and box or column and box
  bool strat4 = true;
  int  strat4threshold = 0;

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
  int z;
  int maxIterations = 100;
  for (int i = 0; i < maxIterations; i++)
  {
    bool updated = false;
    for (int k = 0; k < siz; k++)
    {
      if (not(board.isCellSet(k)))
      {
        int nl = board.getCellNoteLength(k);
        std::vector<bool> note = board.getCellNote(k);
        int row = board.getCellRow(k);
        int col = board.getCellCol(k);
        int box = board.getCellBox(k);
        // first strategy
        if (strat1 && (i >= strat1threshold))
        {
          if (nl == 1)
          {
            int val = board.getSoleNote(k);
            board.setCell(k,val);
            updated = true;
            board.printBoard();
            if (board.getNset() == siz)
            {
              printf("Solved in %i loops!\n", i + 1);
              break;
            }
            continue;
          }
        }

        // second strategy
        if (strat2 && (i >= strat2threshold))
        {
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
              updated = true;
              board.printBoard();
              if (board.getNset() == siz)
              {
                printf("Solved in %i loops!\n", i + 1);
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
                printf("Solved in %i loops!\n", i + 1);
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
                printf("Solved in %i loops!\n", i + 1);
                break;
              }
              continue;
            }
          }
        }

        // third strategy
        if (strat3 && (i >= strat3threshold))
        {
          // check row
          int nMatch = 1;
          std::vector<int> matchList;
          for (int j = 0; j < len; j++)
          {
            z = board.twoToOne(row,j);
            if (z == k) continue;
            if (board.isCellSet(z)) continue;
            if (board.getCellNote(z) == note)
            {
              nMatch++;
              matchList.push_back(z);
            }
          }
          if (nMatch == nl)
          {
            for (int j = 0; j < len; j++)
            {
              z = board.twoToOne(row,j);
              // skip cell in question
              if (z == k) continue;
              // skip also the cells it matched
              if (std::find(matchList.begin(), matchList.end(), z) != matchList.end()) continue;
              // modify notes for other cells in set
              for (int jj = 0; jj < len; jj++)
              {
                if (note[jj] && board.getCellNote(z)[jj])
                {
                  board.setCellNoteFalse(z,jj);
                  updated = true;
                }
              }
            }
          }

          // check column
          nMatch = 1;
          matchList.clear();
          for (int j = 0; j < len; j++)
          {
            z = board.twoToOne(j,col);
            if (z == k) continue;
            if (board.isCellSet(z)) continue;
            if (board.getCellNote(z) == note)
            {
              nMatch++;
              matchList.push_back(z);
            }
          }
          if (nMatch == nl)
          {
            for (int j = 0; j < len; j++)
            {
              z = board.twoToOne(j,col);
              // skip cell in question
              if (z == k) continue;
              // skip also the cells it matched
              if (std::find(matchList.begin(), matchList.end(), z) != matchList.end()) continue;
              // modify notes for other cells in set
              for (int jj = 0; jj < len; jj++)
              {
                if (note[jj] && board.getCellNote(z)[jj])
                {
                  board.setCellNoteFalse(z,jj);
                  updated = true;
                }
              }
            }
          }

          // check box
          nMatch = 1;
          matchList.clear();
          z = (len * dim * floor(box / dim)) + (dim * (box % dim)) - 1;
          for (int j = 0; j < dim; j++)
          {
            for (int jj = 0; jj < dim; jj++)
            {
              z++;
              if (z == k) continue;
              if (board.isCellSet(z)) continue;
              if (board.getCellNote(z) == note)
              {
                nMatch++;
                matchList.push_back(z);
              }
            }
            z += len - dim;
          }
          if (nMatch == nl)
          {
            z = (len * dim * floor(box / dim)) + (dim * (box % dim)) - 1;
            for (int j = 0; j < dim; j++)
            {
              for (int jj = 0; jj < dim; jj++)
              {
                z++;
                // skip cell in question
                if (z == k) continue;
                // skip also the cells it matched
                if (std::find(matchList.begin(), matchList.end(), z) != matchList.end()) continue;
                // modify notes for other cells in set
                for (int jj = 0; jj < len; jj++)
                {
                  if (note[jj] && board.getCellNote(z)[jj])
                  {
                    board.setCellNoteFalse(z,jj);
                    updated = true;
                  };
                }
              }
              z += len - dim;
            }
          }
        }

        // fourth strategy
        if (strat4 && (i >= strat4threshold))
        {
          for (int noteIndex = 0; noteIndex < len; noteIndex++)
          {
            // if value not in note, skip it
            if (not(note[noteIndex])) continue;
            bool isOnly = true;
            // check if a note value is unique in row, outside of box
            // loop over row
            for (int j = 0; j < len; j++)
            {
              int z = board.twoToOne(row,j);
              // if in same box, skip
              if ((board.getCellBox(z) == box)) continue;
              // if set, skip
              if (board.isCellSet(z)) continue;
              // if value is in not in note, skip
              if (not(board.getCellNote(z)[noteIndex])) continue;
              // if none of the above, then it's not unique and we can break
              isOnly = false;
              break;
            }
            // if isOnly is true, then remove this item from notes in box (except current row)
            if (isOnly)
            {
              int z = (len * dim * floor(box / dim)) + (dim * (box % dim)) - 1;
              for (int boxIndex1 = 0; boxIndex1 < dim; boxIndex1++)
              {
                for (int boxIndex2 = 0; boxIndex2 < dim; boxIndex2++)
                {
                  z++;
                  // skip if cell
                  if (board.isCellSet(z)) continue;
                  // skip row in question
                  if ((board.getCellRow(z) == row)) continue;
                  // modify notes for other cells in set
                  board.setCellNoteFalse(z,noteIndex);
                  updated = true;
                }
                z += len - dim;
              }
            }

            // now flip it and check if unique in box
            isOnly = true;
            int z = (len * dim * floor(box / dim)) + (dim * (box % dim)) - 1;
            for (int boxIndex1 = 0; boxIndex1 < dim; boxIndex1++)
            {
              for (int boxIndex2 = 0; boxIndex2 < dim; boxIndex2++)
              {
                z++;
                // skip row in question
                if ((board.getCellRow(z) == row)) continue;
                // skip if cell
                if (board.isCellSet(z)) continue;
                // skip if value not in note
                if (not(board.getCellNote(z)[noteIndex])) continue;
                // if it gets this far, then the pair isn't unique
                isOnly = false;
              }
              z += len - dim;
            }
            // if isOnly is true, then remove this item from notes in row (except current box)
            if (isOnly)
            {
              for (int colIndex = 0; colIndex < len; colIndex++)
              {
                z = board.twoToOne(row,colIndex);
                // skip if cell y is set
                if (board.isCellSet(z)) continue;
                // if in same box, skip
                if ((board.getCellBox(z) == box)) continue;
                // modify notes for other cells in set
                board.setCellNoteFalse(z,noteIndex);
                updated = true;
              }
            }

            // check if a note value is unique in column, outside of box
            // loop over column
            isOnly = true;
            for (int j = 0; j < len; j++)
            {
              z = board.twoToOne(j,col);
              // if in same box, skip
              if ((board.getCellBox(z) == box)) continue;
              // if set, skip
              if (board.isCellSet(z)) continue;
              // if value is in not in note, skip
              if (not(board.getCellNote(z)[noteIndex])) continue;
              // if none of the above, then it's not unique and we can break
              isOnly = false;
              break;
            }
            // if isOnly is true, then remove this item from notes in box (except current column)
            if (isOnly)
            {
              z = (len * dim * floor(box / dim)) + (dim * (box % dim)) - 1;
              for (int boxIndex1 = 0; boxIndex1 < dim; boxIndex1++)
              {
                for (int boxIndex2 = 0; boxIndex2 < dim; boxIndex2++)
                {
                  z++;
                  // skip if cell
                  if (board.isCellSet(z)) continue;
                  // skip row in question
                  if ((board.getCellCol(z) == col)) continue;
                  // modify notes for other cells in set
                  board.setCellNoteFalse(z,noteIndex);
                  updated = true;
                }
                z += len - dim;
              }
            }

            // now flip it and check if unique in box
            isOnly = true;
            z = (len * dim * floor(box / dim)) + (dim * (box % dim)) - 1;
            for (int boxIndex1 = 0; boxIndex1 < dim; boxIndex1++)
            {
              for (int boxIndex2 = 0; boxIndex2 < dim; boxIndex2++)
              {
                z++;
                // skip column in question
                if ((board.getCellCol(z) == col)) continue;
                // skip if cell
                if (board.isCellSet(z)) continue;
                // skip if value not in note
                if (not(board.getCellNote(z)[noteIndex])) continue;
                // if it gets this far, then the pair isn't unique
                isOnly = false;
              }
              z += len - dim;
            }
            // if isOnly is true, then remove this item from notes in column (except current box)
            if (isOnly)
            {
              for (int rowIndex = 0; rowIndex < len; rowIndex++)
              {
                z = board.twoToOne(rowIndex,col);
                // skip if cell y is set
                if (board.isCellSet(z)) continue;
                // if in same box, skip
                if ((board.getCellBox(z) == box)) continue;
                // modify notes for other cells in set
                board.setCellNoteFalse(z,noteIndex);
                updated = true;
              }
            }
          }
        }
      }
    }
    if (not(updated))
    {
      printf("Game stalled after %i loops\n\n", i + 1);
      break;
    }
  }

  // debug: print notes if flag set
  if (printNotes)
  {
    for (int k = 0; k < siz; k++)
    {
      if (board.isCellSet(k))
      {
        printf("Cell %i: set to %i\n", k+1, board.getCellVal(k) + 1);
      }
      else
      {
        printf("Cell %i: not set, options: ", k+1);
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
