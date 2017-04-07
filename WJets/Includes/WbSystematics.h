
#ifndef ____WbSystematics__
#define ____WbSystematics__

#include <iostream>
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <RooUnfoldResponse.h>
#include <string>
#include <vector>
using namespace std;


void computeWbSyst(string leptonFlavor = "SMu", string var = "ZNGoodJets_Zexc", bool closureTest = false);

void UnfoldSystWb(const int *sel, int nsel, string leptonFlavor, string variable, bool logZ, bool decrease, int SVDkterm, int Bayeskterm, bool closureTest = 0, int JetPtMin = 30, int JetPtMax = 0, bool doFlat = 0, bool doVarWidth = 1);


#endif /* defined(____TTbarSystematics__) */
