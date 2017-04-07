
#include <iostream>
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <sstream>
#include <RooUnfoldResponse.h>


void getExcWeights(){
 TFile *a = new TFile("HistoFiles/DMu_8TeV_DYJets_MIX_UNFOLDING_dR_5311_Inf3_EffiCorr_0_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth.root");
 TFile *b  = new TFile("HistoFiles/DMu_8TeV_DYJets_UNFOLDING_dR_5311_Inf3_EffiCorr_0_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth.root");

 TH1D* h1= (TH1D*) a->Get("partonsN");
 TH1D* h2= (TH1D*) b->Get("partonsN");
// TH1D* h1= (TH1D*) a->Get("partonsNAfterGenCut");
// TH1D* h2= (TH1D*) b->Get("partonsNAfterGenCut");

//h2->Divide(h1);
h2->Draw();
for ( int i =1 ; i < 6; i++){
   cout << h2->GetBinContent(i)/ h1->GetBinContent(i)<<"  ";

}
cout << endl;
}

