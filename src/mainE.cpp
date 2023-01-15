#include "../inc/emulator.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]){
  if(argc!=2){
    cout<<"Bad input"<<endl;
    return -1;
  }

  Emulator e;
  if(!e.emulate(argv[1])){
    cout<<"!!! ERROR DURING EMULATION !!!"<<endl;
    return -1;
  }
}