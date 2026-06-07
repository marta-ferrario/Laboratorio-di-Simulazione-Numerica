#include <iostream>
#include "system.h"

using namespace std;

int main (int argc, char *argv[]){

  int nconf = 1;
  System SYS;
  SYS.initialize();
  SYS.initialize_properties();
  SYS.block_reset(0);

  for(int i=0; i < SYS.get_nbl(); i++){
    for(int j=0; j < SYS.get_nsteps(); j++){
        SYS.step();
        SYS.measure();
        SYS.write_observable(j); // stampa solo il valore istantaneo


    }
    SYS.averages(i+1);
    SYS.block_reset(i+1);

    if(i%1000==0){
      cout << i << endl; 
    }


  }


  SYS.finalize();

  return 0;
}