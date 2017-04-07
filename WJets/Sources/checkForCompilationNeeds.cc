#include <iostream>
#include <fstream>
#include <TROOT.h>
#include <TSystem.h>

using namespace std;

void checkForCompilationNeeds(string srcdir, vector<string> sources)
{ 
  //--- Checks if we need to recompile ---
  Long_t mTime[2] = {0,0};
  unsigned int nSources = sources.size();
  for (unsigned int i(0); i < nSources; i++){
    mTime[0] = 0;
    mTime[1] = 0;
    system(string("stat -c \%X " + srcdir + sources[i] + ".cc > tmpModTime.txt").c_str());
    system(string("stat -c \%X " + srcdir + sources[i] + "_cc.so >> tmpModTime.txt").c_str());

    ifstream read("tmpModTime.txt");
    for (unsigned short j(0); j < 2; j++){
      read >> mTime[j];
    }
    cout << sources[i] << ": .cc --> " << mTime[0] << "  .so --> " << mTime[1] << endl;
    if (mTime[0] > mTime[1]){
      cout << "compiling " << sources[i] << endl;
      gROOT->ProcessLine(string(".L " + srcdir + sources[i] + ".cc++").c_str());

    }
  }
  system("rm tmpModTime.txt");

}
