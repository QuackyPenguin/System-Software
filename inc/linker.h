#ifndef LINKER_H
#define LINKER_H

#include "types.h"
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

class ElfToExe{
  public:
  ElfToExe(string output_file);
  bool addFile(string path_to_input_file);
  bool link();
  Section addSection(Section s, int i);
  void formOutputFile();

  string path_to_output_file;
  int program_counter=0;
  vector<Elf>elfs;
  Elf linked;
  int idG_sections=0;
  int idG_symbols=0;
};

char hexToChar(char c1, char c2);
int hexToInt(char c);

#endif