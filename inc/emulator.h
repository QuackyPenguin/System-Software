#ifndef EMULATOR_H
#define EMULATOR_H

#include <map>
#include <vector>

#define i_halt 0x00
#define i_int 0x10
#define i_iret 0x20
#define i_call 0x30
#define i_ret 0x40
#define i_jmp 0x50
#define i_jeq 0x51
#define i_jne 0x52
#define i_jgt 0x53
#define i_xchg 0x60
#define i_add 0x70
#define i_sub 0x71
#define i_mul 0x72
#define i_div 0x73
#define i_cmp 0x74
#define i_not 0x80
#define i_and 0x81
#define i_or 0x82
#define i_xor 0x83
#define i_test 0x84
#define i_shl 0x90
#define i_shr 0x91
#define i_ldr 0xa0
#define i_str 0xb0

#define PC regs[7]
#define PSW regs[8]
#define SP regs[6]

#define loadZ PSW&0x1
#define loadO PSW&0x2
#define loadC PSW&0x4
#define loadN PSW&0x8

#define writeZ(X) {PSW=PSW|(X<<0);}
#define writeO(X) {PSW=PSW|(X<<1);}
#define writeC(X) {PSW=PSW|(X<<2);}
#define writeN(X) {PSW=PSW|(X<<3);}

#define PUSH(X)      \
  {                  \
    SP -= 2;         \
    writeReg(X, SP); \
  }
#define POP(X)      \
  {                 \
    loadReg(X, SP); \
    SP += 2;        \
  }

using namespace std;

class Emulator
{
public:
  Emulator();
  bool emulate(string path_to_input_file);
  bool read(string path_to_input_file);
  void byte(unsigned char &rD, unsigned char &rS);
  void loadReg(short &r, short address);
  void writeReg(short &r, short address);
  short getPayload();
  short jumpAddress(unsigned char regS, unsigned char type);

  map<short, unsigned char> program;
  vector<short> regs;
};

int hexToInt(char c);
char hexToChar(char c1, char c2);

#endif