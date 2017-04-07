#ifndef __myUnfoldingSyst__
#define __myUnfoldingSyst__

#include <TH1.h>
#include <TH2.h>
#include <string>
#include <RooUnfoldResponse.h>

TH2D* CovToCorr(const TH2D *h);
TH2D* CovToCorr2(const TH2D *h, string title);
TH2D* setCovariance(const TH2D *h, const TH1D *hCent, double error);
TH1D* getHisto(string histoFilesDirectory,string leptonFlavor, string energy, string DataHistoFilesName, string syst, string direction, int JetPtMin, int JetPtMax, string variable);
RooUnfoldResponse* getResponse(string histoFilesDirectory, string leptonFlavor, string energy, string DataHistoFilesName, string syst, string direction, int JetPtMin, int JetPtMax, string variable);
TH1D* getSumBG(string histoFilesDirectory, string leptonFlavor, string energy, string* DataHistoFilesNameArray, string syst, string direction, int JetPtMin, int JetPtMax, string variable);
TH1D* Unfold(string unfAlg, RooUnfoldResponse* response, TH1D* hData, TH1D* hSumBG, int Kterm, string hOutName, bool useOverFlow = true);
TH2D* Unfold2D(string unfAlg, RooUnfoldResponse* response, TH2D* hData, TH2D* hSumBG, int Kterm, string hOutName, bool useOverFlow = true);
TH2D* CovFromRoo(string unfAlg, RooUnfoldResponse* response, TH1D* hData, TH1D* hSumBG, int Kterm, string hOutName, int NToys, bool useOverFlow = true);
void hBinCon(TH1D* hData);
TH1D* SetSystErrorsMax(TH1D* hCent, TH1D* hUp, TH1D* hDown, string name);
TH1D* SetSystErrorsMean(TH1D* hData, double Error, string name);
TH1D* SetSystErrorsMean(TH1D* hData, TH1D* hUp, TH1D* hDown, string name);
TH1D* SetSystErrorsMean(TH1D* hData, TH1D* hCentral, TH1D* hUp, TH1D* hDown, string name, int option = 0);
TH1D* SetSystErrorsMean(TH1D* hData, TH1D* hCentral, TH1D* hUp, TH1D* hDown, double Error, string name);
TH2D* SetResponseErrors(TH2D* hCen, TH2D* hUp, TH2D* hDown);
TH1D* ToyMCErrorsStat(string unfAlg, TH1D *hdata, TH1D* hBack0[], RooUnfoldResponse *response, int kterm, TH2D *hCovariance, TH2D *hCorrelation, int nPseudos, int selection, bool useOverFlow = true, int NBkgGroups = 6);
#endif
