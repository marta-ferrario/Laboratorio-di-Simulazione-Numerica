#include <iostream>
#include "system.h"

using namespace std;

int main (int argc, char *argv[]){
 
  int nconf = 1;
  System SYS;

  for(double t=0.5; t<2.1; t=t+0.1){
      SYS.set_temperature(t); 
      SYS.initialize();

      SYS.initialize_properties();
      SYS.block_reset(0);

      cout << t << endl; 
      SYS.set_temperature(t);

      // EQUILIBRAZIONE
      SYS.equilibrate(20000);

      for(int i=0; i < SYS.get_nbl(); i++){ //loop over blocks
        for(int j=0; j < SYS.get_nsteps(); j++){ //loop over steps in a block
          SYS.step();
          SYS.measure();
    //      if(j%50 == 0){
            // SYS.write_XYZ(nconf); //Write actual configuration in XYZ format //Commented to avoid "filesystem full"! 
            // nconf++;
    //      }
        }
        SYS.averages(i+1);
        SYS.block_reset(i+1);
      }
      SYS.finalize();
      }
    return 0;
  }


  