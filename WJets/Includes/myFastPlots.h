#ifndef _myFastPlots_h_
#define _myFastPlots_h_

using namespace std;

void myFastPlots(string leptonFlavor = "DE", string var = "", bool closureTest = false);

void FastPlotsRun(const int *sel, int nsel, string leptonFlavor, string variable, bool logZ, bool decrease, int SVDkterm, int Bayeskterm, bool closureTest = 0, int JetPtMin = 30, int JetPtMax = 0, bool doFlat = 0, bool doVarWidth = 1);


void myplotSelectedMethod2(string method, RooUnfoldResponse *response, TH1D *genMad, int kterm, TH1D *hData, TH1D *hBG[], int nBG, string leptonFlavor, string variable, bool logZ, bool decrease, string outputDirectory, TFile *outputRootFile, bool closureTest = false, bool save = true);


void myplotChi2OfChange(RooUnfoldResponse *response, int kterm, TH1D *hData, TH1D *hBG[], int nBG, string leptonFlavor, string variable, string energy, string outputDirectory, TFile *outputRootFile, bool closureTest = false, bool save = true);


#endif
