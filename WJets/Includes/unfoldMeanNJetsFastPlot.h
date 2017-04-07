#ifndef _unfoldMeanNJetsFastPlot_h_
#define _unfoldMeanNJetsFastPlot_h_

using namespace std;

void unfoldMeanNJetsFastPlot(string leptonFlavor = "DE", string var = "", bool closureTest = false);

void FastPlotsRun(const int *sel, int nsel, string leptonFlavor, string variable, bool logZ, bool decrease, int SVDkterm, int Bayeskterm, bool closureTest = 0, int JetPtMin = 30, int JetPtMax = 0, bool doFlat = 0, bool doVarWidth = 1);


void myplotSelectedMethod2(string method, RooUnfoldResponse *response, TH2D *genMad, int kterm, TH2D *hData, TH2D *hBG[], int nBG, string leptonFlavor, string variable, bool logZ, bool decrease, string outputDirectory, TFile *outputRootFile, bool closureTest = false, bool save = true);

void myplotChi2OfChange(RooUnfoldResponse *response, int kterm, TH2D *hData, TH2D *hBG[], int nBG, string leptonFlavor, string variable, string energy, string outputDirectory, TFile *outputRootFile, bool closureTest = false, bool save = true);

// define function for plot unfolded with varied kterm
void myplotManyKterm(string method, RooUnfoldResponse *response, TH2D *genMad, TH2D *hData, TH2D *hBG[], int nBG, string leptonFlavor, string variable, bool logZ, bool decrease, string outputDirectory, TFile *outputRootFile, bool closureTest = false, bool save = true);


TH2D* get2DHisto(TFile *File, string variable);

TH1D* computeProject1DMeanNJets(TH2D *hUnfoldedC, string variable);

#endif