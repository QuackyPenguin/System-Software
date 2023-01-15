#include "../inc/linker.h"
#include <iomanip>

ElfToExe::ElfToExe(string output_file) : path_to_output_file(output_file) {}

bool ElfToExe::addFile(string path_to_input_file)
{
  ifstream scan(path_to_input_file);

  if (!scan.is_open())
  {
    cout << "Error while trying to open " << path_to_input_file << endl;
    return false;
  }

  int num_sections;
  scan >> num_sections;

  unordered_map<string, Section> section_table;

  for (int i = 0; i < num_sections; i++)
  {
    Section s;
    scan >> s.id >> s.name >> s.address >> s.size;
    section_table[s.name] = s;
  }

  int num_symbols;
  scan >> num_symbols;

  unordered_map<string, Symbol> symbol_table;

  for (int i = 0; i < num_symbols; i++)
  {
    Symbol s;
    scan >> s.id >> s.name >> s.defined >> s.section >> s.offset >> s.section_name;
    symbol_table[s.name] = s;
  }

  int relocation_size;
  scan >> relocation_size;

  vector<Relocation> relocation_table;

  for (int i = 0; i < relocation_size; i++)
  {
    Relocation rt;
    int type;
    scan >> type;
    rt.type = static_cast<relocation_type>(type);
    scan >> rt.data_position;
    scan >> rt.offset;
    scan >> rt.section_id;
    scan >> rt.section_name;
    scan >> rt.sym_id;
    scan >> rt.sym_offset;
    scan >> rt.sym_section;
    scan >> rt.symbol_name;
    relocation_table.push_back(rt);
  }

  int num_data;
  scan >> num_data;

  vector<char> data;

  for (int i = 0; i < num_data; i++)
  {
    char c1, c2;
    scan >> c1 >> c2;
    data.push_back(hexToChar(c1, c2));
  }

  Elf e;
  e.data = data;
  e.relocation_table = relocation_table;
  e.section_table = section_table;
  e.symbol_table = symbol_table;

  elfs.push_back(e);

  /*
/*
  cout<<data.size()<<endl;
  for(int i=0;i<data.size();i++){
    cout << setw(2) << setfill('0') << hex << (int)(data[i] & 0xff);
  }


  */

  return true;
}

bool ElfToExe::link()
{
  for (int i = 0; i < elfs.size(); i++)
  {
    for (unordered_map<string, Section>::iterator it = elfs[i].section_table.begin(); it != elfs[i].section_table.end(); it++)
    {
      if (it->second.id == 0)
        continue;
      it->second = addSection(it->second, i);
    }
  }

  for (int i = 0; i < elfs.size(); i++)
  {
    for (unordered_map<string, Symbol>::iterator it = elfs[i].symbol_table.begin(); it != elfs[i].symbol_table.end(); it++)
    {
      if (it->second.defined)
      {
        if (linked.symbol_table.find(it->second.name) != linked.symbol_table.end())
        {
          cout << "Multiple definitions of symbol " << it->second.name << endl;
          return false;
        }
        Symbol s;
        s.defined = true;
        s.id = ++idG_symbols;
        s.name = it->second.name;
        s.new_section = it->second.new_section;
        s.offset = it->second.offset;
        s.section = it->second.section;
        s.type = it->second.type;
        s.section_name = it->second.section_name;
        linked.symbol_table[it->second.name] = s;
      }
    }
  }

  for (int i = 0; i < elfs.size(); i++)
  {
    for (int j = 0; j < elfs[i].relocation_table.size(); j++)
    {
      int location, value;
      location = elfs[i].relocation_table[j].data_position;
      if (elfs[i].relocation_table[j].type == ABSOLUTE)
      {
        value = linked.symbol_table[elfs[i].relocation_table[j].symbol_name].offset;
      }
      else
      {
        value = linked.symbol_table[elfs[i].relocation_table[j].symbol_name].offset - location - 2;
      }
      linked.data[location] = (value)&0xff;
      linked.data[location+1] = (value >> 8) & 0xff;
    }
  }

  return true;
}

void ElfToExe::formOutputFile()
{
  ofstream print(path_to_output_file);
  string helper_file=path_to_output_file.substr(0, path_to_output_file.find_last_of('.'))+".o";
  ofstream print1(helper_file);

  if (!print.is_open())
  {
    cout << "Unable to open output file" << endl;
    return;
  }

  for (int i = 0; i < linked.data.size(); i++)
  {
    if (i % 8 == 0)
    {
      print << endl;
      print << setw(4) << setfill('0') << hex << (int)(i);
      print << ": ";
    }
    print << setw(2) << setfill('0') << hex << (int)(linked.data[i] & 0xff);
    print << " ";
  }

  print1 <<dec<< linked.data.size() << endl;
  for (int i = 0; i < linked.data.size(); i++)
  {
    print1 << setw(2) << setfill('0') << hex << (int)(linked.data[i] & 0xff);
  }
  print1<<endl<<dec<<linked.symbol_table["my_start"].offset;

  print.close();
}

Section ElfToExe::addSection(Section s, int i)
{
  if (linked.section_table.find(s.name) == linked.section_table.end())
  {
    // nema jos sekcije sa tim imenom
    Section newSection;
    newSection.id = ++idG_sections;
    newSection.size = s.size;
    newSection.name = s.name;
    newSection.address = program_counter;

    linked.section_table[s.name] = newSection;

    // copy data
    for (int k = 0; k < s.size; k++)
    {
      linked.data.push_back(elfs[i].data[k + s.address]);
      program_counter++;
    }

    s.address = program_counter - s.size;

    // update symbol table
    for (unordered_map<string, Symbol>::iterator it = elfs[i].symbol_table.begin(); it != elfs[i].symbol_table.end(); it++)
    {
      if (it->second.section == s.id)
      {
        it->second.new_section = newSection.id;
        it->second.section_name = newSection.name;
        it->second.offset += s.address;
      }
    }

    // update relocation table
    for (int j = 0; j < elfs[i].relocation_table.size(); j++)
    {
      if (elfs[i].relocation_table[j].section_id == s.id)
      {
        elfs[i].relocation_table[j].new_section_id = newSection.id;
        elfs[i].relocation_table[j].data_position = elfs[i].relocation_table[j].offset + s.address;
      }
    }
  }
  else
  {
    // ima sekcije sa tim imenom
    int offset_within_section = linked.section_table[s.name].size;

    Section newSection = linked.section_table[s.name];
    linked.section_table[s.name].size += s.size;

    // copy data
    for (int k = 0; k < s.size; k++)
    {
      linked.data.push_back(elfs[i].data[k + s.address]);
      program_counter++;
    }

    s.address = program_counter - s.size;

    // update symbol table
    for (unordered_map<string, Symbol>::iterator it = elfs[i].symbol_table.begin(); it != elfs[i].symbol_table.end(); it++)
    {
      if (it->second.section == s.id)
      {
        it->second.new_section = newSection.id;
        it->second.section_name = newSection.name;
        it->second.offset += offset_within_section + linked.section_table[s.name].address;
      }
    }

    // update relocation table
    for (int j = 0; j < elfs[i].relocation_table.size(); j++)
    {
      if (elfs[i].relocation_table[j].section_id == s.id)
      {
        elfs[i].relocation_table[j].new_section_id = newSection.id;
        elfs[i].relocation_table[j].data_position = elfs[i].relocation_table[j].offset + s.address;
      }
    }
  }

  s.id = 0;

  for (int k = i + 1; k < elfs.size(); k++)
  {
    for (unordered_map<string, Section>::iterator it = elfs[k].section_table.begin(); it != elfs[k].section_table.end(); it++)
    {
      if (it->second.id == 0 || it->second.name != s.name)
        continue;
      it->second = addSection(it->second, k);
    }
  }

  return s;
}

int hexToInt(char c)
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
    break;
  case 'a':
    return 10;
    break;
  case 'b':
    return 11;
    break;
  case 'c':
    return 12;
    break;
  case 'd':
    return 13;
    break;
  case 'e':
    return 14;
    break;
  case 'f':
    return 15;
    break;
  }
}

char hexToChar(char c1, char c2)
{
  int a, b;
  a = hexToInt(c1);
  b = hexToInt(c2);
  return (a << 4) | b;
}