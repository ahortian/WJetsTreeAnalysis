
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <string.h>
#include <algorithm>

#include <RooUnfoldResponse.h>
#include <TStyle.h>
#include <TFile.h>

#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLegend.h>
#include <TList.h>

#include <RooUnfoldResponse.h>
#include <RooUnfoldSvd.h>
#include <RooUnfoldBayes.h>
#include <RooUnfoldBinByBin.h>
#include <RooUnfoldInvert.h>
#include <TSVDUnfold.h>

#include "unfoldingFunctions.h"
#include "getFilesAndHistograms.h"
#include "fileNames.h"
#include "variablesOfInterestVarWidth.h"
#include "WbSystematics.h"
using namespace std;



void computeWbSyst(string leptonFlavor, string var, bool closureTest)
{
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    gStyle->SetOptStat(0);
    gStyle->SetPaintTextFormat("1.2f");
    
    string command = "mkdir -p PNGFiles/WbSystematics" ;
    system(command.c_str());
    
    closureTest = false;
    
    //--- load the proper VAROFINTERST structure: Z+jets or W+jets ---
    cout << " ... Loading the proper Variable Of Interest ...." << endl;
    int nsel = 0;
    const int *sel = NULL;
    variableStruct *varStruct = NULL;
    TList *varList = new TList();
    int nVarStruct;
    if (leptonFlavor == "DMu" || leptonFlavor == "DE"){
        nVarStruct = NVAROFINTERESTZJETS;
        varStruct = new variableStruct[nVarStruct];
        varStruct = VAROFINTERESTZJETS;
        makeLISTOFVAROFINTERESTZJETS();
        varList = LISTOFVAROFINTERESTZJETS;
        nsel = NFILESDYJETS;
        sel = new int[nsel];
        sel = FilesDYJets;
    }
    else if (leptonFlavor == "SMu" || leptonFlavor == "SE"){
        nVarStruct = NVAROFINTERESTWJETS;
        varStruct = new variableStruct[nVarStruct];
        varStruct = VAROFINTERESTWJETS;
        makeLISTOFVAROFINTERESTWJETS();
        varList = LISTOFVAROFINTERESTWJETS;
        nsel = NFILESTTBAR;
        sel = new int[nsel];
        sel = FilesTTbarWJets;
    }
    
    //---------------------------------------------
    
    //--- check wether muon or electron is used ---
    bool isMuon;
    if (leptonFlavor.find("Mu") != string::npos) isMuon = 1;
    else isMuon = 0;
    //---------------------------------------------
    
    if (var == ""){
        //nVarStruct = 1;
        for (int i(40); i < 50/*nVarStruct*/; i++){
            cout << setw(3) << i << "...>>> Processing variable: " << varStruct[i].name << endl;
            if (!isMuon) UnfoldSystWb(sel, nsel, leptonFlavor, varStruct[i].name, varStruct[i].log, varStruct[i].decrease, varStruct[i].ESVDkterm, varStruct[i].EBayeskterm);
            if (isMuon)  UnfoldSystWb(sel, nsel, leptonFlavor, varStruct[i].name, varStruct[i].log, varStruct[i].decrease, varStruct[i].MuSVDkterm, varStruct[i].MuBayeskterm);
            cout << "Type \"n\" for next" << endl;
            int a = 0;
            cin >> a;
            cin.clear();
            cin.ignore();
        }
    }
    else{
        if(varList->Contains(var.c_str())){
            TObjString *name = new TObjString(var.c_str());
            unsigned short j(varList->IndexOf(name));
            cout << setw(3) << " " << "...>>> Processing variable: " << varStruct[j].name << endl;
            if (!isMuon) UnfoldSystWb(sel, nsel, leptonFlavor, varStruct[j].name, varStruct[j].log, varStruct[j].decrease, varStruct[j].ESVDkterm, varStruct[j].EBayeskterm, closureTest);
            if (isMuon)  UnfoldSystWb(sel, nsel, leptonFlavor, varStruct[j].name, varStruct[j].log, varStruct[j].decrease, varStruct[j].MuSVDkterm, varStruct[j].MuBayeskterm, closureTest);
        }
        else{
            if (!isMuon) UnfoldSystWb(sel, nsel, leptonFlavor, var.c_str(),1,1,5,5);
            if (isMuon)  UnfoldSystWb(sel, nsel, leptonFlavor, var.c_str(),1,1,5,5);
        }
    }
    
}


void UnfoldSystWb(const int *sel, int nsel, string leptonFlavor, string variable, bool logZ, bool decrease, int SVDkterm, int Bayeskterm, bool closureTest, int JetPtMin, int JetPtMax, bool doFlat, bool doVarWidth)
{
    
    //--- make sure we have trailing "_" at the end of leptonFlavor
    if (leptonFlavor == "Muons" || leptonFlavor == "DMu") leptonFlavor = "DMu_";
    else if (leptonFlavor == "Electrons" || leptonFlavor == "DE") leptonFlavor = "DE_";
    else if (leptonFlavor == "Muon" || leptonFlavor == "SE") leptonFlavor = "SE_";
    else if (leptonFlavor == "Electron" || leptonFlavor == "SMu") leptonFlavor = "SMu_";
    
    //--- retreive the year from current location and set the energy accordingly
    string energy = getEnergy();
    
    //if (variable.find("JetPt")!= string::npos && variable.find("Highest") == string::npos && JetPtMin == 30)  JetPtMin = 20 ;
    
    ostringstream nameStr;  nameStr << "PNGFiles/WbSystematics/WbSystematics_";
    nameStr << variable;
    nameStr << ".root";
    TFile *outputRootFile = new TFile(nameStr.str().c_str(), "RECREATE");
    
    
    //-- get data histogram ----------------------------
    TFile *DataFile = NULL;
    DataFile = getFile(FILESDIRECTORY, leptonFlavor, energy, ProcessInfo[sel[0]].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, -1, "", "0", false, 0);
    TH1D *meas = getHisto(DataFile, variable);
    
    //-- get MC madgraph histograms ---------------------
    TFile *DY = NULL;
    DY = getFile(FILESDIRECTORY, leptonFlavor, energy, ProcessInfo[sel[nsel-1]].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, -1, "", "0", false, 0);
    
    TH1D *recoMad = (TH1D*) getHisto(DY, variable);
    string genName = "gen" + variable;
    TH1D *genMad = NULL;
    genMad = (TH1D*) DY->Get(genName.c_str());
    
    
    //-- get MC madgraph histograms for syst ---------------------
    string HistoFilesDirWbSyst = "HistoFilesWbSyst/";
    TFile *DYsyst = NULL;
    DYsyst = getFile(HistoFilesDirWbSyst, leptonFlavor, energy, ProcessInfo[sel[nsel-1]].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, -1, "", "0", false, 0);
    
    
    //-- get BG histograms -----------------------------------------------------------
    int nBG(nsel-2);
    if (closureTest) nBG = 0;
    TFile **BG = new TFile*[nBG];
    TH1D **hBG = new TH1D*[nBG];
    for (int i(0); i < nBG; i++){
        BG[i] = getFile(FILESDIRECTORY, leptonFlavor, energy, ProcessInfo[sel[i+1]].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, -1, "", "0", false, 0);
        hBG[i] = getHisto(BG[i], variable);
        
    }
    
    //-- build DataSubBG ------
    TH1D *hMeas = (TH1D*) meas->Clone();
    int nBins(hMeas->GetNbinsX());
    for (int i(0); i < nBG; i++) {
        hMeas->Add(hBG[i], -1);
    }
    
    //-- build the repsonse matrix ------------------------
    string respName = "response" + variable;
    RooUnfoldResponse *response = (RooUnfoldResponse*) DY->Get(respName.c_str());
    response->UseOverflow();
    
    
    //-- build the repsonse matrix for syst ------------------------
    RooUnfoldResponse *responseSyst = (RooUnfoldResponse*) DYsyst->Get(respName.c_str());
    responseSyst->UseOverflow();
    
    
    string method = "Bayes";
    cout << "...>>> Start processing Unfolding [Central] using method: " << method << " with kterm: " << Bayeskterm << endl;
    RooUnfoldBayes unfoldC(response, hMeas, Bayeskterm);
    unfoldC.SetVerbose(0);
    TH1D *hUnfoldedC = NULL;
    hUnfoldedC = (TH1D*) unfoldC.Hreco();
    
    
    
    cout << "...>>> Start processing Unfolding [WbSyst] using method: " << method << " with kterm: " << Bayeskterm << endl;
    RooUnfoldBayes unfoldT(responseSyst, hMeas, Bayeskterm);
    unfoldT.SetVerbose(0);
    TH1D *hUnfoldedT = NULL;
    hUnfoldedT = (TH1D*) unfoldT.Hreco();
    
    
    cout << " save root file of " << nameStr.str().c_str() << endl;
    outputRootFile->cd();
    hUnfoldedC->Write("central");
    hUnfoldedT->Write("WbSyst");
    genMad->Write("genMad");
    
    for (int j = 1; j <= nBins; j++) {
        cout << hUnfoldedC->GetBinContent(j) << "\t" << hUnfoldedC->GetBinError(j) << "\t" ;
        cout << hUnfoldedT->GetBinContent(j) << "\t" << hUnfoldedT->GetBinError(j) << "\t" ;
        cout << hUnfoldedT->GetBinContent(j)/hUnfoldedC->GetBinContent(j) << endl;
    }
    
    
    //--- plotting
    TH1D *dataCentral = (TH1D*) hUnfoldedC->Clone();
    TH1D *hSyst = (TH1D*) hUnfoldedT->Clone();
    
    TLatex *cmsColl = new TLatex();
    cmsColl->SetTextSize(0.025);
    cmsColl->SetTextFont(42);
    cmsColl->SetLineWidth(2);
    cmsColl->SetTextColor(kBlack);
    cmsColl->SetNDC();
    cmsColl->SetTextAlign(11);
    
    TCanvas *canUnfoldedJES = new TCanvas(variable.c_str(), variable.c_str(), 700, 900);
    canUnfoldedJES->cd();
    
    // PAD 1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1);
    pad1->SetTopMargin(0.11);
    pad1->SetBottomMargin(0);
    pad1->SetRightMargin(0.04);
    
    pad1->Draw();
    pad1->cd();
    if (logZ) pad1->SetLogy();
    dataCentral->SetTitle(variable.c_str());
    dataCentral->GetYaxis()->SetTitleOffset(1.4);
    dataCentral->SetLineColor(kBlack);
    dataCentral->SetLineWidth(2);
    dataCentral->SetFillStyle(3003);
    dataCentral->SetFillColor(kGray+2);
    dataCentral->DrawCopy("HIST");
    
    hSyst->SetTitle("");
    hSyst->SetMarkerStyle(24);
    hSyst->SetMarkerColor(kBlue+1);
    hSyst->SetLineColor(kBlue+1);
    hSyst->SetLineWidth(2);
    hSyst->DrawCopy("same");
    
    
    double xLowLeg(0.76), xHighLeg(0.95);
    if (!decrease){
        xLowLeg = 0.14;
        xHighLeg = 0.47;
    }
    TLegend *leg = new TLegend(xLowLeg, 0.63, xHighLeg, 0.89);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->AddEntry(dataCentral, " Central ", "fl");
    leg->AddEntry(hSyst, "WbSyst", "pl");
    leg->Draw();
    
    cmsColl->DrawLatex(0.11, 0.905, " Systematic: WbSyst");
    pad1->Draw();
    canUnfoldedJES->cd();
    
    // PAD 2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.3);
    pad2->SetRightMargin(0.04);
    
    pad2->SetTicks();
    pad2->SetGridy();
    pad2->Draw();
    pad2->cd();
    
    hSyst->Divide(dataCentral);
    
    hSyst->SetTitle("");
    hSyst->GetXaxis()->SetTickLength(0.03);
    hSyst->GetXaxis()->SetTitleSize(0.1);
    hSyst->GetXaxis()->SetLabelSize(0.09);
    hSyst->GetXaxis()->SetLabelOffset(0.018);
    hSyst->GetXaxis()->SetTitleOffset(1.2);
    
    hSyst->GetYaxis()->SetRangeUser(0.71, 1.29);
    hSyst->GetYaxis()->SetNdivisions(5,5,0);
    hSyst->GetYaxis()->SetTitle(" Syst / Central ");
    hSyst->GetYaxis()->SetTitleSize(0.1);
    hSyst->GetYaxis()->SetTitleOffset(0.5);
    hSyst->GetYaxis()->CenterTitle();
    hSyst->GetYaxis()->SetLabelSize(0.08);
    
    hSyst->DrawCopy("E1");
    pad2->Draw();
    
    string outputDirectory = "PNGFiles/WbSystematics/";
    string outputFileName = outputDirectory + variable + "UnfoldedWbSyst.pdf";
    canUnfoldedJES->Print(outputFileName.c_str());
    cout << " save pdf file of " << outputFileName << " " << hSyst->GetName() << endl;
    
    
    
    //--- close every files ----------------------------------------------------------
    outputRootFile->Close();
    closeFile(DataFile);
    closeFile(DY);
    closeFile(DYsyst);
    for (int i(0); i < nBG; i++) closeFile(BG[i]);
    //-------------------------

}




