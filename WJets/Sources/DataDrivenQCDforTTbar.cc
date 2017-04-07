#include <vector>
#include <TDirectory.h>
#include <TFile.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TKey.h>
#include <TH1.h>
#include <TH2.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TGraph.h>
#include <TPaveStats.h>
#include <TText.h>
#include <TLegend.h>
#include <iostream>
#include <sstream>
#include <TLorentzVector.h>
#include <RooUnfoldResponse.h>
#include <RooUnfold.h>
#include <RooUnfoldBayes.h>
#include <RooUnfoldSvd.h>
#include <RooUnfoldBinByBin.h>
#include <RooUnfoldInvert.h>
#include <TSVDUnfold.h>
#include <TLatex.h>
#include <TStyle.h>
#include <TDatime.h>
#include <TRandom3.h>
#include <TMatrixD.h>
#include "getFilesAndHistograms.h"
#include "DataDrivenQCDforTTbar.h"
using namespace std;

//--  Setting global variables --------------------------------------------------------------
#include "fileNames.h"
#include "variablesOfInterestVarWidth.h"
string outputDirectory="DataDrivenQCD/";

bool isMuon(0);
string unfAlg = "Bayes";
string energy = getEnergy();
int JetPtMin(30);
int JetPtMax(0);
TFile *fOut;// = new TFile("DataDrivenQCD/out.root","recreate");

//-------------------------------------------------------------------------------------------
void DataDrivenQCDforTTbar(){
	for (int i(0); i < 1/*NVAROFINTEREST*/; i++){
		FuncDataDrivenQCD();
	}
	fOut ->Close();
}


void FuncDataDrivenQCD(string variable, string leptonFlavor, bool doFlat , bool doVarWidth, int MET ){
	string outputRootFileName ="SMuE_8TeV_DataQCD_dR_5311_EffiCorr_0_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth.root";
	fOut = new TFile(outputRootFileName.c_str(), "RECREATE");
	cout << " test" << endl;
	//-- fetch the data files and histograms --------------
	const int NQCD = 1 ;
	bool doInvMassCut = 0; 
	TFile *fData[NQCD],*fSignal[NQCD],*fMC[15];             // 0 = central, 1 = JES Up, 2 = JES Down 
	TH1D *hData[NQCD],*hSignal[NQCD],*hBack[NQCD];
	for ( int i=0 ; i < NQCD ; i++){
        //getFiles(FILESDIRECTORY, fData, leptonFlavor, energy, DATAFILENAME, JetPtMin, JetPtMax, doFlat, doVarWidth);
		fData[i] = getFile(FILESDIRECTORY,  leptonFlavor, energy, ProcessInfo[DATAFILENAME].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 1 ,doInvMassCut,MET, "","0");
		cout << " got data " << variable << endl;
		TH1D *hTemp = getHisto(fData[i], variable);
		hData[i] = (TH1D *) hTemp->Clone();
		cout << " got data histos " << ProcessInfo[i].filename << endl;
		//	  fData[i]->Close();
	}
	//-----------------------------------------------------
	cout << " now open all the MC of interest " << endl;
	// ----------------- get all MC histos ---
    int countNfiles = 0 ; 
	for ( int j = 1 ; j < NFILESTTBAR ; j++){
        int sel = FilesTTbar[j];
        if (ProcessInfo[sel].filename.find("QCD") != string::npos) continue;
		fMC[countNfiles] = getFile(FILESDIRECTORY,  leptonFlavor, energy, ProcessInfo[sel].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 1 ,doInvMassCut,MET, "","0");
        countNfiles++;
	}
    
	// QCD estiamtion procedure
	cout << " to QCD estimation " << endl;
	unsigned short nHist = fData[0]->GetListOfKeys()->GetEntries();
	vector<string> histoName(nHist);
	vector<string> histoTitle(nHist);
	for (unsigned short i(0); i < nHist; i++) {
		bool decrease(true);
		string histoNameTemp = fData[0]->GetListOfKeys()->At(i)->GetName();
		if (histoNameTemp.find("gen") != string::npos) continue;
		TH1D* histTemp = (TH1D*) fData[0]->Get(histoNameTemp.c_str());
		TH1D* hOut = (TH1D*)  histTemp->Clone();
		for ( int j = 1 ; j < countNfiles  ; j++){
			TH1D* histTempMC = (TH1D*) fMC[j]->Get(histoNameTemp.c_str());
			hOut->Add(histTempMC,-1);
		}
		fOut->cd();
		hOut->Write();
	}
}
