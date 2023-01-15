#include <iostream>
#include <string>
#include "assembler.h"

using namespace std;

int main(int argc, char* argv[]){
  if(argc<4) {
    cout<<"Bad input";
    return -1;
  }
  string option=argv[1];
  if(option!="-o"){
    cout<<"Output option is mandatory";
    return -2;
  }

  AssemblerToElf a(argv[3], argv[2]);

  if(!a.compileAssembler()){
    cout<<"Assembling unsuccessful"<<endl;
    return 1;
  }
  a.formOutputFile();
  return 0;
}