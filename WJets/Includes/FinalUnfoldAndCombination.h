#ifndef _FinalUnfold_h_
#define _FinalUnfold_h_
#include <TH1.h>
#include "getFilesAndHistograms.h"
#include <iostream>

using namespace std;
/*
TH1D* TH1D(string name, string title, string xTitle, int nBins, double xLow, double xUp){
    TH1D* hist = new TH1D(name.c_str(), title.c_str(), nBins, xLow, xUp);
    hist->GetXaxis()->SetTitle(xTitle.c_str());
    hist->GetYaxis()->SetTitle("# Events");
    hist->SetOption("HIST");
    listOfHistograms.push_back(hist);
    return hist;
}
*/
//TH1D* newTH1D(string, string, string, int, double, double);
//TH2D* newTH2D(string name, string title, int nBinsX, double xLow, double xUp, int nBinsY, double yLow, double yUp);
void getAllFiles( string leptonFlavor = "DMu", bool doFlat = false , bool doVarWidth = true);
void closeAllFiles();
void FinalUnfoldAndCombination( int start = 1 , int finish = 40);
void FuncUnfold(string variable = "ZNGoodJets_Zexc", int UsedKtermBayes = 5, bool doFlat = false , bool doVarWidth = true);

#endif
