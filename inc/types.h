#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

enum symbol_types
{
  UNDEFINED,
  LOCAL,
  GLOBALS,
  EXTERNS
};
enum relocation_type
{
  ABSOLUTE,
  PC_RELATIVE
};

struct Symbol
{
  enum symbol_types type;
  int id;
  int section;
  int offset;
  bool defined;
  string name;
  string section_name;
  int new_section;
};
struct Section
{
  int id;
  int address;
  int size;
  string name;
};
struct Relocation
{
  enum relocation_type type;
  int section_id;
  int offset;
  int data_position;
  int sym_id;
  int sym_offset;
  int sym_section;
  int new_section_id;
  int new_sym_id;
  string symbol_name;
  string section_name;
};

struct Elf
{
  unordered_map<string, Section> section_table;
  unordered_map<string, Symbol> symbol_table;
  vector<Relocation> relocation_table;
  vector<char> data;
};

#endif