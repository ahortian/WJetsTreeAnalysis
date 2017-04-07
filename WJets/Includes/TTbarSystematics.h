
#ifndef ____TTbarSystematics__
#define ____TTbarSystematics__

#include <iostream>
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <RooUnfoldResponse.h>
#include <string>
#include <vector>
using namespace std;


void computeTTbarSyst(string leptonFlavor = "SMu", string var = "ZNGoodJets_Zexc", bool closureTest = false);

void UnfoldSystTTbar(const int *sel, int nsel, string leptonFlavor, string variable, bool logZ, bool decrease, int SVDkterm, int Bayeskterm, bool closureTest = 0, int JetPtMin = 30, int JetPtMax = 0, bool doFlat = 0, bool doVarWidth = 1);


void getStatisticsVar(string leptonFlavor = "Muons", string variable = "ZNGoodJets_Zexc", int JetPtMin = 30, int JetPtMax = 0, bool doFlat = false , bool doVarWidth = true, int doQCD = 0 , bool doSSign = false , bool doInvMassCut = false , int MET = 0 , int doBjets = 0 );

TH1D* SFAnalyzeTTbar(string leptonFlavor = "Muons", string variable = "ZNGoodJets_Zexc", int JetPtMin = 30, int JetPtMax = 0, bool doFlat = false , bool doVarWidth = true, int doQCD = 0 , bool doSSign = false , bool doInvMassCut = false , int MET = 0 , int doBjets = 0 );


#endif /* defined(____TTbarSystematics__) */
