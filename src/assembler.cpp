#include <iostream>
#include <fstream>
#include "assembler.h"
#include "regexes.h"
#include <iomanip>

AssemblerToElf::AssemblerToElf(string input_path, string output_path) : path_to_input_file(input_path), path_to_output_file(output_path)
{
  idG_symbols = 0;
  idG_sections = 0;
}

bool AssemblerToElf::compileAssembler()
{
  vector<string> regs;
  int r1, r2, immediate;
  string help1, help2;
  ifstream input;
  input.open(path_to_input_file);
  if (!input.is_open())
  {
    cout << "File " << path_to_input_file << " can't be opened";
    return false;
  }

  string line;
  while (getline(input, line))
  {
    switch (type_of_line(line))
    {
    case DEFAULT:

      cout << "Syntax error in line " << currentLine << endl;
      return false;
    case IGNORE:
      break;

    case GLOBAL:
      addSymbols(line, symbol_types::GLOBALS);
      break;

    case EXTERN:
      addSymbols(line, symbol_types::EXTERNS);
      break;

    case SECTION:
      addSection(line);
      break;

    case WORD:
      initWords(line);
      break;

    case SKIP:
      skip(line);
      break;

    case END:
      if (currentSection != 0)
      {
        section_table[currentSectionName].size = location_counter;
      }
      return !error;

    case HALT:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      data.push_back(0);
      location_counter++;
      program_counter++;
      break;

    case INT:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      r1 = reg(line.substr(4));
      r2 = 15;
      data.push_back(0x10);
      data.push_back(combineRegs(r1, r2));
      location_counter += 2;
      program_counter += 2;
      break;

    case IRET:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      data.push_back(0x20);
      location_counter++;
      program_counter++;
      break;

    case RET:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      data.push_back(0x40);
      location_counter++;
      program_counter++;
      break;

    case PUSH:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      line = ldst(line);
      r1 = reg(line);
      r2 = reg("sp");
      data.push_back(combineRegs(r1, r2));
      data.push_back(0x12);
      location_counter += 2;
      program_counter += 2;
      break;

    case POP:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      line = ldst(line);
      r1 = reg(line);
      r2 = reg("sp");
      data.push_back(combineRegs(r1, r2));
      data.push_back(0x42);
      location_counter += 2;
      program_counter += 2;
      break;

    case XCHG:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      regs = split(line.substr(5), ',');
      r1 = reg(regs[0]);
      r2 = reg(regs[1]);
      data.push_back(0x60);
      data.push_back(combineRegs(r1, r2));
      location_counter += 2;
      program_counter += 2;
      break;

    case ADD:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      regs = split(line.substr(4), ',');
      r1 = reg(regs[0]);
      r2 = reg(regs[1]);
      data.push_back(0x70);
      data.push_back(combineRegs(r1, r2));
      location_counter += 2;
      program_counter += 2;
      break;

    case SUB:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      regs = split(line.substr(4), ',');
      r1 = reg(regs[0]);
      r2 = reg(regs[1]);
      data.push_back(0x71);
      data.push_back(combineRegs(r1, r2));
      location_counter += 2;
      program_counter += 2;
      break;

    case MUL:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      regs = split(line.substr(4), ',');
      r1 = reg(regs[0]);
      r2 = reg(regs[1]);
      data.push_back(0x72);
      data.push_back(combineRegs(r1, r2));
      location_counter += 2;
      program_counter += 2;
      break;

    case DIV:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      regs = split(line.substr(4), ',');
      r1 = reg(regs[0]);
      r2 = reg(regs[1]);
      data.push_back(0x73);
      data.push_back(combineRegs(r1, r2));
      location_counter += 2;
      program_counter += 2;
      break;

    case CMP:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      regs = split(line.substr(4), ',');
      r1 = reg(regs[0]);
      r2 = reg(regs[1]);
      data.push_back(0x74);
      data.push_back(combineRegs(r1, r2));
      location_counter += 2;
      program_counter += 2;
      break;

    case NOT:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      r1 = reg(line.substr(4));
      r2 = 15;
      data.push_back(0x80);
      data.push_back(combineRegs(r1, r2));
      location_counter += 2;
      program_counter += 2;
      break;

    case AND:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      regs = split(line.substr(4), ',');
      r1 = reg(regs[0]);
      r2 = reg(regs[1]);
      data.push_back(0x81);
      data.push_back(combineRegs(r1, r2));
      location_counter += 2;
      program_counter += 2;
      break;

    case OR:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      regs = split(line.substr(3), ',');
      r1 = reg(regs[0]);
      r2 = reg(regs[1]);
      data.push_back(0x82);
      data.push_back(combineRegs(r1, r2));
      location_counter += 2;
      program_counter += 2;
      break;

    case XOR:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      regs = split(line.substr(4), ',');
      r1 = reg(regs[0]);
      r2 = reg(regs[1]);
      data.push_back(0x83);
      data.push_back(combineRegs(r1, r2));
      location_counter += 2;
      program_counter += 2;
      break;

    case TEST:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      regs = split(line.substr(5), ',');
      r1 = reg(regs[0]);
      r2 = reg(regs[1]);
      data.push_back(0x84);
      data.push_back(combineRegs(r1, r2));
      location_counter += 2;
      program_counter += 2;
      break;

    case SHL:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      regs = split(line.substr(4), ',');
      r1 = reg(regs[0]);
      r2 = reg(regs[1]);
      data.push_back(0x90);
      data.push_back(combineRegs(r1, r2));
      location_counter += 2;
      program_counter += 2;
      break;

    case SHR:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      regs = split(line.substr(4), ',');
      r1 = reg(regs[0]);
      r2 = reg(regs[1]);
      data.push_back(0x91);
      data.push_back(combineRegs(r1, r2));
      location_counter += 2;
      program_counter += 2;
      break;

    case JUMP_LIT_VALUE:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      line = jumps(line);
      data.push_back(0x00);
      data.push_back(0x00);
      immediate = lit(line);
      data.push_back((immediate >> 8) & 0xff);

      location_counter += 4;
      program_counter += 4;
      break;

    case JUMP_LIT_MEM:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      line = jumps(line);
      data.push_back(0x00);
      data.push_back(0x04);
      line = line.substr(1);
      immediate = lit(line);
      data.push_back((immediate)&0xff);
      data.push_back((immediate >> 8) & 0xff);
      location_counter += 4;
      program_counter += 4;
      break;

    case JUMP_REG_IND:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      line = jumps(line);
      help1 = line.substr(2);
      help1.pop_back();
      r1 = 15;
      r2 = reg(help1);
      data.push_back(combineRegs(r1, r2));
      data.push_back(0x02);
      location_counter += 2;
      program_counter += 2;
      break;

    case JUMP_REG_LIT_IND:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      line = jumps(line);
      r1 = 15;
      help1 = line.substr(2, line.find_first_of('+') - 2);
      help2 = line.substr(line.find_first_of('+') + 1);
      help2.pop_back();
      r2 = reg(help1);
      data.push_back(combineRegs(r1, r2));
      data.push_back(0x03);
      immediate = lit(help2);
      data.push_back((immediate)&0xff);
      data.push_back((immediate >> 8) & 0xff);
      location_counter += 4;
      program_counter += 4;
      break;

    case JUMP_REG_VALUE:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      line = jumps(line);
      r1 = 15;
      r2 = reg(line.substr(1));
      data.push_back(combineRegs(r1, r2));
      data.push_back(0x01);
      location_counter += 2;
      program_counter += 2;
      break;

    case JUMP_SYM_VALUE:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      line = jumps(line);
      r1 = r2 = 15;
      data.push_back(combineRegs(r1, r2));
      data.push_back(0x00);
      location_counter += 2;
      program_counter += 2;
      addRelocation(line, ABSOLUTE);
      break;

    case JUMP_SYM_PCREL:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      line = jumps(line);
      line = line.substr(1);
      r1 = 15;
      r2 = 7;
      data.push_back(combineRegs(r1, r2));
      data.push_back(0x05);
      location_counter += 2;
      program_counter += 2;
      addRelocation(line, PC_RELATIVE);
      break;

    case JUMP_SYM_MEM:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      line = jumps(line);
      line = line.substr(1);
      r1 = 15;
      r2 = 15;
      data.push_back(combineRegs(r1, r2));
      data.push_back(0x04);
      location_counter += 2;
      program_counter += 2;
      addRelocation(line, ABSOLUTE);
      break;

    case JUMP_REG_SYM_IND:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      line = jumps(line);
      r1 = 15;
      help1 = line.substr(2, line.find_first_of('+') - 2);
      help2 = line.substr(line.find_first_of('+') + 1);
      help2.pop_back();
      r2 = reg(help1);
      data.push_back(combineRegs(r1, r2));
      data.push_back(0x03);
      location_counter += 2;
      program_counter += 2;
      addRelocation(help2, ABSOLUTE);
      break;

    case LDST_LIT_MEM:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      line = ldst(line);
      regs = split(line, ',');
      immediate = lit(regs[1]);
      r1 = reg(regs[0]);
      r2 = 15;
      data.push_back(combineRegs(r1, r2));
      data.push_back(0x04);
      data.push_back((immediate)&0xff);
      data.push_back((immediate >> 8) & 0xff);
      location_counter += 4;
      program_counter += 4;
      break;

    case LDST_LIT_VAL:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      if (line[0] == 's')
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Store can't be used with immediate adressing have to be inside a section" << endl;
        break;
      }
      line = ldst(line);
      regs = split(line, ',');
      help1 = regs[1].substr(1);
      immediate = lit(help1);
      r1 = reg(regs[0]);
      r2 = 15;
      data.push_back(combineRegs(r1, r2));
      data.push_back(0x00);
      data.push_back((immediate)&0xff);
      data.push_back((immediate >> 8) & 0xff);
      location_counter += 4;
      program_counter += 4;
      break;

    case LDST_REG_IND:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      line = ldst(line);
      regs = split(line, ',');
      help1 = regs[1].substr(1);
      help1.pop_back();
      r1 = reg(regs[0]);
      r2 = reg(help1);
      data.push_back(combineRegs(r1, r2));
      data.push_back(0x02);
      location_counter += 2;
      program_counter += 2;
      break;

    case LDST_REG_LIT_IND:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      line = ldst(line);
      regs = split(line, ',');
      help1 = regs[1].substr(1);
      help1.pop_back();
      r1 = reg(regs[0]);
      regs = split(help1, '+');
      r2 = reg(regs[0]);
      immediate = lit(regs[1]);
      data.push_back(combineRegs(r1, r2));
      data.push_back(0x03);
      data.push_back((immediate)&0xff);
      data.push_back((immediate >> 8) & 0xff);
      location_counter += 4;
      program_counter += 4;
      break;
    case LDST_REG_VALUE:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      line = ldst(line);
      regs = split(line, ',');
      r1 = reg(regs[0]);
      r2 = reg(regs[1]);
      data.push_back(combineRegs(r1, r2));
      data.push_back(0x01);
      location_counter += 2;
      program_counter += 2;
      break;

    case LDST_SYM_VAL:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      if (line[0] == 's')
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Store can't be used with immediate adressing have to be inside a section" << endl;
        break;
      }
      line = ldst(line);
      regs = split(line, ',');
      help1 = regs[1].substr(1);
      r1 = reg(regs[0]);
      r2 = 15;

      data.push_back(combineRegs(r1, r2));
      data.push_back(0x00);
      location_counter += 2;
      program_counter += 2;
      addRelocation(help1, ABSOLUTE);
      break;

    case LDST_SYM_MEM:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      line = ldst(line);
      regs = split(line, ',');
      help1 = regs[1];
      r1 = reg(regs[0]);
      r2 = 15;

      data.push_back(combineRegs(r1, r2));
      data.push_back(0x04);
      location_counter += 2;
      program_counter += 2;
      addRelocation(help1, ABSOLUTE);
      break;

    case LDST_SYM_PCREL:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      line = ldst(line);
      regs = split(line, ',');
      help1 = regs[1].substr(1);
      r1 = reg(regs[0]);
      r2 = 7;

      data.push_back(combineRegs(r1, r2));
      data.push_back(0x03);
      location_counter += 2;
      program_counter += 2;
      addRelocation(help1, PC_RELATIVE);
      break;

    case LDST_REG_SYM_IND:
      if (currentSection == 0)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Instructions have to be inside a section" << endl;
        break;
      }
      line = ldst(line);
      regs = split(line, ',');
      help1 = regs[1];
      help1 = help1.substr(1);
      help1.pop_back();
      r1 = reg(regs[0]);
      regs = split(help1, '+');
      r2 = reg(regs[0]);

      data.push_back(combineRegs(r1, r2));
      data.push_back(0x03);
      location_counter += 2;
      program_counter += 2;
      addRelocation(regs[1], ABSOLUTE);
      break;
    }
    currentLine++;
  }

  cout << "Assembler code has to end with .end directive" << endl;

  return false;
}

void AssemblerToElf::addSymbol(string label)
{
  unordered_map<string, Symbol>::iterator it = symbol_table.find(label);

  if (currentSection == 0)
  {
    cout << "Error in line " << currentLine << endl;
    cout << "Symbols have to be defined in sections" << endl;
    error = true;
    return;
  }
  if (it != symbol_table.end())
  {
    if (it->second.defined)
    {
      cout << "Error in line " << currentLine << endl;
      cout << "Symbol " << label << " defined more than once" << endl;
      error = true;
      return;
    }
    if (it->second.type == EXTERN)
    {
      cout << "Error in line " << currentLine << endl;
      cout << "Symbol " << label << " is already defined as extern" << endl;
      error = true;
      return;
    }
    it->second.offset = location_counter;
    it->second.section = currentSection;
    it->second.section_name = currentSectionName;
    it->second.defined = true;
    if (it->second.type == UNDEFINED)
      it->second.type = LOCAL;
  }
  else
  {
    Symbol s;
    s.defined = true;
    s.type = LOCAL;
    s.id = ++idG_symbols;
    s.name = label;
    s.offset = location_counter;
    s.section = currentSection;
    s.section_name = currentSectionName;
    symbol_table[label] = s;
  }

  Symbol s = symbol_table[label];
  vector<vector<Relocation>::iterator> to_delete;

  for (int i = 0; i < relocation_table.size(); i++)
  {
    if (relocation_table[i].sym_id == s.id)
    {
      relocation_table[i].sym_offset = s.offset;
      relocation_table[i].sym_section = s.section;
      int data_p = relocation_table[i].data_position;
      int val = s.offset;
      if (s.section == relocation_table[i].section_id && relocation_table[i].type == PC_RELATIVE)
      {
        val -= (relocation_table[i].offset + 2);
        to_delete.push_back(relocation_table.begin() + i);
      }
      string forWriting = decToHex2Byte(val);
      data[data_p + 1] = toInt(forWriting[0]) * 16 + toInt(forWriting[1]);
      data[data_p] = toInt(forWriting[2]) * 16 + toInt(forWriting[3]);
    }
  }
  for (int i = 0; i < to_delete.size(); i++)
    relocation_table.erase(to_delete[i]);
}

int lit(string l)
{
  if (l.size() >= 2 && l[1] == 'x')
  {
    return stoi(l.substr(2), NULL, 16);
  }
  return stoi(l);
}

void AssemblerToElf::addRelocation(string word, relocation_type type)
{
  Symbol s;
  Relocation r;
  int val = 0;
  if (symbol_table.find(word) != symbol_table.end())
  {
    s = symbol_table[word];
    val = s.offset;
  }
  else
  {
    s.defined = false;
    s.id = ++idG_symbols;
    s.name = word;
    s.type = UNDEFINED;
    s.section = 0;
    s.offset = 0;
    s.section_name = "UNKNOWNSECTION";
    symbol_table[word] = s;
  }
  if (s.section == currentSection && type == PC_RELATIVE)
  {
    val -= (location_counter + 2);
  }
  else
  {
    r.data_position = program_counter;
    r.offset = location_counter;
    r.section_id = currentSection;
    r.section_name = currentSectionName;
    r.type = type;
    r.sym_id = s.id;
    r.sym_offset = s.offset;
    r.sym_section = s.section;
    r.symbol_name = s.name;
    relocation_table.push_back(r);
  }
  string forWriting = decToHex2Byte(val);
  data.push_back(toInt(forWriting[0]) * 16 + toInt(forWriting[1]));
  data.push_back(toInt(forWriting[2]) * 16 + toInt(forWriting[3]));
  location_counter += 2;
  program_counter += 2;
}

string AssemblerToElf::ldst(string line)
{
  if (line.substr(0, 3) == "ldr")
  {
    data.push_back(0xa0);
    location_counter++;
    program_counter++;
    line = line.substr(4);
    return line;
  }
  if (line.substr(0, 3) == "str")
  {
    data.push_back(0xb0);
    location_counter++;
    program_counter++;
    line = line.substr(4);
    return line;
  }
  if (line.substr(0, 4) == "push")
  {
    data.push_back(0xb0);
    location_counter++;
    program_counter++;
    line = line.substr(5);
    return line;
  }
  if (line.substr(0, 3) == "pop")
  {
    data.push_back(0xa0);
    location_counter++;
    program_counter++;
    line = line.substr(4);
    return line;
  }
}

string AssemblerToElf::jumps(string line)
{
  if (line.substr(0, 4) == "call")
  {
    data.push_back(0x30);
    location_counter++;
    program_counter++;
    line = line.substr(5);
    return line;
  }
  if (line.substr(0, 3) == "jmp")
  {
    data.push_back(0x50);
    location_counter++;
    program_counter++;
    line = line.substr(4);
    return line;
  }
  if (line.substr(0, 3) == "jeq")
  {
    data.push_back(0x51);
    location_counter++;
    program_counter++;
    line = line.substr(4);
    return line;
  }
  if (line.substr(0, 3) == "jne")
  {
    data.push_back(0x52);
    location_counter++;
    program_counter++;
    line = line.substr(4);
    return line;
  }
  if (line.substr(0, 3) == "jgt")
  {
    data.push_back(0x53);
    location_counter++;
    program_counter++;
    line = line.substr(4);
    return line;
  }
}

void AssemblerToElf::skip(string line)
{
  if (currentSection == 0)
  {
    cout << "Error in line " << currentLine << endl;
    cout << "Directive .skip has to be inside a section";
    error = true;
    return;
  }
  int skipping = stoi(line.substr(6, line.size() - 6 + 1));
  for (int i = 0; i < skipping; i++)
  {
    data.push_back(0);
    location_counter++;
    program_counter++;
  }
}

void AssemblerToElf::initWords(string line)
{
  if (currentSection == 0)
  {
    cout << "Error in line " << currentLine << endl;
    cout << "Directive .word has to be inside a section";
    error = true;
    return;
  }
  string words = line.substr(6, line.size() - 6 + 1);
  vector<string> word = split(words, ',');
  for (int i = 0; i < word.size(); i++)
  {
    if (regex_match(word[i], r_symbol))
    {
      addRelocation(word[i], ABSOLUTE);
    }
    else if (regex_match(word[i], r_literal_dec))
    {
      int val = stoi(word[i]);
      string forWriting = decToHex2Byte(val);
      data.push_back(toInt(forWriting[0]) * 16 + toInt(forWriting[1]));
      data.push_back(toInt(forWriting[2]) * 16 + toInt(forWriting[3]));
      location_counter += 2;
      program_counter += 2;
    }
    else
    {
      int val = stoi(word[i].substr(2), NULL, 16);
      string forWriting = decToHex2Byte(val);
      data.push_back(toInt(forWriting[2]) * 10 + toInt(forWriting[3]));
      data.push_back(toInt(forWriting[0]) * 10 + toInt(forWriting[1]));
      location_counter += 2;
      program_counter += 2;
    }
  }
}

void AssemblerToElf::addSection(string line)
{
  string c = line.substr(9, line.size() - 9 + 1);
  if (section_table.find(c) != section_table.end())
  {
    error = true;
    cout << "Error in line " << currentLine << endl;
    cout << "Section " << c << " was already defined" << endl;
  }
  else
  {
    if (idG_sections != 0)
    {
      section_table[currentSectionName].size = location_counter;
    }
    Section s;
    s.id = ++idG_sections;
    s.name = c;
    s.address = program_counter;
    currentSectionName = c;
    currentSection = s.id;
    location_counter = 0;
    section_table[currentSectionName] = s;
  }
}

void AssemblerToElf::addSymbols(string line, symbol_types type)
{
  int startOfSymbols = line.find_first_of(' ');
  line = line.substr(startOfSymbols + 1, line.size() - startOfSymbols + 1);
  vector<string> symbols = split(line, ',');

  for (int i = 0; i < symbols.size(); i++)
  {
    if (symbol_table.find(symbols[i]) != symbol_table.end())
    {
      if (symbol_table[symbols[i]].type == GLOBALS || symbol_table[symbols[i]].type == EXTERNS)
      {
        error = true;
        cout << "Error in line " << currentLine << endl;
        cout << "Symbol " << symbols[i] << " is already defined as global or extern" << endl;
      }
      else
      {
        symbol_table[symbols[i]].type = type;
      }
    }
    else
    {
      Symbol s;
      s.defined = false;
      s.id = ++idG_symbols;
      s.name = symbols[i];
      s.type = type;
      s.offset = 0;
      s.section = 0;
      symbol_table[s.name] = s;
    }
  }
}

vector<string> split(string s, char delim)
{
  vector<string> r;

  int comma;
  while ((comma = s.find_first_of(delim)) != string::npos)
  {
    r.push_back(s.substr(0, comma));
    s = s.substr(comma + 1, s.size() - comma + 1);
  }
  r.push_back(s);
  return r;
}

AssemblerToElf::line_types AssemblerToElf::type_of_line(string &line)
{
  label_line = false;
  line = strip_line(line);

  // is it a label
  if (regex_match(line, label_in_line))
  {
    label_line = true;
    int colon = line.find_first_of(':');
    label = line.substr(0, colon);
    addSymbol(label);
    line = line.substr(colon + 1, line.size() - colon + 1);
  }

  if (line == "" || line == " ")
    return IGNORE;

  // directives
  if (regex_match(line, directive_global))
    return GLOBAL;

  if (regex_match(line, directive_extern))
    return EXTERN;

  if (regex_match(line, directive_section))
    return SECTION;

  if (regex_match(line, directive_skip))
    return SKIP;

  if (regex_match(line, directive_word))
    return WORD;

  if (regex_match(line, directive_ascii))
    return ASCII;

  if (regex_match(line, directive_end))
    return END;

  if (regex_match(line, instruction_halt))
    return HALT;

  if (regex_match(line, instruction_iret))
    return IRET;

  if (regex_match(line, instruction_ret))
    return RET;

  if (regex_match(line, instruction_int))
    return INT;

  if (regex_match(line, instruction_add))
    return ADD;

  if (regex_match(line, instruction_push))
    return PUSH;

  if (regex_match(line, instruction_pop))
    return POP;

  if (regex_match(line, instruction_sub))
    return SUB;

  if (regex_match(line, instruction_mul))
    return MUL;

  if (regex_match(line, instruction_div))
    return DIV;

  if (regex_match(line, instruction_cmp))
    return CMP;

  if (regex_match(line, instruction_not))
    return NOT;

  if (regex_match(line, instruction_and))
    return AND;

  if (regex_match(line, instruction_or))
    return OR;

  if (regex_match(line, instruction_xor))
    return XOR;

  if (regex_match(line, instruction_test))
    return TEST;

  if (regex_match(line, instruction_shl))
    return SHL;

  if (regex_match(line, instruction_shr))
    return SHR;

  if (regex_match(line, instruction_jump_lit_value))
    return JUMP_LIT_VALUE;

  if (regex_match(line, instruction_jump_lit_mem))
    return JUMP_LIT_MEM;

  if (regex_match(line, instruction_jump_reg_ind))
    return JUMP_REG_IND;

  if (regex_match(line, instruction_jump_reg_lit_ind))
    return JUMP_REG_LIT_IND;

  if (regex_match(line, instruction_jump_reg_sym_ind))
    return JUMP_REG_SYM_IND;

  if (regex_match(line, instruction_jump_reg_value))
    return JUMP_REG_VALUE;

  if (regex_match(line, instruction_jump_sym_mem))
    return JUMP_SYM_MEM;

  if (regex_match(line, instruction_jump_sym_value))
    return JUMP_SYM_VALUE;

  if (regex_match(line, instruction_jump_symbol_pcrel))
    return JUMP_SYM_PCREL;

  if (regex_match(line, instruction_ldst_lit_mem))
    return LDST_LIT_MEM;

  if (regex_match(line, instruction_ldst_lit_value))
    return LDST_LIT_VAL;

  if (regex_match(line, instruction_ldst_reg_ind))
    return LDST_REG_IND;

  if (regex_match(line, instruction_ldst_reg_lit_ind))
    return LDST_REG_LIT_IND;

  if (regex_match(line, instruction_ldst_reg_sym_ind))
    return LDST_REG_SYM_IND;

  if (regex_match(line, instruction_ldst_reg_value))
    return LDST_REG_VALUE;

  if (regex_match(line, instruction_ldst_sym_mem))
    return LDST_SYM_MEM;

  if (regex_match(line, instruction_ldst_sym_value))
    return LDST_SYM_VAL;

  if (regex_match(line, instruction_ldst_symbol_pcrel))
    return LDST_SYM_PCREL;

  return DEFAULT;
}

string AssemblerToElf::strip_line(string line)
{
  line = regex_replace(line, comment, "$1");
  line = regex_replace(line, swap_with_single_space, " ");
  line = regex_replace(line, comma_space, ",");
  line = regex_replace(line, colon_space, ":");
  line = regex_replace(line, plus_space, "+");
  line = regex_replace(line, space_plus, "+");
  line = trim(line);
  return line;
}

string AssemblerToElf::trim(const string &str)
{
  char whitespace = ' ';
  const auto strBegin = str.find_first_not_of(whitespace);
  if (strBegin == std::string::npos)
    return ""; // no content

  const auto strEnd = str.find_last_not_of(whitespace);
  const auto strRange = strEnd - strBegin + 1;

  return str.substr(strBegin, strRange);
}

string decToHex2Byte(int number)
{
  int remainder, product = 1, i = 3;
  string hex_dec = "0000";
  while (number != 0)
  {
    remainder = number % 16;
    char ch;
    if (remainder >= 10)
      ch = remainder + 55;
    else
      ch = remainder + 48;
    hex_dec[i--] = ch;

    number = number / 16;
    product *= 10;
  }
  return hex_dec;
}

void AssemblerToElf::printSymbols()
{
  cout << "Symbol table" << endl;
  unordered_map<string, Symbol>::iterator it = symbol_table.begin();
  for (it; it != symbol_table.end(); it++)
  {
    cout << it->first << "\t" << it->second.id << "\t" << it->second.offset << "\t" << it->second.section << endl;
  }
  cout << endl;
}
void AssemblerToElf::printSections()
{
  cout << "Section table" << endl;
  unordered_map<string, Section>::iterator it = section_table.begin();
  for (it; it != section_table.end(); it++)
  {
    cout << it->first << "\t" << it->second.id << "\t" << it->second.size << endl;
  }
  cout << endl;
}
void AssemblerToElf::printRelocations()
{
  cout << "Relocation table" << endl;
  vector<Relocation>::iterator it = relocation_table.begin();
  for (it; it != relocation_table.end(); it++)
  {
    cout << it->sym_id << "\t" << it->sym_section << "\t" << it->section_name << "\t" << it->offset << endl;
  }
  cout << endl;
}

void AssemblerToElf::formOutputFile()
{
  ofstream print(path_to_output_file);

  if (!print.is_open())
  {
    cout << "Error when trying to write to " << path_to_output_file << endl;
    return;
  }
  // print section data
  print << section_table.size() << endl;
  for (unordered_map<string, Section>::iterator it = section_table.begin(); it != section_table.end(); it++)
  {
    print << it->second.id << ' ' << it->second.name << ' ' << it->second.address << ' ' << it->second.size << endl;
  }

  print << symbol_table.size() << endl;
  for (unordered_map<string, Symbol>::iterator it = symbol_table.begin(); it != symbol_table.end(); it++)
  {
    if (it->second.defined == false)
      it->second.section_name = "UNKNOWNSECTION";
    print << it->second.id << ' ' << it->second.name << ' ' << it->second.defined << ' '
          << it->second.section << ' ' << it->second.offset << ' ' << it->second.section_name << endl;
  }

  print << relocation_table.size() << endl;
  for (int i = 0; i < relocation_table.size(); i++)
  {
    print << relocation_table[i].type << ' ';
    print << relocation_table[i].data_position << ' ';
    print << relocation_table[i].offset << ' ';
    print << relocation_table[i].section_id << ' ';
    print << relocation_table[i].section_name << ' ';
    print << relocation_table[i].sym_id << ' ';
    print << relocation_table[i].sym_offset << ' ';
    print << relocation_table[i].sym_section << ' ';
    print << relocation_table[i].symbol_name << endl;
  }

  print << data.size() << endl;
  for (int i = 0; i < data.size(); i++)
  {
    print << setw(2) << setfill('0') << hex << (int)(data[i] & 0xff);
  }
}
void AssemblerToElf::printData()
{
  cout << "Data table" << endl;
  for (int i = 0; i < data.size(); i++)
  {
    if (i % 8 == 0)
    {
      cout << endl;
      cout << setw(2) << setfill('0') << hex << (int)(i & 0xff) << ":  ";
    }
    cout << setw(2) << setfill('0') << hex << (int)(data[i] & 0xff) << ' ';
  }
  cout << endl;
  cout << endl;
}
int AssemblerToElf::reg(string r)
{
  if (r[0] == 'r')
  {
    return r[1] - '0';
  }
  if (r == "sp")
    return 6;
  if (r == "pc")
    return 7;
  if (r == "psw")
    return 8;
  return -1;
}
char AssemblerToElf::combineRegs(int r1, int r2)
{
  return (r1 << 4) | r2;
}
int toInt(char c)
{
  switch (c)
  {
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    return c - '0';
  case 'a':
  case 'A':
    return 10;
  case 'b':
  case 'B':
    return 11;
  case 'C':
  case 'c':
    return 12;
  case 'd':
  case 'D':
    return 13;
  case 'e':
  case 'E':
    return 14;
  case 'F':
  case 'f':
    return 15;
  }
}