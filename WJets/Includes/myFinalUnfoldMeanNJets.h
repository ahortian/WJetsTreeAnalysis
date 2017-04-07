#ifndef _myFinalUnfoldMeanNJets_h_
#define _myFinalUnfoldMeanNJets_h_
#include "getFilesAndHistograms.h"
#include <iostream>

using namespace std;

void myFinalUnfoldMeanNJets(string var = "MeanNJetsHT_Zinc1jet", string leptonFlavor = "SMu", string unfAlg = "Bayes");

void FuncUnfold(string leptonFlavor = "SMu", bool isMuon = true, string unfAlg = "Bayes", string variable = "MeanNJetsHT_Zinc1jet", int UsedKtermBayes = 5, int UsedKtermSVD = 5, bool doFlat = false , bool doVarWidth = true);



TH1D* getErrorRespToy2D(int myNToys, TH2D* hMeas2D, RooUnfoldResponse* response, int kterm, string variable);

TH1D* computeProject1DMeanNJets(TH2D *hUnfoldedC, string variable = "MeanNJetsHT_Zinc1jet");

TH2D* SFAnalyzeTTbarMeanNJets(string leptonFlavor = "Muons", string variable = "MeanNJetsHT_Zinc1jet", int JetPtMin = 30, int JetPtMax = 0, bool doFlat = false , bool doVarWidth = true, int doQCD = 0 , bool doSSign = false , bool doInvMassCut = false , int MET = 0 , int doBjets = 2);

void setNegBinZero(TH2D *histograms2D);



void get2DHistos(TH2D *histograms2D[], TFile *Files[], string variable = "MeanNJetsHT_Zinc1jet", bool isDoubleLep = false);

void getRespsNew(RooUnfoldResponse *responses[], TFile *Files[], string);

void getFilesNew(string, TFile *Files[], string, string, string, int JetPtMin = 20, int JetPtMax = 0, bool doFlat = false , bool doVarWidth = false , int doQCD = 0, bool doSSign = false , bool doInvMassCut = false, int MET = 0 , int doBJets = 0 , bool useUnfoldingFiles = false );

void closeFilesNew(TFile *Files[]);

TH2D* get2DHisto(TFile *File, string variable = "MeanNJetsHT_Zinc1jet");


#endif