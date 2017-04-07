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
#include "myFastPlots.h"

//-- Setting global variables --------------------------------------------------------------
#include "fileNames.h"
#include "variablesOfInterestVarWidth.h"

int UnfoldData(const TString lepSel, const TString algo, int svdKterm, RooUnfoldResponse *resp, TH1D *hRecDataMinusFakes, TH1D* &hUnfData, string variable, TString name);
double MyChi2Test(TH1D *h1, TH1D *h2, int nBinsToSkip);
TH1D* foldUnfData(TH1D *hUnfData, RooUnfoldResponse *hresp);
TH1D* getFakes(TH1D *hRecDYJets, TH1D *hRecData, TH1D *hRecSumBg, TH2D *hResDYJets);
RooUnfoldResponse* getResp(TFile *File, string variable, TH1D *recoMad, TH1D *genMad);

void myFastPlots(string leptonFlavor, string var, bool closureTest)
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
        //--- list of variable from : variablesOfInterestVarWidth.h
        nVarStruct = NVAROFINTERESTWJETS;
        varStruct = new variableStruct[nVarStruct];
        varStruct = VAROFINTERESTWJETS;
        makeLISTOFVAROFINTERESTWJETS();
        varList = LISTOFVAROFINTERESTWJETS;
        //--- list of files to be used from  : fileNames.h
        nsel = NFILESWJETS_M;
        sel = new int[nsel];
        sel = FilesWJets_M;
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
            cout << setw(3) << i << ") Processing variable: " << varStruct[i].name << endl; 
            if (!isMuon) FastPlotsRun(sel, nsel, leptonFlavor, varStruct[i].name, varStruct[i].log, varStruct[i].decrease, varStruct[i].ESVDkterm, varStruct[i].EBayeskterm);
            if (isMuon)  FastPlotsRun(sel, nsel, leptonFlavor, varStruct[i].name, varStruct[i].log, varStruct[i].decrease, varStruct[i].MuSVDkterm, varStruct[i].MuBayeskterm);
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
            cout << setw(3) << " " << ") Processing variable: " << varStruct[j].name << endl; 
            if (!isMuon) FastPlotsRun(sel, nsel, leptonFlavor, varStruct[j].name, varStruct[j].log, varStruct[j].decrease, varStruct[j].ESVDkterm, varStruct[j].EBayeskterm, closureTest);
            if (isMuon)  FastPlotsRun(sel, nsel, leptonFlavor, varStruct[j].name, varStruct[j].log, varStruct[j].decrease, varStruct[j].MuSVDkterm, varStruct[j].MuBayeskterm, closureTest);
        }
        else{
            if (!isMuon) FastPlotsRun(sel, nsel, leptonFlavor, var.c_str(),1,1,5,5);
            if (isMuon)  FastPlotsRun(sel, nsel, leptonFlavor, var.c_str(),1,1,5,5);
        }
    }

}


void FastPlotsRun(const int *sel, int nsel, string leptonFlavor, string variable, bool logZ, bool decrease, int SVDkterm, int Bayeskterm, bool closureTest, int JetPtMin, int JetPtMax, bool doFlat, bool doVarWidth)
{
    //--- make sure we have trailing "_" at the end of leptonFlavor
    if (leptonFlavor == "Muons" || leptonFlavor == "DMu") leptonFlavor = "DMu_";
    else if (leptonFlavor == "Electrons" || leptonFlavor == "DE") leptonFlavor = "DE_";
    else if (leptonFlavor == "Muon" || leptonFlavor == "SE") leptonFlavor = "SE_";
    else if (leptonFlavor == "Electron" || leptonFlavor == "SMu") leptonFlavor = "SMu_";

    //--- retrieve the year from current location and set the energy accordingly
    string energy = getEnergy();

    ostringstream jetPtMin; jetPtMin << JetPtMin;
    string outputDirectory = "PNGFiles/FastPlots_" + jetPtMin.str() ;
    if (doVarWidth)  outputDirectory+= "_VarWidth";
    string command = "mkdir -p " + outputDirectory;
    system(command.c_str());
    outputDirectory +=  "/";

    string outputRootFileName = outputDirectory + leptonFlavor + energy + "_" + variable + "_fastplots";
    if (closureTest) outputRootFileName += "_ClosureTest";
    if (doVarWidth) outputRootFileName += "_VarWidth";
    //string outputTexFileName = outputRootFileName;
    outputRootFileName += ".root";
    //outputTexFileName += ".tex";

    TFile *outputRootFile = new TFile(outputRootFileName.c_str(), "RECREATE");
    //ofstream outputTexFile(outputTexFileName.c_str());
    //writeHeaderPage(outputTexFile, leptonFlavor, variable);

    //-- get data histogram (or MC Even for closure test) ----------------------------
    TFile *DataFile = NULL;
    if (!closureTest) DataFile = getFile(FILESDIRECTORY, leptonFlavor, energy, ProcessInfo[sel[0]].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, -1, "", "0", false, 0);
    else DataFile = getFile(FILESDIRECTORY, leptonFlavor, energy, ProcessInfo[sel[nsel-1]].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, -1, "", "0", false, 0);
    TH1D *meas = getHisto(DataFile, variable);
    
    //-- get BG histograms -----------------------------------------------------------
    int nBG(nsel-2);
    if (closureTest) nBG = 0;
    TFile **BG = new TFile*[nBG];
    TH1D **hBG = new TH1D*[nBG];
    for (int i(0); i < nBG; i++){
        BG[i] = getFile(FILESDIRECTORY, leptonFlavor, energy, ProcessInfo[sel[i+1]].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, -1, "", "0", false, 0);
        hBG[i] = getHisto(BG[i], variable);
    }
    TH1D *hSumBG = NULL;
    for (int i(0); i < nBG; i++) {
        if (i == 0) hSumBG = (TH1D*) hBG[i]->Clone();
        else hSumBG->Add(hBG[i]);
    }
    //--- measSubBG
    TH1D *measSubBG = (TH1D*) meas->Clone();
    if (!closureTest) measSubBG->Add(hSumBG, -1);
    else {
        hSumBG = (TH1D*) meas->Clone();
        hSumBG->Reset();
    }
    

    //-- get MC madgraph histograms (or MC Odd for closure test) ---------------------
    TFile *DY = NULL;
    if (!closureTest) DY = getFile(FILESDIRECTORY, leptonFlavor, energy, ProcessInfo[sel[nsel-1]].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, -1, "", "0", false, 0);
    else DY = getFile(FILESDIRECTORY, leptonFlavor, energy, ProcessInfo[sel[nsel-1]].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, -1, "", "0", false, 0);
    TH1D *recoMad = (TH1D*) getHisto(DY, variable);
    
    string genName = "gen" + variable;
    TH1D *genMad = NULL;
    if (!closureTest) genMad = (TH1D*) DY->Get(genName.c_str());
    else genMad = (TH1D*) DY->Get(genName.c_str());
    
    string hrespName = "hresponse" + variable;
    TH2D *hresponse = NULL;
    hresponse = (TH2D*) DY->Get(hrespName.c_str());
    
    //-- build the repsonse object ------------------------
    RooUnfoldResponse *response = (RooUnfoldResponse*) getResp(DY, variable, recoMad, genMad);
    response->UseOverflow();
    
    TH1D *MCtruth   = (TH1D*) response->Htruth();
    TH1D *MCreco    = (TH1D*) response->Hmeasured();
    TH1D *MCFakes   = (TH1D*) response->Hfakes();
    
    outputRootFile->cd();
    meas      ->Write("DataReco");
    measSubBG ->Write("DataMinusBG");
    recoMad   ->Write("recoMadG");
    genMad    ->Write("genMadG");
    hresponse ->Write("hresponse");
    MCtruth   ->Write("MCtruth");
    MCreco    ->Write("MCreco");
    MCFakes   ->Write("MCFakes");
    
    TH2D *hNormResp = (TH2D*) hresponse->Clone();
    normalizeTH2D(hNormResp);
    
    
    //--- setup for Zjets method
    TH1D *recoMadZJ = (TH1D*) recoMad->Clone();
    recoMadZJ->Reset();
    RooUnfoldResponse *responseZJ = (RooUnfoldResponse*) getResp(DY, variable, recoMadZJ, genMad);
    responseZJ->UseOverflow();
    TH1D *hFakDYJets = getFakes(recoMad, meas, hSumBG, hresponse);
    TH1D *hRecDataMinusFakes = (TH1D*) measSubBG->Clone();
    hRecDataMinusFakes->Add(hFakDYJets, -1);
    outputRootFile->cd();
    hFakDYJets->Write("DataRecoFakes");
    
    //-- draw chi2 between foldUfolded and reco --------------------------------------------------------
    TH1D *hUnfData= NULL;
    int nIter = UnfoldData(leptonFlavor, "Bayes", Bayeskterm, responseZJ, hRecDataMinusFakes, hUnfData, variable, "");
    outputRootFile->cd();
    hUnfData->Write();
    
    TH1D *hUnfDataSVD= NULL;
    int nIterSVD = UnfoldData(leptonFlavor, "SVD", Bayeskterm, responseZJ, hRecDataMinusFakes, hUnfDataSVD, variable, "");
    
    //-- get Powheg histograms -------------------------------------------------------
    //TFile *Powheg = getFile(FILESDIRECTORY, leptonFlavor, energy, DYPOWHEGFILENAME, JetPtMin, JetPtMax, doFlat);

    //-- get Powheg histograms -------------------------------------------------------
    //TFile *Sherpa = getFile(FILESDIRECTORY, leptonFlavor, energy, DYSHERPAFILENAME, JetPtMin, JetPtMax, doFlat);

    //-- get TH2D from Powheg -------------------------------------------------------
    //RooUnfoldResponse *respPow = (RooUnfoldResponse*) Powheg->Get(respName.c_str());
    //TH2D *hRespPow = (TH2D*) respPow->Hresponse();

    //-- get TH2D from Sherpa -------------------------------------------------------
    //RooUnfoldResponse *respShe = (RooUnfoldResponse*) Sherpa->Get(respName.c_str());
    //TH2D *hRespShe = (TH2D*) respShe->Hresponse();

    //-- draw normalized response matrix --------------------------------------------
    plotHNormResp(hNormResp, leptonFlavor, variable, energy, outputDirectory, outputRootFile, closureTest);
    
    //-- draw Bayes selected kTerm and +/- 1 ----------------------------------------
    myplotSelectedMethod2("Bayes", response, genMad, Bayeskterm, meas, hBG, nBG, leptonFlavor, variable, logZ, decrease, outputDirectory, outputRootFile, closureTest, true);
    
    //-- draw chi2 of change --------------------------------------------------------
    myplotChi2OfChange(response, Bayeskterm, meas, hBG, nBG, leptonFlavor, variable, energy, outputDirectory, outputRootFile, closureTest);
    
    //-- Bin by bin method ----------------------------------------------------------
    myplotSelectedMethod2("BinByBin", response, genMad, 0, meas, hBG, nBG, leptonFlavor, variable, logZ, decrease, outputDirectory, outputRootFile, closureTest, true);
    
    //-- draw SVD selected kTerm and +/- 1 ------------------------------------------
    myplotSelectedMethod2("SVD", response, genMad, SVDkterm, meas, hBG, nBG, leptonFlavor, variable, logZ, decrease, outputDirectory, outputRootFile, closureTest, true);
    

    //-- close every files ----------------------------------------------------------
    outputRootFile->Close();
    closeFile(DataFile);
    closeFile(DY);
    //closeFile(Powheg);
    //closeFile(Sherpa);
    for (int i(0); i < nBG; i++) closeFile(BG[i]);
    //outputTexFile.close();
    
    
    
    TFile *CompareAlgoInFile = new TFile(outputRootFileName.c_str(), "READ");
    
    TH1D *hisUnfBayes = (TH1D*) CompareAlgoInFile->Get("Central");
    TH1D *hisUnfSVD   = (TH1D*) CompareAlgoInFile->Get("CentralSVD");
    TH1D *hisUnfBBB   = (TH1D*) CompareAlgoInFile->Get("CentralBinByBin");
    
    TH1D *hisRatBayes = (TH1D*) hisUnfBayes->Clone();
    TH1D *hisRatSVD = (TH1D*) hisUnfSVD->Clone();
    TH1D *hisRatBBB = (TH1D*) hisUnfBBB->Clone();
    
    for (int i = 1; i <= hisUnfBayes->GetNbinsX(); i++){
        if (hisUnfBayes->GetBinContent(i) > 0){
            hisRatBayes->SetBinContent( i, 1 );
            hisRatBayes->SetBinError  ( i, hisUnfBayes->GetBinError(i)/hisUnfBayes->GetBinContent(i) );
            
            hisRatSVD->SetBinContent( i, hisUnfSVD->GetBinContent(i)/hisUnfBayes->GetBinContent(i) );
            hisRatSVD->SetBinError  ( i, hisUnfSVD->GetBinError(i)/hisUnfBayes->GetBinContent(i) );
            
            hisRatBBB->SetBinContent( i, hisRatBBB->GetBinContent(i)/hisUnfBayes->GetBinContent(i) );
            hisRatBBB->SetBinError  ( i, hisRatBBB->GetBinError(i)/hisUnfBayes->GetBinContent(i) );
        }
        else{
            hisRatBayes->SetBinContent( i, 0. );
            hisRatBayes->SetBinError  ( i, 0. );
            
            hisRatSVD->SetBinContent( i, 0. );
            hisRatSVD->SetBinError  ( i, 0. );
            
            hisRatBBB->SetBinContent( i, 0. );
            hisRatBBB->SetBinError  ( i, 0. );
        }
    }
    
    
    TCanvas *can = new TCanvas("can1", "can1", 500, 300);
    can->cd();
    can->SetTicks();
    can->SetGrid();
    
    hisRatBayes->SetTitle(("Comparison of Unfolding Algorithm for " + variable).c_str());
    
    hisRatBayes->GetYaxis()->SetTitle("Ratio");
    hisRatBayes->GetYaxis()->SetRangeUser(0.41,1.59);
    
    
    hisRatBayes->SetLineColor(kBlack);
    hisRatBayes->SetLineWidth(2);
    hisRatBayes->SetFillColor(16);
    hisRatBayes->SetMarkerColor();
    hisRatBayes->SetMarkerStyle();
    //hisRatBayes->Draw("E2");
    hisRatBayes->Draw("E1");
    
    hisRatBBB->SetFillStyle(1001);
    hisRatBBB->SetFillColor(kOrange-2);
    hisRatBBB->SetLineColor(kOrange+10);
    hisRatBBB->SetLineWidth(2);
    hisRatBBB->SetMarkerColor(kOrange+10);
    hisRatBBB->SetMarkerStyle(21);
    hisRatBBB->SetMarkerSize(0.5);
    //hisRatBBB->Draw("E2 same");
    hisRatBBB->Draw("E1 same");
    
    hisRatSVD->SetFillStyle(1001);
    hisRatSVD->SetFillColor(kBlue-10);
    hisRatSVD->SetLineColor(kBlue);
    hisRatSVD->SetLineWidth(2);
    hisRatSVD->SetMarkerColor(kBlue);
    hisRatSVD->SetMarkerStyle(20);
    hisRatSVD->SetMarkerSize(0.5);
    //hisRatSVD->Draw("E2 same");
    hisRatSVD->Draw("E1 same");
    
    TLegend *legend = new TLegend(0.54, 0.75, 0.9, 0.9);
    //legend->SetX1(0.44);
    //legend->SetY1(0.77);
    //legend->SetX2(0.95);
    //legend->SetY2(0.98);
    //legend->SetTextSize(.034);
    legend->SetFillColor(0);
    legend->SetFillStyle(1001);
    legend->SetBorderSize(1);
    legend->AddEntry(hisRatBayes, "Bayes", "PLE");
    legend->AddEntry(hisRatSVD, "SVD", "ple");
    legend->AddEntry(hisRatBBB, "BinByBin", "ple");
    legend->Draw();

    
    string OutDirAlgo = outputDirectory + "CompareAlgo";
    string commandOutDirAlgo = "mkdir -p " + OutDirAlgo;
    system(commandOutDirAlgo.c_str());
    string CompareAlgoOutFileName = OutDirAlgo + "/" + variable + "CompareAlgo.pdf";
    can->Print(CompareAlgoOutFileName.c_str());
    
    CompareAlgoInFile->Close();
    
}


void myplotSelectedMethod2(string method, RooUnfoldResponse *response, TH1D *genMad, int kterm, TH1D *hData, TH1D *hBG[], int nBG, string leptonFlavor, string variable, bool logZ, bool decrease, string outputDirectory, TFile *outputRootFile, bool closureTest, bool save)
{
    cout << " Start processing Unfolding using method: " << method << endl;
    
    if (leptonFlavor == "Muons" || leptonFlavor == "DMu") leptonFlavor = "DMu_";
    else if (leptonFlavor == "Electrons" || leptonFlavor == "DE") leptonFlavor = "DE_";
    else if (leptonFlavor == "Muon" || leptonFlavor == "SE") leptonFlavor = "SE_";
    else if (leptonFlavor == "Electron" || leptonFlavor == "SMu") leptonFlavor = "SMu_";
    
    string energy = getEnergy();
    if (kterm < 1) kterm = 2;
    ostringstream ktermStr; ktermStr << kterm;
    
    TH2D *hResponse = (TH2D*) response->Hresponse();
    TH1D *MCreco    = (TH1D*) response->Hmeasured();
    
    TH1D *gen       = (TH1D*) genMad->Clone();
    
    string yTitle = MCreco->GetXaxis()->GetTitle();
    string respTitle = hResponse->GetTitle();
    string niceTitle = respTitle.substr(0, respTitle.find("#right"));
    niceTitle = niceTitle.substr(niceTitle.find("Response")+9);
    if (niceTitle.find("gen") != string::npos) niceTitle = niceTitle.substr(niceTitle.find("gen")+4);
    
    string title = method + " Unfolded Distributions for " + niceTitle + " (kTerm = " + ktermStr.str() + " #pm 1)";
    if (closureTest) title += " C.T.";
    string name = leptonFlavor + energy + "_" + variable + "_Unfolded_Distribution_" + method;
    if (closureTest) name += "_CT";
    
    TH1D *hMeas = (TH1D*) hData->Clone();
    int nBins(hMeas->GetNbinsX());
    
    for (int i(0); i < nBG; i++) {
        hMeas->Add(hBG[i], -1);
    }
    
    
    TH1D *hUnfoldedC = NULL, *hUnfoldedD = NULL, *hUnfoldedU = NULL;
    if (method == "SVD") {
        cout << " Unfolding using Center Kterm " << endl;
        RooUnfoldSvd unfoldC(response, hMeas, kterm);
        unfoldC.SetVerbose(0);
        hUnfoldedC = (TH1D*) unfoldC.Hreco();
        if (kterm > 1){
            cout << " Unfolding using Down Kterm " << endl;
            RooUnfoldSvd unfoldD(response, hMeas, kterm - 1);
            unfoldD.SetVerbose(0);
            hUnfoldedD = (TH1D*) unfoldD.Hreco();
        }
        else cout << "       Impossible to show kterm - 1" << endl;
        
        if (kterm < nBins){
            cout << " Unfolding using Up Kterm" << endl;
            RooUnfoldSvd unfoldU(response, hMeas, kterm + 1);
            unfoldU.SetVerbose(0);
            hUnfoldedU = (TH1D*) unfoldU.Hreco();
        }
        else cout << "       Impossible to show kterm + 1" << endl;
        
    }
    else if (method == "Bayes") {
        cout << " Unfolding using Center Kterm " << endl;
        RooUnfoldBayes unfoldC(response, hMeas, kterm);
        unfoldC.SetVerbose(0);
        hUnfoldedC = (TH1D*) unfoldC.Hreco();
        if (kterm > 1){
            cout << " Unfolding using Down Kterm " << endl;
            RooUnfoldBayes unfoldD(response, hMeas, kterm - 1);
            unfoldD.SetVerbose(0);
            hUnfoldedD = (TH1D*) unfoldD.Hreco();
        }
        else cout << "       Impossible to show kterm - 1" << endl;
        if (kterm < nBins){
            cout << " Unfolding using Up Kterm" << endl;
            RooUnfoldBayes unfoldU(response, hMeas, kterm + 1);
            unfoldU.SetVerbose(0);
            hUnfoldedU = (TH1D*) unfoldU.Hreco();
        }
        else cout << "       Impossible to show kterm + 1" << endl;
        
    }
    else if (method == "BinByBin"){
        RooUnfoldBinByBin unfoldC(response, hMeas);
        unfoldC.SetVerbose(0);
        hUnfoldedC = (TH1D*) unfoldC.Hreco();
    }
    
    // save unfolded histo to root file
    if (method == "Bayes") {
        outputRootFile->cd();
        hMeas->Write("DataSubBG");
        hUnfoldedC->Write("Central");
        genMad->Write("genMad");
    }
    else if (method == "SVD") hUnfoldedC->Write("CentralSVD");
    else if (method == "BinByBin") hUnfoldedC->Write("CentralBinByBin");
    
    TLatex *cmsColl = new TLatex();
    cmsColl->SetTextSize(0.025);
    cmsColl->SetTextFont(42);
    cmsColl->SetLineWidth(2);
    cmsColl->SetTextColor(kBlack);
    cmsColl->SetNDC();
    cmsColl->SetTextAlign(11);
    
    TLatex *sqrtXTeV = new TLatex();
    sqrtXTeV->SetTextSize(0.025);
    sqrtXTeV->SetTextFont(42);
    sqrtXTeV->SetLineWidth(2);
    sqrtXTeV->SetTextColor(kBlack);
    sqrtXTeV->SetNDC();
    sqrtXTeV->SetTextAlign(21);
    
    TLatex *intLumi = new TLatex();
    intLumi->SetTextSize(0.022);
    intLumi->SetTextFont(42);
    intLumi->SetLineWidth(2);
    intLumi->SetTextColor(kBlack);
    intLumi->SetNDC();
    intLumi->SetTextAlign(31);
    
    
    TCanvas *canUnfoldedDistribution = new TCanvas(name.c_str(), name.c_str(), 700, 900);
    canUnfoldedDistribution->cd();
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1);
    pad1->SetTopMargin(0.11);
    pad1->SetBottomMargin(0);
    pad1->SetRightMargin(0.04);
    pad1->Draw();
    pad1->cd();
    if (logZ) pad1->SetLogy();
    gen->SetTitle(title.c_str());
    gen->GetYaxis()->SetTitleOffset(1.4);
    gen->SetLineColor(kBlack);
    gen->SetLineWidth(2);
    gen->SetFillStyle(3003);
    gen->SetFillColor(kGray+2);
    gen->DrawCopy("HIST");
    
    
    if (kterm > 1 && method != "BinByBin") {
            hUnfoldedD->SetTitle("");
            hUnfoldedD->SetMarkerStyle(32);
            hUnfoldedD->SetMarkerColor(kRed+1);
            hUnfoldedD->SetLineColor(kRed+1);
            hUnfoldedD->SetLineWidth(2);
            hUnfoldedD->DrawCopy("E1same");
    }
    if (kterm < nBins && method != "BinByBin") {
            hUnfoldedU->SetTitle("");
            hUnfoldedU->SetMarkerStyle(26);
            hUnfoldedU->SetMarkerColor(kGreen+2);
            hUnfoldedU->SetLineColor(kGreen+2);
            hUnfoldedU->SetLineWidth(2);
            hUnfoldedU->DrawCopy("E1same");
    }
    
    
    hUnfoldedC->SetMarkerStyle(24);
    hUnfoldedC->SetMarkerColor(kBlue+1);
    hUnfoldedC->SetLineColor(kBlue+1);
    hUnfoldedC->SetLineWidth(2);
    hUnfoldedC->DrawCopy("E1same");
    
    double xLowLeg(0.76), xHighLeg(0.95);
    if (!decrease){
        xLowLeg = 0.15;
        xHighLeg = 0.34;
    }
    
    //=== legend ====//
    TLegend *leg = new TLegend(xLowLeg, 0.63, xHighLeg, 0.89);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    if (!closureTest) leg->AddEntry(gen, "MC Gen.", "fl");
    else leg->AddEntry(gen, "MC Gen.", "fl");
    
    string legEntry = "kTerm = " + ktermStr.str();
    
    if (method != "BinByBin") {leg->AddEntry(hUnfoldedC, legEntry.c_str(), "pl");}
    else if (method == "BinByBin"){leg->AddEntry(hUnfoldedC, "BinByBin", "pl");}
    
    if (kterm < nBins && method != "BinByBin") leg->AddEntry(hUnfoldedU, "kTerm + 1", "pl");
    if (kterm > 1 && method != "BinByBin") leg->AddEntry(hUnfoldedD, "kTerm - 1", "pl");
    
    leg->Draw();
    //=== end legend ====//
    
    if (!closureTest){
        cmsColl->DrawLatex(0.11,0.905, "CMS Preliminary");
        if (energy == "7TeV") sqrtXTeV->DrawLatex(0.54,0.905, "#sqrt{s} = 7 TeV");
        if (energy == "8TeV") sqrtXTeV->DrawLatex(0.54,0.905, "#sqrt{s} = 8 TeV");
        if (energy == "7TeV") intLumi->DrawLatex(0.98,0.91, "#int L dt = 5.05 fb^{-1}");
        if (energy == "8TeV") intLumi->DrawLatex(0.98,0.91, "#int L dt = 19.6 fb^{-1}");
    }
    pad1->Draw();
    canUnfoldedDistribution->cd();
    
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.3);
    pad2->SetRightMargin(0.04);
    pad2->SetTicks();
    pad2->SetGridy();
    pad2->Draw();
    pad2->cd();
    hUnfoldedC->Divide(genMad);
    if (kterm > 1 && method != "BinByBin") hUnfoldedD->Divide(genMad);
    if (kterm < nBins && method != "BinByBin") hUnfoldedU->Divide(genMad);
    
    hUnfoldedC->SetTitle("");
    hUnfoldedC->GetXaxis()->SetTickLength(0.03);
    hUnfoldedC->GetXaxis()->SetTitleSize(0.1);
    hUnfoldedC->GetXaxis()->SetLabelSize(0.09);
    hUnfoldedC->GetXaxis()->SetLabelOffset(0.018);
    hUnfoldedC->GetXaxis()->SetTitleOffset(1.2);
    
    hUnfoldedC->GetYaxis()->SetRangeUser(0.51, 1.49);
    hUnfoldedC->GetYaxis()->SetNdivisions(5,5,0);
    if (!closureTest) {
        hUnfoldedC->GetYaxis()->SetTitle("Gen / Unfolded");
        hUnfoldedC->GetYaxis()->SetTitleSize(0.1);
        hUnfoldedC->GetYaxis()->SetTitleOffset(0.5);
    }
    else {
        hUnfoldedC->GetYaxis()->SetTitle("Gen. / Unf. C.T.");
        hUnfoldedC->GetYaxis()->SetTitleSize(0.07);
        hUnfoldedC->GetYaxis()->SetTitleOffset(0.7);
    }
    hUnfoldedC->GetYaxis()->CenterTitle();
    hUnfoldedC->GetYaxis()->SetLabelSize(0.08);
    
    for (int j(1); j <= nBins; j++){
        double content(hUnfoldedC->GetBinContent(j));
        double contentErr(hUnfoldedC->GetBinError(j));
        if (content > 0){
            hUnfoldedC->SetBinContent(j, 1./content);
            hUnfoldedC->SetBinError(j, contentErr/(content * content));
        }
        
        if(method != "BinByBin"){
            if(kterm > 1){
                content = (hUnfoldedD->GetBinContent(j));
                contentErr = (hUnfoldedD->GetBinError(j));
                if (content > 0){
                    hUnfoldedD->SetBinContent(j, 1./content);
                    hUnfoldedD->SetBinError(j, contentErr/(content * content));                    
                }
            }
            if(kterm < nBins){
                content = (hUnfoldedU->GetBinContent(j));
                contentErr = (hUnfoldedU->GetBinError(j));
                if (content > 0){
                    hUnfoldedU->SetBinContent(j, 1./content);
                    hUnfoldedU->SetBinError(j, contentErr/(content * content));
                }
            }
        }
    }
    
    hUnfoldedC->DrawCopy("E1");
    if (kterm > 1 && method != "BinByBin") hUnfoldedD->DrawCopy("E1same");
    if (kterm < nBins && method != "BinByBin") hUnfoldedU->DrawCopy("E1same");
    pad2->Draw();
    
    string outputFileNamePNG = outputDirectory + name.c_str() + ".pdf";
    if (save){
        canUnfoldedDistribution->Print(outputFileNamePNG.c_str());
        outputRootFile->cd();
        canUnfoldedDistribution->Write();
    }
}


void myplotChi2OfChange(RooUnfoldResponse *response, int kterm, TH1D *hData, TH1D *hBG[], int nBG, string leptonFlavor, string variable, string energy, string outputDirectory, TFile *outputRootFile, bool closureTest, bool save)
{
    if (leptonFlavor == "Muons" || leptonFlavor == "DMu") leptonFlavor = "DMu_";
    else if (leptonFlavor == "Electrons" || leptonFlavor == "DE") leptonFlavor = "DE_";
    else if (leptonFlavor == "Muon" || leptonFlavor == "SE") leptonFlavor = "SE_";
    else if (leptonFlavor == "Electron" || leptonFlavor == "SMu") leptonFlavor = "SMu_";
    
    TH2D *hResponse = (TH2D*) response->Hresponse();
    TH1D *hMeas = (TH1D*) hData->Clone();
    int nBins(hMeas->GetNbinsX()  );
    
    // not allow this to go futher than kterm = 25
    int iterNum = nBins;
    if (iterNum > 25) {iterNum = 25;}
    
    for (int i(0); i < nBG; i++){
        hMeas->Add(hBG[i], -1);
    }
    
    cout << " Start processing Chi2OfChange Plot " << endl;
    
    RooUnfoldBayes unfoldBayesChi2(response, hMeas, iterNum);
    unfoldBayesChi2.SetVerbose(0);
    TH1D *tmp = (TH1D*) unfoldBayesChi2.Hreco();
            //nBins = tmp->GetNbinsX();
    TH1D *Chi2OfChange = unfoldBayesChi2.Chi2OfChange;
    
    double hmax(Chi2OfChange->GetMaximum());
    double kCont(Chi2OfChange->GetBinContent(kterm));
    kCont *= 2;
    
    TArrow *arrow = new TArrow(kterm+0.5, hmax, kterm+0.5, kCont, 0.03, "|>");
    arrow->SetAngle(40);
    arrow->SetLineWidth(2);
    arrow->SetLineColor(kRed+2);
    arrow->SetFillColor(kRed+2);
    
    
    string respTitle = hResponse->GetTitle();
    string niceTitle = respTitle.substr(0, respTitle.find("#right"));
    niceTitle = niceTitle.substr(niceTitle.find("Response")+9);
    if (niceTitle.find("gen") != string::npos) niceTitle = niceTitle.substr(niceTitle.find("gen")+4);
    string chi2Title = "#chi^{2} of change for " + niceTitle;
    if (closureTest) chi2Title += " C.T.";
    string canNameChi2 = leptonFlavor + energy + "_" + variable + "_Chi2";
    if (closureTest) canNameChi2 += "_CT";
    
    Chi2OfChange->SetTitle(chi2Title.c_str());
    Chi2OfChange->SetLineWidth(3);
    Chi2OfChange->SetLineColor(kBlack);
    Chi2OfChange->GetXaxis()->SetTitle("kTerm");
    Chi2OfChange->GetXaxis()->SetTitleSize(0.04);
    Chi2OfChange->GetXaxis()->SetTitleOffset(1.0);
    Chi2OfChange->GetYaxis()->SetTitleSize(0.04);
    Chi2OfChange->GetYaxis()->SetTitleOffset(1.15);
    Chi2OfChange->GetXaxis()->CenterLabels();
    Chi2OfChange->GetXaxis()->SetLabelSize(0.1/sqrt(iterNum));
    Chi2OfChange->GetXaxis()->SetNdivisions(iterNum, 0, 0);
    Chi2OfChange->GetYaxis()->SetTitle("#chi^{2}");
    TCanvas *canChi2 = new TCanvas(canNameChi2.c_str(), canNameChi2.c_str(), 700, 700);
    canChi2->cd();
    canChi2->SetLeftMargin(0.1);
    canChi2->SetRightMargin(0.05);
    canChi2->SetLogy();
    canChi2->SetGrid();
    canChi2->SetTicks();
    Chi2OfChange->DrawCopy("TEXT60");
    arrow->Draw();
    string outputFileNamePNG = outputDirectory + canNameChi2 + ".pdf";
    if (save){
        canChi2->Print(outputFileNamePNG.c_str());
        outputRootFile->cd();
        canChi2->Write();
    }
}


int UnfoldData(const TString lepSel, const TString algo, int svdKterm, RooUnfoldResponse *resp, TH1D *hRecDataMinusFakes, TH1D* &hUnfData, string variable, TString name)
{
    //--- make sure we use OverFlow (should already be set to true) ---
    resp->UseOverflow();
    
    //--- Set the required unfolding algorithm ---
    RooUnfold::Algorithm alg;
    if (algo == "Bayes") {
        alg = RooUnfold::kBayes;
    }
    else if (algo == "SVD") {
        alg = RooUnfold::kSVD;
    }
    else {
        cerr << "Error: the specified algo: " << algo << " is not implemented!" << endl;
        cerr << "       I will proceed with kBayes algo" << endl;
        alg = RooUnfold::kBayes;
    }
    
    std::cout << "-----------------------" << std::endl;
    std::cout << "--------- start finding nIter -----------" << std::endl;
    
    
    string command = "mkdir -p PNGFiles/UnfoldingCheck";
    system(command.c_str());
    
    TFile *f = new TFile("PNGFiles/UnfoldingCheck/" + lepSel + "_" + variable + "_" + name + "_" + algo + ".root", "RECREATE");
    f->cd();
    int finalNIter = -1;
    int nIter = 99;
    int nBinsTmp = hRecDataMinusFakes->GetNbinsX();
    TH1D *hchi2 = new TH1D("hchi2", "hchi2", nBinsTmp, 0.5, nBinsTmp+0.5);
    hchi2->SetTitle("#chi^{2}/ndf for reco vs folded-unfolded for " + TString(hRecDataMinusFakes->GetTitle()));
    hchi2->GetYaxis()->SetTitle("#chi^{2}/ndf");
    hchi2->GetYaxis()->SetTitleOffset(1.40);
    hchi2->GetXaxis()->SetTitle(TString("number of iterations of the " + algo + " method"));
    hchi2->GetXaxis()->CenterTitle();
    hchi2->GetXaxis()->SetNdivisions(nBinsTmp, 0, 0);
    hchi2->GetXaxis()->SetLabelSize(0.03);
    hchi2->SetLineWidth(2);
    for (int i = 1; i <= nBinsTmp; ++i) {
        RooUnfoldResponse *respBis = (RooUnfoldResponse*) resp->Clone();
        TH1D *hRecDataMinusFakesBis = (TH1D*) hRecDataMinusFakes->Clone();
        RooUnfold *RObjectForDataTmp = RooUnfold::New(alg, respBis, hRecDataMinusFakesBis, i);
        RObjectForDataTmp->SetVerbose(0);
        int nBinsToSkip = (TString(hRecDataMinusFakesBis->GetName()).Index("JetPt_Zinc") > 0) ? 2 : 0;
        
        //cout << "nBinsToSkip " << nBinsToSkip << " " << TString(hRecDataMinusFakesBis->GetName()) << " " << TString(hRecDataMinusFakesBis->GetName()).Index("JetPt_Zinc") << endl;
        
        RObjectForDataTmp->IncludeSystematics(0); // new version of RooUnfold: will compute Cov based on Data Statistics only
        std::cout << "niter = " << i << std::endl;
        TH1D* hUnfDataBis = (TH1D*) RObjectForDataTmp->Hreco(RooUnfold::kCovariance);
        
        TString tmpName = "Unf" + name + "_";
        tmpName += i;
        hUnfDataBis->SetName(tmpName);
        hUnfDataBis->SetTitle(tmpName);
        tmpName = "fol" + name + "_";
        tmpName += i;
        
        TH1D *hfoldUnfData = foldUnfData(hUnfDataBis, respBis);
        hfoldUnfData->SetName(tmpName);
        hfoldUnfData->SetTitle(tmpName);
        
        TH1D *hgen = (TH1D*) respBis->Htruth();
        tmpName = "gen" + name + "_";
        tmpName += i;
        hgen->SetName(tmpName);
        hgen->SetTitle(tmpName);
        hgen->Write();
        
        TH1D *hfoldgen = foldUnfData(hgen, respBis);
        tmpName = "folgen" + name + "_";
        tmpName += i;
        hfoldgen->SetName(tmpName);
        hfoldgen->SetTitle(tmpName);
        hfoldgen->Write();
        
        TH1D *hmes = (TH1D*) respBis->Hmeasured();
        tmpName = "mes" + name + "_";
        tmpName += i;
        hmes->SetName(tmpName);
        hmes->SetTitle(tmpName);
        hmes->Write();
        
        double mychi2 = MyChi2Test(hfoldUnfData, hRecDataMinusFakesBis, nBinsToSkip);
       
        hchi2->SetBinContent(i, mychi2);
        if (i==1) hRecDataMinusFakesBis->Write("Unf" + name + "_0");
        hUnfDataBis->Write();
        hfoldUnfData->Write();
        if (mychi2 < 1./sqrt(2) && finalNIter < 0) {
            nIter = i;
            finalNIter = i;
            std::cout << "Will use " << nIter << " iterations with a final Chi2/ndf of: " << mychi2 << std::endl;
            //break;
        }
    }
    
    //nIter = min(nIter, 20);
    nIter = min(nIter, nBinsTmp);
    nIter = max(nIter, 4);
    //nIter = 4;
    
    std::cout << "\n---------------------------------------------------------------------------------------------------------------\n-" << std::endl;
    std::cout << " nIter: " << nIter << std::endl;
    std::cout << " finalNIter: " << finalNIter << std::endl;
    hchi2->GetYaxis()->SetRangeUser(0, max(1.3, 1.1*hchi2->GetMaximum()));
    TArrow *arrowChi2 = new TArrow(nIter, min(1.2, hchi2->GetMaximum()), nIter, 0.8, 0.02, "|>");
    arrowChi2->SetLineColor(kRed);
    arrowChi2->SetFillColor(kRed);
    arrowChi2->SetLineWidth(2);
    TLine *line = new TLine(0.5, 0.7, nBinsTmp + 0.5, 0.7);
    line->SetLineColor(kBlack);
    line->SetLineStyle(kDashed);
    line->SetLineWidth(2);
    TCanvas *chchi2 = new TCanvas("chchi2", "chchi2", 600, 600);
    chchi2->cd();
    chchi2->SetGrid();
    hchi2->DrawCopy();
    arrowChi2->Draw();
    line->Draw();
    hchi2->Write();
    chchi2->Write();
    chchi2->Print("PNGFiles/UnfoldingCheck/" + lepSel + "_" + variable + "_" + name + "_" + algo + "_chi2.pdf");
    
    RooUnfold *RObjectForDataBinByBin = RooUnfold::New(RooUnfold::kBinByBin, resp, hRecDataMinusFakes);
    RObjectForDataBinByBin->SetVerbose(0);
    TH1D *hUnfDataBinByBin = (TH1D*) RObjectForDataBinByBin->Hreco(RooUnfold::kCovariance);
    hUnfDataBinByBin->SetName("UnfDataBinByBin" + name);
    hUnfDataBinByBin->Write();
    
    RooUnfold *RObjectForDataBayes = RooUnfold::New(RooUnfold::kBayes, resp, hRecDataMinusFakes, nIter);
    RObjectForDataBayes->SetVerbose(0);
    TH1D *hUnfDataBayes = (TH1D*) RObjectForDataBayes->Hreco(RooUnfold::kCovariance);
    hUnfDataBayes->SetName("UnfDataBayes" + name);
    hUnfDataBayes->Write();
    
    /*
    for (int i(1); i <= nBinsTmp; i++) {
        RooUnfold *RObjectForDataSVD = RooUnfold::New(RooUnfold::kSVD, resp, hRecDataMinusFakes, i);
        TH1D *hUnfDataSVD = (TH1D*) RObjectForDataSVD->Hreco(RooUnfold::kCovariance);
        hUnfDataSVD->SetName("UnfDataSVD_" + TString::Format("%d", i) + "_" + name);
        hUnfDataSVD->Write();
    }
    
    
    TSVDUnfold *unfoldTSVD = new TSVDUnfold(hRecDataMinusFakes, (TH1D*)resp->Htruth(), (TH1D*)resp->Hmeasured(), (TH2D*)resp->Hresponse());
    TH1D *unfresult = (TH1D*) unfoldTSVD->Unfold(1);
    TH1D *hmodDOriginal = (TH1D*) unfoldTSVD->GetD();
    TH1D *hSV       = (TH1D*) unfoldTSVD->GetSV();
    
    TH1D *hmodD = new TH1D("hmodD", "hmodD", nBinsTmp, 0.5, nBinsTmp+0.5);
    for (int i(0); i <= nBinsTmp+1; i++) {
        hmodD->SetBinContent(i, hmodDOriginal->GetBinContent(i));
    }
    hmodD->SetTitle(hmodDOriginal->GetTitle() + TString(" for ") + TString(hRecDataMinusFakes->GetTitle()));
    hmodD->GetXaxis()->SetNdivisions(nBinsTmp, 0, 0);
    hmodD->GetXaxis()->SetLabelSize(0.03);
    hmodD->GetYaxis()->SetTitle("|d_{i}|");
    hmodD->GetYaxis()->SetTitleOffset(1.40);
    hmodD->GetXaxis()->SetTitle("regularization parameter of the SVD method");
    hmodD->GetXaxis()->CenterTitle();
    TArrow *arrowSvd = new TArrow(svdKterm, 20, svdKterm, 1.1*hmodD->GetBinContent(svdKterm), 0.02, "|>");
    arrowSvd->SetLineColor(kRed);
    arrowSvd->SetFillColor(kRed);
    arrowSvd->SetLineWidth(2);
    
    hmodD->SetName("modD" + name);
    TCanvas *chmodD = new TCanvas("chmodD", "chmodD", 600, 600);
    chmodD->cd();
    chmodD->SetGrid();
    chmodD->SetLogy();
    hmodD->DrawCopy();
    arrowSvd->Draw();
    chmodD->Write();
    hmodD->Write();
    */
    f->Close();
    
    std::cout << "\n---------------------------------------------------------------------------------------------------------------\n-" << std::endl;
    
    //--- Unfold data minus background ---
    RooUnfold *RObjectForData = RooUnfold::New(alg, resp, hRecDataMinusFakes, svdKterm);
    RObjectForData->SetVerbose(0);
    
    //--- get the unfolded result ---
    RObjectForData->IncludeSystematics(0); // new version of RooUnfold: will compute Cov based on Data Statistics only
    hUnfData = (TH1D*) RObjectForData->Hreco(RooUnfold::kCovariance);
    hUnfData->SetName("UnfData" + name);
    hUnfData->SetTitle(hRecDataMinusFakes->GetTitle());
    
    
    return nIter;
}


double MyChi2Test(TH1D *h1, TH1D *h2, int nBinsToSkip)
{
    TH1D *h1Copy = (TH1D*) h1->Clone();
    TH1D *h2Copy = (TH1D*) h2->Clone();
    
    for (int i = 0; i <= nBinsToSkip; ++i) {
        h1Copy->SetBinContent(i, 0.0);
        h2Copy->SetBinContent(i, 0.0);
    }
    return h1Copy->Chi2Test(h2Copy, "WW,P,CHI2/NDF");
    
}

TH1D* foldUnfData(TH1D *hUnfData, RooUnfoldResponse *hresp)
{
    TH1D *hfoldUnfData = (TH1D*) hUnfData->Clone();
    RooUnfoldResponse *resp = (RooUnfoldResponse*) hresp->Clone();
    TH2D *hres = (TH2D*) resp->Hresponse();
    TH1D *hgen = (TH1D*) resp->Htruth();
    
    
    int nBins = hfoldUnfData->GetNbinsX();
    
    for (int i = 0; i <= nBins; ++i) {
        double totGen = hgen->GetBinContent(i);
        for (int j = 0; j <= nBins; ++j) {
            if (totGen != 0.0) {
                hres->SetBinContent(j, i, hres->GetBinContent(j, i)/totGen);
                hres->SetBinError(j, i, hres->GetBinError(j, i)/totGen);
                hres->SetBinError(j, i, 0.0);
            }
            else {
                hres->SetBinContent(i, j, 0);
                hres->SetBinError(i, j, hres->GetBinError(i, j));
                hres->SetBinError(i, j, 0.0);
            }
        }
    }
    
    for (int i = 0; i <= nBins; ++i) {
        double sum = 0.0;
        double error2 = 0.0;
        for (int j = 0; j <= nBins; ++j) {
            sum += hres->GetBinContent(i, j) * hUnfData->GetBinContent(j);
            error2 += pow(hres->GetBinContent(i, j) * hUnfData->GetBinError(j), 2);
        }
        hfoldUnfData->SetBinContent(i, sum);
        hfoldUnfData->SetBinError(i, sqrt(error2));
    }
    return hfoldUnfData;
}

TH1D* getFakes(TH1D *hRecDYJets, TH1D *hRecData, TH1D *hRecSumBg, TH2D *hResDYJets)
{
    string variable = hRecData->GetName(); //cout << " Get Fakes: " << variable << endl;
    TH1D *hFakDYJets = (TH1D*) hRecDYJets->Clone();
    
    int sm= hRecDYJets->GetSumw2N();
    int s = hResDYJets->GetSumw2N();
    int nm = hResDYJets->GetNbinsX() + 2;
    int nt = hResDYJets->GetNbinsY() + 2;
    
    double dyIntegral = hRecDYJets->Integral(0, hRecDYJets->GetNbinsX()+1);
    double dataIntegral = hRecData->Integral(0, hRecData->GetNbinsX()+1);
    double bgIntegral = hRecSumBg->Integral(0, hRecSumBg->GetNbinsX()+1);
    for (int i= 0; i<nm; i++) {
        double nmes= 0.0, wmes= 0.0;
        for (int j= 0; j<nt; j++) {
            nmes += hResDYJets->GetBinContent(i, j);
            if (s) wmes += pow(hResDYJets->GetBinError(i, j), 2);
        }
        double fake = hRecDYJets->GetBinContent(i) - nmes;
        
        if (variable == "ZNGoodJets_Zinc") {
            dyIntegral = hRecDYJets->GetBinContent(i);
            dataIntegral = hRecData->GetBinContent(i);
            bgIntegral = hRecSumBg->GetBinContent(i);
        }
        
        double factor = dyIntegral;
        if (factor != 0) factor = (dataIntegral - bgIntegral) / factor;
        
        if (!s) wmes= nmes;
        hFakDYJets->SetBinContent (i, factor*fake);
        hFakDYJets->SetBinError   (i, sqrt (wmes + (sm ? pow(hRecDYJets->GetBinError(i),2) : hRecDYJets->GetBinContent(i))));
    }
    hFakDYJets->SetEntries (hFakDYJets->GetEffectiveEntries());  // 0 entries if 0 fakes
    
    return hFakDYJets;
    
}

RooUnfoldResponse* getResp(TFile *File, string variable, TH1D *recoMad, TH1D *genMad)
{
    string hrespName = "hresponse" + variable;
    TH2D *hresponse = (TH2D*) File->Get(hrespName.c_str());
    RooUnfoldResponse *response = new RooUnfoldResponse(recoMad, genMad, hresponse);
    return response;
}














