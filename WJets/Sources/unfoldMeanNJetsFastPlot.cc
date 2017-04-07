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
#include "unfoldMeanNJetsFastPlot.h"

//-- Setting global variables --------------------------------------------------------------
#include "fileNames.h"
#include "variablesOfInterestVarWidth.h"

//---
const int NVAROFINTERESTWJETSMEAN(4);
variableStruct VAROFINTERESTWJETSMEAN[] = {
    //--  Name  ----------------------- log - decrease - Mu SVD kterm - Mu Bayes kterm - E SVD kterm - E Bayes kterm - BayesktermCombine
    
    {"MeanNJetsHT_Zinc1jet",             0,      0,           8,              6,             9,            3,           7},
    {"MeanNJetsHT_Zinc2jet",             0,      0,          10,              8,            10,            4,           7},
    {"MeanNJetsdRapidity_Zinc2jet",      0,      0,           7,              3,             7,            5,           7},
    {"MeanNJetsdRapidityFB_Zinc2jet",    0,      0,           6,              2,             5,            9,           7},
};

TList *LISTOFVAROFINTERESTWJETSMEAN = new TList();
void makeLISTOFVAROFINTERESTWJETSMEAN(){
    for (int i(0); i < NVAROFINTERESTWJETSMEAN; i++){
        TObjString *name = new TObjString(VAROFINTERESTWJETSMEAN[i].name.c_str());
        LISTOFVAROFINTERESTWJETSMEAN->Add(name);
    }
}
//---


void unfoldMeanNJetsFastPlot(string leptonFlavor, string var, bool closureTest)
{
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    gStyle->SetOptStat(0);
    gStyle->SetPaintTextFormat("1.2f");
    
    //--- load the proper VAROFINTERST structure for W+jets study---
    int nsel = 0;
    const int *sel = NULL;
    TList *varList = new TList();
    int nVarStruct(0);
    variableStruct *varStruct = NULL;
    
    nsel = NFILESTTBAR; //NFILESTTBAR(9);
    sel = new int[nsel];
    sel = FilesTTbarWJets; // FilesTTbarWJets[NFILESTTBAR] = {0, 4, 3, 2, 23, 20, 22, 11, 24};
    
    nVarStruct = NVAROFINTERESTWJETSMEAN;
    varStruct = new variableStruct[nVarStruct];
    varStruct = VAROFINTERESTWJETSMEAN;
    
    makeLISTOFVAROFINTERESTWJETSMEAN();
    varList = LISTOFVAROFINTERESTWJETSMEAN;
    //---------------------------------------------
    
    
    //--- check wether muon or electron is used ---
    bool isMuon;
    if (leptonFlavor.find("Mu") != string::npos) isMuon = 1;
    else isMuon = 0;
    //---------------------------------------------
    

    TObjString *name = new TObjString(var.c_str());
    unsigned short j(varList->IndexOf(name));
    cout << setw(3) << " " << " Processing variable: " << var << endl;
    if (!isMuon) FastPlotsRun(sel, nsel, leptonFlavor, varStruct[j].name, varStruct[j].log, varStruct[j].decrease, varStruct[j].ESVDkterm, varStruct[j].EBayeskterm, closureTest);
    
    if (isMuon)  FastPlotsRun(sel, nsel, leptonFlavor, varStruct[j].name, varStruct[j].log, varStruct[j].decrease, varStruct[j].MuSVDkterm, varStruct[j].MuBayeskterm, closureTest);
    
}


void FastPlotsRun(const int *sel, int nsel, string leptonFlavor, string variable, bool logZ, bool decrease, int SVDkterm, int Bayeskterm, bool closureTest, int JetPtMin, int JetPtMax, bool doFlat, bool doVarWidth)
{
    //--- make sure we have trailing "_" at the end of leptonFlavor
    if (leptonFlavor == "Muons" || leptonFlavor == "DMu") leptonFlavor = "DMu_";
    else if (leptonFlavor == "Electrons" || leptonFlavor == "DE") leptonFlavor = "DE_";
    else if (leptonFlavor == "Muon" || leptonFlavor == "SE") leptonFlavor = "SE_";
    else if (leptonFlavor == "Electron" || leptonFlavor == "SMu") leptonFlavor = "SMu_";
    
    //--- retreive the year from current location and set the energy accordingly
    string energy = getEnergy();
    
    // output directory name
    ostringstream jetPtMin; jetPtMin << JetPtMin;
    string outputDirectory = "PNGFiles/FastPlots_" + jetPtMin.str() ;
    if (doVarWidth)  outputDirectory+= "_VarWidth";
    string command = "mkdir -p " + outputDirectory;
    system(command.c_str());
    outputDirectory +=  "/";
    
    // output file name
    string outputRootFileName = outputDirectory + leptonFlavor + energy + "_" + variable + "_fastplots";
    if (closureTest) outputRootFileName += "_ClosureTest";
    if (doVarWidth) outputRootFileName += "_VarWidth";
    string outputTexFileName = outputRootFileName;
    outputRootFileName += ".root";
    outputTexFileName += ".tex";
    
    // create output file
    TFile *outputRootFile = new TFile(outputRootFileName.c_str(), "RECREATE");
    ofstream outputTexFile(outputTexFileName.c_str());
    writeHeaderPage(outputTexFile, leptonFlavor, variable);
    
    //-- get data histogram (or WJets MC for closure test) ----------------------------
    TFile *DataFile = NULL;
    if (!closureTest) DataFile = getFile(FILESDIRECTORY, leptonFlavor, energy, ProcessInfo[sel[0]].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, -1, "", "0", false, 0);
    else DataFile = getFile(FILESDIRECTORY, leptonFlavor, energy, ProcessInfo[sel[nsel-1]].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, -1, "", "0", false, 0);
    
    TH2D *meas = get2DHisto(DataFile, variable);
    
    //-- get MC madgraph histograms (or MC madgraph for closure test) ---------------------
    TFile *DY = NULL;
    if (!closureTest) DY = getFile(FILESDIRECTORY, leptonFlavor, energy, ProcessInfo[sel[nsel-1]].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, -1, "", "0", false, 0);
    else DY = getFile(FILESDIRECTORY, leptonFlavor, energy, ProcessInfo[sel[nsel-1]].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, -1, "", "0", false, 0);
    
    string genName = "gen" + variable;
    TH2D *genMad = NULL;
    genMad = get2DHisto(DY, genName);
    
    //-- get BG histograms -----------------------------------------------------------
    int nBG(nsel-2);
    if (closureTest) nBG = 0;
    TFile **BG = new TFile*[nBG];
    TH2D **hBG = new TH2D*[nBG];
    for (int i(0); i < nBG; i++){
        //change the name of QCD file got from ProcessInfo[sel[i+1]]
        string fileNameTemp =  ProcessInfo[sel[i+1]].filename ;
        if (fileNameTemp.find("QCD") != string::npos) fileNameTemp = "DataQCD_ForMeanNJets_dR_5311";
        
        BG[i] = getFile(FILESDIRECTORY, leptonFlavor, energy, fileNameTemp, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, -1, "", "0", false, 0);
        hBG[i] = get2DHisto(BG[i], variable);
    }
    
    //-- get response object from file ------------------------
    string respName = "response" + variable;
    RooUnfoldResponse *response = (RooUnfoldResponse*) DY->Get(respName.c_str());
    //response->UseOverflow();
    
    // get response matrix to draw // response matrix is not TH2D ?
    /*
    TH2D *hResponse = (TH2D*) response->Hresponse();
    TH2D *hNormResp = (TH2D*) hResponse->Clone();
    normalizeTH2D(hNormResp);
    //-- draw normalized response matrix --------------------------------------------
    plotHNormResp(hNormResp, leptonFlavor, variable, energy, outputDirectory, outputRootFile, closureTest);
    */
    
    //-- draw Bayes selected kTerm and +/- 1 ----------------------------------------
    myplotSelectedMethod2("Bayes", response, genMad, Bayeskterm, meas, hBG, nBG, leptonFlavor, variable, logZ, decrease, outputDirectory, outputRootFile, closureTest, true);
    
    //-- draw Bayes many kterm ----------------------------------------
    //myplotManyKterm("Bayes", response, genMad, meas, hBG, nBG, leptonFlavor, variable, logZ, decrease, outputDirectory, outputRootFile, closureTest, true);
    
    //-- draw chi2 of change --------------------------------------------------------
    myplotChi2OfChange(response, Bayeskterm, meas, hBG, nBG, leptonFlavor, variable, energy, outputDirectory, outputRootFile, closureTest);
    
    //-- close every files ----------------------------------------------------------
    outputRootFile->Close();
    closeFile(DataFile);
    closeFile(DY);
    
    for (int i(0); i < nBG; i++) closeFile(BG[i]);
    outputTexFile.close();
}


void myplotSelectedMethod2(string method, RooUnfoldResponse *response, TH2D *genMad, int kterm, TH2D *hData, TH2D *hBG[], int nBG, string leptonFlavor, string variable, bool logZ, bool decrease, string outputDirectory, TFile *outputRootFile, bool closureTest, bool save)
{
    //--- make sure we have trailing "_" at the end of leptonFlavor
    if (leptonFlavor == "Muons" || leptonFlavor == "DMu") leptonFlavor = "DMu_";
    else if (leptonFlavor == "Electrons" || leptonFlavor == "DE") leptonFlavor = "DE_";
    else if (leptonFlavor == "Muon" || leptonFlavor == "SE") leptonFlavor = "SE_";
    else if (leptonFlavor == "Electron" || leptonFlavor == "SMu") leptonFlavor = "SMu_";
    
    string energy = getEnergy();
    if (kterm < 1) kterm = 2;
    ostringstream ktermStr; ktermStr << kterm;
    
    // build "title string" and "name string"
    TH2D *hMeasTemp = (TH2D*) response->Hmeasured();
    string respTitle = hMeasTemp->GetTitle();
        //string niceTitle = respTitle.substr(0, respTitle.find("#right"));
    string niceTitle = "Mean " + respTitle;
        //niceTitle = niceTitle.substr(niceTitle.find("Response")+9);
    if (niceTitle.find("gen") != string::npos) niceTitle = niceTitle.substr(niceTitle.find("gen")+4);
    
    string title = method + " Unfolded Distributions for " + niceTitle + " (kTerm = " + ktermStr.str() + " #pm 1)";
    if (closureTest) title += " C.T.";
    string name = leptonFlavor + energy + "_" + variable + "_Unfolded_Distribution_" + method;
    if (closureTest) name += "_CT";
    
    // get gen histo and (meas-bg) histo
    TH2D *gen   = (TH2D*) genMad->Clone();
    TH2D *hMeas = (TH2D*) hData->Clone();
    for (int i(0); i < nBG; i++) {
        hMeas->Add(hBG[i], -1);
    }
    
    // get number of bins on X axis or Y axis
    int nBinsKterm (0);
    int nBinsTempX(hMeas->GetNbinsX());
    int nBinsTempY(hMeas->GetNbinsY());
    if (nBinsTempX <= nBinsTempY) nBinsKterm = nBinsTempX;
    else nBinsKterm = nBinsTempY;
    cout << " nBinsKTerm: " << nBinsKterm << endl;
    
    // unfolding
    TH2D *hUnfoldedC = NULL, *hUnfoldedD = NULL, *hUnfoldedU = NULL;
    
    if (method == "Bayes") {
        RooUnfoldBayes unfoldC(response, hMeas, kterm);
        //unfoldC.SetVerbose(0);
        hUnfoldedC = (TH2D*) unfoldC.Hreco();
        
        if (kterm > 1){
            RooUnfoldBayes unfoldD(response, hMeas, kterm - 1);
            unfoldD.SetVerbose(0);
            hUnfoldedD = (TH2D*) unfoldD.Hreco();
        }
        else cout << "       Impossible to show kterm - 1" << endl;
        if (kterm < nBinsKterm){
            RooUnfoldBayes unfoldU(response, hMeas, kterm + 1);
            unfoldU.SetVerbose(0);
            hUnfoldedU = (TH2D*) unfoldU.Hreco();
        }
        else cout << "       Impossible to show kterm + 1" << endl;
        
    }
    
    //--- produce projected 1D hist for hUnfoldedC/D/U and gen ----
                //--- get Xaxis Title ---    
                //const char *xTitle = hData->GetXaxis()->GetTitle();
    
    cout << "line th " << __LINE__ << endl;
    ///-------------------------------------------------------------///
    //--- produce projected 1D hist for hUnfoldedC ---
    TH1D* hUnfoldedCMeanJ = NULL;
    hUnfoldedCMeanJ = computeProject1DMeanNJets(hUnfoldedC, variable);
    hUnfoldedCMeanJ->SetName("Central");
    outputRootFile->cd();
    hUnfoldedCMeanJ->Write();
    
    //--- produce projected 1D hist for hUnfoldedU ---
    TH1D* hUnfoldedUMeanJ = NULL;
    if (kterm < nBinsKterm && method != "BinByBin") {
        hUnfoldedUMeanJ = computeProject1DMeanNJets(hUnfoldedU, variable);
    }
    //--- produce projected 1D hist for hUnfoldedD ---
    TH1D* hUnfoldedDMeanJ = NULL;
    if (kterm > 1 && method != "BinByBin") {
        hUnfoldedDMeanJ = computeProject1DMeanNJets(hUnfoldedD, variable);
    }

    //--- produce projected 1D hist for gen ----
    TH1D* genMadMeanJ = NULL;
    genMadMeanJ = computeProject1DMeanNJets(gen, variable);
    genMadMeanJ->SetName("genMad");
    outputRootFile->cd();
    genMadMeanJ->Write();
    ///-------------------------------------------------------------///
    
    // text to draw
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
    //pad1->SetRightMargin(0.02);
    pad1->Draw();
    pad1->cd();
    if (logZ) pad1->SetLogy();
    
    genMadMeanJ->SetTitle(title.c_str());
    genMadMeanJ->GetYaxis()->SetTitleOffset(1.4);
    genMadMeanJ->SetLineColor(kBlack);
    genMadMeanJ->SetLineWidth(2);
    genMadMeanJ->SetFillStyle(3003);
    genMadMeanJ->SetFillColor(kGray+2);
    
    if (variable == "MeanNJetsdRapidity_Zinc2jet") {
        genMadMeanJ->SetMaximum(2.45);
        genMadMeanJ->SetMinimum(2.05);
    }
    else genMadMeanJ->SetMaximum(1.2*genMadMeanJ->GetMaximum());
    
    genMadMeanJ->DrawCopy("HIST");
    
    if (kterm > 1 && method != "BinByBin") {
        hUnfoldedDMeanJ->SetTitle("");
        hUnfoldedDMeanJ->SetMarkerStyle(32);
        hUnfoldedDMeanJ->SetMarkerColor(kRed+1);
        hUnfoldedDMeanJ->SetLineColor(kRed+1);
        hUnfoldedDMeanJ->SetLineWidth(2);
        hUnfoldedDMeanJ->DrawCopy("E1same");
    }
    if (kterm < nBinsKterm && method != "BinByBin") {
        hUnfoldedUMeanJ->SetTitle("");
        hUnfoldedUMeanJ->SetMarkerStyle(26);
        hUnfoldedUMeanJ->SetMarkerColor(kGreen+2);
        hUnfoldedUMeanJ->SetLineColor(kGreen+2);
        hUnfoldedUMeanJ->SetLineWidth(2);
        hUnfoldedUMeanJ->DrawCopy("E1same");
    }
    
    hUnfoldedCMeanJ->SetMarkerStyle(24);
    hUnfoldedCMeanJ->SetMarkerColor(kBlue+1);
    hUnfoldedCMeanJ->SetLineColor(kBlue+1);
    hUnfoldedCMeanJ->SetLineWidth(2);
    hUnfoldedCMeanJ->DrawCopy("E1same");
    
    // set legend position
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
    
    if (method != "BinByBin") {leg->AddEntry(hUnfoldedCMeanJ, legEntry.c_str(), "pl");}
    else if (method == "BinByBin"){leg->AddEntry(hUnfoldedCMeanJ, "BinByBin", "pl");}
    
    if (kterm < nBinsKterm && method != "BinByBin") leg->AddEntry(hUnfoldedUMeanJ, "kTerm + 1", "pl");
    if (kterm > 1 && method != "BinByBin") leg->AddEntry(hUnfoldedDMeanJ, "kTerm - 1", "pl");
    
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
    //pad2->SetRightMargin(0.02);
    pad2->SetTicks();
    pad2->SetGridy();
    pad2->Draw();
    pad2->cd();
    
    //--- Calculate Ratio >> want to plot Gen/Unfolded ---
    hUnfoldedCMeanJ->Divide(genMadMeanJ);
    if (kterm > 1 && method != "BinByBin") hUnfoldedDMeanJ->Divide(genMadMeanJ);
    if (kterm < nBinsKterm && method != "BinByBin") hUnfoldedUMeanJ->Divide(genMadMeanJ);
    
    int nBinsX(hUnfoldedC->GetNbinsX());
    //cout << " nBinsX: " << nBinsX << endl;
    for (int j(1); j <= nBinsX; j++){
        double content(hUnfoldedCMeanJ->GetBinContent(j));
        double contentErr(hUnfoldedCMeanJ->GetBinError(j));
        if (content > 0){
            hUnfoldedCMeanJ->SetBinContent(j, 1./content);
            hUnfoldedCMeanJ->SetBinError(j, contentErr/(content * content));
        }
        
        if(method != "BinByBin"){
            if (kterm > 1){
                content = (hUnfoldedDMeanJ->GetBinContent(j));
                contentErr = (hUnfoldedDMeanJ->GetBinError(j));
                if (content > 0){
                    hUnfoldedDMeanJ->SetBinContent(j, 1./content);
                    hUnfoldedDMeanJ->SetBinError(j, contentErr/(content * content));
                }
            }
            if (kterm < nBinsKterm){
                content = (hUnfoldedUMeanJ->GetBinContent(j));
                contentErr = (hUnfoldedUMeanJ->GetBinError(j));
                if (content > 0){
                    hUnfoldedUMeanJ->SetBinContent(j, 1./content);
                    hUnfoldedUMeanJ->SetBinError(j, contentErr/(content * content));
                }
            }
        }
    }
    
    hUnfoldedCMeanJ->SetTitle("");
    hUnfoldedCMeanJ->GetXaxis()->SetTickLength(0.03);
    hUnfoldedCMeanJ->GetXaxis()->SetTitleSize(0.1);
    hUnfoldedCMeanJ->GetXaxis()->SetLabelSize(0.09);
    hUnfoldedCMeanJ->GetXaxis()->SetLabelOffset(0.018);
    hUnfoldedCMeanJ->GetXaxis()->SetTitleOffset(1.2);
    //hUnfoldedCMeanJ->GetYaxis()->SetRangeUser(0.51, 1.49);
    hUnfoldedCMeanJ->GetYaxis()->SetRangeUser(0.78, 1.22);
    hUnfoldedCMeanJ->GetYaxis()->SetNdivisions(5,5,0);
    
    if (!closureTest) {
        hUnfoldedCMeanJ->GetYaxis()->SetTitle("Gen / Unfolded");
        hUnfoldedCMeanJ->GetYaxis()->SetTitleSize(0.1);
        hUnfoldedCMeanJ->GetYaxis()->SetTitleOffset(0.5);
    }
    else {
        hUnfoldedCMeanJ->GetYaxis()->SetTitle("Gen. / Unf. C.T.");
        hUnfoldedCMeanJ->GetYaxis()->SetTitleSize(0.07);
        hUnfoldedCMeanJ->GetYaxis()->SetTitleOffset(0.7);
    }
    hUnfoldedCMeanJ->GetYaxis()->CenterTitle();
    hUnfoldedCMeanJ->GetYaxis()->SetLabelSize(0.08);
    
    hUnfoldedCMeanJ->DrawCopy("E1");
    if (kterm > 1 && method != "BinByBin") hUnfoldedDMeanJ->DrawCopy("E1same");
    if (kterm < nBinsKterm && method != "BinByBin") hUnfoldedUMeanJ->DrawCopy("E1same");
    
    pad2->Draw();
    
    string outputFileNamePNG = outputDirectory + name.c_str() + ".pdf";
    if (save){
        canUnfoldedDistribution->Print(outputFileNamePNG.c_str());
        outputRootFile->cd();
        canUnfoldedDistribution->Write();
    }
}





void myplotChi2OfChange(RooUnfoldResponse *response, int kterm, TH2D *hData, TH2D *hBG[], int nBG, string leptonFlavor, string variable, string energy, string outputDirectory, TFile *outputRootFile, bool closureTest, bool save)
{
    cout << "------ Begin Function to plot chi2 of change ------" << endl;
    if (leptonFlavor == "Muons" || leptonFlavor == "DMu") leptonFlavor = "DMu_";
    else if (leptonFlavor == "Electrons" || leptonFlavor == "DE") leptonFlavor = "DE_";
    else if (leptonFlavor == "Muon" || leptonFlavor == "SE") leptonFlavor = "SE_";
    else if (leptonFlavor == "Electron" || leptonFlavor == "SMu") leptonFlavor = "SMu_";
    
    TH2D *hMeas = (TH2D*) hData->Clone();
    int nBins(hMeas->GetNbinsX()  );
    cout << " nBinsX before unfolding: " << nBins << endl;
    
    // get number of bins on X axis or Y axis
    int nBinsKterm (0);
    int nBinsTempX(hMeas->GetNbinsX());
    int nBinsTempY(hMeas->GetNbinsY());
    if (nBinsTempX <= nBinsTempY) nBinsKterm = nBinsTempX;
    else nBinsKterm = nBinsTempY;
    cout << " nBinsKTerm: " << nBinsKterm << endl;
    
    for (int i(0); i < nBG; i++){
        hMeas->Add(hBG[i], -1);
    }
    
    cout << "--- Begin calculating chi2 of change ---" << endl;
    RooUnfoldBayes unfoldBayesChi2(response, hMeas, nBinsKterm);
    unfoldBayesChi2.SetVerbose(0);
    
    TH2D *tmp = (TH2D*) unfoldBayesChi2.Hreco();
    nBins = tmp->GetNbinsX();
    cout << " nBinsX after unfolding: " << nBins << endl;
    
    TH1D *Chi2OfChange = unfoldBayesChi2.Chi2OfChange;
    
    double hmax(Chi2OfChange->GetMaximum());
    double kCont(Chi2OfChange->GetBinContent(kterm));
    kCont *= 2;
    
    TArrow *arrow = new TArrow(kterm+0.5, hmax, kterm+0.5, kCont, 0.03, "|>");
    arrow->SetAngle(40);
    arrow->SetLineWidth(2);
    arrow->SetLineColor(kRed+2);
    arrow->SetFillColor(kRed+2);
    
    
    //string respTitle = hResponse->GetTitle();
    //string niceTitle = respTitle.substr(0, respTitle.find("#right"));
    //niceTitle = niceTitle.substr(niceTitle.find("Response")+9);
    //if (niceTitle.find("gen") != string::npos) niceTitle = niceTitle.substr(niceTitle.find("gen")+4);
    string niceTitle = "   ";
    string chi2Title = "#chi^{2} of change for " + niceTitle;
    if (closureTest) chi2Title += " C.T.";
    string canNameChi2 = leptonFlavor + energy + "_" + variable + "_Chi2";
    if (closureTest) canNameChi2 += "_CT";
    
    Chi2OfChange->SetTitle(chi2Title.c_str());
    Chi2OfChange->SetLineWidth(3);
    Chi2OfChange->SetLineColor(kBlack);
    Chi2OfChange->GetXaxis()->SetTitle("Bin Numb.");
    Chi2OfChange->GetXaxis()->SetTitleSize(0.04);
    Chi2OfChange->GetXaxis()->SetTitleOffset(1.0);
    Chi2OfChange->GetYaxis()->SetTitleSize(0.04);
    Chi2OfChange->GetYaxis()->SetTitleOffset(1.15);
    Chi2OfChange->GetXaxis()->CenterLabels();
    Chi2OfChange->GetXaxis()->SetLabelSize(0.1/sqrt(nBinsKterm));
    Chi2OfChange->GetXaxis()->SetNdivisions(nBinsKterm, 0, 0);
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


TH2D* get2DHisto(TFile *File, string variable){
    TH2D *histo2D = (TH2D*) File->Get(variable.c_str());
    histo2D->SetDirectory(0);
    return histo2D;
}

TH1D* computeProject1DMeanNJets( TH2D *hUnfoldedC, string variable ){
    
    int nBinsX(hUnfoldedC->GetNbinsX());
    int nBinsY(hUnfoldedC->GetNbinsY());
    double xmin = hUnfoldedC->GetXaxis()->GetXmin();
    double xmax = hUnfoldedC->GetXaxis()->GetXmax();
    cout << " nBinsX: " << nBinsX << endl;
    cout << " nBinsY: " << nBinsY << endl;
    
    const double *xBins = new double[nBinsX+1];
    xBins = hUnfoldedC->GetXaxis()->GetXbins()->GetArray();
    
    TH1D* hUnfoldedCMeanJ;
    
    if (xBins == 0){
        hUnfoldedCMeanJ = new TH1D(variable.c_str(), variable.c_str(), nBinsX, xmin, xmax);
    }
    else{
        hUnfoldedCMeanJ = new TH1D(variable.c_str(), variable.c_str(), nBinsX, xBins);
    }
    
    //--- produce projected 1D hist for hUnfoldedC ---
    for (int i=0; i<nBinsX; i++){
        // calculate mean # of jets
        double meanNJets(0);
        double NEvents(0);
        double NJetsTot(0);
        for(int j=0; j<nBinsY; j++){
            double NEventsTemp(0);
            double NJetsTemp(0);
            NEventsTemp = hUnfoldedC->GetBinContent(i+1,j+1);
            NEvents = NEvents + NEventsTemp;
            
            NJetsTemp = (hUnfoldedC->GetBinContent(i+1,j+1)) * (j+1);
            NJetsTot = NJetsTot + NJetsTemp;
        }
        if (NEvents > 0) meanNJets = NJetsTot/NEvents;
        
        hUnfoldedCMeanJ->SetBinContent(i+1, meanNJets);
        
        // calculate error
        double errOfMean(0);
        if (NEvents > 0) {
            for(int k=0; k<nBinsY; k++){
                double errTemp(0);
                errTemp = (((NEvents * (k+1)) - NJetsTot) / pow(NEvents, 2)) * (hUnfoldedC->GetBinError(i+1,k+1));
                errTemp = pow(errTemp, 2);
                errOfMean = errOfMean + errTemp;
            }
            errOfMean = sqrt(errOfMean);
        }
        hUnfoldedCMeanJ->SetBinError(i+1, errOfMean);
        cout << " loop " << i << endl;
        
    }
    
    return hUnfoldedCMeanJ;

}


void myplotManyKterm(string method, RooUnfoldResponse *response, TH2D *genMad, TH2D *hData, TH2D *hBG[], int nBG, string leptonFlavor, string variable, bool logZ, bool decrease, string outputDirectory, TFile *outputRootFile, bool closureTest, bool save)
{
    //--- make sure we have trailing "_" at the end of leptonFlavor
    if (leptonFlavor == "Muons" || leptonFlavor == "DMu") leptonFlavor = "DMu_";
    else if (leptonFlavor == "Electrons" || leptonFlavor == "DE") leptonFlavor = "DE_";
    else if (leptonFlavor == "Muon" || leptonFlavor == "SE") leptonFlavor = "SE_";
    else if (leptonFlavor == "Electron" || leptonFlavor == "SMu") leptonFlavor = "SMu_";
    
    string energy = getEnergy();
    
    // build "title string" and "name string"
    //TH2D *hMeasTemp = (TH2D*) response->Hmeasured();
    //string respTitle = hMeasTemp->GetTitle();
    
    //string niceTitle = respTitle.substr(0, respTitle.find("#right"));
    //string niceTitle = "Mean " + respTitle;
    //niceTitle = niceTitle.substr(niceTitle.find("Response")+9);
    //if (niceTitle.find("gen") != string::npos) niceTitle = niceTitle.substr(niceTitle.find("gen")+4);
    
    //string title = method + " Unfolded Distributions for " + niceTitle + " (kTerm = " + ktermStr.str() + " #pm 1)";
    //if (closureTest) title += " C.T.";
    
    string name = leptonFlavor + energy + "_" + variable + "_Unfolded_Distribution_" + method;
    if (closureTest) name += "_CT";
    
    // get gen histo and (meas-bg) histo
    TH2D *gen   = (TH2D*) genMad->Clone();
    TH2D *hMeas = (TH2D*) hData->Clone();
    for (int i(0); i < nBG; i++) {
        hMeas->Add(hBG[i], -1);
    }
    
    //--- Set graph and Pad ---
    // text to draw
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
    //pad1->SetRightMargin(0.02);
    pad1->Draw();
    pad1->cd();
    if (logZ) pad1->SetLogy();
    
    if (!closureTest){
        cmsColl->DrawLatex(0.11,0.905, "CMS Preliminary");
        if (energy == "7TeV") sqrtXTeV->DrawLatex(0.54,0.905, "#sqrt{s} = 7 TeV");
        if (energy == "8TeV") sqrtXTeV->DrawLatex(0.54,0.905, "#sqrt{s} = 8 TeV");
        if (energy == "7TeV") intLumi->DrawLatex(0.98,0.91, "#int L dt = 5.05 fb^{-1}");
        if (energy == "8TeV") intLumi->DrawLatex(0.98,0.91, "#int L dt = 19.6 fb^{-1}");
    }
    //--- End Set graph and Pad ---
    
    
    //--- set legend position
    double xLowLeg(0.76), xHighLeg(0.95);
    if (!decrease){
        xLowLeg = 0.15;
        xHighLeg = 0.34;
    }
    TLegend *leg = new TLegend(xLowLeg, 0.63, xHighLeg, 0.89);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    
    
    //--- get number of bins on X axis or Y axis
    int nBinsKterm (0);
    int nBinsTempX(hMeas->GetNbinsX());
    int nBinsTempY(hMeas->GetNbinsY());
    if (nBinsTempX <= nBinsTempY) nBinsKterm = nBinsTempX;
    else nBinsKterm = nBinsTempY;
    cout << " nBinsKTerm: " << nBinsKterm << endl;
    
    
    
    
    //--- Looping for producing unfolded plots using varied kterm ---
    
    for (int kterm = 1; kterm < 8; kterm++){
        
        // unfolding
        TH2D *hUnfoldedC = NULL;
        if (method == "Bayes") {
            RooUnfoldBayes unfoldC(response, hMeas, kterm);
            //unfoldC.SetVerbose(0);
            hUnfoldedC = (TH2D*) unfoldC.Hreco();
        }
        
        //--- produce projected 1D hist for hUnfoldedC
        //--- get Xaxis Title ---
        //const char *xTitle = hData->GetXaxis()->GetTitle();
        int nBinsX(hUnfoldedC->GetNbinsX());
        int nBinsY(hUnfoldedC->GetNbinsY());
        double xmin = hUnfoldedC->GetXaxis()->GetXmin();
        double xmax = hUnfoldedC->GetXaxis()->GetXmax();
        cout << " nBinsX: " << nBinsX << endl;
        const double *xBins = new double[nBinsX+1];
        xBins = hUnfoldedC->GetXaxis()->GetXbins()->GetArray();
        
        //--- produce projected 1D hist for hUnfoldedC ---
        TH1D* hUnfoldedCMeanJ;
        if (xBins == 0) hUnfoldedCMeanJ = new TH1D(variable.c_str(), variable.c_str(), nBinsX, xmin, xmax);
        else hUnfoldedCMeanJ = new TH1D(variable.c_str(), variable.c_str(), nBinsX, xBins);
        
        for (int i=0; i<nBinsX; i++){
            // calculate mean # of jets
            double meanNJets(0);
            double NEvents(0);
            double NJetsTot(0);
            for(int j=0; j<nBinsY; j++){
                double NEventsTemp(0);
                double NJetsTemp(0);
                NEventsTemp = hUnfoldedC->GetBinContent(i+1,j+1);
                NEvents = NEvents + NEventsTemp;
                
                NJetsTemp = (hUnfoldedC->GetBinContent(i+1,j+1)) * (j+1);
                NJetsTot = NJetsTot + NJetsTemp;
            }
            if (NEvents > 0) meanNJets = NJetsTot/NEvents;
            //else meanNJets = 0;
            hUnfoldedCMeanJ->SetBinContent(i+1, meanNJets);
            
            // calculate error
            double errOfMean(0);
            if (NEvents > 0) {
                for(int k=0; k<nBinsY; k++){
                    double errTemp(0);
                    errTemp = (((NEvents * (k+1)) - NJetsTot) / pow(NEvents, 2)) * (hUnfoldedC->GetBinError(i+1,k+1));
                    errTemp = pow(errTemp, 2);
                    errOfMean = errOfMean + errTemp;
                }
                errOfMean = sqrt(errOfMean);
            }
            hUnfoldedCMeanJ->SetBinError(i+1, errOfMean);
            cout << " loop C " << i << endl;
            
        }
        
        //--- produce projected 1D hist for gen ----
        if (kterm == 1){
            TH1D* genMadMeanJ;
            if (xBins == 0) genMadMeanJ = new TH1D(variable.c_str(), variable.c_str(), nBinsX, xmin, xmax);
            else genMadMeanJ = new TH1D(variable.c_str(), variable.c_str(), nBinsX, xBins);
            
            for (int i=0; i<nBinsX; i++){
                // calculate mean # of jets
                double meanNJets_W(0);
                double NEvents_W(0);
                double NJetsTot_W(0);
                for(int j=0; j<nBinsY; j++){
                    double NJetsTemp_W(0);
                    double NEventsTemp_W(0);
                    NEventsTemp_W = gen->GetBinContent(i+1,j+1);
                    NEvents_W = NEvents_W + NEventsTemp_W;
                    
                    NJetsTemp_W = (gen->GetBinContent(i+1,j+1)) * (j+1);
                    NJetsTot_W = NJetsTot_W + NJetsTemp_W;
                }
                if (NEvents_W > 0) meanNJets_W = NJetsTot_W/NEvents_W;
                //else meanNJets_W = 0;
                genMadMeanJ->SetBinContent(i+1, meanNJets_W);
                
                // calculate error
                double errOfMean_W(0);
                if (NEvents_W > 0) {
                    for(int k=0; k<nBinsY; k++){
                        double errTemp_W(0);
                        errTemp_W = (((NEvents_W * (k+1)) - NJetsTot_W) / pow(NEvents_W, 2)) * (gen->GetBinError(i+1,k+1));
                        errTemp_W = pow(errTemp_W, 2);
                        errOfMean_W = errOfMean_W + errTemp_W;
                    }
                    errOfMean_W = sqrt(errOfMean_W);
                }
                genMadMeanJ->SetBinError(i+1, errOfMean_W);
                cout << " loop Gen " << i << endl;
            }
            
            //genMadMeanJ->SetTitle(title.c_str());
            //genMadMeanJ->GetYaxis()->SetTitleOffset(1.4);
            genMadMeanJ->SetLineColor(kBlack);
            genMadMeanJ->SetLineWidth(2);
            genMadMeanJ->SetFillStyle(3003);
            genMadMeanJ->SetFillColor(kGray+2);
            if (variable == "MeanNJetsdRapidity_Zinc2jet") {
                genMadMeanJ->SetMaximum(2.45);
                genMadMeanJ->SetMinimum(2.05);
            }
            else genMadMeanJ->SetMaximum(1.2*genMadMeanJ->GetMaximum());
            genMadMeanJ->DrawCopy("HIST");
            
            leg->AddEntry(gen, "MC Gen.", "fl");
        }
        
        
        hUnfoldedCMeanJ->SetMarkerStyle(24);
        hUnfoldedCMeanJ->SetMarkerColor(kterm+1);
        hUnfoldedCMeanJ->SetLineColor(kterm+1);
        hUnfoldedCMeanJ->SetLineWidth(2);
        hUnfoldedCMeanJ->DrawCopy("E1same");
        
        //if (kterm < 1) kterm = 2;
        ostringstream ktermStr; ktermStr << kterm;
        string legEntry = "kTerm = " + ktermStr.str();
        leg->AddEntry(hUnfoldedCMeanJ, legEntry.c_str(), "pl");
        
    }
    //--- End Looping -----------------------------------------------
    
    
    
    leg->Draw();
    pad1->Draw();
    canUnfoldedDistribution->cd();
    /*
     TPad *pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.3);
     pad2->SetTopMargin(0);
     pad2->SetBottomMargin(0.3);
     pad2->SetRightMargin(0.04);
     //pad2->SetRightMargin(0.02);
     pad2->SetTicks();
     pad2->SetGridy();
     pad2->Draw();
     pad2->cd();
     
     //--- want to plot Gen/Unfolded ---
     hUnfoldedCMeanJ->Divide(genMadMeanJ);
     
     if (kterm > 1 && method != "BinByBin") hUnfoldedDMeanJ->Divide(genMadMeanJ);
     if (kterm < nBinsKterm && method != "BinByBin") hUnfoldedUMeanJ->Divide(genMadMeanJ);
     
     hUnfoldedCMeanJ->SetTitle("");
     hUnfoldedCMeanJ->GetXaxis()->SetTickLength(0.03);
     hUnfoldedCMeanJ->GetXaxis()->SetTitleSize(0.1);
     hUnfoldedCMeanJ->GetXaxis()->SetLabelSize(0.1);
     hUnfoldedCMeanJ->GetXaxis()->SetLabelOffset(0.018);
     hUnfoldedCMeanJ->GetXaxis()->SetTitleOffset(1.2);
     //hUnfoldedCMeanJ->GetYaxis()->SetRangeUser(0.51, 1.49);
     hUnfoldedCMeanJ->GetYaxis()->SetRangeUser(0.78, 1.22);
     hUnfoldedCMeanJ->GetYaxis()->SetNdivisions(5,5,0);
     
     if (!closureTest) {
     hUnfoldedCMeanJ->GetYaxis()->SetTitle("Gen / Unfolded");
     hUnfoldedCMeanJ->GetYaxis()->SetTitleSize(0.1);
     hUnfoldedCMeanJ->GetYaxis()->SetTitleOffset(0.5);
     }
     else {
     hUnfoldedCMeanJ->GetYaxis()->SetTitle("Gen. / Unf. C.T.");
     hUnfoldedCMeanJ->GetYaxis()->SetTitleSize(0.07);
     hUnfoldedCMeanJ->GetYaxis()->SetTitleOffset(0.7);
     }
     hUnfoldedCMeanJ->GetYaxis()->CenterTitle();
     hUnfoldedCMeanJ->GetYaxis()->SetLabelSize(0.08);
     
     for (int j(1); j <= nBinsX; j++){
     double content(hUnfoldedCMeanJ->GetBinContent(j));
     double contentErr(hUnfoldedCMeanJ->GetBinError(j));
     if (content > 0){
     hUnfoldedCMeanJ->SetBinContent(j, 1./content);
     hUnfoldedCMeanJ->SetBinError(j, contentErr/(content * content));
     }
     
     if(method != "BinByBin"){
     if (kterm > 1){
     content = (hUnfoldedDMeanJ->GetBinContent(j));
     contentErr = (hUnfoldedDMeanJ->GetBinError(j));
     if (content > 0){
     hUnfoldedDMeanJ->SetBinContent(j, 1./content);
     hUnfoldedDMeanJ->SetBinError(j, contentErr/(content * content));
     }
     }
     if (kterm < nBinsKterm){
     content = (hUnfoldedUMeanJ->GetBinContent(j));
     contentErr = (hUnfoldedUMeanJ->GetBinError(j));
     if (content > 0){
     hUnfoldedUMeanJ->SetBinContent(j, 1./content);
     hUnfoldedUMeanJ->SetBinError(j, contentErr/(content * content));
     }
     }
     }
     }
     hUnfoldedCMeanJ->DrawCopy("E1");
     
     if (kterm > 1 && method != "BinByBin") hUnfoldedDMeanJ->DrawCopy("E1same");
     if (kterm < nBinsKterm && method != "BinByBin") hUnfoldedUMeanJ->DrawCopy("E1same");
     
     pad2->Draw();
     */
    
    string outputFileNamePNG = outputDirectory + name.c_str() + "AllKterm" + ".pdf";
    if (save){
        canUnfoldedDistribution->Print(outputFileNamePNG.c_str());
        outputRootFile->cd();
        canUnfoldedDistribution->Write();
    }
    
}


