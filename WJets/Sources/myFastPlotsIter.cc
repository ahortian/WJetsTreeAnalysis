#include <TFile.h>
#include <TStyle.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <RooUnfoldResponse.h>
#include <RooUnfoldSvd.h>
#include <RooUnfoldBayes.h>
#include <RooUnfoldBinByBin.h>
#include <RooUnfoldInvert.h>
#include <TSVDUnfold.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLegend.h>
#include <TList.h>
#include "getFilesAndHistograms.h"
#include "unfoldingFunctions.h"
#include "writeFastPlotsTex.h"
#include "myFastPlotsIter.h"

//-- Setting global variables --------------------------------------------------------------
#include "fileNames.h"
#include "variablesOfInterestVarWidth.h"
//---


void myFastPlotsIter(string leptonFlavor, string var, string var2, bool closureTest) // var = std binning, var2 = fine binning
{
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    gStyle->SetOptStat(0);
    gStyle->SetPaintTextFormat("1.2f");

    //--- load the proper VAROFINTERST structure: Z+jets or W+jets ---
    int nsel = 0;
    const int *sel = NULL;
    variableStruct *varStruct = NULL;
    TList *varList = new TList();
    
    int nVarStruct(0);
    nsel = NFILESTTBAR;
    sel = new int[nsel];
    sel = FilesTTbarWJets;
    nVarStruct = NVAROFINTERESTWJETS;
    varStruct = new variableStruct[nVarStruct];
    varStruct = VAROFINTERESTWJETS;
    makeLISTOFVAROFINTERESTWJETS();
    varList = LISTOFVAROFINTERESTWJETS;
    //---------------------------------------------

    //--- check wether muon or electron is used ---
    bool isMuon;
    if (leptonFlavor.find("Mu") != string::npos) isMuon = 1;
    else isMuon = 0;
    //---------------------------------------------
    
    TObjString *name = new TObjString(var.c_str());
    unsigned short j(varList->IndexOf(name));
    
    cout << setw(3) << " " << ">> Std  binning Processing variable: " << varStruct[j].name << endl;
    cout << setw(3) << " " << ">> Fine binning Processing variable: " << var2 << endl;
    
    if (!isMuon) FastPlotsRun(sel, nsel, leptonFlavor, varStruct[j].name, var2, varStruct[j].log, varStruct[j].decrease, varStruct[j].ESVDkterm, varStruct[j].EBayeskterm, closureTest);
    if (isMuon)  FastPlotsRun(sel, nsel, leptonFlavor, varStruct[j].name, var2, varStruct[j].log, varStruct[j].decrease, varStruct[j].MuSVDkterm, varStruct[j].MuBayeskterm, closureTest);
}


void FastPlotsRun(const int *sel, int nsel, string leptonFlavor, string variableStd, string variable, bool logZ, bool decrease, int SVDkterm, int Bayeskterm, bool closureTest, int JetPtMin, int JetPtMax, bool doFlat, bool doVarWidth)
{
    //--- make sure we have trailing "_" at the end of leptonFlavor
    if (leptonFlavor == "Muons" || leptonFlavor == "DMu") leptonFlavor = "DMu_";
    else if (leptonFlavor == "Electrons" || leptonFlavor == "DE") leptonFlavor = "DE_";
    else if (leptonFlavor == "Muon" || leptonFlavor == "SE") leptonFlavor = "SE_";
    else if (leptonFlavor == "Electron" || leptonFlavor == "SMu") leptonFlavor = "SMu_";
    cout << "Stop after line " << __LINE__ << endl;
    //--- retreive the year from current location and set the energy accordingly
    string energy = getEnergy();

    ostringstream jetPtMin; jetPtMin << JetPtMin;
    string outputDirectory = "PNGFiles/FastPlots_Iter_" + jetPtMin.str() ;
    if (doVarWidth)  outputDirectory+= "_VarWidth";
    string command = "mkdir -p " + outputDirectory;
    system(command.c_str());
    outputDirectory +=  "/";

    string outputRootFileName = outputDirectory + leptonFlavor + energy + "_" + variable + "_fastplots";
    if (closureTest) outputRootFileName += "_ClosureTest";
    if (doVarWidth) outputRootFileName += "_VarWidth";
    string outputTexFileName = outputRootFileName;
    outputRootFileName += ".root";
    outputTexFileName += ".tex";

    TFile *outputRootFile = new TFile(outputRootFileName.c_str(), "RECREATE");
    ofstream outputTexFile(outputTexFileName.c_str());
    writeHeaderPage(outputTexFile, leptonFlavor, variable);
    
    cout << "Stop after line " << __LINE__ << endl;
    
    //-- get data histogram (or MC Even for closure test) ----------------------------
    TFile *DataFile = NULL;
    if (!closureTest) DataFile = getFile(FILESDIRECTORY, leptonFlavor, energy, ProcessInfo[sel[0]].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, -1, "", "0", false, 0);
    else DataFile = getFile(FILESDIRECTORY, leptonFlavor, energy, ProcessInfo[sel[nsel-1]].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, -1, "", "0", false, 0);
    
    TH1D *meas = getHisto(DataFile, variable);
    TH1D *measStd = getHisto(DataFile, variableStd);
    cout << "Stop after line " << __LINE__ << endl;
    
    //-- get BG histograms -----------------------------------------------------------
    int nBG(nsel-2);
    if (closureTest) nBG = 0;
    TFile **BG = new TFile*[nBG];
    TH1D **hBG = new TH1D*[nBG];
    TH1D **hBGStd = new TH1D*[nBG];
    for (int i(0); i < nBG; i++){
        BG[i] = getFile(FILESDIRECTORY, leptonFlavor, energy, ProcessInfo[sel[i+1]].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, -1, "", "0", false, 0);
        hBG[i] = getHisto(BG[i], variable);
        hBGStd[i] = getHisto(BG[i], variableStd);
    }
    
    //-- get meas - BG histograms -------------------------------------
    TH1D *measSubBG = (TH1D*) meas->Clone();
    TH1D *measSubBGStd = (TH1D*) measStd->Clone();
    for (int i(0); i < nBG; i++) {
        measSubBG->Add(hBG[i], -1);
        measSubBGStd->Add(hBGStd[i], -1);
    }
    
    //-- get MC madgraph file (or MC Odd for closure test) ---------------------
    TFile *DY = NULL;
    if (!closureTest) DY = getFile(FILESDIRECTORY, leptonFlavor, energy, ProcessInfo[sel[nsel-1]].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, -1, "", "0", false, 0);
    else DY = getFile(FILESDIRECTORY, leptonFlavor, energy, ProcessInfo[sel[nsel-1]].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, -1, "", "0", false, 0);
    
    //-- build the repsonse matrix, MCtruth, MCreco ------------------------
    string respName = "response" + variable;
    RooUnfoldResponse *response = (RooUnfoldResponse*) DY->Get(respName.c_str());
    response->UseOverflow();
    TH2D *hResponse = (TH2D*) response->Hresponse();
    TH1D *MCtruth   = (TH1D*) response->Htruth();
    TH1D *MCreco    = (TH1D*) response->Hmeasured();
    
    string respNameStd = "response" + variableStd;
    RooUnfoldResponse *responseStd = (RooUnfoldResponse*) DY->Get(respNameStd.c_str());
    responseStd->UseOverflow();
    TH2D *hResponseStd = (TH2D*) responseStd->Hresponse();
    TH1D *MCtruthStd   = (TH1D*) responseStd->Htruth();
    TH1D *MCrecoStd    = (TH1D*) responseStd->Hmeasured();
    
    //-- get MC madgraph Gen histogram --------------------------------
    string genName = "gen" + variable;
    TH1D *genMad = NULL;
    if (!closureTest) genMad = (TH1D*) DY->Get(genName.c_str());
    else genMad = (TH1D*) DY->Get(genName.c_str());
    
    string genNameStd = "gen" + variableStd;
    TH1D *genMadStd = NULL;
    if (!closureTest) genMadStd = (TH1D*) DY->Get(genNameStd.c_str());
    else genMadStd = (TH1D*) DY->Get(genNameStd.c_str());
    
    //-- get MC madgraph RECO histogram -------------------------------
    TH1D *recoMad = getHisto(DY, variable);
    TH1D *recoMadStd = getHisto(DY, variableStd);
    
    //--- Bins definition : nominal binning ---
    const int nBinsStd(measStd->GetNbinsX());
    double BinsArray[nBinsStd+1];
    for (int i = 0; i <= nBinsStd; i++){
        if (i == 0) BinsArray[i] = measStd->GetXaxis()->GetBinLowEdge(i+1);
        else BinsArray[i] = measStd->GetXaxis()->GetBinUpEdge(i);
    }
    
    
    //--- REBINNING --------------------------------------//
    
    cout << " Number of bin used = " << measSubBG->GetNbinsX() << endl;
    
    //--- No need to Rebin: So just clone it -----
    TH1D *histoMeasSubBG = (TH1D*) measSubBG->Clone();
    TH1D *histoMCtruth   = (TH1D*) MCtruth  ->Clone();
    TH1D *histoMCreco    = (TH1D*) MCreco   ->Clone();
    TH1D *histoGenMad    = (TH1D*) genMad   ->Clone();
    TH1D *histoRecoMad   = (TH1D*) recoMad  ->Clone();
    
    cout << " number of bins of hist = " << histoMeasSubBG->GetNbinsX() << histoMCtruth->GetNbinsX() << histoMCreco->GetNbinsX() << histoGenMad->GetNbinsX() << histoRecoMad->GetNbinsX() << endl;
    
    //--- Rebin 2D response matrix (no error consideration) ---
    //------ No need to Rebin: So just clone it -----
    TH2D *hResponseRebin = (TH2D*) hResponse->Clone();

    
    //--- Get nominal binning histo for comparison -----------
    TH1D *MeasSubBGCompare = (TH1D*) measSubBGStd->Clone();
    TH1D *MCtruthCompare   = (TH1D*) MCtruthStd  ->Clone();
    TH1D *MCrecoCompare    = (TH1D*) MCrecoStd   ->Clone();
    TH1D *GenMadCompare    = (TH1D*) genMadStd   ->Clone();
    TH1D *RecoMadCompare   = (TH1D*) recoMadStd  ->Clone();
    RooUnfoldResponse *responseCompare = (RooUnfoldResponse*)responseStd->Clone();
    RooUnfoldBayes unfoldObjCompare(responseCompare, MeasSubBGCompare, Bayeskterm);
    unfoldObjCompare.SetVerbose(0);
    TH1D *hUnfoldedCompare = (TH1D*) unfoldObjCompare.Hreco();
    //----------------------------------------------------------------
    
    
    //-------------------------- NEW UNFOLDING ----------------------------------------------
    //--- calculate 1st scaling factor
    const int nBinsFine(histoRecoMad->GetNbinsX());
    double fwFirst[nBinsFine];
    cout << "----------- fw 1st is -------------" << endl;
    for (int i = 1; i <= nBinsFine; i++){
        fwFirst[i] = 1;
        if ((histoRecoMad->GetBinContent(i) > 0) & (histoMeasSubBG->Integral() > 0)){
            fwFirst[i] = (histoMeasSubBG->GetBinContent(i) / histoRecoMad->GetBinContent(i)) * (histoRecoMad->Integral() / histoMeasSubBG->Integral());
        }
        //cout << fwFirst[i] << endl;
    }
    
    TH1D *histoFw = (TH1D*) histoMeasSubBG->Clone();
    histoFw->Divide(histoRecoMad);
    histoFw->Scale(histoRecoMad->Integral() / histoMeasSubBG->Integral());
    
    //***********************************
    
    double RecoToProjectX[nBinsFine];
    for (int j = 1; j <= hResponseRebin->GetXaxis()->GetNbins(); j++){
        RecoToProjectX[j] = 1;
        if(hResponseRebin->ProjectionX()->GetBinContent(j) > 0){
            RecoToProjectX[j]  = (histoMCreco->GetBinContent(j) / hResponseRebin->ProjectionX()->GetBinContent(j));
        }
        //cout << " RecoToProjectX   " << RecoToProjectX[j] << endl;
    }
    
    double TruthToProjectY[nBinsFine];
    for (int j = 1; j <= hResponseRebin->GetYaxis()->GetNbins(); j++){
        TruthToProjectY[j] = 1;
        if(hResponseRebin->ProjectionY()->GetBinContent(j) > 0){
            TruthToProjectY[j]  = (histoMCtruth->GetBinContent(j) / hResponseRebin->ProjectionY()->GetBinContent(j));
        }
        //cout << " TruthToProjectY   " << TruthToProjectY[j] << endl;
    }
    
    double fwNew[nBinsFine];
    double fwTemp[nBinsFine];
    
    for (int j = 1; j <= nBinsFine; j++){
        fwNew[j]  = fwFirst[j];
    }
    
    TH2D *hResponseTemp = (TH2D*) hResponseRebin->Clone();
    TH1D *MCtruthTemp = (TH1D*) histoMCtruth->Clone();
    TH1D *MCrecoTemp = (TH1D*) histoMCreco->Clone();
    
    TH1D *hUnfoldedTempNominal = (TH1D*) genMadStd->Clone();
    
    
    TCanvas* canvas2 = new TCanvas("c2", "canvas2", 700, 900);
    TCanvas* canvas3 = new TCanvas("c3", "canvas3", 900, 700);
    
    // loop here
    
    //--- set fwTemp to new values
    for (int j = 1; j <= nBinsFine; j++){
        fwTemp[j]  = fwNew[j];
        //cout << fwFirst[j] << " " << fwNew[j] << " " << fwTemp[j] << " " << histoFw->GetBinContent(j) << endl;
    }
    
    //----------- Set the current looping values:
        //--- hResponseTemp
    for (int j = 1; j <= nBinsFine; j++){
        for (int i = 1; i <= nBinsFine; i++){
            hResponseTemp->SetBinContent(i, j, fwTemp[i] * hResponseRebin->GetBinContent(i, j));
        }
    }
        //--- MCtruthTemp, MCrecoTemp
    for (int j = 1; j <= nBinsFine; j++){
        MCtruthTemp->SetBinContent(j, TruthToProjectY[j] * hResponseTemp->ProjectionY()->GetBinContent(j));
        MCrecoTemp->SetBinContent(j, fwTemp[j] * histoMCreco->GetBinContent(j));
    }
    
    
    //--- Produce the response and unfolded with nominal binning.
        //--- Note that in real analysis measSubBG should be taken from filling the nominal binning.
        //--- Only response-relevant should be produced here.
    TH1D *MCtruthTempNominal    = (TH1D*) MCtruthTemp      ->Rebin(nBinsStd,"MCtruthNorminal",   BinsArray);
    TH1D *MCrecoTempNominal     = (TH1D*) MCrecoTemp       ->Rebin(nBinsStd,"MCrecoNorminal",    BinsArray);
        //--- Rebin 2D response matrix
    TH2D *hResponseTempNominal = new TH2D("responseNorminal", hResponse->GetTitle(), nBinsStd, BinsArray, nBinsStd, BinsArray);
    for (int j = 1; j <= hResponseTemp->GetYaxis()->GetNbins(); j++){
        for (int i = 1; i <= hResponseTemp->GetXaxis()->GetNbins(); i++){
            hResponseTempNominal->Fill(hResponseTemp->GetXaxis()->GetBinCenter(i), hResponseTemp->GetYaxis()->GetBinCenter(j), hResponseTemp->GetBinContent(i,j));
        }
    }
    RooUnfoldResponse *responseReweightedNominal = new RooUnfoldResponse (MCrecoTempNominal, MCtruthTempNominal, hResponseTempNominal);
        //--- RooUnfold Object constructed with measSubBGStd
    RooUnfoldBayes unfoldObjTempNominal(responseReweightedNominal, measSubBGStd, Bayeskterm);
    unfoldObjTempNominal.SetVerbose(0);
    hUnfoldedTempNominal = (TH1D*) unfoldObjTempNominal.Hreco();
    
    

    canvas2->cd();
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1);
    pad1->SetTopMargin(0.11);
    pad1->SetBottomMargin(0);
    pad1->SetRightMargin(0.04);
    pad1->Draw();
    pad1->cd();
    if (logZ) pad1->SetLogy();
    pad1->SetTicks();
    
    GenMadCompare->SetTitle(variableStd.c_str());
    GenMadCompare->GetYaxis()->SetTitleOffset(1.4);
    GenMadCompare->SetLineColor(kBlack);
    GenMadCompare->SetLineWidth(2);
    GenMadCompare->SetFillStyle(3003);
    GenMadCompare->SetFillColor(kGray+2);
    GenMadCompare->DrawCopy("HIST");
    
    hUnfoldedCompare->SetTitle("");
    hUnfoldedCompare->SetMarkerStyle(24);
    hUnfoldedCompare->SetMarkerColor(kBlue+1);
    hUnfoldedCompare->SetLineColor(kBlue+1);
    hUnfoldedCompare->SetLineWidth(2);
    hUnfoldedCompare->DrawCopy("E1same");
    
    hUnfoldedTempNominal->SetTitle("");
    hUnfoldedTempNominal->SetMarkerStyle(24);
    hUnfoldedTempNominal->SetMarkerColor(kRed+1);
    hUnfoldedTempNominal->SetLineColor(kRed+1);
    hUnfoldedTempNominal->SetLineWidth(2);
    hUnfoldedTempNominal->DrawCopy("E1same");
    
    TLegend *legend1 = new TLegend(0.76, 0.66, 0.95, 0.91);
    legend1->SetHeader("  ");
    legend1->SetFillStyle(0);
    legend1->SetBorderSize(0);
    legend1->AddEntry(GenMadCompare, "Gen", "pl");
    legend1->AddEntry(hUnfoldedCompare, "unf standard resp", "pl");
    legend1->AddEntry(hUnfoldedTempNominal, "unf weighted resp", "pl");
    legend1->Draw();
    
    pad1->Draw();
    canvas2->cd();
    
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.3);
    pad2->SetRightMargin(0.04);
    pad2->SetTicks();
    pad2->SetGridy();
    pad2->Draw();
    pad2->cd();
    
    TH1D *Clone1 = (TH1D*)hUnfoldedTempNominal->Clone();
    Clone1->Divide(hUnfoldedCompare);
    Clone1->SetMarkerStyle(24);
    Clone1->SetMarkerColor(kBlue+1);
    Clone1->SetLineColor(kBlue+1);
    Clone1->SetLineWidth(2);
    Clone1->SetTitle("");
    Clone1->GetXaxis()->SetTickLength(0.03);
    Clone1->GetXaxis()->SetTitleSize(0.1);
    Clone1->GetXaxis()->SetLabelSize(0.1);
    Clone1->GetXaxis()->SetLabelOffset(0.018);
    Clone1->GetXaxis()->SetTitleOffset(1.2);
    
    Clone1->GetYaxis()->SetRangeUser(0.81, 1.19);
    Clone1->GetYaxis()->SetNdivisions(5,5,0);
    Clone1->GetYaxis()->SetTitle("weighted / std");
    Clone1->GetYaxis()->SetTitleSize(0.1);
    Clone1->GetYaxis()->SetTitleOffset(0.5);
    Clone1->GetYaxis()->CenterTitle();
    Clone1->GetYaxis()->SetLabelSize(0.08);
    
    Clone1->DrawCopy("E1");
    pad2->Draw();
    
    string outputFileNameCompare = outputDirectory + variableStd + "UnfoldedResp.pdf";
    canvas2->Print(outputFileNameCompare.c_str());
    
    canvas3->cd();
    //canvas3->SetLogx();
    canvas3->SetGridy();
    canvas3->SetTicks();
    histoFw->SetTitle((variableStd + " Weighting Factors").c_str());
    histoFw->SetMarkerColor(kBlack);
    histoFw->SetMarkerStyle(24);
    histoFw->SetLineColor(kBlack);
    histoFw->SetLineWidth(2);
    
    histoFw->GetYaxis()->SetTitle("");
    histoFw->GetYaxis()->SetTitleOffset(1.4);
    
    histoFw->DrawCopy("E1");
    
    string outputFileNameFw = outputDirectory + variableStd +"_FwHist.pdf";
    canvas3->Print(outputFileNameFw.c_str());
    
    
    cout << "---------- Systematic Error in % --------" << endl;
    for(int i = 1; i <= Clone1->GetNbinsX(); i++){
        cout << fabs(Clone1->GetBinContent(i) - 1) * 100  << endl;
    }
    
    // save the systematic error value (absolute)
    TH1D *Clone2 = (TH1D*)hUnfoldedCompare->Clone();
    for(int i = 1; i <= Clone2->GetNbinsX(); i++){
        Clone2->SetBinContent(i, fabs(hUnfoldedTempNominal->GetBinContent(i) - hUnfoldedCompare->GetBinContent(i)));
    }
    outputRootFile->cd();
    Clone2->Write("Resp");
    
        //
        // loop end
    
    //***********************************
    //***********************************
    
    
    //-- close every files ----------------------------------------------------------
    outputRootFile->Close();
    closeFile(DataFile);
    closeFile(DY);
    for (int i(0); i < nBG; i++) closeFile(BG[i]);
    outputTexFile.close();
}




