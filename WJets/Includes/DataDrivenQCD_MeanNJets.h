#ifndef _DataDrivenQCD_MeanNJets_h_
#define _DataDrivenQCD_MeanNJets_h_

#include <iostream>

using namespace std;

void DataDrivenQCD_MeanNJets(string leptonFlavor = "SMu", int METcut = 0, int doBJets = 0);

void FuncOpenAllFiles(string leptonFlavor = "SMu",  int METcut = 0, bool doFlat = false, bool doVarWidth = true , int doBJets = 0 );

vector<string> getVectorOfHistoNames_MeanNJets();

TH2D* get2DHisto(TFile *File, string variable = "MeanNJetsHT_Zinc1jet");

void FuncQCD_MeanNJets(string variable = "MeanNJetsHT_Zinc1jet");



#endif

