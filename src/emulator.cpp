#include "../inc/emulator.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <bitset>

Emulator::Emulator()
{
  regs.resize(9);
}

bool Emulator::emulate(string path_to_input_file)
{
  read(path_to_input_file);
  if (!read(path_to_input_file))
  {
    return false;
  }

  bool halt = false;
  unsigned char rS, rD;
  unsigned char type, up;
  short oldPC, temp;

  while (!halt)
  {
    int instruction = program[PC++];
    instruction = instruction & 0xff;

    switch (instruction)
    {
    case i_halt:
      cout << "----------------------------------------------" << endl;
      cout << "Emulated processor executed halt instruction" << endl;
      cout << "Emulated processor state: psw=0B" << bitset<16>(PSW) << endl;
      for (int i = 0; i < 8; i++)
      {
        cout << "r" << i << "=0x" << setfill('0') << setw(4) << hex << regs[i];
        if (i % 4 == 3)
          cout << endl;
        else
          cout << "\t";
      }
      halt = true;
      break;

    case i_int:
      byte(rD, rS);
      PUSH(PC)
      PUSH(PSW);
      loadReg(PC, (regs[rD] % 8) * 2);
      break;

    case i_iret:
      POP(PSW);
      POP(PC);
      break;

    case i_call:
      byte(rD, rS);
      byte(up, type);
      oldPC = jumpAddress(rS, type);
      PUSH(PC);
      PC = oldPC;
      break;

    case i_ret:
      POP(PC);
      break;

    case i_jmp:
      byte(rD, rS);
      byte(up, type);
      PC = jumpAddress(rS, type);
      break;

    case i_jeq:
      byte(rD, rS);
      byte(up, type);
      oldPC = PC;
      PC = jumpAddress(rS, type);
      if (!loadZ)
        PC = oldPC;
      break;

    case i_jne:
      byte(rD, rS);
      byte(up, type);
      oldPC = PC;
      PC = jumpAddress(rS, type);
      if (loadZ)
        PC = oldPC;
      break;

    case i_jgt:
      byte(rD, rS);
      byte(up, type);
      oldPC = PC;
      PC = jumpAddress(rS, type);
      if (loadZ || loadN != loadO)
        PC = oldPC;
      break;

    case i_xchg:
      byte(rD, rS);
      temp = regs[rD];
      regs[rD] = regs[rS];
      regs[rS] = temp;
      break;

    case i_add:
      byte(rD, rS);
      regs[rD] = regs[rD] + regs[rS];
      break;

    case i_sub:
      byte(rD, rS);
      regs[rD] = regs[rD] - regs[rS];
      break;

    case i_mul:
      byte(rD, rS);
      regs[rD] = regs[rD] * regs[rS];
      break;

    case i_div:
      byte(rD, rS);
      regs[rD] = regs[rD] / regs[rS];
      break;

    case i_cmp:
      byte(rD, rS);
      writeZ(regs[rD] == regs[rS] ? 1 : 0);
      writeN(regs[rD] - regs[rS] < 0 ? 1 : 0);
      writeC(regs[rD] < regs[rS] ? 1 : 0);
      writeO(((regs[rD] > 0 && regs[rS] < 0) && (regs[rD] - regs[rS] < 0)) ||
                     ((regs[rD] < 0 && regs[rS] > 0) && (regs[rD] - regs[rS] > 0))
                 ? 1
                 : 0);
      break;

    case i_not:
      byte(rD, rS);
      regs[rD] = ~regs[rD];
      break;

    case i_and:
      byte(rD, rS);
      regs[rD] = regs[rD] & regs[rS];
      break;

    case i_or:
      byte(rD, rS);
      regs[rD] = regs[rD] | regs[rS];
      break;

    case i_xor:
      byte(rD, rS);
      regs[rD] = regs[rD] ^ regs[rS];
      break;

    case i_test:
      byte(rD, rS);
      temp = regs[rD] & regs[rS];
      writeZ(temp == 0 ? 1 : 0);
      writeN(temp & 0x8000 != 0 ? 1 : 0);
      break;

    case i_shl:
      byte(rD, rS);
      temp = regs[rD];
      regs[rD] = regs[rD] << regs[rS];
      writeC(temp & 0x8000 != 0 && regs[rS] != 0 ? 1 : 0);
      writeN(regs[rD] < 0 ? 1 : 0);
      writeZ(regs[rD] == 0 ? 1 : 0);
      break;

    case i_shr:
      byte(rD, rS);
      temp = regs[rD];
      regs[rD] = regs[rD] >> regs[rS];
      writeC(temp & 0x0001 != 0 && regs[rS] != 0 ? 1 : 0);
      writeN(regs[rD] < 0 ? 1 : 0);
      writeZ(regs[rD] == 0 ? 1 : 0);
      break;

    case i_ldr:
      byte(rD, rS);
      byte(up, type);
      switch (type)
      {
      case 0x0:
        regs[rD] = getPayload();
        break;

      case 0x1:
        regs[rD] = regs[rS];
        break;

      case 0x2:
        if (up == 0x1)
        {
          regs[rS] -= 2;
        }
        else if (up == 0x2)
        {
          regs[rS] += 2;
        }
        loadReg(regs[rD], regs[rS]);
        if (up == 0x3)
        {
          regs[rS] -= 2;
        }
        else if (up == 0x4)
        {
          regs[rS] += 2;
        }
        break;

      case 0x3:
        if (up == 0x1)
        {
          regs[rS] -= 2;
        }
        else if (up == 0x2)
        {
          regs[rS] += 2;
        }
        loadReg(regs[rD], regs[rS] + getPayload());
        if (up == 0x3)
        {
          regs[rS] -= 2;
        }
        else if (up == 0x4)
        {
          regs[rS] += 2;
        }
        break;

      case 0x4:
        loadReg(regs[rD], getPayload());
        break;

      case 0x5:
        regs[rD] = regs[rS] + getPayload();
        break;
      }
      break;

    case i_str:
      byte(rD, rS);
      byte(up, type);
      switch (type)
      {
      case 0x0:
        cout << "Cannot use instruction store with immediate addressing" << endl;
        return false;

      case 0x1:
        regs[rS] = regs[rD];
        break;

      case 0x2:
        if (up == 0x1)
        {
          regs[rS] -= 2;
        }
        else if (up == 0x2)
        {
          regs[rS] += 2;
        }
        writeReg(regs[rD], regs[rS]);
        if (up == 0x3)
        {
          regs[rS] -= 2;
        }
        else if (up == 0x4)
        {
          regs[rS] += 2;
        }
        break;

      case 0x3:
        if (up == 0x1)
        {
          regs[rS] -= 2;
        }
        else if (up == 0x2)
        {
          regs[rS] += 2;
        }
        writeReg(regs[rD], regs[rS] + getPayload());
        if (up == 0x3)
        {
          regs[rS] -= 2;
        }
        else if (up == 0x4)
        {
          regs[rS] += 2;
        }
        break;

      case 0x4:
        writeReg(regs[rD], getPayload());
        break;

      case 0x5:
        cout << "Cannot use register direct with payload with instruction store" << endl;
        return false;
      }
      break;

    default:
      for (int i = 0; i < 8; i++)
      {
        cout << "r" << i << ": 0x" << setw(4) << hex << regs[i] << "\t";
        if (i % 4 == 3)
          cout << endl;
      }
      return false;
      break;
    }
  }

  return true;
}

short Emulator::jumpAddress(unsigned char rS, unsigned char type)
{
  short address;

  switch (type)
  {
  case 0x0:
    return getPayload();

  case 0x1:
    return regs[rS];

  case 0x2:
    loadReg(address, regs[rS]);
    return address;

  case 0x3:
    loadReg(address, regs[rS] + getPayload());
    return address;

  case 0x4:
    loadReg(address, getPayload());
    return address;

  case 0x5:
    return getPayload() + regs[rS];
  }
}

short Emulator::getPayload()
{
  unsigned char dataL = program[PC++];
  unsigned char dataH = program[PC++];
  short r = (dataH << 8) & 0xff00;
  r = r | (dataL & 0x00ff);
  return r;
}

void Emulator::writeReg(short &r, short address)
{
  unsigned char dataL = r & 0x00ff;
  unsigned char dataH = (r >> 8) & 0x00ff;
  program[address] = dataL;
  program[address + 1] = dataH;
}

void Emulator::loadReg(short &r, short address)
{
  unsigned char dataL = program[address];
  unsigned char dataH = program[address + 1];
  r = (dataH << 8) & 0xff00;
  r = r | (dataL & 0x00ff);
}

void Emulator::byte(unsigned char &rD, unsigned char &rS)
{
  char rDS = program[PC++];
  rD = rDS >> 4;
  rS = rDS & 0xf;
}

bool Emulator::read(string path_to_input_file)
{
  int size;
  path_to_input_file = path_to_input_file.substr(0, path_to_input_file.find_last_of('.')) + ".o";
  ifstream scan(path_to_input_file);

  if (!scan.is_open())
  {
    cout << "Cannot open " << path_to_input_file << endl;
    return false;
  }

  scan >> size;
  char c1, c2;
  for (int i = 0; i < size; i++)
  {
    scan >> c1 >> c2;

    program[i] = hexToChar(c1, c2);
  }

  scan >> PC;

  return true;
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