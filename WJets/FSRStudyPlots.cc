#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TH1.h>
#include <TLegend.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;


void plotAllDeltaRCuts(string fileName)
{
  TH1::SetDefaultSumw2();
  string leptonFlavor;
  if (fileName.find("DMu") != string::npos) leptonFlavor = "Muons";
  else if (fileName.find("DE") != string::npos) leptonFlavor = "Electrons";
  string FILENAME = "HistoFiles/" + fileName + ".root";
  TFile *file = new TFile(FILENAME.c_str());

  string histoName = "ZMass_dressed_";
  vector<string> histoName_v, deltaRCut_v;
  int nDeltaRCut(0);
  for (int i = 0; i < file->GetListOfKeys()->GetEntries(); i++){
    string name = file->GetListOfKeys()->At(i)->GetName();
    if (name.find(histoName) != string::npos){
      nDeltaRCut++;
      histoName_v.push_back(name);
      deltaRCut_v.push_back(name.substr(name.find("0")));
    }
  }
  cout << nDeltaRCut << endl;

  TLegend *legend = new TLegend(0.645,0.6,1,0.89);
  legend->SetFillStyle(0);
  legend->SetBorderSize(0);

  TH1D *ZMass_dressed[nDeltaRCut];
  TH1D *ZMass_dressed_cop[nDeltaRCut];
  TH1D *ZMass_St3;
  TH1D *ZMass_St3_cop;
  for (int i = 0; i < nDeltaRCut; i++){
    cout << "DeltaR cut " << deltaRCut_v[i] << endl;
    ZMass_dressed[i] = (TH1D*) file->Get(histoName_v[i].c_str());
  }
  ZMass_St3 = (TH1D*) file->Get("ZMass_St3");
  int Colors[] = {kBlack,  kSpring+5, kOrange, kOrange-3, kRed+1, kPink-6, kViolet+5, kBlue, kCyan+1};

  string canvasName = leptonFlavor + "_AlldRCuts";
  TCanvas *can = new TCanvas(canvasName.c_str(), canvasName.c_str(), 1200, 600);
  can->Divide(2);
  //-- plot Z Invariant Mass for each delta R Cut 
  can->cd(1);
  can->GetPad(1)->SetLogy();
  ZMass_St3->SetLineStyle(kDashed);
  ZMass_St3->SetLineWidth(2);
  ZMass_St3->SetFillStyle(3001);
  ZMass_St3->SetFillColor(kAzure+1);
  ZMass_St3->Draw();
  string title = "Generated " + leptonFlavor +" Invariant Mass";
  ZMass_St3->SetTitle(title.c_str());
  ZMass_St3->GetYaxis()->SetTitleOffset(1.4);
  for (int i = 0; i < nDeltaRCut; i++){
    ZMass_dressed[i]->SetMarkerStyle(20);
    ZMass_dressed[i]->SetMarkerSize(0.8);
    ZMass_dressed[i]->SetLineWidth(2);
    if (i < 9){
      ZMass_dressed[i]->SetLineColor(Colors[i]);
      ZMass_dressed[i]->SetMarkerColor(Colors[i]);
    }
    ZMass_dressed[i]->Draw("SAME");
    string legName = "#Delta R = " + deltaRCut_v[i];
    legend->AddEntry(ZMass_dressed[i], legName.c_str(), "lp");
  }
  TLegend *legend2 = (TLegend*) legend->Clone();
  string leg = leptonFlavor + " St. 3";
  legend->AddEntry(ZMass_St3, leg.c_str(), "lf");
  legend2->AddEntry(ZMass_St3, leg.c_str(), "l");
  legend->Draw();

  //-- plot ratio to Status 3 leptons
  can->cd(2);
  title = "Ratio to " + leptonFlavor + " St. 3";
  ZMass_St3_cop = (TH1D*) ZMass_St3->Clone();
  ZMass_St3_cop->Divide(ZMass_St3);
  ZMass_St3_cop->SetFillColor(0);
  ZMass_St3_cop->Draw();
  ZMass_St3_cop->SetTitle(title.c_str());
  ZMass_St3_cop->GetYaxis()->SetTitle(title.c_str());
  ZMass_St3_cop->GetYaxis()->SetRangeUser(0.7, 1.6);
  for (int i = 0; i < nDeltaRCut; i++){
    ZMass_dressed_cop[i] = (TH1D*) ZMass_dressed[i]->Clone();   
    ZMass_dressed_cop[i]->Divide(ZMass_St3);
    ZMass_dressed_cop[i]->Draw("SAME");
  }
  legend2->Draw();
  canvasName = "PNGFiles/FSRStudy/" + canvasName + ".png";
  can->Print(canvasName.c_str());

}

void plotEEvsMuMu(string ee_fileName, string mumu_fileName)
{
  TH1::SetDefaultSumw2();
  string ee_FILENAME = "HistoFiles/" + ee_fileName + ".root";
  TFile *ee_file = new TFile(ee_FILENAME.c_str());
  string mumu_FILENAME = "HistoFiles/" + mumu_fileName + ".root";
  TFile *mumu_file = new TFile(mumu_FILENAME.c_str());

  string histoName = "ZMass_dressed_";
  vector<string> histoName_v, deltaRCut_v;
  int nDeltaRCut(0);
  for (int i = 0; i < ee_file->GetListOfKeys()->GetEntries(); i++){
    string name = ee_file->GetListOfKeys()->At(i)->GetName();
    if (name.find(histoName) != string::npos){
      nDeltaRCut++;
      histoName_v.push_back(name);
      deltaRCut_v.push_back(name.substr(name.find("0")));
    }
  }
  cout << nDeltaRCut << endl;


  TH1D *ZMass_dressed[2][nDeltaRCut];
  TH1D *ZMass_dressed_cop[2][nDeltaRCut];
  TH1D *ZMass_St3[2];
  TH1D *ZMass_St3_cop[2];
  for (int i = 0; i < nDeltaRCut; i++){
    cout << "DeltaR cut " << deltaRCut_v[i] << endl;
    ZMass_dressed[0][i] = (TH1D*) ee_file->Get(histoName_v[i].c_str());
    ZMass_dressed[1][i] = (TH1D*) mumu_file->Get(histoName_v[i].c_str());
  }
  ZMass_St3[0] = (TH1D*) ee_file->Get("ZMass_St3");
  ZMass_St3[1] = (TH1D*) mumu_file->Get("ZMass_St3");
  //int Colors[] = {kBlack,  kSpring+5, kOrange, kOrange-3, kRed+1, kPink-6, kViolet+5, kBlue, kCyan+1};

  TCanvas *canSt3 = new TCanvas("St3_mu_ele", "St3_mu_ele", 1200, 600);
  canSt3->Divide(2);
  canSt3->cd(1);
  canSt3->GetPad(1)->SetLogy();

  TLegend *legend = new TLegend(0.55,0.7,0.98,0.8);
  legend->SetFillStyle(0);
  legend->SetBorderSize(0);

  ZMass_St3[0]->Scale(30294954./29976625.);
  ZMass_St3[0]->SetMarkerStyle(20);
  ZMass_St3[0]->SetMarkerColor(kRed+1);
  ZMass_St3[0]->SetLineColor(kRed+1);
  ZMass_St3[0]->SetTitle("Electrons vs. Muons Invariant Mass");
  ZMass_St3[0]->GetYaxis()->SetTitleOffset(1.4);
  ZMass_St3_cop[0] = (TH1D*) ZMass_St3[0]->Clone();
  ZMass_St3[1]->SetLineColor(kBlue);
  ZMass_St3[1]->SetLineWidth(2);
  ZMass_St3_cop[1] = (TH1D*) ZMass_St3[1]->Clone();
  ZMass_St3[1]->DrawCopy();
  ZMass_St3[0]->DrawCopy("SAME");
  legend->AddEntry(ZMass_St3[1], "Muons St. 3", "l");
  legend->AddEntry(ZMass_St3[0], "Electrons St. 3", "lp");
  legend->Draw();  

  canSt3->cd(2);
  ZMass_St3_cop[0]->Divide(ZMass_St3_cop[1]);
  ZMass_St3_cop[0]->GetYaxis()->SetRangeUser(0.8, 1.2);
  ZMass_St3_cop[0]->GetYaxis()->SetTitleOffset(1.4);
  ZMass_St3_cop[0]->GetYaxis()->SetTitle("Ratio: Electrons to Muons");
  ZMass_St3_cop[0]->SetTitle("Ratio: Electrons to Muons Invariant Mass");
  ZMass_St3_cop[0]->Fit("pol0");
  ZMass_St3_cop[0]->DrawCopy("E1");
  string canSt3_str = canSt3->GetTitle();
  canSt3_str = "PNGFiles/FSRStudy/" + canSt3_str + ".png";
  canSt3->Print(canSt3_str.c_str());

  TCanvas *canvasEMu[3][3];
  TLegend *legEMu;
  for (int i = 0; i < 3; i++){
    for (int j = 0; j < 3; j++){
      legEMu = new TLegend(0.55,0.7,0.98,0.8);
      legEMu->SetFillStyle(0);
      legEMu->SetBorderSize(0);
      string eleLeg = "Electrons dR = " + deltaRCut_v[i];
      string muLeg = "Muons dR = " + deltaRCut_v[j];
      string canTitle = "Electrons_dR_" + deltaRCut_v[i] + "_Muons_dR_" + deltaRCut_v[j];
      canvasEMu[i][j] = new TCanvas(canTitle.c_str(), canTitle.c_str(), 1200, 600);
      canvasEMu[i][j]->Divide(2);
      canvasEMu[i][j]->cd(1);
      canvasEMu[i][j]->GetPad(1)->SetLogy();
      ZMass_dressed[0][i]->SetLineColor(kRed+1);
      ZMass_dressed[0][i]->SetMarkerColor(kRed+1);
      ZMass_dressed[0][i]->SetMarkerStyle(20);
      ZMass_dressed[1][j]->SetTitle("Electrons vs. Muons Invariant Mass");
      ZMass_dressed[1][j]->SetLineColor(kBlue);
      ZMass_dressed[1][j]->SetLineWidth(2);
      legEMu->AddEntry((TH1D*)ZMass_dressed[1][j], muLeg.c_str(), "l");
      legEMu->AddEntry((TH1D*)ZMass_dressed[0][i], eleLeg.c_str(), "LP");
      ZMass_dressed[1][j]->Draw();
      ZMass_dressed[0][i]->Draw("SAME");
      legEMu->Draw();

      canvasEMu[i][j]->cd(2);
      ZMass_dressed_cop[0][i] = (TH1D*) ZMass_dressed[0][i]->Clone();
      ZMass_dressed_cop[1][j] = (TH1D*) ZMass_dressed[1][j]->Clone();
      ZMass_dressed_cop[0][i]->Scale(30294954./29976625.);
      ZMass_dressed_cop[0][i]->Divide(ZMass_dressed_cop[1][j]);
      ZMass_dressed_cop[0][i]->GetYaxis()->SetRangeUser(0.8, 1.2);
      ZMass_dressed_cop[0][i]->SetTitle("Ratio: Electrons to Muons Invariant Mass");
      ZMass_dressed_cop[0][i]->GetYaxis()->SetTitle("Ratio: Electrons to Muons");
      ZMass_dressed_cop[0][i]->GetYaxis()->SetTitleOffset(1.4);
      ZMass_dressed_cop[0][i]->Fit("pol0");
      ZMass_dressed_cop[0][i]->Draw("E1");

      while (canTitle.find(".") != string::npos) canTitle.replace(canTitle.find("."), string(".").length(),  "_"); 
      canTitle = "PNGFiles/FSRStudy/" + canTitle + ".png";
      canvasEMu[i][j]->Print(canTitle.c_str());

    }
  }

  //ee_file->Close();
  //mumu_file->Close(); 
}

void FSRStudyPlots()
{
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1111);
  string ee_fileName = "DE_8TeV_DYJets_UNFOLDING_dR_5311_Inf3_FSRStudies";
  string mumu_fileName = "DMu_8TeV_DYJets_UNFOLDING_dR_5311_Inf3_FSRStudies";
  plotAllDeltaRCuts(ee_fileName);
  plotAllDeltaRCuts(mumu_fileName);
  plotEEvsMuMu(ee_fileName, mumu_fileName);
}
