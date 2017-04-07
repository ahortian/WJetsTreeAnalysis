#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <RooUnfoldResponse.h>
#include <TDatime.h>
#include <TMath.h>
#include <TRandom3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <string>
//--  Setting global variables --------------------------------------------------------------
#include "fileNames.h"
#include "getFilesAndHistograms.h"
#include "functions.h"
#include "plottingFunctions.h"

//using namespace std;
string energy = getEnergy();


TCanvas*  plottingFunctions(int NHistos, TH1D* histos[], bool logZ, bool decrease, string variable, string sLegend[], float YMin, float YMax)
{
  cout << " I entered the plottingFunctions.cc    " << NHistos <<"   " << sLegend[0] << endl;
  gStyle->SetOptStat(0);
  // create ratios
  TH1D* hRatio[10];
   for ( int i = 0 ; i < NHistos ; i++ ){
	hRatio[i] = (TH1D*) histos[i]->Clone();
	hRatio[i]->Divide(histos[0]);
  }


   TCanvas *can = new TCanvas(variable.c_str(), variable.c_str(), 600, 1000);
   can->cd();
   gStyle->SetOptStat(0);
   TPad *pad1 = new TPad("pad1","pad1", 0, 0.4, 1, 1);
   pad1->SetTopMargin(0.11);
   pad1->SetBottomMargin(0);
   pad1->SetRightMargin(0.02);
   pad1->SetLeftMargin(0.11);
   pad1->SetTicks();
   pad1->Draw();
   pad1->cd();
   if (logZ) pad1->SetLogy();
   double MineYMax = histos[0]->GetMaximum();
   double lowerLimit = histos[0]->GetXaxis()->GetXmin();
   cout << lowerLimit << "  " << MineYMax << endl;
   if ( histos[1]->GetMaximum() > MineYMax) MineYMax =  histos[1]->GetMaximum();
   /*if (variable.find("ZNGood") != string::npos){
      //MineYMax = 85.;
      cout << "Remove first bin: " << histos[0]->GetXaxis()->GetXmin() + histos[0]->GetBinWidth(1) << endl;
      histos[0]->GetXaxis()->SetRangeUser(histos[0]->GetXaxis()->GetXmin() + histos[0]->GetBinWidth(1), histos[0]->GetXaxis()->GetXmax());
      cout << "Remove first bin: " << histos[0]->GetXaxis()->GetXmin() + histos[0]->GetBinWidth(1) << endl;
      lowerLimit = histos[0]->GetXaxis()->GetXmin() + histos[0]->GetBinWidth(1);
   }
   */
   cout << __LINE__ << endl;
//    histos[1]->GetXaxis()->SetLimits(lowerLimit, histos[0]->GetXaxis()->GetXmax());
   //histos[0]->SetFillStyle(3001);
   histos[0]->SetFillColor(kAzure-9);
   histos[0]->SetLineColor(kAzure-9);
   histos[0]->GetYaxis()->SetTitleSize(0.04);
   histos[0]->GetYaxis()->SetTitleOffset(1.45);
   histos[0]->SetMaximum(MineYMax*1.2);
   histos[0]->Draw("P");
   string temp;
   cout << __LINE__ << endl;
   //mcGraph->Draw("L same");
   //mcGraph->Draw("L");
   histos[1]->SetFillStyle(3001);
   histos[1]->SetFillColor(kOrange-3);
   histos[1]->SetLineColor(kOrange-3);
   histos[1]->Draw("P same");
   cout << __LINE__ << endl;
   if ( NHistos == 3 ) histos[2]->Draw("P same");
   pad1->Draw();
   cout << __LINE__ << endl;
   // set the legend
   double xLowLeg(0.65), xHighLeg(0.98);
   if (!decrease){
      xLowLeg = 0.14;
      xHighLeg = 0.47;
   }
   cout << __LINE__ << endl;
   TLegend *legend = new TLegend(xLowLeg, 0.66, xHighLeg, 0.91);
   legend->SetHeader("  ");
   cout << " TITLE:  " << histos[0]->GetTitle() << endl; 
   string title = histos[0]->GetTitle() ;
   histos[0]->SetTitle(title.c_str());
   histos[0]->SetTitle("");
   legend->SetFillStyle(0);
   legend->SetBorderSize(0);
   //legend->AddEntry(histos[0], "MG+Py6 at NNLO Stat. Erro ", "pl");
   legend->AddEntry(histos[0], sLegend[0].c_str(), "pl");
   legend->AddEntry(histos[1], sLegend[1].c_str(), "pl");
   if ( NHistos == 3 ) legend->AddEntry(histos[2], sLegend[2].c_str(), "pl");
   //legend->AddEntry(histos[1], "Powheg+Py6 Stat. Error", "pf");
//   legend->AddEntry(histos[0], "MG+Py6 at NNLO Stat. Error", "pf");
//   legend->AddEntry(histos[2], "Sherpa NLO Stat. Error", "pf");
//   legend->AddEntry(histos[2], "Data Stat. Error ", "pl");
//   legend->AddEntry( histos[1], "Data Tot. Error", "l");
   legend->Draw();

   TLatex *variableLabel = new TLatex();
   variableLabel->SetTextSize(0.035);
   variableLabel->SetTextFont(42);
   variableLabel->SetLineWidth(2);
   variableLabel->SetTextColor(kBlack);
   variableLabel->SetNDC();
   variableLabel->SetTextAlign(21);
//   variableLabel->DrawLatex(0.54,0.965,histos[0]->GetTitle());
   variableLabel->DrawLatex(0.54,0.965,title.c_str());

   TLatex *cmsColl = new TLatex();
   cmsColl->SetTextSize(0.025);
   cmsColl->SetTextFont(42);
   cmsColl->SetLineWidth(2);
   cmsColl->SetTextColor(kBlack);
   cmsColl->SetNDC();
   cmsColl->SetTextAlign(11);
//   cmsColl->DrawLatex(0.11,0.905, "CMS Collaboration");

   TLatex *sqrtXTeV = new TLatex();
   sqrtXTeV->SetTextSize(0.025);
   sqrtXTeV->SetTextFont(42);
   sqrtXTeV->SetLineWidth(2);
   sqrtXTeV->SetTextColor(kBlack);
   sqrtXTeV->SetNDC();
   sqrtXTeV->SetTextAlign(21);
   if (energy == "7TeV") sqrtXTeV->DrawLatex(0.54,0.905, "#sqrt{s} = 7 TeV");
   if (energy == "8TeV") sqrtXTeV->DrawLatex(0.54,0.905, "#sqrt{s} = 8 TeV");


  TLatex *ytitle = new TLatex();
   ytitle->SetTextSize(0.035);
   ytitle->SetTextFont(42);
   ytitle->SetLineWidth(2);
   ytitle->SetTextColor(kBlack);
   ytitle->SetNDC();
   ytitle->SetTextAlign(33);


   ////         NEW TPAD
   can->cd();
   TPad *pad2;
   if ( NHistos == 2 ) pad2 = new TPad("pad2","pad2", 0, 0.0, 1, 0.4);
   else  pad2 = new TPad("pad2","pad2", 0, 0.2286, 1, 0.4);
   pad2->SetTopMargin(0);
   pad2->SetBottomMargin(0);
   pad2->SetRightMargin(0.02);
   pad2->SetLeftMargin(0.11);
   pad2->SetGridy();
   pad2->SetTicks();
   pad2->Draw();
   pad2->cd();
   hRatio[1]->SetMarkerStyle(20);
   hRatio[1]->SetMarkerSize(0.8);
   hRatio[1]->SetMarkerColor(kBlack);
   hRatio[1]->SetLineColor(kBlack);
   // changing Syst to stat
   //hRatio[1]->SetLineColor(kRed+1);
   hRatio[1]->SetLineColor(kBlack+1);
   hRatio[1]->SetMarkerStyle(20);
   hRatio[1]->SetMarkerSize(0.8);
//   hRatio[1]->GetXaxis()->SetLimits(lowerLimit, hRatio[1]->GetXaxis()->GetXmax());
//   hRatio[1]->GetYaxis()->SetRangeUser(0.5,1.19);
   hRatio[1]->GetYaxis()->SetRangeUser(YMin, YMax);
   hRatio[1]->GetYaxis()->SetTitle("Ratio");
   hRatio[1]->GetYaxis()->SetTitleSize(0.1);
   hRatio[1]->GetYaxis()->SetTitleOffset(0.6);
   hRatio[1]->GetYaxis()->CenterTitle();
   hRatio[1]->GetYaxis()->SetLabelSize(0.12);
   hRatio[1]->GetYaxis()->SetLabelOffset(0.01);
   hRatio[1]->SetTitle("");
   hRatio[1]->GetXaxis()->SetTickLength(0.06);
   // end gangind syst tto stat
   //grCentralSystMadRatio->Draw("AP");
   //hRatio[1]->GetYaxis()->SetRangeUser(0.5,1.19);
   hRatio[1]->GetYaxis()->SetRangeUser(YMin, YMax);
   hRatio[1]->Draw("P");
   //PDFSystMad->SetFillColor(kGreen-3);
   //PDFSystMad->SetLineColor(kGreen-3);
   //PDFSystMad->SetFillStyle(3001);
   //PDFSystMad->Draw("E3 same");

//   grCentralSystMadRatio->Draw("P same");
//   hRatio[1]->Draw("P same");
   TLatex *latexLabel = new TLatex();
   latexLabel->SetTextSize(0.15);
   latexLabel->SetTextFont(42);
   latexLabel->SetLineWidth(2);
   latexLabel->SetTextColor(kAzure-5);
   latexLabel->SetNDC();
   latexLabel->DrawLatex(0.15,0.09,sLegend[1].c_str());
   TLegend *legend2 = new TLegend(0.75,0.85,0.9,0.95);
   legend2->SetFillStyle(0);
   legend2->SetBorderSize(0);
   //legend2->AddEntry(PDFSystMad, " PDF Syst.", "f");
   legend2->Draw();

   pad2->Draw();
   pad2->cd();

   can->cd();
   if ( NHistos == 3 ) {
   TPad *pad3 = new TPad("pad3","pad3", 0, 0., 1, 0.2286);
   pad3->SetTopMargin(0);
   pad3->SetBottomMargin(0.25);
   pad3->SetRightMargin(0.02);
   pad3->SetLeftMargin(0.11);
   pad3->SetGridy();
   pad3->SetTicks();
   pad3->Draw();
   pad3->cd();
   hRatio[2]->SetMarkerStyle(20);
   hRatio[2]->SetMarkerSize(0.8);
   hRatio[2]->SetMarkerColor(kBlack);
   hRatio[2]->SetLineColor(kBlack);
   // changing Syst to stat
   //hRatio[2]->SetLineColor(kRed+1);
   hRatio[2]->SetLineColor(kBlack+1);
   hRatio[2]->SetMarkerStyle(20);
   hRatio[2]->SetMarkerSize(0.8);
//   hRatio[2]->GetXaxis()->SetLimits(lowerLimit, hRatio[2]->GetXaxis()->GetXmax());
   hRatio[2]->GetXaxis()->SetTitle(hRatio[1]->GetXaxis()->GetTitle());
   hRatio[2]->GetXaxis()->SetTitleSize(0.1);
   hRatio[2]->GetXaxis()->SetTitleOffset(1.05);
   hRatio[2]->GetXaxis()->SetLabelSize(0.12);
   hRatio[2]->GetXaxis()->SetLabelOffset(0.01);
//   hRatio[2]->GetYaxis()->SetRangeUser(0.5,1.19);
   hRatio[2]->GetYaxis()->SetRangeUser(YMin, YMax);
   hRatio[2]->GetYaxis()->SetTitle("Ratio");
   hRatio[2]->GetYaxis()->SetTitleSize(0.1);
   hRatio[2]->GetYaxis()->SetTitleOffset(0.6);
   hRatio[2]->GetYaxis()->CenterTitle();
   hRatio[2]->GetYaxis()->SetLabelSize(0.09);
   hRatio[2]->GetYaxis()->SetLabelOffset(0.012);
   hRatio[2]->SetTitle("");
   hRatio[2]->GetXaxis()->SetTickLength(0.06);
   hRatio[2]->Draw("P");
    // end gangind syst tto stat

   //hRatio[2]->Draw("P same");
   //PDFSystShe->SetFillColor(kOrange-3);
   //PDFSystShe->SetLineColor(kOrange-3);
   //PDFSystShe->SetFillStyle(3001);
   //PDFSystShe->Draw("E3 same");
   //grCentralSystSheRatio->Draw("P same");
   hRatio[2]->Draw("P same");
   TLatex *latexLabel1 = new TLatex();
   latexLabel1->SetTextSize(0.11);
   latexLabel1->SetTextFont(42);
   latexLabel1->SetLineWidth(2);
   latexLabel1->SetTextColor(kOrange+2);
   latexLabel1->SetNDC();
   latexLabel1->DrawLatex(0.15,0.35,sLegend[2].c_str());
   pad3->Draw();
   pad3->cd();
   }
   can->cd();


  cout << " used legend:"<< sLegend[0] << endl;
  return can;

}
