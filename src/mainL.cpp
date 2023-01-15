#include "../inc/linker.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
  if (argc < 5)
  {
    cout << "Bad input";
    return -1;
  }
  string option = argv[1];
  if (option != "-hex")
  {
    cout << "Hex option is mandatory";
    return -2;
  }
  option = argv[2];
  if (option != "-o")
  {
    cout << "Output option is mandatory";
    return -2;
  }

  ElfToExe l(argv[3]);
  for (int i = 4; i < argc; i++)
    l.addFile(argv[i]);
  if (l.link())
  {
    l.formOutputFile();
  }
}