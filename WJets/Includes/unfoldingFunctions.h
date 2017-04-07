#ifndef _unfoldingFunctions_h_ 
#define _unfoldingFunctions_h_

#include <iostream>
#include <sstream>
#include <fstream>
#include <TArrow.h>
#include <TLatex.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <RooUnfoldResponse.h>
#include <RooUnfoldSvd.h>
#include <RooUnfoldBayes.h>
#include <RooUnfoldBinByBin.h>
#include <RooUnfoldInvert.h>

#include "writeFastPlotsTex.h"
#include "getFilesAndHistograms.h"
#include "functions.h"

using namespace std;

void normalizeTH2D(TH2D *h);
void plotHNormResp(TH2D *hNormResp_, string leptonFlavor, string variable, string energy, string outputDirectory, TFile *outputRootFile, bool closureTest = false, bool save = true);
void plotDVector(TH1D *hmodD_, int kterm, TH2D *hNormResp, string leptonFlavor, string variable, string energy, string outputDirectory, TFile *outputRootFile, bool closureTest = false, bool save = true);
void plotSVVector(TH1D *hSV_, TH2D *hNormResp, string leptonFlavor, string variable, string energy, string outputDirectory, TFile *outputRootFile, bool closureTest = false, bool save = true);
void plotSVDUnfoldedHistograms(RooUnfoldResponse *response, TH1D *hData, TH1D *hBG[], int nBG, string leptonFlavor, string variable, string energy, bool logZ, bool decrease, string outputDirectory, TFile *outputRootFile, bool closureTest = false, bool save = true);
//void plotChi2OfChange(RooUnfoldResponse *response, int kterm, TH1D *hData, TH1D *hBG[], int nBG, string leptonFlavor, string variable, string energy, string outputDirectory, TFile *outputRootFile, bool closureTest = false, bool save = true);
void plotBayesUnfoldedHistograms(RooUnfoldResponse *response, TH1D *hData, TH1D *hBG[], int nBG, string leptonFlavor, string variable, string energy, bool logZ, bool decrease, string outputDirectory, TFile *outputRootFile, bool closureTest = false, bool save = true);
void plotSelectedMethod(string method, RooUnfoldResponse *response, TH1D *genMad, int kterm, TH1D *hData, TH1D *hBG[], int nBG, string leptonFlavor, string variable, bool logZ, bool decrease, string outputDirectory, TFile *outputRootFile, bool closureTest = false, bool save = true);
void plotComparisonMadPowShe(string method, TH2D *hRespPow, TH2D *hRespShe, RooUnfoldResponse *response, int kterm, TH1D *hData, TH1D *hBG[], int nBG, string leptonFlavor, string variable, bool logZ, bool decrease, string outputDirectory, TFile *outputRootFile, bool closureTest = false, bool save = true);
void plotSVDvsBayesvsBBB(RooUnfoldResponse *response, TH1D *genMad, int SVDkterm, int Bayeskterm, TH1D *hData, TH1D *hBG[], int nBG, string leptonFlavor, string variable, bool logZ, bool decrease, string outputDirectory, TFile *outputRootFile, bool closureTest = false, bool save = true);
TH1D* combinedHisto(TH1D* hMuon, TH1D* hEle, int NBins);
TH1D* mergeGenHisto(TH1D* hMuon, TH1D* hEle, int NBinsOrig);
TCanvas * createCanvas(TH1D* h2Dtemp11 , string name );
TH2D* combineTH2DRes(TH2D* hMuon, TH2D* hEle, int NBins);
#endif
