#define FSRStudy_cxx
#define PI 3.141592
#define NEVENTS_10000      0
#define BARREDEPROGRESSION 1
#define DEBUG              0

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "FSRStudy.h"
#include "functions.h"

using namespace std;

void FSRStudy::Loop()
{
  TH1::SetDefaultSumw2();
  //==========================================================================================================//
  //         Ouput File Name           //
  //===================================//
  string outputFileName = "HistoFiles/" + fileName + "_FSRStudies.root";
  TFile *outputFile = new TFile(outputFileName.c_str(),"RECREATE");
  cout << endl << "Run FSRStudy.cc  on " << fileName << "   --->   " << outputFileName << endl << endl;
  //==========================================================================================================//


  //==========================================================================================================//
  //  Histograms and variables list    //
  //===================================//
  //-- set the array of different cone sizes (dR)
  int nDeltaRCut(8);
  //float deltaRCut[] = {0.00, 0.04, 0.06, 0.08, 0.10, 0.12, 0.14, 0.16}; 
  float deltaRCut[] = {0.00, 0.1, 0.15, 0.2, 0.25, 0.3, 0.4, 0.5}; 

  //-- define the arrays of histograms with proper name
  TH1D *ZMass[8];
  TH1D *ZMassSt3;
  TH1D *hLeptonPtSt1;
  ostringstream name, title;
  for (int i = 0; i < nDeltaRCut; i++){
    name  << "ZMass_dressed_" << deltaRCut[i]; title << "Z Invariant Mass (dressed leptons dR = " << deltaRCut[i] << ")"; 
    ZMass[i] = new TH1D(name.str().c_str(), title.str().c_str(),40,71.,111.); name.str(""); title.str("");
  }
  name  << "ZMass_St3"; title << "Z Invariant Mass (Status 3 leptons )"; 
  ZMassSt3 = new TH1D(name.str().c_str(), title.str().c_str(),40,71.,111.); name.str(""); title.str("");
  hLeptonPtSt1 = new TH1D(" Lepton status 1 pt after mass cut on dressed","Lepton status 1 pt after mass cut on dressed", 200, 0, 100);
  // additional muons variables
  // -- set the lepton mass and pdgId according to the flavor
  float leptonMass(0);
  int leptonID(11);
  if (leptonFlavor == "Muons"){
    leptonMass = 0.105658;
    leptonID = 13;
  }
  else if (leptonFlavor == "Electrons"){
    leptonMass = 0.00051;
    leptonID = 11;
  }
  cout << "Lepton Flavor: " << leptonFlavor << endl;
  //==========================================================================================================//

  if (DEBUG) cout << "Stop after line " << __LINE__ << endl;

  //==========================================================================================================//
  // Start looping over all the events //
  //===================================//
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;
  if (NEVENTS_10000) nentries = 10000;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    if (BARREDEPROGRESSION && !DEBUG) barre_de_progression((int) 100*(jentry+1)/nentries);
    if (DEBUG) cout << "Event  :"<<jentry<<endl;
    if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
    // check if status 1 lepton is Tau ( more generally, if it is not lepton it is suppose to be )
    int countTauS3 = 0 ;
    for (int i = 0; i < int(genLepEta_->size()) ; i++) {
      if ( genLepSt_->at(i) == 3 &&  abs(genLepId_->at(i)) != leptonID)       countTauS3++;
    }
    if ( countTauS3 > 0 ) continue;

    //==========================================================================================================//
    //  Start looping over deltaR cuts   //
    //===================================//
    for (int l = 0; l < nDeltaRCut; l++){

      if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
      //=======================================================================================================//
      //       Retrieving gen leptons       //
      //====================================//
      int passesGenLeptonCut(0);
      int nTotGenLeptons(genLepEta_->size());
      int nTotGenPhotons(genPhoEta_->size());
      vector<leptonStruct> genLeptons,genLeptonsClone;
      vector<leptonStruct> genLeptonsSt1;
      vector<leptonStruct> genLeptonsSt3;
      TLorentzVector genLep1, genLep2, genZ;
      TLorentzVector genLep1St3, genLep2St3, genZSt3;
      leptonStruct genLepton1, genLepton2;
      leptonStruct genLepton1St3, genLepton2St3;
      vector<int> usedGenPho;

      if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
      //-- retriveing generated leptons
      for (int i = 0; i < nTotGenLeptons; i++) {
        if (abs(genLepId_->at(i)) != leptonID || countTauS3 > 0 ) continue;
        leptonStruct genLep = {genLepPt_->at(i), genLepEta_->at(i), genLepPhi_->at(i), genLepE_->at(i), genLepQ_->at(i)};
        leptonStruct genLep0 = {genLepPt_->at(i), genLepEta_->at(i), genLepPhi_->at(i), genLepE_->at(i), genLepQ_->at(i)};
        //-- dress the leptons with status 1
        if (genLepSt_->at(i) == 1 ){
          TLorentzVector tmpGenLep;
          TLorentzVector tmpGenLep0;
          tmpGenLep.SetPtEtaPhiM(genLep.pt, genLep.eta, genLep.phi, leptonMass);
          tmpGenLep0.SetPtEtaPhiM(genLep.pt, genLep.eta, genLep.phi, leptonMass);
          for (int j = 0; j < nTotGenPhotons; j++){
            TLorentzVector tmpGenPho;
            tmpGenPho.SetPtEtaPhiM(genPhoPt_->at(j), genPhoEta_->at(j), genPhoPhi_->at(j), 0.);
            int used = 0;
            for (int k = 0; k < int(usedGenPho.size()); k++){
              if (j == usedGenPho[k]) used = 1;
            }
            if (deltaR(tmpGenPho.Phi(), tmpGenPho.Eta(), tmpGenLep.Phi(), tmpGenLep.Eta()) < deltaRCut[l] && !used){
              tmpGenLep = tmpGenLep + tmpGenPho;
              usedGenPho.push_back(j);
            }
          }
          genLep.pt = tmpGenLep.Pt(); genLep.eta = tmpGenLep.Eta(); genLep.phi = tmpGenLep.Phi();
          if (genLep.pt >= 20 && fabs(genLep.eta) <= 2.4) {
            genLeptons.push_back(genLep);
            genLeptonsClone.push_back(genLep);
            genLeptonsSt1.push_back(genLep0);
          }
        }
        // save the leptons with status 3 for comparison (only MadGraph has status 3 however)
        else if (l == 0 && genLepSt_->at(i) == 3 && genLep.pt >= 20 && fabs(genLep.eta) <= 2.4) genLeptonsSt3.push_back(genLep);
      }
      int nStatus1(genLeptons.size());
      int nStatus3(genLeptonsSt3.size());

      if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
      //-- do the matching between Status 3 and Status 1 leptons
      vector<int> status3Index(nStatus3, 0);
      vector<vector<int> > matchingTable(nStatus1, status3Index);
      for (int i = 0; i < nStatus1; i++){
        float mindR(0.5);
        int index(-1);
        float dR(9999);
        for (int j = 0; j < nStatus3; j++){
          dR = deltaR(genLeptonsSt3[j].phi, genLeptonsSt3[j].eta, genLeptons[i].phi, genLeptons[i].eta);
          if (dR < mindR){
            mindR = dR;
            index = j;
          }
        }
        if (index > -1 ) matchingTable[i][index] = 1; 
      }

      //-- print the mathcing table
      /*cout << "\n matching Table: \n" << endl; 
        for (int i = 0; i < int(matchingTable.size()); i++){
        for (int j = 0; j < int(matchingTable[i].size()); j++){
        cout << matchingTable[i][j] << "  ";
        }
        cout << endl;
        }*/

      if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
      //-- determine if the event passes the leptons requirements
      //---- treating status 1 leptons
      passesGenLeptonCut = 0;
      if (genLeptons.size() >= 2){
        // sort leptons by descending pt
        sort(genLeptons.begin(), genLeptons.end(), LepDescendingOrder);
        genLepton1 = genLeptons[0];
        genLepton2 = genLeptons[1];
        // select the first two leptons with opposite charge        
        if (genLepton1.charge * genLepton2.charge > 0 && genLeptons.size() > 2) genLepton2 = genLeptons[2];
        // build the TLorentzVectors, the Z candidate and the kinematic
        genLep1.SetPtEtaPhiM(genLepton1.pt, genLepton1.eta, genLepton1.phi, leptonMass);
        genLep2.SetPtEtaPhiM(genLepton2.pt, genLepton2.eta, genLepton2.phi, leptonMass);
        genZ = genLep1 + genLep2;
        // apply charge, mass and eta cut
        if (genLepton1.charge * genLepton2.charge < 0 && genZ.M() > 71 && genZ.M() < 111 ) passesGenLeptonCut = 1;
      }
      if (passesGenLeptonCut) { ZMass[l]->Fill(genZ.M());
        for (int countL=0 ; countL < int (genLeptons.size() ) ; countL++){
          if (  fabs( genLeptonsClone[countL].pt -genLeptons[0].pt)<0.001 ||  fabs( genLeptonsClone[countL].pt -genLeptons[1].pt)<0.001 )	hLeptonPtSt1->Fill(genLeptonsSt1[0].pt);

        }
      }



      //---- treating status 3 leptons
      passesGenLeptonCut = 0;
      if (l == 0 && genLeptonsSt3.size() >= 2){
        // sort leptons by descending pt
        sort(genLeptonsSt3.begin(), genLeptonsSt3.end(), LepDescendingOrder);
        genLepton1St3 = genLeptonsSt3[0];
        genLepton2St3 = genLeptonsSt3[1];
        // select the first two leptons with opposite charge        
        if (genLepton1St3.charge * genLepton2St3.charge > 0 && genLeptonsSt3.size() > 2) genLepton2St3 = genLeptonsSt3[2];
        // build the TLorentzVectors, the Z candidate and the kinematic
        genLep1St3.SetPtEtaPhiM(genLepton1St3.pt, genLepton1St3.eta, genLepton1St3.phi, leptonMass);
        genLep2St3.SetPtEtaPhiM(genLepton2St3.pt, genLepton2St3.eta, genLepton2St3.phi, leptonMass);
        genZSt3 = genLep1St3 + genLep2St3;
        // apply charge, mass and eta cut
        if (genLepton1St3.charge * genLepton2St3.charge < 0 && genZSt3.M() > 71 && genZSt3.M() < 111 ) passesGenLeptonCut = 1;
      }
      if (l == 0 && passesGenLeptonCut) ZMassSt3->Fill(genZSt3.M());
    }
    //=======================================================================================================//
  }
  //==========================================================================================================//

  if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
  outputFile->cd();
  outputFile->Write();
  outputFile->Close();
}
