#include "Board.H"
#include "Cell.H"
#include <fstream>
#include <cctype>

int main(int argc, char* argv[])
{
  int dim = 3;
  int len = pow(dim,2);
  int siz = pow(dim,4);
  // create the board
  Board board(dim);

  // read input
  std::ifstream file("game1");
  std::string line;
  char c;
  int k = 0;
  std::vector<int> gameInput(siz,0);
  if (file.is_open())
  {
    while (getline(file, line))
    {
      printf("%s\n", line.c_str());
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
          gameInput[k] = (int)c  - '0';
          k++;
        }
      }
    }
    file.close();
    printf("\n\n\n");
  }

  k = 0;
  for (int i = 0; i < dim; i++)
  {
    for (int j = 0; j < dim; j++)
    {
      for (int m = 0; m < dim; m++)
      {
        for (int n = 0; n < dim; n++)
        {
          if (gameInput[k] == 0)
          {
            printf("_ ");
          }
          else
          {
            printf("%i ", gameInput[k]);
          }
          k++;
        }
        if (m < (dim-1)) printf("| ");
      }
      printf("\n");
    }
    if (i < (dim-1))
    {
      for (int p = 0; p < dim; p++)
      {
        for (int l = 0; l < (2 * dim); l++)
        {
          printf("-");
        }
        if (p < (dim-1)) printf("+-");
      }
      printf("\n");
    }
  }


}
