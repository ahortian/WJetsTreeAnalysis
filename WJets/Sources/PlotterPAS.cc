#include <vector>
#include <TFile.h>
#include <TStyle.h>
#include <TH1.h>
#include <TH2.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TPaveStats.h>
#include <TLatex.h>
#include <TLegend.h>
#include <iostream>
#include <sstream>
#include "getFilesAndHistograms.h"

using namespace std;

//--  Setting global variables --------------------------------------------------------------
#include "fileNames.h"
#include "variablesOfInterestVarWidth.h"
//-------------------------------------------------------------------------------------------

void PlotterPAS(string leptonFlavor = "Electrons", string DYSample = "MadGraph", int JetPtMin = 30, int JetPtMax = 0, int ZEtaMin = -999999, int ZEtaMax = 999999, bool doRoch = 0, bool doFlat = 0, bool doVarWidth = 1)
{
  makeLISTOFVAROFINTERESTZJETS();
  string energy = getEnergy();

  cout << endl << "Running the Plotter with the following files as input: " << endl;
  TH1::SetDefaultSumw2();
  gStyle->SetOptStat(0);

  ostringstream JetPtMinStr;  JetPtMinStr << JetPtMin;
  ostringstream JetPtMaxStr;  JetPtMaxStr << JetPtMax;
  ostringstream ZEtaMinStr;   ZEtaMinStr << abs(ZEtaMin); 
  ostringstream ZEtaMaxStr;   ZEtaMaxStr << abs(ZEtaMax); 

  int nFiles(10);
  int DYSampleNum(0);
  if (DYSample == "MadGraph") DYSampleNum = 0;
  else if (DYSample == "Powheg") DYSampleNum = 1;
  else if (DYSample == "Sherpa") DYSampleNum = 2;
  string year = "2012"; 
  if (energy == "7TeV") year = "2011";
  //string legendNames[] = {" #mu#mu ", " ZZJets2L2Nu", " ZZJets4L", " ZZJets2L2Q", " WZJets3LNu", " WZJets2L2Q", " WWJets2L2Nu", " DYtautau", " TTJets", " DYJets MD", " DYJets Po", " DYJets Sh"};
  string legendNames[] = {" #mu#mu ", " VVJets", " Tops", " DYJets MD", " DYJets Po", " DYJets Sh"};
  if (leptonFlavor == "Electrons") legendNames[0] = " ee ";
  legendNames[0] += "Data";




  //-- get the files ------------------------------------
  int NFiles = NFILESDYJETS ;
  TFile *file[20];
  bool isDoubleLep(1);
  if ( leptonFlavor == "SMuE" || leptonFlavor == "SMu" || leptonFlavor == "Muon" || leptonFlavor == "Electron") {
      isDoubleLep = 0;
      nFiles = NFILESTTBAR;
  }

  for (int i = 0; i < NFiles; i++) {
      int fileSelect = FilesDYJets[i] ;
      if (!isDoubleLep) fileSelect = FilesTTbarWJets[i] ;
      if ( leptonFlavor == "SMuE"  ) fileSelect = FilesTTbar[i] ;

      string fileNameTemp =  ProcessInfo[fileSelect].filename ;

      file[i] = getFile(FILESDIRECTORY, leptonFlavor, energy, fileNameTemp, JetPtMin, JetPtMax, doFlat, doVarWidth);
      
  }
  //-----------------------------------------------------


  string outputFileName = "PNGFiles/PAS_Comparison_" + leptonFlavor + "_" + energy + "_Data_All_MC_" + DYSample + "_";

  outputFileName += "JetPtMin_" + JetPtMinStr.str();
  if (JetPtMax > JetPtMin) outputFileName += "_JetPtMax_" + JetPtMaxStr.str();
  if (ZEtaMin > -999999 && ZEtaMin <  0 ) outputFileName += "_ZEtaMin_m" + ZEtaMinStr.str();
  if (ZEtaMin > -999999 && ZEtaMin >= 0 ) outputFileName += "_ZEtaMin_" + ZEtaMinStr.str();
  if (ZEtaMax <  999999 && ZEtaMax >= 0 ) outputFileName += "_ZEtaMax_" + ZEtaMaxStr.str();
  if (ZEtaMax <  999999 && ZEtaMax <  0 ) outputFileName += "_ZEtaMax_m" + ZEtaMaxStr.str();
  if (doRoch) outputFileName += "_rochester";
  if (doFlat) outputFileName += "_Flat";
  if (doVarWidth) outputFileName += "_VarWidth";
  outputFileName += "_SFInvers";
  // create the directory if it doesn't exist
  string command = "mkdir -p " + outputFileName;
  system(command.c_str());
  string outputFileRoot = outputFileName + ".root";

  cout << "Output directory is: " << outputFileName << endl;
  cout << "Output root file is: " << outputFileRoot << endl;

  TFile *outputFile = new TFile(outputFileRoot.c_str(), "RECREATE");
  outputFile->cd();

  int nHist = file[0]->GetListOfKeys()->GetEntries();
  vector<string> histoName(nHist);
  vector<string> histoTitle(nHist);
  string histoNameTemp;
  TCanvas *canvas[nHist];
  TPad *pad1[nHist];
  TPad *pad2[nHist];

  TH1D *hist[12][nHist];
  TH1D *histVVJets[nHist];
  TH1D *histSignal[nHist];
  TH1D *histTTJets[nHist];
  TH1D *histOther[nHist];
  TH1D *histTemp;
  THStack *histSumMC[nHist];
  TLegend *legend[nHist];
  TLatex *cmsColl[nHist];
  TLatex *sqrtXTeV[nHist];
  TLatex *intLumi[nHist];
  int nHistNoGen=0;
  for (int i = 0; i < nHist; i++) {
      bool decrease(true);
      histoNameTemp = file[0]->GetListOfKeys()->At(i)->GetName();
      if (histoNameTemp.find("gen") != string::npos) continue;
      if (!file[0]->Get(histoNameTemp.c_str())->InheritsFrom(TH1::Class())) continue;
      if (LISTOFVAROFINTERESTZJETS->Contains(histoNameTemp.c_str())) {
          TObjString *name = new TObjString(histoNameTemp.c_str());
          int indexOfVariable(LISTOFVAROFINTERESTZJETS->IndexOf(name));
          decrease = VAROFINTERESTZJETS[indexOfVariable].decrease;
      }
      histTemp = (TH1D*) file[0]->Get(histoNameTemp.c_str());
      if (histTemp->GetEntries()<1) continue;
      histoName[nHistNoGen] = file[0]->GetListOfKeys()->At(i)->GetName();
      histoTitle[nHistNoGen] = file[0]->GetListOfKeys()->At(i)->GetTitle();
      histSumMC[nHistNoGen] = new THStack(histoName[nHistNoGen].c_str(), histoTitle[nHistNoGen].c_str());
      //double xLowLeg(0.71), xHighLeg(1.);
      double xLowLeg(0.79), xHighLeg(0.89);
      if (!decrease){
          xLowLeg = 0.125;
          xHighLeg = 0.225;
      }
      legend[nHistNoGen] = new TLegend(xLowLeg, 0.65, xHighLeg, 0.87);
      legend[nHistNoGen]->SetFillStyle(0);
      legend[nHistNoGen]->SetBorderSize(0);
      legend[nHistNoGen]->SetTextSize(0.03);
      legend[nHistNoGen]->SetTextFont(42);

      cmsColl[nHistNoGen] = new TLatex();
      cmsColl[nHistNoGen]->SetTextSize(0.04);
      cmsColl[nHistNoGen]->SetTextFont(42);
      cmsColl[nHistNoGen]->SetLineWidth(2);
      cmsColl[nHistNoGen]->SetTextColor(kBlack);
      cmsColl[nHistNoGen]->SetNDC();
      cmsColl[nHistNoGen]->SetTextAlign(11);

      sqrtXTeV[nHistNoGen] = new TLatex();
      sqrtXTeV[nHistNoGen]->SetTextSize(0.04);
      sqrtXTeV[nHistNoGen]->SetTextFont(42);
      sqrtXTeV[nHistNoGen]->SetLineWidth(2);
      sqrtXTeV[nHistNoGen]->SetTextColor(kBlack);
      sqrtXTeV[nHistNoGen]->SetNDC();
      sqrtXTeV[nHistNoGen]->SetTextAlign(21);

      intLumi[nHistNoGen] = new TLatex();
      intLumi[nHistNoGen]->SetTextSize(0.03);
      intLumi[nHistNoGen]->SetTextFont(42);
      intLumi[nHistNoGen]->SetLineWidth(2);
      intLumi[nHistNoGen]->SetTextColor(kBlack);
      intLumi[nHistNoGen]->SetNDC();
      intLumi[nHistNoGen]->SetTextAlign(31);

      nHistNoGen++;
  } 
  nHist=nHistNoGen; 
  //nHist=10;
  int Colors[] = {kBlack, kSpring+5, kOrange, kOrange-3, kRed+1, kPink-6, kViolet+5, kAzure+4, kBlue, kCyan+1, kCyan+1, kCyan+1}; 
  
  for (int j = 0; j < nHist; j++){
      int countVV = 0 ;
      int countTops = 0;
      int countOther = 0;
      for (int i = 0; i < NFiles; i++) {
          int fileSelect = FilesDYJets[i] ;
          if (!isDoubleLep) fileSelect = FilesTTbarWJets[i] ;
          if ( leptonFlavor == "SMuE"  ) fileSelect = FilesTTbar[i] ;

          string fileNameTemp =  ProcessInfo[fileSelect].filename ;

          cout << "hists" << j <<"    "<<i <<"   " << fileNameTemp<<"   " << histoName[j] <<  endl;

          if ( fileNameTemp.find("Data") != string::npos ) {
              hist[0][j] = getHisto(file[i], histoName[j]);
              hist[0][j]->SetMarkerStyle(20);
              hist[0][j]->SetMarkerColor(Colors[0]);
              hist[0][j]->SetLineColor(Colors[0]);
          }
          else {
              //  histSumMC[j]->Add(getHisto(file[i], histoName[j]));

              if (  fileNameTemp.find("WW") != string::npos ||  fileNameTemp.find("WZ") != string::npos || fileNameTemp.find("ZZ") != string::npos ){
                  if ( countVV == 0 ) histVVJets[j] = getHisto(file[i], histoName[j]);
                  else histVVJets[j]->Add(getHisto(file[i], histoName[j]));
                  countVV++;
                  histVVJets[j]->SetFillColor(Colors[3]);
                  histVVJets[j]->SetLineColor(Colors[3]);
              }
              else if (  fileNameTemp.find("Top") != string::npos ||  fileNameTemp.find("TT") != string::npos){
                  if ( countTops == 0 )  histTTJets[j] = getHisto(file[i], histoName[j]);
                  else histTTJets[j] ->Add(getHisto(file[i], histoName[j]));
                  countTops++;
                  histTTJets[j]->SetFillColor(Colors[5]);
                  histTTJets[j]->SetLineColor(Colors[5]);
              }
              else if ( isDoubleLep && fileNameTemp.find("DYJets") != string::npos &&  fileNameTemp.find("UNF") != string::npos && fileNameTemp.find("Tau") == string::npos){
                  histSignal[j] =  getHisto(file[i], histoName[j]);
                  histSignal[j]->SetFillColor(Colors[7]);
                  histSignal[j]->SetLineColor(Colors[7]);

              }
              else {
                if ( countOther == 0 ) histOther[j] =  getHisto(file[i], histoName[j]);
                else histOther[j] ->Add(getHisto(file[i], histoName[j]));
                histOther[j] ->SetFillColor(Colors[4]);
                histOther[j] ->SetLineColor(Colors[4]);
                  cout << __LINE__<<endl;
              }
          }
      }
      // add them
      histSumMC[j]->Add(histOther[j]);
      histSumMC[j]->Add(histTTJets[j]);
      histSumMC[j]->Add(histVVJets[j]);
      histSumMC[j]->Add(histSignal[j]);
    if ( isDoubleLep) legend[j]->AddEntry(histOther[j], " Tau");
    legend[j]->AddEntry(histVVJets[j], " VV", "f");
    legend[j]->AddEntry(histTTJets[j], " TT/T", "f");
    if ( isDoubleLep) legend[j]->AddEntry(histSignal[j], " DYJets MD", "f");
    else  legend[j]->AddEntry(histSignal[j], " WJets MD", "f");
  }




  for (int i = 0; i < nHist; i++) {
      if ( ! file[0]->Get(histoName[i].c_str())->InheritsFrom(TH1::Class())) continue;
      legend[i]->AddEntry(hist[0][i], legendNames[0].c_str(), "ep");
      canvas[i] = new TCanvas(histoName[i].c_str(), histoName[i].c_str(), 700, 900);
      pad1[i] = new TPad("pad1", "pad1", 0, 0.3, 1, 1);
      pad1[i]->SetTopMargin(0.11);
      pad1[i]->SetBottomMargin(0.);
      pad1[i]->SetRightMargin(0.02);
      pad1[i]->SetTicks();
      pad1[i]->SetLogy();
      pad1[i]->Draw();
      pad1[i]->cd();

      histSumMC[i]->SetTitle(""); 
      histSumMC[i]->Draw("HIST"); 
      histSumMC[i]->GetYaxis()->SetLabelSize(0.04); 
      histSumMC[i]->GetYaxis()->SetLabelOffset(0.002); 
      histSumMC[i]->GetYaxis()->SetTitle("# Events"); 
      histSumMC[i]->GetYaxis()->SetTitleSize(0.04); 
      histSumMC[i]->GetYaxis()->SetTitleOffset(1.32); 
      histSumMC[i]->SetMinimum(8);
      histSumMC[i]->SetMaximum(2.5*histSumMC[i]->GetMaximum()); 
      hist[0][i]->DrawCopy("E SAME");
      legend[i]->Draw();
      cmsColl[i]->DrawLatex(0.11,0.94, "CMS Preliminary");
      if (energy == "7TeV") sqrtXTeV[i]->DrawLatex(0.54,0.94, "#sqrt{s} = 7 TeV");
      if (energy == "8TeV") sqrtXTeV[i]->DrawLatex(0.54,0.94, "#sqrt{s} = 8 TeV");
      if (energy == "7TeV") intLumi[i]->DrawLatex(0.98,0.94, "#int L dt = 5.05 fb^{-1}");
      if (energy == "8TeV") intLumi[i]->DrawLatex(0.98,0.94, "#int L dt = 19.6 fb^{-1}");
      pad1[i]->Draw();


      canvas[i]->cd();
      pad2[i] = new TPad("pad2", "pad2", 0, 0, 1, 0.3);
      pad2[i]->SetTopMargin(0.);
      pad2[i]->SetBottomMargin(0.3);
      pad2[i]->SetRightMargin(0.02);
      pad2[i]->SetGridy();
      pad2[i]->SetTicks();
      pad2[i]->Draw();
      pad2[i]->cd();
/*
      TH1D* hMCAll = (TH1D*) histSumMC[i]->GetStack()->Last() ;
      hMCAll->SetStats(0);
      hMCAll->SetTitle("");


      hMCAll->GetXaxis()->SetTickLength(0.03);
      hMCAll->GetXaxis()->SetTitleSize(0.1);
      hMCAll->GetXaxis()->SetTitleOffset(1.2);
      hMCAll->GetXaxis()->SetLabelSize(0.1);
      hMCAll->GetXaxis()->SetLabelOffset(0.018);

      hMCAll->GetYaxis()->SetRangeUser(0.51,1.49);
      hMCAll->GetYaxis()->SetNdivisions(5,5,0);
      hMCAll->GetYaxis()->SetTitle("MC/Data");
      hMCAll->GetYaxis()->SetTitleSize(0.1);
      hMCAll->GetYaxis()->SetTitleOffset(0.5);
      hMCAll->GetYaxis()->CenterTitle();
      //hist[0][i]->GetYaxis()->SetLabelOffset(0.006);
      hMCAll->GetYaxis()->SetLabelSize(0.08);
*/

      hist[0][i]->SetStats(0);
      hist[0][i]->SetTitle("");


      hist[0][i]->GetXaxis()->SetTickLength(0.03);
      hist[0][i]->GetXaxis()->SetTitleSize(0.1);
      hist[0][i]->GetXaxis()->SetTitleOffset(1.2);
      hist[0][i]->GetXaxis()->SetLabelSize(0.1);
      hist[0][i]->GetXaxis()->SetLabelOffset(0.018);

      hist[0][i]->GetYaxis()->SetRangeUser(0.51,1.49);
      hist[0][i]->GetYaxis()->SetNdivisions(5,5,0);
      hist[0][i]->GetYaxis()->SetTitle("MC/Data");
      hist[0][i]->GetYaxis()->SetTitleSize(0.1);
      hist[0][i]->GetYaxis()->SetTitleOffset(0.5);
      hist[0][i]->GetYaxis()->CenterTitle();
      //hist[0][i]->GetYaxis()->SetLabelOffset(0.006);
      hist[0][i]->GetYaxis()->SetLabelSize(0.08);

      hist[0][i]->Divide((TH1D*) histSumMC[i]->GetStack()->Last());
      for ( int k = 1 ; k <= hist[0][i]->GetNbinsX(); k++){
            double value = hist[0][i]->GetBinContent(k);
            if ( fabs(value) > 0 ) hist[0][i]->SetBinContent(k, 1/value);
            else hist[0][i]->GetBinContent(k, 0 );
      }
      hist[0][i]->DrawCopy("EP");

      //hMCAll->Divide((TH1D*) hist[0][i]);
      //hMCAll->DrawCopy("EP");
      canvas[i]->cd();

      string outputFilePNG = outputFileName + "/" + histoName[i] + ".pdf";
      canvas[i]->Print(outputFilePNG.c_str());
      outputFile->cd();
      canvas[i]->Write();
      pad1[i]->SetLogy(0);
      histSumMC[i]->SetMaximum(1.2*histSumMC[i]->GetMaximum()); 
      histoName[i] += "_Lin";
      canvas[i]->SetName(histoName[i].c_str());
      canvas[i]->SetTitle(histoName[i].c_str());
      string outputFileLinPNG = outputFileName + "/" + histoName[i] + ".pdf";
      canvas[i]->Print(outputFileLinPNG.c_str());
      outputFile->cd();
      canvas[i]->Write();

  }
  outputFile->cd();
  outputFile->Close();

  //-- Close all the files ------------------------------
  for (int i(0); i < 12; i++) closeFile(file[i]);
  //-----------------------------------------------------

}
