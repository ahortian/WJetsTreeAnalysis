#ifndef _Systematics_h_
#define _Systematics_h_

#include <iostream>
#include <TH1.h>
#include <TFile.h>

using namespace std;

void Systematics();
void systematics(string variable, bool log = true, bool decrease = true, string leptonFlavor = "Muons", int JetPtMin = 30, int JetPtMax = 0, bool doFlat = 0, bool doVarWidth = 1);
void plotSystematics(int JetPtMin, string Syst, TH1D *hcent, TH1D *hup, TH1D *hdown, TFile *outputFile, string leptonFlavor, string energy, bool log = true, bool decrease = true);
//void writeSystematicsTexFile(string leptonFlavor, string energy, string variable, ofstream& texFile);
//----------------------------------------------------------

#endif
