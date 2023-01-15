#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "elf.h"
#include <stdlib.h>
#include <fstream>
#include "types.h"

using namespace std;

class AssemblerToElf
{

public:
  
  string path_to_input_file;
  string path_to_output_file;
  bool label_line = false;
  string label;
  int location_counter = 0;
  int program_counter=0;
  int currentLine = 1;
  int currentSection=1;
  string currentSectionName;
  int idG_symbols, idG_sections;
  bool error;
  unordered_map<string, Symbol> symbol_table;
  unordered_map<string, Section> section_table;
  vector<Relocation> relocation_table;
  vector<char> data;

  enum line_types
  {
    IGNORE,
    DEFAULT,
    GLOBAL,
    EXTERN,
    SECTION,
    WORD,
    SKIP,
    ASCII,
    END,
    HALT,
    INT,
    IRET,
    CALL,
    RET,
    JUMP_LIT_VALUE,
    JUMP_SYM_VALUE,
    JUMP_LIT_MEM,
    JUMP_SYM_MEM,
    JUMP_SYM_PCREL,
    JUMP_REG_VALUE,
    JUMP_REG_IND,
    JUMP_REG_LIT_IND,
    JUMP_REG_SYM_IND,
    PUSH,
    POP,
    XCHG,
    ADD,
    SUB,
    MUL,
    DIV,
    CMP,
    NOT,
    AND,
    OR,
    XOR,
    TEST,
    SHL,
    SHR,
    LDST_LIT_VAL,
    LDST_SYM_VAL,
    LDST_LIT_MEM,
    LDST_SYM_MEM,
    LDST_SYM_PCREL,
    LDST_REG_VALUE,
    LDST_REG_IND,
    LDST_REG_LIT_IND,
    LDST_REG_SYM_IND
  };

  line_types type_of_line(string &line);
  string strip_line(string line);
  AssemblerToElf(string input_path, string output_path);
  bool compileAssembler();
  string printError();
  string trim(const string &str);
  void addSymbols(string line, symbol_types type);
  void addSection(string line);
  void initWords(string line);
  void printSymbols();
  void printSections();
  void printRelocations();
  void printData();
  void skip(string line);
  int reg(string r);
  char combineRegs(int r1, int r2);
  string jumps(string line);
  string ldst(string line);
  void addRelocation(string word, relocation_type type);
  void addSymbol(string label);
  void formOutputFile();
};

vector<string> split(string s, char delim);
string decToHex2Byte(int number);
int toInt(char c);
int lit(string l);

#endif