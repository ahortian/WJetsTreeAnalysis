
#include <iostream>
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <sstream>
#include <RooUnfoldResponse.h>


void getSFforEMu(){
 TFile *a = new TFile("HistoFiles/DE_8TeV_Data_dR_5311_EffiCorr_0_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth.root");
 TFile *b  = new TFile("HistoFiles/DMu_8TeV_Data_dR_5311_EffiCorr_0_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth.root");

 TH2D* h1= (TH2D*) a->Get("lepEtaEta_Zinc0jet");
 TH2D* h2= (TH2D*) b->Get("lepEtaEta_Zinc0jet");
// TH1D* h1= (TH1D*) a->Get("partonsNAfterGenCut");
// TH1D* h2= (TH1D*) b->Get("partonsNAfterGenCut");

//h2->Divide(h1);
h2->Draw();
int Nbin = h1->GetNbinsX();
for ( int i =1 ; i < Nbin; i++){
for ( int j =1 ; j < Nbin; j++){
   cout << h2->GetBinContent(i,j)/ h1->GetBinContent(i,j)<<"  ";
}
cout << endl;
}
cout << endl;


for ( int i =1 ; i < Nbin; i++){
for ( int j =1 ; j < Nbin; j++){
   cout << h1->GetBinContent(i,j)<<"  ";
}
cout << endl;
}
cout << endl;

}

