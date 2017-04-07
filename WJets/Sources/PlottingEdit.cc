// History
//---- 2015_05_16
// Add line to expand y-scale for dRap and dR plots
// ### FIXME : need to delete the temp fix label for ang. dist.
//---- 2015_05_10
// MES and MER is now included
// Sherpa2 is used. But the lines for getting sherpa1 is still there but are commented out.
// BH is done here and due to the structure of BH files this part of code is long. This motivates me to put all this into FinalUnfold.cc but I have not done yet. (Contrast with MeanNjets code which is already move this part to FinalUnfold.cc)

#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <TFile.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TLatex.h>
#include "getFilesAndHistograms.h"

//-- Setting global variables -------------
#include "fileNames.h"
#include "variablesOfInterestVarWidth.h"
#include "xsecVariableNames.h"

using namespace std;
//--------------------------------


//--- Declaring functions -------------
void PlottingEdit(int start = 0, int end = -1, bool doNPTB = false);
void FuncPlot(string variable = "ZNGoodJets_Zexc", bool log = 1, bool decrease = 1, string varRivetName = "njetWJet_excl", string varBlackHatName = "njet_WMuNu", bool doNPTB = false);

void FuncPlot2(string variable = "ZNGoodJets_Zexc", bool log = 1, bool decrease = 1, string varRivetName = "njetWJet_excl", string varBlackHatName = "njet_WMuNu");

void GetSysErrorTable (string variable, string outputTableName, TH1D* dataCenBackUp = NULL, TH1D* hDiffer[] = {NULL}, TH1D* hStatError = NULL, TH1D* hTotalError = NULL, int nGroup = 10);

void plotSystematicBreakdown (string outputDirectory, string variable, TH1D* dataCentral = NULL, TH1D* hSyst[] = NULL);
void plotSystematicBreakdownCombine (string outputDirectory, string variable, TH1D* dataCentral = NULL, TH1D* hSyst[] = NULL);

void plotXsecIncJetMultipli(string variable, TH1D* dataCentral = NULL, TH1D* genMad = NULL, TH1D* genBhatALL = NULL, TH1D* genShe = NULL, int nBins = 11, double xCoor[] = NULL, double yCoor[] = NULL, double xErr[] = NULL, double ySystDown[] = NULL, double ySystUp[] = NULL);

//--------------------------------


//--- More global variables -------------
double Luminosity(19.244 * 1000); // for SMu
bool isMuon = 1;
string leptonFlavor = "SMu";
string unfAlg = "Bayes";
bool doVarWidth = true ;
//--------------------------------
const int ZJetsFillStyle = 1001;


void PlottingEdit(int start, int end, bool doNPTB)
{
    //NVAROFINTEREST = 41;
    //for (int i(0); i < 1 /*NVAROFINTEREST*/; i++) {
    if (end == -1) end = start + 1;
    for (int i(start); i < end /*NVAROFINTERESTZJETS*/; i++) {
        if(VAROFINTERESTWJETS[i].name.find("Zinc5jet") != string::npos)
        {
            FuncPlot2(VAROFINTERESTWJETS[i].name, VAROFINTERESTWJETS[i].log, VAROFINTERESTWJETS[i].decrease, xsecVarNames[i].RivetName, xsecVarNames[i].BlackHatName);
        }
        else{
            FuncPlot(VAROFINTERESTWJETS[i].name, VAROFINTERESTWJETS[i].log, VAROFINTERESTWJETS[i].decrease, xsecVarNames[i].RivetName, xsecVarNames[i].BlackHatName, doNPTB);
        }
        
    }
    
}

void FuncPlot(string variable, bool logZ, bool decrease, string varRivetName, string varBlackHatName, bool doNPTB)
{
    cout << "processing variable: " << variable <<  "  " << varBlackHatName << endl;
    string energy = getEnergy();
    if ( !isMuon ) leptonFlavor = "DE";
    gStyle->SetOptStat(0);
    TH1::SetDefaultSumw2();
    
    // set file name for openning
    string fileName = "PNGFiles/FinalUnfold/" + leptonFlavor + "_" + energy +   "_unfolded_" + variable + "_histograms_" + unfAlg ;
    if (doVarWidth) fileName += "_VarWidth";
    fileName += ".root";
    cout << " opening : " << fileName <<endl;
    TFile *f  = new TFile(fileName.c_str());
    
    // get cental and gen
    TH1D *data              = (TH1D*) f->Get("Data"); // reco data
    TH1D *dataCentral       = (TH1D*) f->Get("Central"); // unfolded central (data -BG)
    
    TH1D *hm = (TH1D*) f->Get("Central");
    cout << "--- print central ---" << endl;
    for (int i =0; i<= hm->GetNbinsX()+1; i++){
        cout << hm->GetBinContent(i) << endl;
    }
    
    TH1D *genMad            = (TH1D*) f->Get("genMad");
    
    // get unfolded histogram for systematics
    int nSyst(18);
    TH1D *hSyst[19];
    hSyst[0]  = (TH1D*) f->Get("JESup");
    hSyst[1]  = (TH1D*) f->Get("JESdown");
    hSyst[2]  = (TH1D*) f->Get("PUup");
    hSyst[3]  = (TH1D*) f->Get("PUdown");
    hSyst[4]  = (TH1D*) f->Get("XSECup");
    hSyst[5]  = (TH1D*) f->Get("XSECdown");
    hSyst[6]  = (TH1D*) f->Get("JERup");
    hSyst[7]  = (TH1D*) f->Get("JERdown");
    hSyst[8]  = (TH1D*) f->Get("LepSFup");
    hSyst[9]  = (TH1D*) f->Get("LepSFdown");
    hSyst[10] = (TH1D*) f->Get("BtagEFFup");
    hSyst[11] = (TH1D*) f->Get("BtagEFFdown");
    hSyst[12] = (TH1D*) f->Get("MESup");
    hSyst[13] = (TH1D*) f->Get("MESdown");
    hSyst[14] = (TH1D*) f->Get("MER");
    hSyst[15] = (TH1D*) f->Get("WB");
    hSyst[16] = (TH1D*) f->Get("TTBAR");
    hSyst[17] = (TH1D*) f->Get("MC");
    if (variable.find("ZNGoodJets") == string::npos) {
        hSyst[18] = (TH1D*) f->Get("RESP");
        nSyst = 19;
    }
    
    
    //--- Get BlackHat ------
    //TH1D *genBhatALL = (TH1D*) f->Get("genBhat");
    
    
    TFile *fBhat[4];
    fBhat[0] = new TFile("PNGFiles/BlackHat/W1j_all.root", "READ");
    fBhat[1] = new TFile("PNGFiles/BlackHat/W2j_all.root", "READ");
    fBhat[2] = new TFile("PNGFiles/BlackHat/W3j_all.root", "READ");
    fBhat[3] = new TFile("PNGFiles/BlackHat/W4j_all.root", "READ");
    cout << " Opening: " << "W1j_all.root" << "   --->   Opened ? " << fBhat[0]->IsOpen() << endl;
    cout << " Opening: " << "W2j_all.root" << "   --->   Opened ? " << fBhat[1]->IsOpen() << endl;
    cout << " Opening: " << "W3j_all.root" << "   --->   Opened ? " << fBhat[2]->IsOpen() << endl;
    cout << " Opening: " << "W4j_all.root" << "   --->   Opened ? " << fBhat[3]->IsOpen() << endl;

    TH1D *genBhat[4];
    TH1D *genBhatALL;
    if (variable == "ZNGoodJets_Zexc") {
        genBhat[0] = (TH1D*) fBhat[0]->Get("njet_WMuNu");
        genBhat[1] = (TH1D*) fBhat[1]->Get("njet_WMuNu");
        genBhat[2] = (TH1D*) fBhat[2]->Get("njet_WMuNu");
        genBhat[3] = (TH1D*) fBhat[3]->Get("njet_WMuNu");
        genBhatALL = (TH1D*) genMad->Clone();
        for(int i = 1; i<= dataCentral->GetNbinsX(); i++){
            if (i==2) {
                genBhatALL->SetBinContent(i, genBhat[0]->GetBinContent(i)/1000.);
                genBhatALL->SetBinError  (i, genBhat[0]->GetBinError(i)/1000.);
            }
            else if (i==3){
                genBhatALL->SetBinContent(i, genBhat[1]->GetBinContent(i)/1000.);
                genBhatALL->SetBinError  (i, genBhat[1]->GetBinError(i)/1000.);
            }
            else if (i==4){
                genBhatALL->SetBinContent(i, genBhat[2]->GetBinContent(i)/1000.);
                genBhatALL->SetBinError  (i, genBhat[2]->GetBinError(i)/1000.);
            }
            else if (i==5){
                genBhatALL->SetBinContent(i, genBhat[3]->GetBinContent(i)/1000.);
                genBhatALL->SetBinError  (i, genBhat[3]->GetBinError(i)/1000.);
            }
            else {
                genBhatALL->SetBinContent(i, 0);
                genBhatALL->SetBinError  (i, 0);
            }
            //cout << "genBhatALL " << genBhatALL->GetBinContent(i) << endl;
        }
    }
    else if (variable == "ZNGoodJets_Zinc"){
        genBhatALL = (TH1D*) genMad->Clone();
        genBhatALL->Scale(1000); // to make it does not appear in the plot
        cout << " BH >> variable is not found" << endl;
    }
    else if (varBlackHatName == "NA"){
        genBhatALL = (TH1D*) genMad->Clone();
        genBhatALL->Scale(1000); // to make it does not appear in the plot
        cout << " BH >> variable is not found" << endl;
    }
    else {
        genBhat[0] = (TH1D*) fBhat[0]->Get(varBlackHatName.c_str()); genBhat[0]->Scale(0.001);
        genBhat[1] = (TH1D*) fBhat[1]->Get(varBlackHatName.c_str()); genBhat[1]->Scale(0.001);
        genBhat[2] = (TH1D*) fBhat[2]->Get(varBlackHatName.c_str()); genBhat[2]->Scale(0.001);
        genBhat[3] = (TH1D*) fBhat[3]->Get(varBlackHatName.c_str()); genBhat[3]->Scale(0.001);
        
        if (variable.find("Zinc1jet") != string::npos){
            genBhatALL = (TH1D*) genBhat[0]->Clone();
            cout << " BH >> variable is Zinc1jet >> W1j_all.root is used" << endl;
        }
        else if (variable.find("Zinc2jet") != string::npos) {
            genBhatALL = (TH1D*) genBhat[1]->Clone();
            cout << " BH >> variable is Zinc2jet >> W2j_all.root is used" << endl;
        }
        else if (variable.find("Zinc3jet") != string::npos) {
            genBhatALL = (TH1D*) genBhat[2]->Clone();
            cout << " BH >> variable is Zinc3jet >> W3j_all.root is used" << endl;
        }
        else if (variable.find("Zinc4jet") != string::npos) {
            genBhatALL = (TH1D*) genBhat[3]->Clone();
            cout << " BH >> variable is Zinc4jet >> W4j_all.root is used" << endl;
        }
        else {
            genBhatALL = (TH1D*) genMad->Clone();
            genBhatALL->Scale(1000); // to make it does not appear in the plot
            cout << " BH >> variable is not found" << endl;
        }
    }
    //--- End Get BlackHat ------
    
    //--- Get Hadronization correction for BH ------
    if (doNPTB){
        TFile *fHCorr = new TFile("PNGFiles/BlackHat/BHatHadronizeCorr.root");
        cout << " Opening: " << "BHatHadronizeCorr.root" << "   --->   Opened ? " << fHCorr->IsOpen() << endl;
        string hname = "hadSF_" + variable;
        TH1D *hisHCorr = (TH1D*) fHCorr->Get(hname.c_str());
        cout << "got fHCorr: " << hisHCorr->Integral() << endl;
        
        if (variable == "ZNGoodJets_Zexc") {
            for(int i = 2; i<= 5; i++){
                genBhatALL->SetBinContent(i, genBhatALL->GetBinContent(i) * hisHCorr->GetBinContent(i));
                genBhatALL->SetBinError  (i, genBhatALL->GetBinError(i) * hisHCorr->GetBinContent(i));
            }
        }
        else genBhatALL->Multiply(hisHCorr);
    }
    //------------------------

    
    //------ Get Sherpa ------
    //-- for getting Sherpa2
        // sherpa2 results in # of events >> need to do normalization
    TFile *fShe = new TFile("PNGFiles/Sherpa2/SMu_8TeV_WToLNu_Sherpa2jNLO4jLO_v5_EffiCorr_0_TrigCorr_0_Syst_0_JetPtMin_30_VarWidth.root");
    cout << " Opening: " << "SMu_8TeV_WToLNu_Sherpa2jNLO4jLO_v5_EffiCorr_0_TrigCorr_0_Syst_0_JetPtMin_30_VarWidth.root" << "   --->   Opened ? " << fShe->IsOpen() << endl;
    string genName = "gen" + variable;
    TH1D *genShe = (TH1D*) fShe->Get(genName.c_str());
    //--- End Get Sherpa2 ------
    
    //-- for getting sherpa1
        // sherpa1 results in pb-1 already >> no need to do normalization
    /*
    TFile *fShe = new TFile("PNGFiles/Sherpa1/wjets_sherpa.root");
    cout << " Opening: " << "wjets_sherpa.root" << "   --->   Opened ? " << fShe->IsOpen() << endl;
    TH1F *histoShe;
    if (varRivetName == "NA") {
        histoShe = (TH1F*) genMad->Clone();
        histoShe->Scale(1000); // to make it does not appear in the plot
        cout << " Sherpa >> variable is not found" << endl;
    }
    else {
        histoShe = (TH1F*) fShe->Get(varRivetName.c_str());
    }
    TH1D *genShe = (TH1D*) histoShe->Clone();
    */
    //-------------------------
    
    
    // print out integral value for testing
    const int nBins(dataCentral->GetNbinsX());
    cout << " central integral:  " << variable << "   " << dataCentral->Integral(1, nBins) << " with under/over:  " << dataCentral->Integral(0, nBins+1) << endl;
    cout << " sherpa integral:  " << variable << "   " << genShe->Integral(1, nBins) << " with under/over:  " << dataCentral->Integral(0, nBins+1) << endl;
    
    cout << "nSyst" << nSyst << endl;
    for (int syst(0); syst < nSyst; syst++) {
        double tempScale = hSyst[syst]->Integral(1, nBins) ;
        cout << " original integral for " << syst << " : " << tempScale << endl;
    }
    
    // cross section or normalized
    bool doXSec(1);
    bool doNormalize(0);
    if (doXSec) doNormalize = false;
    if (doNormalize) doXSec = false;
    
    if (doXSec && !doNormalize ) {
        for (int i(1); i <= nBins; i++){
            double binW = data->GetBinWidth(i);
            data->SetBinContent(i, data->GetBinContent(i)*1./(Luminosity*binW));
            dataCentral->SetBinContent(i, dataCentral->GetBinContent(i)*1./(Luminosity*binW));
            genMad->SetBinContent(i, genMad->GetBinContent(i)*1./(Luminosity*binW));
            genShe->SetBinContent(i, genShe->GetBinContent(i)*1./(Luminosity*binW));
            
            data->SetBinError(i, data->GetBinError(i)*1./(Luminosity*binW));
            dataCentral->SetBinError(i, dataCentral->GetBinError(i)*1./(Luminosity*binW));
            genMad->SetBinError(i, genMad->GetBinError(i)*1./(Luminosity*binW));
            genShe->SetBinError(i, genShe->GetBinError(i)*1./(Luminosity*binW));
        }
        for (int syst(0); syst < nSyst; syst++){
            for (int i(1); i <= nBins; i++){
                double binW = data->GetBinWidth(i);
                hSyst[syst]->SetBinContent(i, hSyst[syst]->GetBinContent(i)*1./(Luminosity*binW));
                hSyst[syst]->SetBinError(i, hSyst[syst]->GetBinError(i)*1./(Luminosity*binW));
            }
        }
    }
    if (doNormalize) {
        data->Scale(1/data->Integral(1,nBins));
        dataCentral->Scale(1/dataCentral->Integral(1,nBins));
        genMad->Scale(1/genMad->Integral(1,nBins));
        for (int syst(0); syst < nSyst; syst++){
            double tempScale = hSyst[syst]->Integral(1,nBins) ;
            cout << " scale for syst " << syst << " : " << tempScale << endl;
            hSyst[syst]->Scale(1/tempScale);
        }
    }
    

    //--- systematic error computation --------------------------
    cout << "go to uncertainty computation " << endl;
    double xCoor[nBins], yCoor[nBins], xErr[nBins], ySystDown[nBins], ySystUp[nBins];
    for (int bin(1); bin <= nBins; bin++) {
        
        double centralValue (dataCentral->GetBinContent(bin));
        double totalStatistics (dataCentral->GetBinError(bin));
        double totalSystematicsUp (0.);
        double totalSystematicsDown (0.);
        
        // set effSF = 0. So it has no contribution.
        double effSF = 0.00;
        totalSystematicsUp += pow( pow((1.+effSF), 2) - 1 , 2);
        totalSystematicsDown += pow( pow((1.+effSF), 2) - 1 , 2);
        
        for(int i = 0; i < 12; i++) {
            // contribution from JES PU Xsec JER LepSF Btag MES
            if (i <= 6) {
                double diffFromUp(hSyst[2*i]->GetBinContent(bin) - centralValue);
                double diffFromDown(hSyst[(2*i)+1]->GetBinContent(bin) - centralValue);
                double SysDiffUPTemp(0);
                double SysDiffDownTemp(0);
                
                if (diffFromUp >= diffFromDown) {
                    SysDiffUPTemp = diffFromUp;
                    SysDiffDownTemp = diffFromDown;
                }
                else {
                    SysDiffUPTemp = diffFromDown;
                    SysDiffDownTemp = diffFromUp;
                }
                
                if (SysDiffUPTemp >= 0) {
                    totalSystematicsUp += SysDiffUPTemp * SysDiffUPTemp;
                }
                if (SysDiffDownTemp <= 0) {
                    totalSystematicsDown += SysDiffDownTemp * SysDiffDownTemp;
                }
            }
            // contribution from MER hSyst[14] = (TH1D*) f->Get("MER");
            if (i == 7) {
                double difference(hSyst[14]->GetBinContent(bin) - centralValue);
                totalSystematicsUp += difference * difference;
                totalSystematicsDown += difference * difference;
            }
            // contribution from Wb hSyst[15] = (TH1D*) f->Get("WB");
            if (i == 8) {
                double difference(hSyst[15]->GetBinContent(bin) - centralValue);
                totalSystematicsUp += difference * difference;
                totalSystematicsDown += difference * difference;
            }
            // contribution from ttbar hSyst[16] = (TH1D*) f->Get("TTBAR");
            if (i == 9) {
                double difference(hSyst[16]->GetBinContent(bin) - centralValue);
                totalSystematicsUp += difference * difference;
                totalSystematicsDown += difference * difference;
            }
            // contribution from MC hSyst[17] = (TH1D*) f->Get("MC");
            if (i == 10) {
                double difference(hSyst[17]->GetBinContent(bin) - centralValue);
                totalSystematicsUp += difference * difference;
                totalSystematicsDown += difference * difference;
            }
            // contribution from Resp hSyst[18] = (TH1D*) f->Get("RESP");
            if (i == 11) {
                if (variable.find("ZNGoodJets") == string::npos){
                    double difference(hSyst[18]->GetBinContent(bin) - centralValue);
                    totalSystematicsUp += difference * difference;
                    totalSystematicsDown += difference * difference;
                }
            }
        }
        
        // contribution from integrated lumi = 2.6%
        totalSystematicsUp += pow( ((2.6/100.0) * centralValue), 2 );
        totalSystematicsDown += pow( ((2.6/100.0) * centralValue), 2 );
        
        
        //--- Set up for systematic plot on pad 1
            // set x-coordinate and error in x
        xCoor[bin-1]    = dataCentral->GetBinCenter(bin);
        xErr[bin-1]     = 0.5 * dataCentral->GetBinWidth(bin);
            // set y-coordinate
        yCoor[bin-1]    = centralValue;
            // set systematics errors in y => combine both statistical and systematics
        ySystUp[bin-1]     = sqrt(totalStatistics * totalStatistics + totalSystematicsUp );
        ySystDown[bin-1]   = sqrt(totalStatistics * totalStatistics + totalSystematicsDown );
    }
    cout << " finished uncertainty computation " << endl;
    cout << "line th " << __LINE__ << endl;
    
    //===========================================================
    //--- plot the inclusive jet multiplicity
    if (variable.find("ZNGoodJets") != string::npos){
        cout << " producing inclusive jet multiplicity plot" << endl;
        plotXsecIncJetMultipli(variable, dataCentral, genMad, genBhatALL, genShe, nBins, xCoor, yCoor, xErr, ySystDown, ySystUp);
    }
    //---
    //===========================================================
    
    //--- Set up for systematic plot on pad 1
    TGraphAsymmErrors *grCentralSyst = new TGraphAsymmErrors(nBins, xCoor, yCoor, xErr, xErr, ySystDown, ySystUp);
    
    //--- plotting ---
    TH1D *hisUnfolded = (TH1D*) dataCentral->Clone();
    
    TCanvas *can = new TCanvas(variable.c_str(), variable.c_str(), 600, 800);
    can->cd();
    //--- Set pad1.
    TPad *pad1 = new TPad("pad1","pad1", 0.01, 0.55, 0.99, 0.99);
    pad1->SetTopMargin(0.11);
    pad1->SetBottomMargin(0);
    pad1->SetRightMargin(0.04);
    pad1->SetLeftMargin(0.11);
    pad1->SetTicks();
    pad1->Draw();
    pad1->cd();
    pad1->SetLogy();
    
    // set y axis title
    string temp;
    string xtitle = genMad->GetXaxis()->GetTitle();
    string shortVar = xtitle.substr(0, xtitle.find(" "));
    
    string unit = "";
    if (xtitle.find("[") != string::npos) {
        size_t begin = xtitle.find("[") + 1;
        unit = xtitle.substr(begin);
        unit = unit.substr(0, unit.find("]"));
    }
    //--- temporary fix label: FIXME
    //if (variable.find("dRapidity") != string::npos) shortVar = "#Deltay";
    //if (variable.find("dRJets") != string::npos)    shortVar = "#DeltaR";
    if (variable.find("diJetPt") != string::npos)    shortVar = "p_{T}";
    //---
    cout << shortVar << "   unit: " << unit << endl;
    temp = "d#sigma / d" + shortVar;
    temp += "  [pb";
    if (unit != "" ) temp += " / " + unit;
    temp += "]";
    //-------
    
    double MineYMax = genMad->GetMaximum();
    //double MineYMin = genMad->GetMinimum();
    if (hisUnfolded->GetMaximum() >= MineYMax) MineYMax = hisUnfolded->GetMaximum();
    //if (hisUnfolded->GetMinimum() <= MineYMin) MineYMin = hisUnfolded->GetMinimum();
    
    hisUnfolded->SetTitle("");
    if (variable.find("ZNGoodJets") != string::npos) {
        hisUnfolded->GetXaxis()->SetRange(1, 8);
    }
    hisUnfolded->GetYaxis()->SetTitle(temp.c_str());
    hisUnfolded->GetYaxis()->SetTitleSize(0.04);
    hisUnfolded->GetYaxis()->SetTitleOffset(1.45);
    hisUnfolded->GetYaxis()->SetTitleFont(42);
    //hisUnfolded->SetMinimum(MineYMin*0.8);
    if ( (variable.find("dRapidity") != string::npos) || (variable.find("dRJets") != string::npos) ){
        hisUnfolded->SetMaximum(MineYMax*10); // remove or modify this when appropriate
    }
    
    hisUnfolded->SetFillStyle(3354);
    hisUnfolded->SetFillColor(12);
    hisUnfolded->SetLineColor(kBlack);
    hisUnfolded->SetLineWidth(2);
    hisUnfolded->SetMarkerColor(kBlack);
    hisUnfolded->SetMarkerStyle(20);
    hisUnfolded->SetMarkerSize(0.8);
    hisUnfolded->Draw("E1");
    
    grCentralSyst->SetFillStyle(3354);
    grCentralSyst->SetFillColor(12);
    grCentralSyst->Draw("2");
    
    genMad->SetFillStyle(ZJetsFillStyle);
    genMad->SetFillColor(kBlue-10);
    //genMad->SetFillColorAlpha(kBlue-10, 0.60);
    genMad->SetLineColor(kBlue);
    genMad->SetLineWidth(2);
    genMad->SetMarkerColor(kBlue);
    genMad->SetMarkerStyle(24);
    //genMad->Draw("E2 same");
    genMad->Draw("E same");
    
    genBhatALL->SetFillStyle(ZJetsFillStyle);
    genBhatALL->SetFillColor(kOrange-2);
    //genBhatALL->SetFillColorAlpha(kOrange-2, 0.60);
    genBhatALL->SetLineColor(kOrange+10);
    genBhatALL->SetLineWidth(2);
    genBhatALL->SetMarkerColor(kOrange+10);
    genBhatALL->SetMarkerStyle(25);
    //genBhatALL->Draw("E2 same");
    genBhatALL->Draw("E same");
    
    genShe->SetFillStyle(ZJetsFillStyle);
    genShe->SetFillColor(kGreen-8);
    //genShe->SetFillColorAlpha(kGreen-8, 0.60);
    genShe->SetLineColor(kGreen+3);
    genShe->SetLineWidth(2);
    genShe->SetMarkerColor(kGreen+3);
    genShe->SetMarkerStyle(26);
    //genShe->Draw("E2 same");
    genShe->Draw("E same");
    
    pad1->Draw();
    //--- TLegend ---
    TLegend *legend = new TLegend(0.47, 0.74, 0.99, 0.98);
    legend->SetX1(0.44);
    legend->SetY1(0.77);
    legend->SetX2(0.95);
    legend->SetY2(0.98);
    legend->SetTextSize(.034);
    legend->SetFillColor(0);
    legend->SetFillStyle(1001);
    legend->SetBorderSize(1);
    legend->AddEntry(hisUnfolded, "Data", "PLEF");
    legend->AddEntry(genMad, "MG5 + PY6 (#leq 4j LO + PS)", "plef");
    legend->AddEntry(genBhatALL, "BLACKHAT+SHERPA (NLO)", "plef");
    legend->AddEntry(genShe, "SHERPA 2 (#leq 2j NLO 3,4j LO + PS)", "plef");
    legend->Draw();
    //------------------
    
    //--- TLatex stuff ---
    string njetStr;
    if (variable.find("Zinc1jet") != string::npos){njetStr = " + #geq 1jet";}
    else if (variable.find("Zinc2jet") != string::npos) {njetStr = " + #geq 2jets";}
    else if (variable.find("Zinc3jet") != string::npos) {njetStr = " + #geq 3jets";}
    else if (variable.find("Zinc4jet") != string::npos) {njetStr = " + #geq 4jets";}
    else if (variable.find("Zinc5jet") != string::npos) {njetStr = " + #geq 5jets";}
    else {njetStr = " channel";}
    string procStr = "W (#rightarrow #mu#nu)" + njetStr;
    
    //--- TLatex stuff ---
    TLatex *latexLabel = new TLatex();
    latexLabel->SetNDC();
    latexLabel->SetTextSize(0.05);
    latexLabel->SetTextFont(42);
    latexLabel->SetLineWidth(2);
    latexLabel->SetTextFont(61);
    latexLabel->DrawLatex(0.13,0.95,"CMS");
    latexLabel->SetTextFont(52);
    latexLabel->DrawLatex(0.20,0.95,"Preliminary");
    latexLabel->SetTextFont(42);
    latexLabel->DrawLatex(0.13,0.95-0.045,"19.6 fb^{-1} (8 TeV)");
    if (decrease){
        latexLabel->DrawLatex(0.18,0.21-0.05,"anti-k_{T} (R = 0.5) Jets");
        latexLabel->DrawLatex(0.18,0.21-0.11,"p_{T}^{jet} > 30 GeV, |#eta^{jet}| < 2.4 ");
        latexLabel->DrawLatex(0.18,0.21-0.17, procStr.c_str());
        latexLabel->SetName("latexLabel");
    }
    else {
        latexLabel->DrawLatex(0.58,0.21-0.05,"anti-k_{T} (R = 0.5) Jets");
        latexLabel->DrawLatex(0.58,0.21-0.11,"p_{T}^{jet} > 30 GeV, |#eta^{jet}| < 2.4 ");
        latexLabel->DrawLatex(0.58,0.21-0.17, procStr.c_str());
        latexLabel->SetName("latexLabel");
    }
    latexLabel->Draw("same");
    
    
    //--- calculate ratio >> pad2, pad3
    
    // for data statistical uncer
    TH1D  *data1 = (TH1D*) hisUnfolded->Clone();
    for(int i = 1; i <= hisUnfolded->GetNbinsX(); i++) {
        data1->SetBinContent(i, 1.0);
        if (hisUnfolded->GetBinContent(i) > 0.) {
            data1->SetBinError(i, hisUnfolded->GetBinError(i)/hisUnfolded->GetBinContent(i));
        }
        else data1->SetBinError(i, 0.);
        
    }
    TH1D  *data2 = (TH1D*) data1->Clone();
    TH1D  *data3 = (TH1D*) data1->Clone();
    
    // for gen statistical uncer
    TH1D  *gen1 = (TH1D*) genMad->Clone();
    for(int i = 1; i <= hisUnfolded->GetNbinsX(); i++) {
        if (hisUnfolded->GetBinContent(i) > 0.) {
            gen1->SetBinContent(i, genMad->GetBinContent(i)/hisUnfolded->GetBinContent(i));
            gen1->SetBinError(i, genMad->GetBinError(i)/hisUnfolded->GetBinContent(i));
        }
        else {
            gen1->SetBinContent(i, 0.);
            gen1->SetBinError(i, 0.);
        }
    }
    TH1D  *gen2 = (TH1D*) hisUnfolded->Clone();
    for(int i = 1; i <= hisUnfolded->GetNbinsX(); i++) {
        if (hisUnfolded->GetBinContent(i) > 0.) {
            gen2->SetBinContent(i, genBhatALL->GetBinContent(i)/hisUnfolded->GetBinContent(i));
            gen2->SetBinError(i, genBhatALL->GetBinError(i)/hisUnfolded->GetBinContent(i));
        }
        else {
            gen2->SetBinContent(i, 0.);
            gen2->SetBinError(i, 0.);
        }
    }
    TH1D  *gen3 = (TH1D*) hisUnfolded->Clone();
    for(int i = 1; i <= hisUnfolded->GetNbinsX(); i++) {
        if (hisUnfolded->GetBinContent(i) > 0.) {
            gen3->SetBinContent(i, genShe->GetBinContent(i)/hisUnfolded->GetBinContent(i));
            gen3->SetBinError(i, genShe->GetBinError(i)/hisUnfolded->GetBinContent(i));
        }
        else {
            gen3->SetBinContent(i, 0.);
            gen3->SetBinError(i, 0.);
        }
        //cout << " sherpa " << gen3->GetBinContent(i) << endl;
    }
    
    //--- Set up for systematic plot on pad 1
    double xCoorCenRatio[nBins], yCoorCenRatio[nBins], xErrCenRatio[nBins], ySystDownCenRatio[nBins], ySystUpCenRatio[nBins];
    for (int bin(1); bin <= nBins; bin++) {
        // set x-coordinate and error in x
        xCoorCenRatio[bin-1] = hisUnfolded->GetBinCenter(bin);
        xErrCenRatio[bin-1]  = 0.5 * hisUnfolded->GetBinWidth(bin);
        // set y-coordinate
        yCoorCenRatio[bin-1] = 1.;
        // set systematics errors in y => combine both statistical and systematics
        if (hisUnfolded->GetBinContent(bin) > 0) {
            ySystUpCenRatio[bin-1]     = ySystUp[bin-1]/hisUnfolded->GetBinContent(bin);
            ySystDownCenRatio[bin-1]   = ySystDown[bin-1]/hisUnfolded->GetBinContent(bin);
        }
        else {
            ySystUpCenRatio[bin-1] = 0.;
            ySystDownCenRatio[bin-1] = 0.;
        }
        
    }
    TGraphAsymmErrors *grCentralSystRatio = new TGraphAsymmErrors(nBins, xCoorCenRatio, yCoorCenRatio, xErrCenRatio, xErrCenRatio, ySystDownCenRatio, ySystUpCenRatio);
    

    //--- NEW TPAD => pad2 ---//
    can->cd();
    TPad *pad2 = new TPad("pad2","pad2", 0.01, 0.39, 0.99, 0.55);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0);
    pad2->SetRightMargin(0.04);
    pad2->SetLeftMargin(0.11);
    pad2->SetGridy();
    pad2->SetTicks();
    pad2->Draw();
    pad2->cd();
    
    data1->SetTitle("");
    data1->GetYaxis()->SetRangeUser(0.41,1.59);
    data1->GetYaxis()->SetTitle("MG5/Data");
    data1->GetYaxis()->SetTitleSize(0.13);
    data1->GetYaxis()->SetTitleOffset(0.44);
    data1->GetYaxis()->CenterTitle();
    data1->GetYaxis()->SetLabelSize(0.12);
    data1->GetYaxis()->SetLabelOffset(0.01);
    
    data1->GetXaxis()->SetTickLength(0.06);
    
    data1->SetLineColor(kBlack);
    data1->SetLineWidth(1);
    data1->SetMarkerColor();
    data1->SetMarkerStyle();
    data1->Draw("E1");
    
    gen1->SetFillStyle(ZJetsFillStyle);
    gen1->SetFillColor(kBlue-10);
    gen1->SetLineColor(kBlue);
    gen1->SetLineWidth(2);
    gen1->SetMarkerColor(kBlue);
    gen1->SetMarkerStyle(24);
    gen1->Draw("E2 same");
    gen1->Draw("E1 same");
    
    grCentralSystRatio->SetFillStyle(3354);
    grCentralSystRatio->SetFillColor(12);
    grCentralSystRatio->Draw("2");
    
    TLegend *legend2 = new TLegend(0.16, 0.05, 0.33, 0.20);
    legend2->SetFillColor(0);
    legend2->SetFillStyle(ZJetsFillStyle);
    legend2->SetBorderSize(0);
    //legend2->SetTextSize(.12);
    legend2->SetY1(0.07);
    legend2->SetX1(0.15);
    legend2->SetY2(0.20);
    legend2->SetTextSize(0.09);
    TLegendEntry *leEntry2;
    leEntry2 = legend2->AddEntry(gen1, "Stat. unc. (gen)", "f");
    legend2->Draw("same");
    
    // Text Madgraph
    /*
    TLatex *latexLabel0 = new TLatex();
    latexLabel0->SetTextSize(0.15);
    latexLabel0->SetTextFont(42);
    latexLabel0->SetLineWidth(2);
    latexLabel0->SetTextColor(kAzure-5);
    latexLabel0->SetNDC();
    latexLabel0->DrawLatex(0.35,0.09,"Madgraph");
    */
    pad2->Draw();
    
    
    //--- NEW TPAD => pad3 ---//
    can->cd();
    TPad *pad3 = new TPad("pad3","pad3", 0.01, 0.23, 0.99, 0.39);
    pad3->SetTopMargin(0);
    pad3->SetBottomMargin(0);
    pad3->SetRightMargin(0.04);
    pad3->SetLeftMargin(0.11);
    pad3->SetGridy();
    pad3->SetTicks();
    pad3->Draw();
    pad3->cd();
    
    data2->SetTitle("");
    data2->GetYaxis()->SetRangeUser(0.41,1.59);
    data2->GetYaxis()->SetTitle("BLACKHAT/Data");
    data2->GetYaxis()->SetTitleSize(0.13);
    data2->GetYaxis()->SetTitleOffset(0.44);
    data2->GetYaxis()->CenterTitle();
    data2->GetYaxis()->SetLabelSize(0.12);
    data2->GetYaxis()->SetLabelOffset(0.01);
    
    data2->GetXaxis()->SetTickLength(0.06);
    
    data2->SetLineColor(kBlack);
    data2->SetLineWidth(1);
    data2->SetMarkerColor();
    data2->SetMarkerStyle();
    data2->Draw("E1");
    
    TLegend *legend3 = new TLegend(0.16, 0.05, 0.33, 0.20);
    legend3->SetFillColor(0);
    legend3->SetFillStyle(ZJetsFillStyle);
    legend3->SetBorderSize(0);
    //legend3->SetTextSize(.12);
    legend3->SetY1(0.07);
    legend3->SetX1(0.15);
    legend3->SetY2(0.20);
    legend3->SetTextSize(0.09);
    TLegendEntry *leEntry3;
    leEntry3 = legend3->AddEntry(gen2, "Stat. unc. (gen)", "f");
    legend3->Draw("same");
    
    gen2->SetFillStyle(ZJetsFillStyle);
    gen2->SetFillColor(kOrange-2);
    gen2->SetLineColor(kOrange+10);
    gen2->SetLineWidth(2);
    gen2->SetMarkerColor(kOrange+10);
    gen2->SetMarkerStyle(25);
    gen2->Draw("E2 same");
    gen2->Draw("E1 same");
    
    grCentralSystRatio->SetFillStyle(3354);
    grCentralSystRatio->SetFillColor(12);
    grCentralSystRatio->Draw("2");
    
    
    
    // Text BlackHat
    /*
    TLatex *latexLabel1 = new TLatex();
    latexLabel1->SetTextSize(0.15);
    latexLabel1->SetTextFont(42);
    latexLabel1->SetLineWidth(2);
    latexLabel1->SetTextColor(kOrange+2);
    latexLabel1->SetNDC();
    latexLabel1->DrawLatex(0.35,0.09,"BlackHat");
    */
    pad3->Draw();
    
    //--- NEW TPAD => pad4 ---//
    can->cd();
    TPad *pad4 = new TPad("pad3","pad3", 0.01, 0.01, 0.99, 0.23);
    pad4->SetTopMargin(0);
    pad4->SetBottomMargin(0.25);
    pad4->SetRightMargin(0.04);
    pad4->SetLeftMargin(0.11);
    pad4->SetGridy();
    pad4->SetTicks();
    pad4->Draw();
    pad4->cd();
    
    data3->SetTitle("");
    data3->GetYaxis()->SetRangeUser(0.41,1.59);
    data3->GetYaxis()->SetTitle("SHERPA2/Data");
    data3->GetYaxis()->SetTitleSize(0.096);
    data3->GetYaxis()->SetTitleOffset(0.6);
    data3->GetYaxis()->CenterTitle();
    data3->GetYaxis()->SetLabelSize(0.085);
    data3->GetYaxis()->SetLabelOffset(0.012);
    
    data3->GetXaxis()->SetTitle(gen1->GetXaxis()->GetTitle());
    data3->GetXaxis()->SetTitleSize(0.1);
    data3->GetXaxis()->SetTitleOffset(1.05);
    data3->GetXaxis()->SetLabelSize(0.09);
    data3->GetXaxis()->SetLabelOffset(0.014);
    data3->GetXaxis()->SetTickLength(0.06);
    
    data3->SetLineColor(kBlack);
    data3->SetLineWidth(1);
    data3->SetMarkerColor();
    data3->SetMarkerStyle();
    data3->Draw("E1");
    
    gen3->SetFillStyle(ZJetsFillStyle);
    gen3->SetFillColor(kGreen-8);
    gen3->SetLineColor(kGreen+3);
    gen3->SetLineWidth(2);
    gen3->SetMarkerColor(kGreen+3);
    gen3->SetMarkerStyle(26);
    gen3->Draw("E2 same");
    gen3->Draw("E1 same");
    
    grCentralSystRatio->SetFillStyle(3354);
    grCentralSystRatio->SetFillColor(12);
    grCentralSystRatio->Draw("2");
    
    TLegend *legend4 = new TLegend(0.16, 0.05, 0.33, 0.20);
    legend4->SetFillColor(0);
    legend4->SetFillStyle(ZJetsFillStyle);
    legend4->SetBorderSize(0);
    //legend4->SetTextSize(.12);
    legend4->SetY1(0.32);
    legend4->SetX1(0.15);
    legend4->SetY2(0.42);
    legend4->SetTextSize(0.065);
    TLegendEntry *leEntry4;
    leEntry4 = legend4->AddEntry(gen3, "Stat. unc. (gen)", "f");
    legend4->Draw("same");
    
    // Text Sherpa (position not relative to pad4)
    /*
    TLatex *latexLabel2 = new TLatex();
    latexLabel2->SetTextSize(0.11);
    latexLabel2->SetTextFont(42);
    latexLabel2->SetLineWidth(2);
    latexLabel2->SetTextColor(kGreen+3);
    latexLabel2->SetNDC();
    latexLabel2->DrawLatex(0.35,0.34,"Sherpa");
    */
    pad4->Draw();
    can->cd();
    
    //// Set output .pdf file in the direcctory "PNGFiles/FinalResults/"
    string command = "mkdir -p PNGFiles/FinalResults" ;
    system(command.c_str());
    string outputDirectory = "PNGFiles/FinalResults/";
    string outputFileNamePNG = outputDirectory + leptonFlavor + "_" + energy + "_RunPlotting_";
    
    if (doXSec) outputFileNamePNG += "XSec_";
    else if (doNormalize) outputFileNamePNG += "Normalized_";
    
    outputFileNamePNG += variable + "_" + unfAlg ;
    if (doVarWidth)  outputFileNamePNG += "_VarWidth";
    outputFileNamePNG += ".pdf";
    can->Print(outputFileNamePNG.c_str());
    
    
    //--- Generate new plot to see Contribution : all input histo are already scaled to the final unit ---
    //plotSystematicBreakdown(outputDirectory, variable, dataCentral, hSyst);
    plotSystematicBreakdownCombine(outputDirectory, variable, dataCentral, hSyst);

}

void FuncPlot2(string variable, bool logZ, bool decrease, string varRivetName, string varBlackHatName)
{
    cout << "processing variable: " << variable <<  "  " << varBlackHatName << endl;
    string energy = getEnergy();
    if ( !isMuon ) leptonFlavor = "DE";
    gStyle->SetOptStat(0);
    TH1::SetDefaultSumw2();
    
    // set file name for openning
    string fileName = "PNGFiles/FinalUnfold/" + leptonFlavor + "_" + energy +   "_unfolded_" + variable + "_histograms_" + unfAlg ;
    if (doVarWidth) fileName += "_VarWidth";
    fileName += ".root";
    cout << " opening : " << fileName <<endl;
    TFile *f  = new TFile(fileName.c_str());
    
    // get cental and gen
    TH1D *data              = (TH1D*) f->Get("Data"); // reco data
    TH1D *dataCentral       = (TH1D*) f->Get("Central"); // unfolded central (data -BG)
    
    TH1D *hm = (TH1D*) f->Get("Central");
    cout << "--- print central ---" << endl;
    for (int i =0; i<= hm->GetNbinsX()+1; i++){
        cout << hm->GetBinContent(i) << endl;
    }
    
    TH1D *genMad            = (TH1D*) f->Get("genMad");
    
    // get unfolded histogram for systematics
    int nSyst(18);
    TH1D *hSyst[19];
    hSyst[0]  = (TH1D*) f->Get("JESup");
    hSyst[1]  = (TH1D*) f->Get("JESdown");
    hSyst[2]  = (TH1D*) f->Get("PUup");
    hSyst[3]  = (TH1D*) f->Get("PUdown");
    hSyst[4]  = (TH1D*) f->Get("XSECup");
    hSyst[5]  = (TH1D*) f->Get("XSECdown");
    hSyst[6]  = (TH1D*) f->Get("JERup");
    hSyst[7]  = (TH1D*) f->Get("JERdown");
    hSyst[8]  = (TH1D*) f->Get("LepSFup");
    hSyst[9]  = (TH1D*) f->Get("LepSFdown");
    hSyst[10] = (TH1D*) f->Get("BtagEFFup");
    hSyst[11] = (TH1D*) f->Get("BtagEFFdown");
    hSyst[12] = (TH1D*) f->Get("MESup");
    hSyst[13] = (TH1D*) f->Get("MESdown");
    hSyst[14] = (TH1D*) f->Get("MER");
    hSyst[15] = (TH1D*) f->Get("WB");
    hSyst[16] = (TH1D*) f->Get("TTBAR");
    hSyst[17] = (TH1D*) f->Get("MC");
    if (variable.find("ZNGoodJets") == string::npos) {
        hSyst[18] = (TH1D*) f->Get("RESP");
        nSyst = 19;
    }
    
    
    //--- Get BlackHat ------
    /*
    //TH1D *genBhatALL = (TH1D*) f->Get("genBhat");
    TFile *fBhat[3];
    fBhat[0] = new TFile("PNGFiles/BlackHat/W1j_all.root", "READ");
    fBhat[1] = new TFile("PNGFiles/BlackHat/W2j_all.root", "READ");
    fBhat[2] = new TFile("PNGFiles/BlackHat/W3j_all.root", "READ");
    cout << " Opening: " << "W1j_all.root" << "   --->   Opened ? " << fBhat[0]->IsOpen() << endl;
    cout << " Opening: " << "W2j_all.root" << "   --->   Opened ? " << fBhat[1]->IsOpen() << endl;
    cout << " Opening: " << "W3j_all.root" << "   --->   Opened ? " << fBhat[2]->IsOpen() << endl;
    
    TH1D *genBhat[3];
    TH1D *genBhatALL;
    if (variable == "ZNGoodJets_Zexc") {
        genBhat[0] = (TH1D*) fBhat[0]->Get("njet_WMuNu");
        genBhat[1] = (TH1D*) fBhat[1]->Get("njet_WMuNu");
        genBhat[2] = (TH1D*) fBhat[2]->Get("njet_WMuNu");
        genBhatALL = (TH1D*) genMad->Clone();
        for(int i = 1; i<= dataCentral->GetNbinsX(); i++){
            if (i==2) {
                genBhatALL->SetBinContent(i, genBhat[0]->GetBinContent(i)/1000.);
                genBhatALL->SetBinError  (i, genBhat[0]->GetBinError(i)/1000.);
            }
            else if (i==3){
                genBhatALL->SetBinContent(i, genBhat[1]->GetBinContent(i)/1000.);
                genBhatALL->SetBinError  (i, genBhat[1]->GetBinError(i)/1000.);
            }
            else if (i==4){
                genBhatALL->SetBinContent(i, genBhat[2]->GetBinContent(i)/1000.);
                genBhatALL->SetBinError  (i, genBhat[2]->GetBinError(i)/1000.);
            }
            else {
                genBhatALL->SetBinContent(i, 0);
                genBhatALL->SetBinError  (i, 0);
            }
            //cout << "genBhatALL " << genBhatALL->GetBinContent(i) << endl;
        }
    }
    else if (variable == "ZNGoodJets_Zinc"){
        genBhatALL = (TH1D*) genMad->Clone();
        genBhatALL->Scale(1000); // to make it does not appear in the plot
        cout << " BH >> variable is not found" << endl;
    }
    else if (varBlackHatName == "NA"){
        genBhatALL = (TH1D*) genMad->Clone();
        genBhatALL->Scale(1000); // to make it does not appear in the plot
        cout << " BH >> variable is not found" << endl;
    }
    else {
        genBhat[0] = (TH1D*) fBhat[0]->Get(varBlackHatName.c_str()); genBhat[0]->Scale(0.001);
        genBhat[1] = (TH1D*) fBhat[1]->Get(varBlackHatName.c_str()); genBhat[1]->Scale(0.001);
        genBhat[2] = (TH1D*) fBhat[2]->Get(varBlackHatName.c_str()); genBhat[2]->Scale(0.001);
        
        if (variable.find("Zinc1jet") != string::npos){
            genBhatALL = (TH1D*) genBhat[0]->Clone();
            cout << " BH >> variable is Zinc1jet >> W1j_all.root is used" << endl;
        }
        else if (variable.find("Zinc2jet") != string::npos) {
            genBhatALL = (TH1D*) genBhat[1]->Clone();
            cout << " BH >> variable is Zinc2jet >> W2j_all.root is used" << endl;
        }
        else if (variable.find("Zinc3jet") != string::npos) {
            genBhatALL = (TH1D*) genBhat[2]->Clone();
            cout << " BH >> variable is Zinc3jet >> W3j_all.root is used" << endl;
        }
        else {
            genBhatALL = (TH1D*) genMad->Clone();
            genBhatALL->Scale(1000); // to make it does not appear in the plot
            cout << " BH >> variable is not found" << endl;
        }
    }
     */
    //--- End Get BlackHat ------
    
    //------ Get Sherpa ------
    //-- for getting Sherpa2
    // sherpa2 results in # of events >> need to do normalization
    TFile *fShe = new TFile("PNGFiles/Sherpa2/SMu_8TeV_WToLNu_Sherpa2jNLO4jLO_v5_EffiCorr_0_TrigCorr_0_Syst_0_JetPtMin_30_VarWidth.root");
    cout << " Opening: " << "SMu_8TeV_WToLNu_Sherpa2jNLO4jLO_v5_EffiCorr_0_TrigCorr_0_Syst_0_JetPtMin_30_VarWidth.root" << "   --->   Opened ? " << fShe->IsOpen() << endl;
    string genName = "gen" + variable;
    TH1D *genShe = (TH1D*) fShe->Get(genName.c_str());
    //--- End Get Sherpa2 ------
    
    //-- for getting sherpa1
    // sherpa1 results in pb-1 already >> no need to do normalization
    /*
     TFile *fShe = new TFile("PNGFiles/Sherpa1/wjets_sherpa.root");
     cout << " Opening: " << "wjets_sherpa.root" << "   --->   Opened ? " << fShe->IsOpen() << endl;
     TH1F *histoShe;
     if (varRivetName == "NA") {
     histoShe = (TH1F*) genMad->Clone();
     histoShe->Scale(1000); // to make it does not appear in the plot
     cout << " Sherpa >> variable is not found" << endl;
     }
     else {
     histoShe = (TH1F*) fShe->Get(varRivetName.c_str());
     }
     TH1D *genShe = (TH1D*) histoShe->Clone();
     */
    //-------------------------
    
    
    // print out integral value for testing
    const int nBins(dataCentral->GetNbinsX());
    cout << " central integral:  " << variable << "   " << dataCentral->Integral(1, nBins) << " with under/over:  " << dataCentral->Integral(0, nBins+1) << endl;
    cout << " sherpa integral:  " << variable << "   " << genShe->Integral(1, nBins) << " with under/over:  " << dataCentral->Integral(0, nBins+1) << endl;
    
    cout << "nSyst" << nSyst << endl;
    for (int syst(0); syst < nSyst; syst++) {
        double tempScale = hSyst[syst]->Integral(1, nBins) ;
        cout << " original integral for " << syst << " : " << tempScale << endl;
    }
    
    // cross section or normalized
    bool doXSec(1);
    bool doNormalize(0);
    if (doXSec) doNormalize = false;
    if (doNormalize) doXSec = false;
    
    if (doXSec && !doNormalize ) {
        for (int i(1); i <= nBins; i++){
            double binW = data->GetBinWidth(i);
            data->SetBinContent(i, data->GetBinContent(i)*1./(Luminosity*binW));
            dataCentral->SetBinContent(i, dataCentral->GetBinContent(i)*1./(Luminosity*binW));
            genMad->SetBinContent(i, genMad->GetBinContent(i)*1./(Luminosity*binW));
            genShe->SetBinContent(i, genShe->GetBinContent(i)*1./(Luminosity*binW));
            
            data->SetBinError(i, data->GetBinError(i)*1./(Luminosity*binW));
            dataCentral->SetBinError(i, dataCentral->GetBinError(i)*1./(Luminosity*binW));
            genMad->SetBinError(i, genMad->GetBinError(i)*1./(Luminosity*binW));
            genShe->SetBinError(i, genShe->GetBinError(i)*1./(Luminosity*binW));
        }
        for (int syst(0); syst < nSyst; syst++){
            for (int i(1); i <= nBins; i++){
                double binW = data->GetBinWidth(i);
                hSyst[syst]->SetBinContent(i, hSyst[syst]->GetBinContent(i)*1./(Luminosity*binW));
                hSyst[syst]->SetBinError(i, hSyst[syst]->GetBinError(i)*1./(Luminosity*binW));
            }
        }
    }
    if (doNormalize) {
        data->Scale(1/data->Integral(1,nBins));
        dataCentral->Scale(1/dataCentral->Integral(1,nBins));
        genMad->Scale(1/genMad->Integral(1,nBins));
        for (int syst(0); syst < nSyst; syst++){
            double tempScale = hSyst[syst]->Integral(1,nBins) ;
            cout << " scale for syst " << syst << " : " << tempScale << endl;
            hSyst[syst]->Scale(1/tempScale);
        }
    }
    
    
    //--- systematic error computation --------------------------
    cout << "go to uncertainty computation " << endl;
    double xCoor[nBins], yCoor[nBins], xErr[nBins], ySystDown[nBins], ySystUp[nBins];
    for (int bin(1); bin <= nBins; bin++) {
        
        double centralValue (dataCentral->GetBinContent(bin));
        double totalStatistics (dataCentral->GetBinError(bin));
        double totalSystematicsUp (0.);
        double totalSystematicsDown (0.);
        
        // set effSF = 0. So it has no contribution.
        double effSF = 0.00;
        totalSystematicsUp += pow( pow((1.+effSF), 2) - 1 , 2);
        totalSystematicsDown += pow( pow((1.+effSF), 2) - 1 , 2);
        
        for(int i = 0; i < 12; i++) {
            // contribution from JES PU Xsec JER LepSF Btag MES
            if (i <= 6) {
                double diffFromUp(hSyst[2*i]->GetBinContent(bin) - centralValue);
                double diffFromDown(hSyst[(2*i)+1]->GetBinContent(bin) - centralValue);
                double SysDiffUPTemp(0);
                double SysDiffDownTemp(0);
                
                if (diffFromUp >= diffFromDown) {
                    SysDiffUPTemp = diffFromUp;
                    SysDiffDownTemp = diffFromDown;
                }
                else {
                    SysDiffUPTemp = diffFromDown;
                    SysDiffDownTemp = diffFromUp;
                }
                
                if (SysDiffUPTemp >= 0) {
                    totalSystematicsUp += SysDiffUPTemp * SysDiffUPTemp;
                }
                if (SysDiffDownTemp <= 0) {
                    totalSystematicsDown += SysDiffDownTemp * SysDiffDownTemp;
                }
            }
            // contribution from MER hSyst[14] = (TH1D*) f->Get("MER");
            if (i == 7) {
                double difference(hSyst[14]->GetBinContent(bin) - centralValue);
                totalSystematicsUp += difference * difference;
                totalSystematicsDown += difference * difference;
            }
            // contribution from Wb hSyst[15] = (TH1D*) f->Get("WB");
            if (i == 8) {
                double difference(hSyst[15]->GetBinContent(bin) - centralValue);
                totalSystematicsUp += difference * difference;
                totalSystematicsDown += difference * difference;
            }
            // contribution from ttbar hSyst[16] = (TH1D*) f->Get("TTBAR");
            if (i == 9) {
                double difference(hSyst[16]->GetBinContent(bin) - centralValue);
                totalSystematicsUp += difference * difference;
                totalSystematicsDown += difference * difference;
            }
            // contribution from MC hSyst[17] = (TH1D*) f->Get("MC");
            if (i == 10) {
                double difference(hSyst[17]->GetBinContent(bin) - centralValue);
                totalSystematicsUp += difference * difference;
                totalSystematicsDown += difference * difference;
            }
            // contribution from Resp hSyst[18] = (TH1D*) f->Get("RESP");
            if (i == 11) {
                if (variable.find("ZNGoodJets") == string::npos){
                    double difference(hSyst[18]->GetBinContent(bin) - centralValue);
                    totalSystematicsUp += difference * difference;
                    totalSystematicsDown += difference * difference;
                }
            }
        }
        
        // contribution from integrated lumi = 2.6%
        totalSystematicsUp += pow( ((2.6/100.0) * centralValue), 2 );
        totalSystematicsDown += pow( ((2.6/100.0) * centralValue), 2 );
        
        
        //--- Set up for systematic plot on pad 1
        // set x-coordinate and error in x
        xCoor[bin-1]    = dataCentral->GetBinCenter(bin);
        xErr[bin-1]     = 0.5 * dataCentral->GetBinWidth(bin);
        // set y-coordinate
        yCoor[bin-1]    = centralValue;
        // set systematics errors in y => combine both statistical and systematics
        ySystUp[bin-1]     = sqrt(totalStatistics * totalStatistics + totalSystematicsUp );
        ySystDown[bin-1]   = sqrt(totalStatistics * totalStatistics + totalSystematicsDown );
    }
    cout << " finished uncertainty computation " << endl;
    cout << "line th " << __LINE__ << endl;
    
    /*
    //===========================================================
    //--- plot the inclusive jet multiplicity
    if (variable.find("ZNGoodJets") != string::npos){
        cout << " producing inclusive jet multiplicity plot" << endl;
        plotXsecIncJetMultipli(variable, dataCentral, genMad, genBhatALL, genShe, nBins, xCoor, yCoor, xErr, ySystDown, ySystUp);
    }
    //---
    //===========================================================
    */
    
    //--- Set up for systematic plot on pad 1
    TGraphAsymmErrors *grCentralSyst = new TGraphAsymmErrors(nBins, xCoor, yCoor, xErr, xErr, ySystDown, ySystUp);
    
    //--- plotting ---
    TH1D *hisUnfolded = (TH1D*) dataCentral->Clone();
    
    TCanvas *can = new TCanvas(variable.c_str(), variable.c_str(), 600, 800);
    can->cd();
    //--- Set pad1.
    TPad *pad1 = new TPad("pad1","pad1", 0.01, 0.45, 0.99, 0.99);
    pad1->SetTopMargin(0.11);
    pad1->SetBottomMargin(0);
    pad1->SetRightMargin(0.04);
    pad1->SetLeftMargin(0.11);
    pad1->SetTicks();
    pad1->Draw();
    pad1->cd();
    pad1->SetLogy();
    
    // set y axis title
    string temp;
    string xtitle = genMad->GetXaxis()->GetTitle();
    string shortVar = xtitle.substr(0, xtitle.find(" "));
    
    string unit = "";
    if (xtitle.find("[") != string::npos) {
        size_t begin = xtitle.find("[") + 1;
        unit = xtitle.substr(begin);
        unit = unit.substr(0, unit.find("]"));
    }
    //--- temporary fix label: FIXME
    if (variable.find("dRapidity") != string::npos) shortVar = "#Deltay";
    if (variable.find("dRJets") != string::npos)    shortVar = "#DeltaR";
    if (variable.find("diJetPt") != string::npos)    shortVar = "p_{T}";
    //---
    cout << shortVar << "   unit: " << unit << endl;
    temp = "d#sigma / d" + shortVar;
    temp += "  [pb";
    if (unit != "" ) temp += " / " + unit;
    temp += "]";
    //-------
    
    double MineYMax = genMad->GetMaximum();
    //double MineYMin = genMad->GetMinimum();
    if (hisUnfolded->GetMaximum() >= MineYMax) MineYMax = hisUnfolded->GetMaximum();
    //if (hisUnfolded->GetMinimum() <= MineYMin) MineYMin = hisUnfolded->GetMinimum();
    
    hisUnfolded->SetTitle("");
    if (variable.find("ZNGoodJets") != string::npos) {
        hisUnfolded->GetXaxis()->SetRange(1, 8);
    }
    hisUnfolded->GetYaxis()->SetTitle(temp.c_str());
    hisUnfolded->GetYaxis()->SetTitleSize(0.04);
    hisUnfolded->GetYaxis()->SetTitleOffset(1.35);
    hisUnfolded->GetYaxis()->SetTitleFont(42);
    //hisUnfolded->SetMinimum(MineYMin*0.8);
    if ( (variable.find("dRapidity") != string::npos) || (variable.find("dRJets") != string::npos) ){
        hisUnfolded->SetMaximum(MineYMax*10); // remove or modify this when appropriate
    }
    
    hisUnfolded->SetFillStyle(3354);
    hisUnfolded->SetFillColor(12);
    hisUnfolded->SetLineColor(kBlack);
    hisUnfolded->SetLineWidth(2);
    hisUnfolded->SetMarkerColor(kBlack);
    hisUnfolded->SetMarkerStyle(20);
    hisUnfolded->SetMarkerSize(0.8);
    hisUnfolded->Draw("E1");
    
    grCentralSyst->SetFillStyle(3354);
    grCentralSyst->SetFillColor(12);
    grCentralSyst->Draw("2");
    
    genMad->SetFillStyle(ZJetsFillStyle);
    genMad->SetFillColor(kBlue-10);
    //genMad->SetFillColorAlpha(kBlue-10, 0.60);
    genMad->SetLineColor(kBlue);
    genMad->SetLineWidth(2);
    genMad->SetMarkerColor(kBlue);
    genMad->SetMarkerStyle(24);
    //genMad->Draw("E2 same");
    genMad->Draw("E same");
    
    /*
    genBhatALL->SetFillStyle(ZJetsFillStyle);
    genBhatALL->SetFillColor(kOrange-2);
    //genBhatALL->SetFillColorAlpha(kOrange-2, 0.60);
    genBhatALL->SetLineColor(kOrange+10);
    genBhatALL->SetLineWidth(2);
    genBhatALL->SetMarkerColor(kOrange+10);
    genBhatALL->SetMarkerStyle(25);
    //genBhatALL->Draw("E2 same");
    genBhatALL->Draw("E same");
     */
    
    genShe->SetFillStyle(ZJetsFillStyle);
    genShe->SetFillColor(kGreen-8);
    //genShe->SetFillColorAlpha(kGreen-8, 0.60);
    genShe->SetLineColor(kGreen+3);
    genShe->SetLineWidth(2);
    genShe->SetMarkerColor(kGreen+3);
    genShe->SetMarkerStyle(26);
    //genShe->Draw("E2 same");
    genShe->Draw("E same");
    
    pad1->Draw();
    //--- TLegend ---
    TLegend *legend = new TLegend(0.47, 0.74, 0.99, 0.98);
    legend->SetX1(0.44);
    legend->SetY1(0.77);
    legend->SetX2(0.95);
    legend->SetY2(0.98);
    legend->SetTextSize(.034);
    legend->SetFillColor(0);
    legend->SetFillStyle(1001);
    legend->SetBorderSize(1);
    legend->AddEntry(hisUnfolded, "Data", "PLEF");
    legend->AddEntry(genMad, "MG5 + PY6 (#leq 4j LO + PS)", "plef");
    //legend->AddEntry(genBhatALL, "BLACKHAT+SHERPA (NLO)", "plef");
    legend->AddEntry(genShe, "SHERPA 2 (#leq 2j NLO 3,4j LO + PS)", "plef");
    legend->Draw();
    //------------------
    
    //--- TLatex stuff ---
    TLatex *latexLabel = new TLatex();
    latexLabel->SetNDC();
    latexLabel->SetTextSize(0.05);
    latexLabel->SetTextFont(42);
    latexLabel->SetLineWidth(2);
    latexLabel->SetTextFont(61);
    latexLabel->DrawLatex(0.13,0.95,"CMS");
    latexLabel->SetTextFont(52);
    latexLabel->DrawLatex(0.20,0.95,"Preliminary");
    latexLabel->SetTextFont(42);
    latexLabel->DrawLatex(0.13,0.95-0.045,"19.6 fb^{-1} (8 TeV)");
    latexLabel->DrawLatex(0.18,0.21-0.05,"anti-k_{T} (R = 0.5) Jets");
    latexLabel->DrawLatex(0.18,0.21-0.11,"p_{T}^{jet} > 30 GeV, |#eta^{jet}| < 2.4 ");
    latexLabel->DrawLatex(0.18,0.21-0.17,"W#rightarrow #mu#nu channel");
    latexLabel->SetName("latexLabel");
    latexLabel->Draw("same");
    
    
    //--- calculate ratio >> pad2, pad3
    
    // for data statistical uncer
    TH1D  *data1 = (TH1D*) hisUnfolded->Clone();
    for(int i = 1; i <= hisUnfolded->GetNbinsX(); i++) {
        data1->SetBinContent(i, 1.0);
        if (hisUnfolded->GetBinContent(i) > 0.) {
            data1->SetBinError(i, hisUnfolded->GetBinError(i)/hisUnfolded->GetBinContent(i));
        }
        else data1->SetBinError(i, 0.);
        
    }
    TH1D  *data2 = (TH1D*) data1->Clone();
    TH1D  *data3 = (TH1D*) data1->Clone();
    
    // for gen statistical uncer
    TH1D  *gen1 = (TH1D*) genMad->Clone();
    for(int i = 1; i <= hisUnfolded->GetNbinsX(); i++) {
        if (hisUnfolded->GetBinContent(i) > 0.) {
            gen1->SetBinContent(i, genMad->GetBinContent(i)/hisUnfolded->GetBinContent(i));
            gen1->SetBinError(i, genMad->GetBinError(i)/hisUnfolded->GetBinContent(i));
        }
        else {
            gen1->SetBinContent(i, 0.);
            gen1->SetBinError(i, 0.);
        }
    }
    /*
    TH1D  *gen2 = (TH1D*) hisUnfolded->Clone();
    for(int i = 1; i <= hisUnfolded->GetNbinsX(); i++) {
        if (hisUnfolded->GetBinContent(i) > 0.) {
            gen2->SetBinContent(i, genBhatALL->GetBinContent(i)/hisUnfolded->GetBinContent(i));
            gen2->SetBinError(i, genBhatALL->GetBinError(i)/hisUnfolded->GetBinContent(i));
        }
        else {
            gen2->SetBinContent(i, 0.);
            gen2->SetBinError(i, 0.);
        }
    }
    */
    
    TH1D  *gen3 = (TH1D*) hisUnfolded->Clone();
    for(int i = 1; i <= hisUnfolded->GetNbinsX(); i++) {
        if (hisUnfolded->GetBinContent(i) > 0.) {
            gen3->SetBinContent(i, genShe->GetBinContent(i)/hisUnfolded->GetBinContent(i));
            gen3->SetBinError(i, genShe->GetBinError(i)/hisUnfolded->GetBinContent(i));
        }
        else {
            gen3->SetBinContent(i, 0.);
            gen3->SetBinError(i, 0.);
        }
        //cout << " sherpa " << gen3->GetBinContent(i) << endl;
    }
    
    //--- Set up for systematic plot on pad 1
    double xCoorCenRatio[nBins], yCoorCenRatio[nBins], xErrCenRatio[nBins], ySystDownCenRatio[nBins], ySystUpCenRatio[nBins];
    for (int bin(1); bin <= nBins; bin++) {
        // set x-coordinate and error in x
        xCoorCenRatio[bin-1] = hisUnfolded->GetBinCenter(bin);
        xErrCenRatio[bin-1]  = 0.5 * hisUnfolded->GetBinWidth(bin);
        // set y-coordinate
        yCoorCenRatio[bin-1] = 1.;
        // set systematics errors in y => combine both statistical and systematics
        if (hisUnfolded->GetBinContent(bin) > 0) {
            ySystUpCenRatio[bin-1]     = ySystUp[bin-1]/hisUnfolded->GetBinContent(bin);
            ySystDownCenRatio[bin-1]   = ySystDown[bin-1]/hisUnfolded->GetBinContent(bin);
        }
        else {
            ySystUpCenRatio[bin-1] = 0.;
            ySystDownCenRatio[bin-1] = 0.;
        }
        
    }
    TGraphAsymmErrors *grCentralSystRatio = new TGraphAsymmErrors(nBins, xCoorCenRatio, yCoorCenRatio, xErrCenRatio, xErrCenRatio, ySystDownCenRatio, ySystUpCenRatio);
    
    
    //--- NEW TPAD => pad2 ---//
    can->cd();
    TPad *pad2 = new TPad("pad2","pad2", 0.01, 0.27, 0.99, 0.45);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0);
    pad2->SetRightMargin(0.04);
    pad2->SetLeftMargin(0.11);
    pad2->SetGridy();
    pad2->SetTicks();
    pad2->Draw();
    pad2->cd();
    
    data1->SetTitle("");
    data1->GetYaxis()->SetRangeUser(0.41,1.59);
    data1->GetYaxis()->SetTitle("MG5/Data");
    data1->GetYaxis()->SetTitleSize(0.1425);
    data1->GetYaxis()->SetTitleOffset(0.4);
    data1->GetYaxis()->CenterTitle();
    data1->GetYaxis()->SetLabelSize(0.12);
    data1->GetYaxis()->SetLabelOffset(0.01);
    
    data1->GetXaxis()->SetTickLength(0.06);
    
    data1->SetLineColor(kBlack);
    data1->SetLineWidth(1);
    data1->SetMarkerColor();
    data1->SetMarkerStyle();
    data1->Draw("E1");
    
    gen1->SetFillStyle(ZJetsFillStyle);
    gen1->SetFillColor(kBlue-10);
    gen1->SetLineColor(kBlue);
    gen1->SetLineWidth(2);
    gen1->SetMarkerColor(kBlue);
    gen1->SetMarkerStyle(24);
    gen1->Draw("E2 same");
    gen1->Draw("E1 same");
    
    grCentralSystRatio->SetFillStyle(3354);
    grCentralSystRatio->SetFillColor(12);
    grCentralSystRatio->Draw("2");
    
    TLegend *legend2 = new TLegend(0.16, 0.05, 0.33, 0.20);
    legend2->SetFillColor(0);
    legend2->SetFillStyle(ZJetsFillStyle);
    legend2->SetBorderSize(0);
    //legend2->SetTextSize(.12);
    legend2->SetY1(0.07);
    legend2->SetX1(0.15);
    legend2->SetY2(0.20);
    legend2->SetTextSize(0.09);
    TLegendEntry *leEntry2;
    leEntry2 = legend2->AddEntry(gen1, "Stat. unc. (gen)", "f");
    legend2->Draw("same");
    
    // Text Madgraph
    /*
    TLatex *latexLabel0 = new TLatex();
    latexLabel0->SetTextSize(0.15);
    latexLabel0->SetTextFont(42);
    latexLabel0->SetLineWidth(2);
    latexLabel0->SetTextColor(kAzure-5);
    latexLabel0->SetNDC();
    latexLabel0->DrawLatex(0.40,0.09,"Madgraph");
    */
    pad2->Draw();
    
    
    //--- NEW TPAD => pad3 ---//
    /*
    can->cd();
    TPad *pad3 = new TPad("pad3","pad3", 0.01, 0.23, 0.99, 0.39);
    pad3->SetTopMargin(0);
    pad3->SetBottomMargin(0);
    pad3->SetRightMargin(0.04);
    pad3->SetLeftMargin(0.11);
    pad3->SetGridy();
    pad3->SetTicks();
    pad3->Draw();
    pad3->cd();
    
    data2->SetTitle("");
    data2->GetYaxis()->SetRangeUser(0.41,1.59);
    data2->GetYaxis()->SetTitle("MC / Data");
    data2->GetYaxis()->SetTitleSize(0.135);
    data2->GetYaxis()->SetTitleOffset(0.44);
    data2->GetYaxis()->CenterTitle();
    data2->GetYaxis()->SetLabelSize(0.12);
    data2->GetYaxis()->SetLabelOffset(0.01);
    
    data2->GetXaxis()->SetTickLength(0.06);
    
    data2->SetLineColor(kBlack);
    data2->SetLineWidth(1);
    data2->SetMarkerColor();
    data2->SetMarkerStyle();
    data2->Draw("E1");
    
    
    gen2->SetFillStyle(ZJetsFillStyle);
    gen2->SetFillColor(kOrange-2);
    gen2->SetLineColor(kOrange+10);
    gen2->SetLineWidth(2);
    gen2->SetMarkerColor(kOrange+10);
    gen2->SetMarkerStyle(25);
    gen2->Draw("E2 same");
    gen2->Draw("E1 same");
    
    
    grCentralSystRatio->SetFillStyle(3354);
    grCentralSystRatio->SetFillColor(12);
    grCentralSystRatio->Draw("2");
    
    TLegend *legend3 = new TLegend(0.16, 0.05, 0.33, 0.20);
    legend3->SetFillColor(0);
    legend3->SetFillStyle(ZJetsFillStyle);
    legend3->SetBorderSize(0);
    //legend3->SetTextSize(.12);
    legend3->SetY1(0.07);
    legend3->SetX1(0.15);
    legend3->SetY2(0.20);
    legend3->SetTextSize(0.09);
    TLegendEntry *leEntry3;
    //leEntry3 = legend3->AddEntry(gen2, "Stat. unc. (gen)", "f");
    //legend3->Draw("same");
    
    // Text BlackHat
    
    TLatex *latexLabel1 = new TLatex();
    latexLabel1->SetTextSize(0.15);
    latexLabel1->SetTextFont(42);
    latexLabel1->SetLineWidth(2);
    latexLabel1->SetTextColor(kOrange+2);
    latexLabel1->SetNDC();
    latexLabel1->DrawLatex(0.35,0.09,"BlackHat");
    
    pad3->Draw();
    */
    
    //--- NEW TPAD => pad4 ---//
    can->cd();
    TPad *pad4 = new TPad("pad3","pad3", 0.01, 0.01, 0.99, 0.27);
    pad4->SetTopMargin(0);
    pad4->SetBottomMargin(0.25);
    pad4->SetRightMargin(0.04);
    pad4->SetLeftMargin(0.11);
    pad4->SetGridy();
    pad4->SetTicks();
    pad4->Draw();
    pad4->cd();
    
    data3->SetTitle("");
    data3->GetYaxis()->SetRangeUser(0.41,1.59);
    data3->GetYaxis()->SetTitle("SHERPA2/Data");
    data3->GetYaxis()->SetTitleSize(0.096);
    data3->GetYaxis()->SetTitleOffset(0.6);
    data3->GetYaxis()->CenterTitle();
    data3->GetYaxis()->SetLabelSize(0.085);
    data3->GetYaxis()->SetLabelOffset(0.012);
    
    data3->GetXaxis()->SetTitle(gen1->GetXaxis()->GetTitle());
    data3->GetXaxis()->SetTitleSize(0.1);
    data3->GetXaxis()->SetTitleOffset(1.05);
    data3->GetXaxis()->SetLabelSize(0.09);
    data3->GetXaxis()->SetLabelOffset(0.014);
    data3->GetXaxis()->SetTickLength(0.06);
    
    data3->SetLineColor(kBlack);
    data3->SetLineWidth(1);
    data3->SetMarkerColor();
    data3->SetMarkerStyle();
    data3->Draw("E1");
    
    gen3->SetFillStyle(ZJetsFillStyle);
    gen3->SetFillColor(kGreen-8);
    gen3->SetLineColor(kGreen+3);
    gen3->SetLineWidth(2);
    gen3->SetMarkerColor(kGreen+3);
    gen3->SetMarkerStyle(26);
    gen3->Draw("E2 same");
    gen3->Draw("E1 same");
    
    grCentralSystRatio->SetFillStyle(3354);
    grCentralSystRatio->SetFillColor(12);
    grCentralSystRatio->Draw("2");
    
    TLegend *legend4 = new TLegend(0.16, 0.05, 0.33, 0.20);
    legend4->SetFillColor(0);
    legend4->SetFillStyle(ZJetsFillStyle);
    legend4->SetBorderSize(0);
    //legend4->SetTextSize(.12);
    legend4->SetY1(0.32);
    legend4->SetX1(0.15);
    legend4->SetY2(0.42);
    legend4->SetTextSize(0.065);
    TLegendEntry *leEntry4;
    leEntry4 = legend4->AddEntry(gen3, "Stat. unc. (gen)", "f");
    legend4->Draw("same");
    
    // Text Sherpa (position not relative to pad4)
    /*
    TLatex *latexLabel2 = new TLatex();
    latexLabel2->SetTextSize(0.11);
    latexLabel2->SetTextFont(42);
    latexLabel2->SetLineWidth(2);
    latexLabel2->SetTextColor(kGreen+3);
    latexLabel2->SetNDC();
    latexLabel2->DrawLatex(0.40,0.34,"Sherpa");
    */
    pad4->Draw();
    can->cd();
    
    //// Set output .pdf file in the direcctory "PNGFiles/FinalResults/"
    string command = "mkdir -p PNGFiles/FinalResults" ;
    system(command.c_str());
    string outputDirectory = "PNGFiles/FinalResults/";
    string outputFileNamePNG = outputDirectory + leptonFlavor + "_" + energy + "_RunPlotting_";
    
    if (doXSec) outputFileNamePNG += "XSec_";
    else if (doNormalize) outputFileNamePNG += "Normalized_";
    
    outputFileNamePNG += variable + "_" + unfAlg ;
    if (doVarWidth)  outputFileNamePNG += "_VarWidth";
    outputFileNamePNG += ".pdf";
    can->Print(outputFileNamePNG.c_str());
    
    
    //--- Generate new plot to see Contribution : all input histo are already scaled to the final unit ---
    //plotSystematicBreakdown(outputDirectory, variable, dataCentral, hSyst);
    plotSystematicBreakdownCombine(outputDirectory, variable, dataCentral, hSyst);
    
}


void plotSystematicBreakdown (string outputDirectory, string variable, TH1D* dataCentral, TH1D* hSyst[])
{
    outputDirectory = "PNGFiles/SystBreakDown/";
    string command = "mkdir -p PNGFiles/SystBreakDown" ;
    system(command.c_str());
    
    TH1D *dataCenBackUp = (TH1D*) dataCentral->Clone();
    
    const int nGroup = 13;
    const int nBins(dataCentral->GetNbinsX());
    
    int group = nGroup -1;
    if (variable.find("ZNGoodJets") == string::npos) {
        group = nGroup;
    }
    
    TH1D *hUpDiffer[nGroup] = {NULL}, *hDownDiffer[nGroup] = {NULL}, *hStatError = {NULL}, *hTotalErrorUp = {NULL}, *hTotalErrorDown = {NULL} ; //nSyst(7);
    
    // set up histogram
    hStatError =  (TH1D*) dataCentral->Clone();
    hStatError->SetTitle("");
    hStatError->GetYaxis()->SetTitle("");
    
    hTotalErrorUp =  (TH1D*) dataCentral->Clone();
    hTotalErrorUp->SetTitle("");
    hTotalErrorUp->GetYaxis()->SetTitle("");
    
    hTotalErrorDown =  (TH1D*) dataCentral->Clone();
    hTotalErrorDown->SetTitle("");
    hTotalErrorDown->GetYaxis()->SetTitle("");
    
    // Initialize hUpDiffer hDownDiffer
    for (int syst(0); syst < group; syst++){
        hUpDiffer[syst] = (TH1D*) dataCentral->Clone();
        hDownDiffer[syst] = (TH1D*) dataCentral->Clone();
        
        hUpDiffer[syst]->SetTitle("");
        hUpDiffer[syst]->GetYaxis()->SetTitle("");
        hDownDiffer[syst]->SetTitle("");
        hDownDiffer[syst]->GetYaxis()->SetTitle("");
    }
    
    // Assign Value
    for (int bin(1); bin <= nBins; bin++){
        for(int i = 0; i < group; i++){
            
            hUpDiffer[i]->SetBinContent(bin, 0.);
            hDownDiffer[i]->SetBinContent(bin, 0.);
            
            if( dataCentral->GetBinContent(bin) > 0 ){
                // contribution from JES PU Xsec JER LepSF Btag MES
                if (i <= 6){
                    double SysDiffUPTemp(0);
                    double SysDiffDownTemp(0);
                    
                    double diffFromUp( hSyst[2*i]->GetBinContent(bin) - (dataCentral->GetBinContent(bin)) );
                    double diffFromDown( hSyst[(2*i)+1]->GetBinContent(bin) - (dataCentral->GetBinContent(bin)) );
                    
                    
                    if (diffFromUp >= diffFromDown) {
                        SysDiffUPTemp = diffFromUp;
                        SysDiffDownTemp = diffFromDown;
                    }
                    else{
                        SysDiffUPTemp = diffFromDown;
                        SysDiffDownTemp = diffFromUp;
                    }
                    
                    if (SysDiffUPTemp >= 0){
                        hUpDiffer[i]->SetBinContent( bin, SysDiffUPTemp * (100. / dataCentral->GetBinContent(bin)) ) ;
                    }
                    if (SysDiffDownTemp <= 0){
                        hDownDiffer[i]->SetBinContent( bin, (fabs(SysDiffDownTemp)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                    }
                }
                // contribution from MER
                if (i == 7){
                    double difference( hSyst[14]->GetBinContent(bin) - (dataCentral->GetBinContent(bin)) );
                    hUpDiffer[i]  ->SetBinContent( bin, (fabs(difference)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                    hDownDiffer[i]->SetBinContent( bin, (fabs(difference)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                }
                // contribution from Wb
                if (i == 8){
                    double difference( hSyst[15]->GetBinContent(bin) - (dataCentral->GetBinContent(bin)) );
                    hUpDiffer[i]  ->SetBinContent( bin, (fabs(difference)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                    hDownDiffer[i]->SetBinContent( bin, (fabs(difference)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                }
                // contribution from ttbar
                if (i == 9){
                    double difference( hSyst[16]->GetBinContent(bin) - (dataCentral->GetBinContent(bin)) );
                    hUpDiffer[i]  ->SetBinContent( bin, (fabs(difference)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                    hDownDiffer[i]->SetBinContent( bin, (fabs(difference)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                }
                // contribution from MC
                if (i == 10){
                    double difference( hSyst[17]->GetBinContent(bin) - (dataCentral->GetBinContent(bin)) );
                    hUpDiffer[i]  ->SetBinContent( bin, (fabs(difference)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                    hDownDiffer[i]->SetBinContent( bin, (fabs(difference)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                }
                // contribution from integrated lumi
                if (i == 11){
                    hUpDiffer[i]  ->SetBinContent( bin, 2.6 ) ;
                    hDownDiffer[i]->SetBinContent( bin, 2.6 ) ;
                }
                // contribution from Resp
                if (i == nGroup-1){
                    double difference( hSyst[18]->GetBinContent(bin) - (dataCentral->GetBinContent(bin)) );
                    hUpDiffer[i]  ->SetBinContent( bin, (fabs(difference)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                    hDownDiffer[i]->SetBinContent( bin, (fabs(difference)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                    //double uncer(hSyst[18]->GetBinContent(bin));
                    //hUpDiffer[i]  ->SetBinContent( bin, (fabs(uncer)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                    //hDownDiffer[i]->SetBinContent( bin, (fabs(uncer)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                }
            }
        }
        
        hStatError->SetBinContent(bin, 0.);
        if (dataCentral->GetBinContent(bin) > 0){
            hStatError->SetBinContent( bin, (dataCentral->GetBinError(bin)) * (100. / dataCentral->GetBinContent(bin)) ) ;
            //cout << " Binth: " << bin << " Stat  difference = " << dataCentral->GetBinError(bin)  << "  % difference = " << (dataCentral->GetBinError(bin)) * (100 / dataCentral->GetBinContent(bin))  << endl;
        }
        //cout << endl;
    }
    
    // sum all
    for (int bin(1); bin <= nBins; bin++){
        hTotalErrorUp->SetBinContent(bin, 0.);
        hTotalErrorDown->SetBinContent(bin, 0.);
        
        double sumErrorUp(0.);
        double sumErrorDown(0.);
        for (int syst(0); syst < group; syst++){
            sumErrorUp += pow(hUpDiffer[syst]->GetBinContent(bin), 2);
            sumErrorDown += pow(hDownDiffer[syst]->GetBinContent(bin), 2);
        }
        sumErrorUp += pow(hStatError->GetBinContent(bin), 2);
        sumErrorDown += pow(hStatError->GetBinContent(bin), 2);
        sumErrorUp = sqrt(sumErrorUp);
        sumErrorDown = sqrt(sumErrorDown);
        
        hTotalErrorUp->SetBinContent(bin, sumErrorUp) ;
        hTotalErrorDown->SetBinContent(bin, sumErrorDown) ;
    }
    
    // canvas1
    TCanvas* canvas1 = new TCanvas("c1", "canvas1", 900, 900);
    canvas1->cd();
    canvas1->SetLogy();
    canvas1->SetTicks();
    
    string titleCan1 = "Systematic Up : " + variable;
    
    if (variable.find("ZNGoodJets") != string::npos){
        hUpDiffer[0]->GetXaxis()->SetRange(1, 8);
    }
    hUpDiffer[0]->SetMaximum(1000);
    hUpDiffer[0]->SetMinimum(0.1);
    //hUpDiffer[0]->SetTitle(titleCan1.c_str());
    hUpDiffer[0]->SetTitle("");
    hUpDiffer[0]->GetYaxis()->SetTitle("Uncertainty (%)");
    
    //JES
    hUpDiffer[0]->SetLineColor(kRed);
    hUpDiffer[0]->SetLineWidth(2);
    hUpDiffer[0]->SetLineStyle(2);
    hUpDiffer[0]->DrawCopy("HIST");
    //PU
    hUpDiffer[1]->SetLineColor(kBlue);
    hUpDiffer[1]->SetLineWidth(2);
    hUpDiffer[1]->SetLineStyle(5);
    hUpDiffer[1]->DrawCopy("HIST same");
    //Xsec
    hUpDiffer[2]->SetLineColor(kGreen);
    hUpDiffer[2]->SetLineWidth(2);
    hUpDiffer[2]->SetLineStyle(5);
    hUpDiffer[2]->DrawCopy("HIST same");
    //JER
    hUpDiffer[3]->SetLineColor(kGray+2);
    hUpDiffer[3]->SetLineWidth(3);
    hUpDiffer[3]->SetLineStyle(3);
    hUpDiffer[3]->DrawCopy("HIST same");
    //LepSF
    hUpDiffer[4]->SetLineColor(kPink+5);
    hUpDiffer[4]->SetLineWidth(2);
    hUpDiffer[4]->SetLineStyle(2);
    hUpDiffer[4]->DrawCopy("HIST same");
    //BtagSF
    hUpDiffer[5]->SetLineColor(kPink+2);
    hUpDiffer[5]->SetLineWidth(2);
    hUpDiffer[5]->SetLineStyle(5);
    hUpDiffer[5]->DrawCopy("HIST same");
    //MES
    hUpDiffer[6]->SetLineColor(kSpring+9);
    hUpDiffer[6]->SetLineWidth(2);
    hUpDiffer[6]->SetLineStyle(3);
    hUpDiffer[6]->DrawCopy("HIST same");
    //MER
    hUpDiffer[7]->SetLineColor(kAzure+4);
    hUpDiffer[7]->SetLineWidth(2);
    hUpDiffer[7]->SetLineStyle(2);
    hUpDiffer[7]->DrawCopy("HIST same");
    //WB
    hUpDiffer[8]->SetLineColor(kOrange);
    hUpDiffer[8]->SetLineWidth(3);
    hUpDiffer[8]->SetLineStyle(3);
    hUpDiffer[8]->DrawCopy("HIST same");
    //ttbar
    hUpDiffer[9]->SetLineColor(kGreen+3);
    hUpDiffer[9]->SetLineWidth(2);
    hUpDiffer[9]->SetLineStyle(5);
    hUpDiffer[9]->DrawCopy("HIST same");
    //MC
    hUpDiffer[10]->SetLineColor(kCyan);
    hUpDiffer[10]->SetLineWidth(3);
    hUpDiffer[10]->SetLineStyle(3);
    hUpDiffer[10]->DrawCopy("HIST same");
    //lumi
    hUpDiffer[11]->SetLineColor(kViolet+5);
    hUpDiffer[11]->SetLineWidth(2);
    hUpDiffer[11]->SetLineStyle(5);
    hUpDiffer[11]->DrawCopy("HIST same");
    //Resp
    if (variable.find("ZNGoodJets") == string::npos) {
        hUpDiffer[nGroup-1]->SetLineColor(kYellow);
        hUpDiffer[nGroup-1]->SetLineWidth(3);
        hUpDiffer[nGroup-1]->SetLineStyle(3);
        hUpDiffer[nGroup-1]->DrawCopy("HIST same");
    }
    
    hStatError->SetLineColor(kMagenta);
    hStatError->SetLineWidth(2);
    hStatError->SetLineStyle(2);
    hStatError->DrawCopy("HIST same");
    
    hTotalErrorUp->SetLineColor(kBlack);
    hTotalErrorUp->SetLineWidth(2);
    hTotalErrorUp->SetLineStyle(1);
    hTotalErrorUp->DrawCopy("HIST same");
    
    TLegend *leg = new TLegend(0.15, 0.63, 0.34, 0.89);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->AddEntry(hUpDiffer[0]  , "JES"    , "l");
    leg->AddEntry(hUpDiffer[1]  , "PU"     , "l");
    leg->AddEntry(hUpDiffer[2]  , "XSEC"   , "l");
    leg->AddEntry(hUpDiffer[3]  , "JER"    , "l");
    leg->AddEntry(hUpDiffer[4]  , "LepSF"  , "l");
    leg->AddEntry(hUpDiffer[5]  , "BtagSF" , "l");
    leg->AddEntry(hUpDiffer[6]  , "MES"    , "l");
    leg->AddEntry(hUpDiffer[7]  , "MER"    , "l");
    leg->AddEntry(hUpDiffer[8]  , "Wb"     , "l");
    leg->AddEntry(hUpDiffer[9]  , "ttbar"  , "l");
    leg->AddEntry(hUpDiffer[10] , "MC"     , "l");
    leg->AddEntry(hUpDiffer[11] , "Lumi"   , "l");
    if (variable.find("ZNGoodJets") == string::npos) {
        leg->AddEntry(hUpDiffer[nGroup-1]  , "Resp", "l");
    }
    leg->AddEntry(hStatError    , "Statistical", "l");
    leg->AddEntry(hTotalErrorUp , "Total"      , "l");
    leg->Draw();
    
    string outputFileNameErrorUpPNG = outputDirectory + variable + "ErrorUp.pdf";
    canvas1->Print(outputFileNameErrorUpPNG.c_str());
    
    // canvas2
    TCanvas* canvas2 = new TCanvas("c2", "canvas2", 900, 900);
    canvas2->cd();
    canvas2->SetLogy();
    canvas2->SetTicks();
    
    string titleCan2 = "Systematic Down : " + variable;
    
    if (variable.find("ZNGoodJets") != string::npos){
        hDownDiffer[0]->GetXaxis()->SetRange(1, 8);
    }
    hDownDiffer[0]->SetMaximum(1000);
    hDownDiffer[0]->SetMinimum(0.1);
    //hDownDiffer[0]->SetTitle(titleCan2.c_str());
    hDownDiffer[0]->SetTitle("");
    hDownDiffer[0]->GetYaxis()->SetTitle("Uncertainty (%)");
    
    //JES
    hDownDiffer[0]->SetLineColor(kRed);
    hDownDiffer[0]->SetLineWidth(2);
    hDownDiffer[0]->SetLineStyle(2);
    hDownDiffer[0]->DrawCopy("HIST");
    //PU
    hDownDiffer[1]->SetLineColor(kBlue);
    hDownDiffer[1]->SetLineWidth(2);
    hDownDiffer[1]->SetLineStyle(5);
    hDownDiffer[1]->DrawCopy("HIST same");
    //Xsec
    hDownDiffer[2]->SetLineColor(kGreen);
    hDownDiffer[2]->SetLineWidth(2);
    hDownDiffer[2]->SetLineStyle(5);
    hDownDiffer[2]->DrawCopy("HIST same");
    //JER
    hDownDiffer[3]->SetLineColor(kGray+2);
    hDownDiffer[3]->SetLineWidth(3);
    hDownDiffer[3]->SetLineStyle(3);
    hDownDiffer[3]->DrawCopy("HIST same");
    //LepSF
    hDownDiffer[4]->SetLineColor(kPink+5);
    hDownDiffer[4]->SetLineWidth(2);
    hDownDiffer[4]->SetLineStyle(2);
    hDownDiffer[4]->DrawCopy("HIST same");
    //BtagSF
    hDownDiffer[5]->SetLineColor(kPink+2);
    hDownDiffer[5]->SetLineWidth(2);
    hDownDiffer[5]->SetLineStyle(5);
    hDownDiffer[5]->DrawCopy("HIST same");
    //MES
    hDownDiffer[6]->SetLineColor(kSpring+9);
    hDownDiffer[6]->SetLineWidth(2);
    hDownDiffer[6]->SetLineStyle(3);
    hDownDiffer[6]->DrawCopy("HIST same");
    //MER
    hDownDiffer[7]->SetLineColor(kAzure+4);
    hDownDiffer[7]->SetLineWidth(2);
    hDownDiffer[7]->SetLineStyle(2);
    hDownDiffer[7]->DrawCopy("HIST same");
    //WB
    hDownDiffer[8]->SetLineColor(kOrange);
    hDownDiffer[8]->SetLineWidth(3);
    hDownDiffer[8]->SetLineStyle(3);
    hDownDiffer[8]->DrawCopy("HIST same");
    //ttbar
    hDownDiffer[9]->SetLineColor(kGreen+3);
    hDownDiffer[9]->SetLineWidth(2);
    hDownDiffer[9]->SetLineStyle(5);
    hDownDiffer[9]->DrawCopy("HIST same");
    //MC
    hDownDiffer[10]->SetLineColor(kCyan);
    hDownDiffer[10]->SetLineWidth(3);
    hDownDiffer[10]->SetLineStyle(3);
    hDownDiffer[10]->DrawCopy("HIST same");
    //lumi
    hDownDiffer[11]->SetLineColor(kViolet+5);
    hDownDiffer[11]->SetLineWidth(2);
    hDownDiffer[11]->SetLineStyle(5);
    hDownDiffer[11]->DrawCopy("HIST same");
    //Resp
    if (variable.find("ZNGoodJets") == string::npos) {
        hDownDiffer[nGroup-1]->SetLineColor(kYellow);
        hDownDiffer[nGroup-1]->SetLineWidth(3);
        hDownDiffer[nGroup-1]->SetLineStyle(3);
        hDownDiffer[nGroup-1]->DrawCopy("HIST same");
    }
    
    hStatError->SetLineColor(kMagenta);
    hStatError->SetLineWidth(2);
    hStatError->SetLineStyle(2);
    hStatError->DrawCopy("HIST same");
    
    hTotalErrorDown->SetLineColor(kBlack);
    hTotalErrorDown->SetLineWidth(2);
    hTotalErrorDown->SetLineStyle(1);
    hTotalErrorDown->DrawCopy("HIST same");
    
    TLegend *leg2 = new TLegend(0.15, 0.63, 0.34, 0.89);
    leg2->SetFillStyle(0);
    leg2->SetBorderSize(0);
    leg2->AddEntry(hDownDiffer[0]  , "JES"    , "l");
    leg2->AddEntry(hDownDiffer[1]  , "PU"     , "l");
    leg2->AddEntry(hDownDiffer[2]  , "XSEC"   , "l");
    leg2->AddEntry(hDownDiffer[3]  , "JER"    , "l");
    leg2->AddEntry(hDownDiffer[4]  , "LepSF"  , "l");
    leg2->AddEntry(hDownDiffer[5]  , "BtagSF" , "l");
    leg2->AddEntry(hDownDiffer[6]  , "MES"    , "l");
    leg2->AddEntry(hDownDiffer[7]  , "MER"    , "l");
    leg2->AddEntry(hDownDiffer[8]  , "Wb"     , "l");
    leg2->AddEntry(hDownDiffer[9]  , "ttbar"  , "l");
    leg2->AddEntry(hDownDiffer[10] , "MC"     , "l");
    leg2->AddEntry(hDownDiffer[11] , "Lumi"   , "l");
    
    if (variable.find("ZNGoodJets") == string::npos) {
        leg2->AddEntry(hDownDiffer[nGroup-1]   , "Resp", "l");
    }
    leg2->AddEntry(hStatError       , "Statistical", "l");
    leg2->AddEntry(hTotalErrorDown  , "Total"      , "l");
    leg2->Draw();
    
    string outputFileNameErrorDownPNG = outputDirectory + variable + "ErrorDown.pdf";
    canvas2->Print(outputFileNameErrorDownPNG.c_str());
    
    
    string command1 = "mkdir -p PNGFiles/Table" ;
    system(command1.c_str());
    
    string outputDirectoryTable = "PNGFiles/Table/";
    string outputTableErrorDown = outputDirectoryTable + variable + "TableErrorDown.tex";
    string outputTableErrorUp   = outputDirectoryTable + variable + "TableErrorUp.tex";
    
    GetSysErrorTable(variable, outputTableErrorDown, dataCenBackUp, hDownDiffer, hStatError, hTotalErrorDown, nGroup);
    GetSysErrorTable(variable, outputTableErrorUp,   dataCenBackUp, hUpDiffer,   hStatError, hTotalErrorUp, nGroup);
}


void plotSystematicBreakdownCombine (string outputDirectory, string variable, TH1D* dataCentral, TH1D* hSyst[])
{
    outputDirectory = "PNGFiles/SystBreakDownComb/";
    string command = "mkdir -p PNGFiles/SystBreakDownComb" ;
    system(command.c_str());
    
    TH1D *dataCenBackUp = (TH1D*) dataCentral->Clone();
    
    const int nGroup = 13;
    const int nBins(dataCentral->GetNbinsX());
    
    int group = nGroup -1;
    if (variable.find("ZNGoodJets") == string::npos) {
        group = nGroup;
    }
    
    TH1D *hUpDiffer[nGroup] = {NULL}, *hDownDiffer[nGroup] = {NULL}, *hStatError = {NULL}, *hTotalErrorUp = {NULL}, *hTotalErrorDown = {NULL}; //nSyst(7);
    TH1D *hCombDiffer[nGroup] = {NULL}, *hTotalCombError = {NULL}, *hSumErrorOthers = {NULL};
    
    // set up histogram
    hStatError =  (TH1D*) dataCentral->Clone();
    hStatError->SetTitle("");
    hStatError->GetYaxis()->SetTitle("");
    
    hTotalErrorUp =  (TH1D*) dataCentral->Clone();
    hTotalErrorUp->SetTitle("");
    hTotalErrorUp->GetYaxis()->SetTitle("");
    
    hTotalErrorDown =  (TH1D*) dataCentral->Clone();
    hTotalErrorDown->SetTitle("");
    hTotalErrorDown->GetYaxis()->SetTitle("");
    
    hTotalCombError =  (TH1D*) dataCentral->Clone();
    hTotalCombError->SetTitle("");
    hTotalCombError->GetYaxis()->SetTitle("");
    
    hSumErrorOthers =  (TH1D*) dataCentral->Clone();
    hSumErrorOthers->SetTitle("");
    hSumErrorOthers->GetYaxis()->SetTitle("");
    
    // Initialize hUpDiffer hDownDiffer hCombDiffer
    for (int syst(0); syst < group; syst++){
        hUpDiffer[syst] = (TH1D*) dataCentral->Clone();
        hDownDiffer[syst] = (TH1D*) dataCentral->Clone();
        
        hUpDiffer[syst]->SetTitle("");
        hUpDiffer[syst]->GetYaxis()->SetTitle("");
        hDownDiffer[syst]->SetTitle("");
        hDownDiffer[syst]->GetYaxis()->SetTitle("");
        
        hCombDiffer[syst] = (TH1D*) dataCentral->Clone();
        hCombDiffer[syst]->SetTitle("");
        hCombDiffer[syst]->GetYaxis()->SetTitle("");
    }
    
    // Assign Value
    for (int bin(1); bin <= nBins; bin++){
        for(int i = 0; i < group; i++){
            
            hUpDiffer[i]->SetBinContent(bin, 0.);
            hDownDiffer[i]->SetBinContent(bin, 0.);
            
            if( dataCentral->GetBinContent(bin) > 0 ){
                // contribution from JES PU Xsec JER LepSF Btag MES
                if (i <= 6){
                    double SysDiffUPTemp(0);
                    double SysDiffDownTemp(0);
                    
                    double diffFromUp( hSyst[2*i]->GetBinContent(bin) - (dataCentral->GetBinContent(bin)) );
                    double diffFromDown( hSyst[(2*i)+1]->GetBinContent(bin) - (dataCentral->GetBinContent(bin)) );
                    
                    
                    if (diffFromUp >= diffFromDown) {
                        SysDiffUPTemp = diffFromUp;
                        SysDiffDownTemp = diffFromDown;
                    }
                    else{
                        SysDiffUPTemp = diffFromDown;
                        SysDiffDownTemp = diffFromUp;
                    }
                    
                    if (SysDiffUPTemp >= 0){
                        hUpDiffer[i]->SetBinContent( bin, SysDiffUPTemp * (100. / dataCentral->GetBinContent(bin)) ) ;
                    }
                    if (SysDiffDownTemp <= 0){
                        hDownDiffer[i]->SetBinContent( bin, (fabs(SysDiffDownTemp)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                    }
                }
                // contribution from MER
                if (i == 7){
                    double difference( hSyst[14]->GetBinContent(bin) - (dataCentral->GetBinContent(bin)) );
                    hUpDiffer[i]  ->SetBinContent( bin, (fabs(difference)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                    hDownDiffer[i]->SetBinContent( bin, (fabs(difference)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                }
                // contribution from Wb
                if (i == 8){
                    double difference( hSyst[15]->GetBinContent(bin) - (dataCentral->GetBinContent(bin)) );
                    hUpDiffer[i]  ->SetBinContent( bin, (fabs(difference)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                    hDownDiffer[i]->SetBinContent( bin, (fabs(difference)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                }
                // contribution from ttbar
                if (i == 9){
                    double difference( hSyst[16]->GetBinContent(bin) - (dataCentral->GetBinContent(bin)) );
                    hUpDiffer[i]  ->SetBinContent( bin, (fabs(difference)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                    hDownDiffer[i]->SetBinContent( bin, (fabs(difference)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                }
                // contribution from MC
                if (i == 10){
                    double difference( hSyst[17]->GetBinContent(bin) - (dataCentral->GetBinContent(bin)) );
                    hUpDiffer[i]  ->SetBinContent( bin, (fabs(difference)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                    hDownDiffer[i]->SetBinContent( bin, (fabs(difference)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                }
                // contribution from integrated lumi
                if (i == 11){
                    hUpDiffer[i]  ->SetBinContent( bin, 2.6 ) ;
                    hDownDiffer[i]->SetBinContent( bin, 2.6 ) ;
                }
                // contribution from Resp
                if (i == nGroup-1){
                    double difference( hSyst[18]->GetBinContent(bin) - (dataCentral->GetBinContent(bin)) );
                    hUpDiffer[i]  ->SetBinContent( bin, (fabs(difference)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                    hDownDiffer[i]->SetBinContent( bin, (fabs(difference)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                    //double uncer(hSyst[18]->GetBinContent(bin));
                    //hUpDiffer[i]  ->SetBinContent( bin, (fabs(uncer)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                    //hDownDiffer[i]->SetBinContent( bin, (fabs(uncer)) * (100. / dataCentral->GetBinContent(bin)) ) ;
                }
            }
        }
        
        hStatError->SetBinContent(bin, 0.);
        if (dataCentral->GetBinContent(bin) > 0){
            hStatError->SetBinContent( bin, (dataCentral->GetBinError(bin)) * (100. / dataCentral->GetBinContent(bin)) ) ;
            //cout << " Binth: " << bin << " Stat  difference = " << dataCentral->GetBinError(bin)  << "  % difference = " << (dataCentral->GetBinError(bin)) * (100 / dataCentral->GetBinContent(bin))  << endl;
        }
        //cout << endl;
    }
    
    // Assign value for hCombDiffer in %
    for(int i = 0; i < group; i++){
        for (int bin(1); bin <= nBins; bin++){
            hCombDiffer[i]->SetBinContent(bin, 0.);
            hCombDiffer[i]->SetBinError(bin, 0.);
            hCombDiffer[i]->SetBinContent(bin, max(fabs(hUpDiffer[i]->GetBinContent(bin)), fabs(hDownDiffer[i]->GetBinContent(bin))));
        }
    }
    
    // sum all
    for (int bin(1); bin <= nBins; bin++){
        hTotalErrorUp->SetBinContent(bin, 0.);
        hTotalErrorDown->SetBinContent(bin, 0.);
        hSumErrorOthers->SetBinContent(bin, 0.);
        
        double sumErrorUp(0.);
        double sumErrorDown(0.);
        double sumErrorComb(0.);
        double sumErrorOthers(0.);
        for (int syst(0); syst < group; syst++){
            sumErrorUp += pow(hUpDiffer[syst]->GetBinContent(bin), 2);
            sumErrorDown += pow(hDownDiffer[syst]->GetBinContent(bin), 2);
            sumErrorComb += pow(hCombDiffer[syst]->GetBinContent(bin), 2);
            
            if (syst == 0 || syst == 2 || syst == 5 || syst == 10 || syst == 12) continue;
            sumErrorOthers += pow(hCombDiffer[syst]->GetBinContent(bin), 2);
            
        }
        sumErrorUp += pow(hStatError->GetBinContent(bin), 2);
        sumErrorDown += pow(hStatError->GetBinContent(bin), 2);
        sumErrorUp = sqrt(sumErrorUp);
        sumErrorDown = sqrt(sumErrorDown);
        
        sumErrorComb += pow(hStatError->GetBinContent(bin), 2);
        sumErrorComb = sqrt(sumErrorComb);
        
        sumErrorOthers = sqrt(sumErrorOthers);
        
        hTotalErrorUp->SetBinContent(bin, sumErrorUp) ;
        hTotalErrorDown->SetBinContent(bin, sumErrorDown) ;
        hTotalCombError->SetBinContent(bin, sumErrorComb) ;
        hSumErrorOthers->SetBinContent(bin, sumErrorOthers) ;
    }
    
    // canvas1
    TCanvas* canvas1 = new TCanvas("c1", "canvas1", 900, 900);
    canvas1->cd();
    canvas1->SetLogy();
    canvas1->SetTicks();
    
    string titleCan1 = "Systematic Comb : " + variable;
    
    if (variable.find("ZNGoodJets") != string::npos){
        hCombDiffer[0]->GetXaxis()->SetRange(1, 8);
    }
    hCombDiffer[0]->SetMaximum(1000);
    hCombDiffer[0]->SetMinimum(0.1);
    //hCombDiffer[0]->SetTitle(titleCan1.c_str());
    hCombDiffer[0]->SetTitle("");
    hCombDiffer[0]->GetYaxis()->SetTitle("Uncertainty (%)");
    
    //JES
    hCombDiffer[0]->SetLineColor(kRed);
    hCombDiffer[0]->SetLineWidth(2);
    hCombDiffer[0]->SetLineStyle(2);
    hCombDiffer[0]->DrawCopy("HIST");
    //PU
    /*
    hCombDiffer[1]->SetLineColor(kBlue);
    hCombDiffer[1]->SetLineWidth(2);
    hCombDiffer[1]->SetLineStyle(5);
    hCombDiffer[1]->DrawCopy("HIST same");
     */
    //Xsec
    hCombDiffer[2]->SetLineColor(kGreen);
    hCombDiffer[2]->SetLineWidth(2);
    hCombDiffer[2]->SetLineStyle(5);
    hCombDiffer[2]->DrawCopy("HIST same");
    //JER
    /*
    hCombDiffer[3]->SetLineColor(kGray+2);
    hCombDiffer[3]->SetLineWidth(3);
    hCombDiffer[3]->SetLineStyle(3);
    hCombDiffer[3]->DrawCopy("HIST same");
    //LepSF
    hCombDiffer[4]->SetLineColor(kPink+5);
    hCombDiffer[4]->SetLineWidth(2);
    hCombDiffer[4]->SetLineStyle(2);
    hCombDiffer[4]->DrawCopy("HIST same");
     */
    //BtagSF
    hCombDiffer[5]->SetLineColor(28);
    hCombDiffer[5]->SetLineWidth(2);
    hCombDiffer[5]->SetLineStyle(5);
    hCombDiffer[5]->DrawCopy("HIST same");
    //MES
    /*
    hCombDiffer[6]->SetLineColor(kSpring+9);
    hCombDiffer[6]->SetLineWidth(2);
    hCombDiffer[6]->SetLineStyle(3);
    hCombDiffer[6]->DrawCopy("HIST same");
    //MER
    hCombDiffer[7]->SetLineColor(kAzure+4);
    hCombDiffer[7]->SetLineWidth(2);
    hCombDiffer[7]->SetLineStyle(2);
    hCombDiffer[7]->DrawCopy("HIST same");
    //WB
    hCombDiffer[8]->SetLineColor(kYellow);
    hCombDiffer[8]->SetLineWidth(3);
    hCombDiffer[8]->SetLineStyle(3);
    hCombDiffer[8]->DrawCopy("HIST same");
    //ttbar
    hCombDiffer[9]->SetLineColor(kGreen+3);
    hCombDiffer[9]->SetLineWidth(2);
    hCombDiffer[9]->SetLineStyle(5);
    hCombDiffer[9]->DrawCopy("HIST same");
     */
    //MC
    hCombDiffer[10]->SetLineColor(kCyan);
    hCombDiffer[10]->SetLineWidth(3);
    hCombDiffer[10]->SetLineStyle(3);
    hCombDiffer[10]->DrawCopy("HIST same");
    //lumi
    /*
    hCombDiffer[11]->SetLineColor(kViolet+5);
    hCombDiffer[11]->SetLineWidth(2);
    hCombDiffer[11]->SetLineStyle(5);
    hCombDiffer[11]->DrawCopy("HIST same");
     */
    //Resp
    if (variable.find("ZNGoodJets") == string::npos) {
        hCombDiffer[nGroup-1]->SetLineColor(kOrange);
        hCombDiffer[nGroup-1]->SetLineWidth(3);
        hCombDiffer[nGroup-1]->SetLineStyle(3);
        hCombDiffer[nGroup-1]->DrawCopy("HIST same");
    }
    
    hStatError->SetLineColor(kMagenta);
    hStatError->SetLineWidth(2);
    hStatError->SetLineStyle(2);
    hStatError->DrawCopy("HIST same");
    
    hSumErrorOthers->SetLineColor(kBlue);
    hSumErrorOthers->SetLineWidth(2);
    hSumErrorOthers->SetLineStyle(2);
    hSumErrorOthers->DrawCopy("HIST same");
    
    hTotalCombError->SetLineColor(kBlack);
    hTotalCombError->SetLineWidth(2);
    hTotalCombError->SetLineStyle(1);
    hTotalCombError->DrawCopy("HIST same");
    
    TLegend *leg = new TLegend(0.15, 0.63, 0.34, 0.89);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->AddEntry(hCombDiffer[0]  , "JES"    , "l");
    //leg->AddEntry(hCombDiffer[1]  , "PU"     , "l");
    leg->AddEntry(hCombDiffer[2]  , "BG"   , "l");
    //leg->AddEntry(hCombDiffer[3]  , "JER"    , "l");
    //leg->AddEntry(hCombDiffer[4]  , "LepSF"  , "l");
    leg->AddEntry(hCombDiffer[5]  , "BtagSF" , "l");
    //leg->AddEntry(hCombDiffer[6]  , "MES"    , "l");
    //leg->AddEntry(hCombDiffer[7]  , "MER"    , "l");
    //leg->AddEntry(hCombDiffer[8]  , "Wb"     , "l");
    //leg->AddEntry(hCombDiffer[9]  , "ttbar"  , "l");
    leg->AddEntry(hCombDiffer[10] , "MC"     , "l");
    //leg->AddEntry(hCombDiffer[11] , "Lumi"   , "l");
    if (variable.find("ZNGoodJets") == string::npos) {
        leg->AddEntry(hCombDiffer[nGroup-1]  , "Resp", "l");
    }
    leg->AddEntry(hSumErrorOthers   , "Other", "l");
    leg->AddEntry(hStatError    , "Statistical", "l");
    leg->AddEntry(hTotalCombError , "Total"      , "l");
    leg->Draw();
    
    string outputFileNameErrorCombPNG = outputDirectory + variable + "ErrorComb.pdf";
    canvas1->Print(outputFileNameErrorCombPNG.c_str());
    
    string command1 = "mkdir -p PNGFiles/TableSystComb" ;
    system(command1.c_str());
    
    string outputDirectoryTable = "PNGFiles/TableSystComb/";
    string outputTableErrorComb = outputDirectoryTable + variable + "TableErrorComb.tex";
    
    GetSysErrorTable(variable, outputTableErrorComb, dataCenBackUp, hCombDiffer, hStatError, hTotalCombError, nGroup);
}




void GetSysErrorTable (string variable, string outputTableName, TH1D* dataCenBackUp, TH1D* hDiffer[], TH1D* hStatError, TH1D* hTotalError, const int nGroup)
{
    cout << "Now producing table of " << outputTableName << endl;
    
    string tableTitle;
    if (outputTableName.find("TableErrorDown") != string::npos){
        tableTitle = variable + "TableErrorDown";
    }
    else if (outputTableName.find("TableErrorUp") != string::npos){
        tableTitle = variable + "TableErrorUp";
    }
    else{
        tableTitle = variable + "TableErrorComb";
    }
    
    int nXbin = hStatError->GetNbinsX();
    int nbinStart = 1;
    int group = nGroup;
    if (variable.find("ZNGoodJets") != string::npos){
        nbinStart = 1;
        group = group -1;
        nXbin = 8;
    }
    
    //------------ set column headers
    string temp;
    string xtitle = dataCenBackUp->GetXaxis()->GetTitle();
    string shortVar = xtitle.substr(0, xtitle.find(" "));
    string unit = "";
    if (xtitle.find("[") != string::npos) {
        size_t begin = xtitle.find("[") + 1;
        unit = xtitle.substr(begin);
        unit = unit.substr(0, unit.find("]"));
    }
    
    //--- temporary fix label
    if (variable.find("dRapidity") != string::npos) shortVar = "#Deltay";
    if (variable.find("dRJets") != string::npos)    shortVar = "#DeltaR";
    if (variable.find("diJetPt") != string::npos)    shortVar = "p_{T}";
    //---
    
    cout << shortVar << "   unit: " << unit << endl;
    temp = "\\frac{d\\sigma}{d" + shortVar + "}";
    if (unit == "" ) {
        temp += "  [pb";
    }
    else {
        temp += " [\\frac{pb}{";
        temp += unit;
        temp += "}";
    }
    temp += "]";
    
    // formating text to match latex style
    size_t pos_m;
    if (xtitle.find("#") != string::npos) {
        pos_m = xtitle.find("#");
        xtitle.replace(pos_m, 1, "\\");
    }
    if (temp.find("#") != string::npos) {
        pos_m = temp.find("#");
        temp.replace(pos_m, 1, "\\");
    }
    if (tableTitle.find("_") != string::npos) {
        pos_m = tableTitle.find("_");
        tableTitle.replace(pos_m, 1, "\\_");
    }
    //-------------------
    
    cout << "Stop after line " << __LINE__ << endl;
    
    
    FILE *outFile = fopen(outputTableName.c_str(),"w");
    cout << "Stop after line " << __LINE__ << endl;
    fprintf(outFile, "\\input{header.tex} \n ");
    fprintf(outFile, "\\usepackage[cm]{fullpage} \n ");
    fprintf(outFile, "\\begin{document} \n ");
    fprintf(outFile, "%s \\\\ \n ", tableTitle.c_str());
    fprintf(outFile, "\\\\ \n ");
    fprintf(outFile, "\\resizebox{\\textwidth}{!}     {\n\\begin{tabular}{l|cccccccccccccccc} \n ");
    fprintf(outFile, "$%s$ & ", xtitle.c_str());
    fprintf(outFile, "$%s$ & ", temp.c_str());
    fprintf(outFile, "$Tot. Err.(\\%%)$ & $Stat(\\%%)$ & $JES(\\%%)$ & $PU(\\%%)$ & $BG(\\%%)$ & $JER(\\%%)$ & $LepSF(\\%%)$ & $BtagCorr(\\%%)$ & $MES(\\%%)$ & $MER(\\%%)$ & $Wb(\\%%)$ & $ttbar(\\%%)$ & $MC(\\%%)$ & $Int.Lumi(\\%%)$ & $Resp(\\%%)$ \\\\ \\hline \n ");
    
    
    cout << "Stop after line " << __LINE__ << endl;
    // get array of bin edges
    double arrX[50];
    const double *arr = new double[nXbin+1];
    arr = dataCenBackUp->GetXaxis()->GetXbins()->GetArray();
    if (arr == 0){ // non-variable binning is defined
        if (variable.find("ZNGoodJets") != string::npos) {
        }
        else{
            for (int j = 0; j < nXbin ; j++){
                arrX[j] = dataCenBackUp->GetXaxis()->GetBinLowEdge(j+1);
            }
            arrX[nXbin] = dataCenBackUp->GetXaxis()->GetBinUpEdge(nXbin);
        }
    }
    else{ // variable binning is defined
        for (int j = 0; j <= nXbin ; j++){
            arrX[j] = dataCenBackUp->GetXaxis()->GetXbins()->At(j);
        }
    }
    
    
    cout << "Stop after line " << __LINE__ << endl;
    // fill out the table
    for (int i = nbinStart; i<= nXbin ; i++){
        if (variable.find("ZNGoodJets") != string::npos) {
            fprintf(outFile, " $N_{\\text{jets}} =$ %d  & ", i-1);
        }
        else{
            fprintf(outFile, " %g - %g  & ", arrX[i-1], arrX[i]);
        }
        fprintf(outFile, "%.3g  &", double(dataCenBackUp->GetBinContent(i)));
        fprintf(outFile, "%.3g  &", double(hTotalError->GetBinContent(i)));
        fprintf(outFile, "%.3g  &", double(hStatError->GetBinContent(i)));
        
        for (int j = 0 ; j < group  ; j++ ) {
            if (j != group -1){
                fprintf(outFile, "%.3g & ", double(hDiffer[j]->GetBinContent(i)));
            }
            else {
                fprintf(outFile, "%.3g ", double(hDiffer[j]->GetBinContent(i)));
            }
        }
        fprintf(outFile, "\\\\ \n");
    }
    fprintf( outFile, "\\end{tabular}} \\\\ \\\\  \n");
    fprintf( outFile, "\\end{document}");
    
    fclose(outFile);
}


void plotXsecIncJetMultipli(string variable, TH1D* dataCentralIni, TH1D* genMadIni, TH1D* genBhatALLIni, TH1D* genSheIni, int nBins, double xCoor[], double yCoorIni[], double xErr[], double ySystDownIni[], double ySystUpIni[])
{
    
    cout << "...producing inclusive jet multiplicity plot...." << endl;
    cout << " NBin = " << nBins << endl;
    variable = "ZNGoodJets_Zinc";
    string energy = getEnergy();
    bool doXSec(1);
    bool doNormalize(0);
    
    TH1D *dataCentral = (TH1D*) dataCentralIni->Clone();
    TH1D *genMad      = (TH1D*) genMadIni     ->Clone();
    TH1D *genBhatALL  = (TH1D*) genBhatALLIni ->Clone();
    TH1D *genShe      = (TH1D*) genSheIni     ->Clone();
    
    for(int i = 1; i <= nBins; i++) {
        dataCentral->SetBinContent(i, 0.0);
        dataCentral->SetBinError  (i, 0.0);
        
        genMad->SetBinContent(i, 0.0);
        genMad->SetBinError  (i, 0.0);
        
        genBhatALL->SetBinContent(i, 0.0);
        genBhatALL->SetBinError  (i, 0.0);
        
        genShe->SetBinContent(i, 0.0);
        genShe->SetBinError  (i, 0.0);
    }
    
    for(int i = 1; i <= nBins; i++){
        double binContentInc(0);
        double binErrorInc(0);
        for (int j = 0; j < (nBins+1-i); j++){
            binContentInc += dataCentralIni->GetBinContent(nBins-j);
            binErrorInc   += pow(dataCentralIni->GetBinError(nBins-j), 2);
        }
        binErrorInc = sqrt(binErrorInc);
        
        dataCentral->SetBinContent(i, binContentInc);
        dataCentral->SetBinError(i, binErrorInc);
    }
    
    for(int i = 1; i <= nBins; i++){
        double binContentInc(0);
        double binErrorInc(0);
        for (int j = 0; j < (nBins+1-i); j++){
            binContentInc += genMadIni->GetBinContent(nBins-j);
            binErrorInc   += pow(genMadIni->GetBinError(nBins-j), 2);
        }
        binErrorInc = sqrt(binErrorInc);
        
        genMad->SetBinContent(i, binContentInc);
        genMad->SetBinError(i, binErrorInc);
    }
    
    for(int i = 1; i <= nBins; i++){
        double binContentInc(0);
        double binErrorInc(0);
        for (int j = 0; j < (nBins+1-i); j++){
            binContentInc += genBhatALLIni->GetBinContent(nBins-j);
            binErrorInc   += pow(genBhatALLIni->GetBinError(nBins-j), 2);
        }
        binErrorInc = sqrt(binErrorInc);
        
        genBhatALL->SetBinContent(i, binContentInc);
        genBhatALL->SetBinError(i, binErrorInc);
        
        if (i == 1){
            genBhatALL->SetBinContent(i, 0.0);
            genBhatALL->SetBinError(i, 0.0);
        }
    }
    
    for(int i = 1; i <= nBins; i++){
        double binContentInc(0);
        double binErrorInc(0);
        for (int j = 0; j < (nBins+1-i); j++){
            binContentInc += genSheIni->GetBinContent(nBins-j);
            binErrorInc   += pow(genSheIni->GetBinError(nBins-j), 2);
        }
        binErrorInc = sqrt(binErrorInc);
        
        genShe->SetBinContent(i, binContentInc);
        genShe->SetBinError(i, binErrorInc);
    }
    
    double yCoor[11], ySystDown[11], ySystUp[11];
    
    for (int i(1); i <= nBins; i++) {
        yCoor[i-1] = dataCentral->GetBinContent(i);
        
        double ErrDown(0);
        double ErrUp(0);
        for (int j = 0; j < (nBins+1-i); j++){
            ErrDown += pow(ySystDownIni[nBins-j-1], 2);
            ErrUp += pow(ySystUpIni[nBins-j-1], 2);
        }
        ySystDown[i-1] = sqrt(ErrDown);
        ySystUp[i-1] = sqrt(ErrUp);
    }
    
    
    
    
    //--- Set up for systematic plot on pad 1
    TGraphAsymmErrors *grCentralSyst = new TGraphAsymmErrors(nBins, xCoor, yCoor, xErr, xErr, ySystDown, ySystUp);
    
    //--- plotting ---
    TH1D *hisUnfolded = (TH1D*) dataCentral->Clone();
    
    TCanvas *can = new TCanvas(variable.c_str(), variable.c_str(), 600, 800);
    can->cd();
    //--- Set pad1.
    TPad *pad1 = new TPad("pad1","pad1", 0.01, 0.55, 0.99, 0.99);
    pad1->SetTopMargin(0.11);
    pad1->SetBottomMargin(0);
    pad1->SetRightMargin(0.04);
    pad1->SetLeftMargin(0.11);
    pad1->SetTicks();
    pad1->Draw();
    pad1->cd();
    pad1->SetLogy();
    
    // set y axis title
    string temp;
    string xtitle = genMad->GetXaxis()->GetTitle();
    string shortVar = xtitle.substr(0, xtitle.find(" "));
    
    string unit = "";
    if (xtitle.find("[") != string::npos) {
        size_t begin = xtitle.find("[") + 1;
        unit = xtitle.substr(begin);
        unit = unit.substr(0, unit.find("]"));
    }
    //--- temporary fix label: FIXME
    if (variable.find("dRapidity") != string::npos) shortVar = "#Deltay";
    if (variable.find("dRJets") != string::npos)    shortVar = "#DeltaR";
    if (variable.find("diJetPt") != string::npos)    shortVar = "p_{T}";
    //---
    cout << shortVar << "   unit: " << unit << endl;
    temp = "d#sigma / d" + shortVar;
    temp += "  [pb";
    if (unit != "" ) temp += " / " + unit;
    temp += "]";
    //-------
    
    double MineYMax = genMad->GetMaximum();
    //double MineYMin = genMad->GetMinimum();
    if (hisUnfolded->GetMaximum() >= MineYMax) MineYMax = hisUnfolded->GetMaximum();
    //if (hisUnfolded->GetMinimum() <= MineYMin) MineYMin = hisUnfolded->GetMinimum();
    
    hisUnfolded->SetTitle("");
    if (variable.find("ZNGoodJets") != string::npos) {
        hisUnfolded->GetXaxis()->SetRange(1, 8);
    }
    hisUnfolded->GetYaxis()->SetTitle(temp.c_str());
    hisUnfolded->GetYaxis()->SetTitleSize(0.04);
    hisUnfolded->GetYaxis()->SetTitleOffset(1.45);
    hisUnfolded->GetYaxis()->SetTitleFont(42);
    //hisUnfolded->SetMinimum(MineYMin*0.8);
    if ( (variable.find("dRapidity") != string::npos) || (variable.find("dRJets") != string::npos) ){
        hisUnfolded->SetMaximum(MineYMax*10); // remove or modify this when appropriate
    }
    
    hisUnfolded->SetFillStyle(3354);
    hisUnfolded->SetFillColor(12);
    hisUnfolded->SetLineColor(kBlack);
    hisUnfolded->SetLineWidth(2);
    hisUnfolded->SetMarkerColor(kBlack);
    hisUnfolded->SetMarkerStyle(20);
    hisUnfolded->SetMarkerSize(0.8);
    hisUnfolded->Draw("E1");
    
    grCentralSyst->SetFillStyle(3354);
    grCentralSyst->SetFillColor(12);
    grCentralSyst->Draw("2");
    
    genMad->SetFillStyle(ZJetsFillStyle);
    genMad->SetFillColor(kBlue-10);
    //genMad->SetFillColorAlpha(kBlue-10, 0.60);
    genMad->SetLineColor(kBlue);
    genMad->SetLineWidth(2);
    genMad->SetMarkerColor(kBlue);
    genMad->SetMarkerStyle(24);
    //genMad->Draw("E2 same");
    genMad->Draw("E same");
    
    genBhatALL->SetFillStyle(ZJetsFillStyle);
    genBhatALL->SetFillColor(kOrange-2);
    //genBhatALL->SetFillColorAlpha(kOrange-2, 0.60);
    genBhatALL->SetLineColor(kOrange+10);
    genBhatALL->SetLineWidth(2);
    genBhatALL->SetMarkerColor(kOrange+10);
    genBhatALL->SetMarkerStyle(25);
    //genBhatALL->Draw("E2 same");
    genBhatALL->Draw("E same");
    
    genShe->SetFillStyle(ZJetsFillStyle);
    genShe->SetFillColor(kGreen-8);
    //genShe->SetFillColorAlpha(kGreen-8, 0.60);
    genShe->SetLineColor(kGreen+3);
    genShe->SetLineWidth(2);
    genShe->SetMarkerColor(kGreen+3);
    genShe->SetMarkerStyle(26);
    //genShe->Draw("E2 same");
    genShe->Draw("E same");
    
    pad1->Draw();
    //--- TLegend ---
    TLegend *legend = new TLegend(0.47, 0.74, 0.99, 0.98);
    legend->SetX1(0.44);
    legend->SetY1(0.77);
    legend->SetX2(0.95);
    legend->SetY2(0.98);
    legend->SetTextSize(.034);
    legend->SetFillColor(0);
    legend->SetFillStyle(1001);
    legend->SetBorderSize(1);
    legend->AddEntry(hisUnfolded, "Data", "PLEF");
    legend->AddEntry(genMad, "MG5 + PY6 (#leq 4j LO + PS)", "plef");
    legend->AddEntry(genBhatALL, "BLACKHAT+SHERPA (NLO)", "plef");
    legend->AddEntry(genShe, "SHERPA 2 (#leq 2j NLO 3,4j LO + PS)", "plef");
    legend->Draw();
    //------------------
    
    //--- TLatex stuff ---
    TLatex *latexLabel = new TLatex();
    latexLabel->SetNDC();
    latexLabel->SetTextSize(0.05);
    latexLabel->SetTextFont(42);
    latexLabel->SetLineWidth(2);
    latexLabel->SetTextFont(61);
    latexLabel->DrawLatex(0.13,0.95,"CMS");
    latexLabel->SetTextFont(52);
    latexLabel->DrawLatex(0.20,0.95,"Preliminary");
    latexLabel->SetTextFont(42);
    latexLabel->DrawLatex(0.13,0.95-0.045,"19.6 fb^{-1} (8 TeV)");
    latexLabel->DrawLatex(0.18,0.21-0.05,"anti-k_{T} (R = 0.5) Jets");
    latexLabel->DrawLatex(0.18,0.21-0.11,"p_{T}^{jet} > 30 GeV, |#eta^{jet}| < 2.4 ");
    latexLabel->DrawLatex(0.18,0.21-0.17,"W (#rightarrow #mu#nu) channel");
    latexLabel->SetName("latexLabel");
    latexLabel->Draw("same");
    
    
    //--- calculate ratio >> pad2, pad3
    
    // for data statistical uncer
    TH1D  *data1 = (TH1D*) hisUnfolded->Clone();
    for(int i = 1; i <= hisUnfolded->GetNbinsX(); i++) {
        data1->SetBinContent(i, 1.0);
        if (hisUnfolded->GetBinContent(i) > 0.) {
            data1->SetBinError(i, hisUnfolded->GetBinError(i)/hisUnfolded->GetBinContent(i));
        }
        else data1->SetBinError(i, 0.);
        
    }
    TH1D  *data2 = (TH1D*) data1->Clone();
    TH1D  *data3 = (TH1D*) data1->Clone();
    
    // for gen statistical uncer
    TH1D  *gen1 = (TH1D*) genMad->Clone();
    for(int i = 1; i <= hisUnfolded->GetNbinsX(); i++) {
        if (hisUnfolded->GetBinContent(i) > 0.) {
            gen1->SetBinContent(i, genMad->GetBinContent(i)/hisUnfolded->GetBinContent(i));
            gen1->SetBinError(i, genMad->GetBinError(i)/hisUnfolded->GetBinContent(i));
        }
        else {
            gen1->SetBinContent(i, 0.);
            gen1->SetBinError(i, 0.);
        }
    }
    TH1D  *gen2 = (TH1D*) hisUnfolded->Clone();
    for(int i = 1; i <= hisUnfolded->GetNbinsX(); i++) {
        if (hisUnfolded->GetBinContent(i) > 0.) {
            gen2->SetBinContent(i, genBhatALL->GetBinContent(i)/hisUnfolded->GetBinContent(i));
            gen2->SetBinError(i, genBhatALL->GetBinError(i)/hisUnfolded->GetBinContent(i));
        }
        else {
            gen2->SetBinContent(i, 0.);
            gen2->SetBinError(i, 0.);
        }
    }
    TH1D  *gen3 = (TH1D*) hisUnfolded->Clone();
    for(int i = 1; i <= hisUnfolded->GetNbinsX(); i++) {
        if (hisUnfolded->GetBinContent(i) > 0.) {
            gen3->SetBinContent(i, genShe->GetBinContent(i)/hisUnfolded->GetBinContent(i));
            gen3->SetBinError(i, genShe->GetBinError(i)/hisUnfolded->GetBinContent(i));
        }
        else {
            gen3->SetBinContent(i, 0.);
            gen3->SetBinError(i, 0.);
        }
        //cout << " sherpa " << gen3->GetBinContent(i) << endl;
    }
    
    //--- Set up for systematic plot on pad 2
    double xCoorCenRatio[nBins], yCoorCenRatio[nBins], xErrCenRatio[nBins], ySystDownCenRatio[nBins], ySystUpCenRatio[nBins];
    for (int bin(1); bin <= nBins; bin++) {
        // set x-coordinate and error in x
        xCoorCenRatio[bin-1] = hisUnfolded->GetBinCenter(bin);
        xErrCenRatio[bin-1]  = 0.5 * hisUnfolded->GetBinWidth(bin);
        // set y-coordinate
        yCoorCenRatio[bin-1] = 1.;
        // set systematics errors in y => combine both statistical and systematics
        if (hisUnfolded->GetBinContent(bin) > 0) {
            ySystUpCenRatio[bin-1]     = ySystUp[bin-1]/hisUnfolded->GetBinContent(bin);
            ySystDownCenRatio[bin-1]   = ySystDown[bin-1]/hisUnfolded->GetBinContent(bin);
        }
        else {
            ySystUpCenRatio[bin-1] = 0.;
            ySystDownCenRatio[bin-1] = 0.;
        }
        
    }
    TGraphAsymmErrors *grCentralSystRatio = new TGraphAsymmErrors(nBins, xCoorCenRatio, yCoorCenRatio, xErrCenRatio, xErrCenRatio, ySystDownCenRatio, ySystUpCenRatio);
    
    
    //--- NEW TPAD => pad2 ---//
    can->cd();
    TPad *pad2 = new TPad("pad2","pad2", 0.01, 0.39, 0.99, 0.55);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0);
    pad2->SetRightMargin(0.04);
    pad2->SetLeftMargin(0.11);
    pad2->SetGridy();
    pad2->SetTicks();
    pad2->Draw();
    pad2->cd();
    
    data1->SetTitle("");
    data1->GetYaxis()->SetRangeUser(0.41,1.59);
    data1->GetYaxis()->SetTitle("MG5/Data");
    data1->GetYaxis()->SetTitleSize(0.13);
    data1->GetYaxis()->SetTitleOffset(0.44);
    data1->GetYaxis()->CenterTitle();
    data1->GetYaxis()->SetLabelSize(0.12);
    data1->GetYaxis()->SetLabelOffset(0.01);
    
    data1->GetXaxis()->SetTickLength(0.06);
    
    data1->SetLineColor(kBlack);
    data1->SetLineWidth(1);
    data1->SetMarkerColor();
    data1->SetMarkerStyle();
    data1->Draw("E1");
    
    gen1->SetFillStyle(ZJetsFillStyle);
    gen1->SetFillColor(kBlue-10);
    gen1->SetLineColor(kBlue);
    gen1->SetLineWidth(2);
    gen1->SetMarkerColor(kBlue);
    gen1->SetMarkerStyle(24);
    gen1->Draw("E2 same");
    gen1->Draw("E1 same");
    
    grCentralSystRatio->SetFillStyle(3354);
    grCentralSystRatio->SetFillColor(12);
    grCentralSystRatio->Draw("2");
    
    TLegend *legend2 = new TLegend(0.16, 0.05, 0.33, 0.20);
    legend2->SetFillColor(0);
    legend2->SetFillStyle(ZJetsFillStyle);
    legend2->SetBorderSize(0);
    //legend2->SetTextSize(.12);
    legend2->SetY1(0.07);
    legend2->SetX1(0.15);
    legend2->SetY2(0.20);
    legend2->SetTextSize(0.09);
    TLegendEntry *leEntry2;
    leEntry2 = legend2->AddEntry(gen1, "Stat. unc. (gen)", "f");
    legend2->Draw("same");
    
    // Text Madgraph
    /*
    TLatex *latexLabel0 = new TLatex();
    latexLabel0->SetTextSize(0.15);
    latexLabel0->SetTextFont(42);
    latexLabel0->SetLineWidth(2);
    latexLabel0->SetTextColor(kAzure-5);
    latexLabel0->SetNDC();
    latexLabel0->DrawLatex(0.35,0.09,"Madgraph");
    */
    pad2->Draw();
    
    
    //--- NEW TPAD => pad3 ---//
    can->cd();
    TPad *pad3 = new TPad("pad3","pad3", 0.01, 0.23, 0.99, 0.39);
    pad3->SetTopMargin(0);
    pad3->SetBottomMargin(0);
    pad3->SetRightMargin(0.04);
    pad3->SetLeftMargin(0.11);
    pad3->SetGridy();
    pad3->SetTicks();
    pad3->Draw();
    pad3->cd();
    
    data2->SetTitle("");
    data2->GetYaxis()->SetRangeUser(0.41,1.59);
    data2->GetYaxis()->SetTitle("BLACKHAT/Data");
    data2->GetYaxis()->SetTitleSize(0.13);
    data2->GetYaxis()->SetTitleOffset(0.44);
    data2->GetYaxis()->CenterTitle();
    data2->GetYaxis()->SetLabelSize(0.12);
    data2->GetYaxis()->SetLabelOffset(0.01);
    
    data2->GetXaxis()->SetTickLength(0.06);
    
    data2->SetLineColor(kBlack);
    data2->SetLineWidth(1);
    data2->SetMarkerColor();
    data2->SetMarkerStyle();
    data2->Draw("E1");
    
    gen2->SetFillStyle(ZJetsFillStyle);
    gen2->SetFillColor(kOrange-2);
    gen2->SetLineColor(kOrange+10);
    gen2->SetLineWidth(2);
    gen2->SetMarkerColor(kOrange+10);
    gen2->SetMarkerStyle(25);
    gen2->Draw("E2 same");
    gen2->Draw("E1 same");
    
    grCentralSystRatio->SetFillStyle(3354);
    grCentralSystRatio->SetFillColor(12);
    grCentralSystRatio->Draw("2");
    
    TLegend *legend3 = new TLegend(0.16, 0.05, 0.33, 0.20);
    legend3->SetFillColor(0);
    legend3->SetFillStyle(ZJetsFillStyle);
    legend3->SetBorderSize(0);
    //legend3->SetTextSize(.12);
    legend3->SetY1(0.07);
    legend3->SetX1(0.15);
    legend3->SetY2(0.20);
    legend3->SetTextSize(0.09);
    TLegendEntry *leEntry3;
    leEntry3 = legend3->AddEntry(gen2, "Stat. unc. (gen)", "f");
    legend3->Draw("same");
    
    // Text BlackHat
    /*
    TLatex *latexLabel1 = new TLatex();
    latexLabel1->SetTextSize(0.15);
    latexLabel1->SetTextFont(42);
    latexLabel1->SetLineWidth(2);
    latexLabel1->SetTextColor(kOrange+2);
    latexLabel1->SetNDC();
    latexLabel1->DrawLatex(0.35,0.09,"BlackHat");
    */
    pad3->Draw();
    
    //--- NEW TPAD => pad4 ---//
    can->cd();
    TPad *pad4 = new TPad("pad3","pad3", 0.01, 0.01, 0.99, 0.23);
    pad4->SetTopMargin(0);
    pad4->SetBottomMargin(0.25);
    pad4->SetRightMargin(0.04);
    pad4->SetLeftMargin(0.11);
    pad4->SetGridy();
    pad4->SetTicks();
    pad4->Draw();
    pad4->cd();
    
    
    data3->GetXaxis()->SetBinLabel(1, "#geq 0");
    data3->GetXaxis()->SetBinLabel(2, "#geq 1");
    data3->GetXaxis()->SetBinLabel(3, "#geq 2");
    data3->GetXaxis()->SetBinLabel(4, "#geq 3");
    data3->GetXaxis()->SetBinLabel(5, "#geq 4");
    data3->GetXaxis()->SetBinLabel(6, "#geq 5");
    data3->GetXaxis()->SetBinLabel(7, "#geq 6");
    data3->GetXaxis()->SetBinLabel(8, "#geq 7");
    data3->GetXaxis()->SetBinLabel(9, "#geq 8");
    data3->GetXaxis()->SetBinLabel(10,"#geq 9");
    data3->GetXaxis()->SetBinLabel(11,"#geq 10");
    data3->GetXaxis()->SetRange(1, 8);
    
    
    data3->SetTitle("");
    data3->GetYaxis()->SetRangeUser(0.41,1.59);
    data3->GetYaxis()->SetTitle("SHERPA2/Data");
    data3->GetYaxis()->SetTitleSize(0.096);
    data3->GetYaxis()->SetTitleOffset(0.6);
    data3->GetYaxis()->CenterTitle();
    data3->GetYaxis()->SetLabelSize(0.085);
    data3->GetYaxis()->SetLabelOffset(0.012);
    
    data3->GetXaxis()->SetTitle(gen1->GetXaxis()->GetTitle());
    data3->GetXaxis()->SetTitleSize(0.1);
    data3->GetXaxis()->SetTitleOffset(1.05);
    data3->GetXaxis()->SetLabelSize(0.09);
    data3->GetXaxis()->SetLabelOffset(0.014);
    data3->GetXaxis()->SetTickLength(0.06);
    
    data3->SetLineColor(kBlack);
    data3->SetLineWidth(1);
    data3->SetMarkerColor();
    data3->SetMarkerStyle();
    data3->Draw("E1");
    
    gen3->SetFillStyle(ZJetsFillStyle);
    gen3->SetFillColor(kGreen-8);
    gen3->SetLineColor(kGreen+3);
    gen3->SetLineWidth(2);
    gen3->SetMarkerColor(kGreen+3);
    gen3->SetMarkerStyle(26);
    gen3->Draw("E2 same");
    gen3->Draw("E1 same");
    
    grCentralSystRatio->SetFillStyle(3354);
    grCentralSystRatio->SetFillColor(12);
    grCentralSystRatio->Draw("2");
    
    TLegend *legend4 = new TLegend(0.16, 0.05, 0.33, 0.20);
    legend4->SetFillColor(0);
    legend4->SetFillStyle(ZJetsFillStyle);
    legend4->SetBorderSize(0);
    //legend4->SetTextSize(.12);
    legend4->SetY1(0.32);
    legend4->SetX1(0.15);
    legend4->SetY2(0.42);
    legend4->SetTextSize(0.065);
    TLegendEntry *leEntry4;
    leEntry4 = legend4->AddEntry(gen3, "Stat. unc. (gen)", "f");
    legend4->Draw("same");
    
    // Text Sherpa (position not relative to pad4)
    /*
    TLatex *latexLabel2 = new TLatex();
    latexLabel2->SetTextSize(0.11);
    latexLabel2->SetTextFont(42);
    latexLabel2->SetLineWidth(2);
    latexLabel2->SetTextColor(kGreen+3);
    latexLabel2->SetNDC();
    latexLabel2->DrawLatex(0.35,0.34,"Sherpa");
    */
    pad4->Draw();
    can->cd();
    
    //// Set output .pdf file in the direcctory "PNGFiles/FinalResults/"
    string command = "mkdir -p PNGFiles/FinalResults" ;
    system(command.c_str());
    string outputDirectory = "PNGFiles/FinalResults/";
    string outputFileNamePNG = outputDirectory + leptonFlavor + "_" + energy + "_RunPlotting_";
    
    if (doXSec) outputFileNamePNG += "XSec_";
    else if (doNormalize) outputFileNamePNG += "Normalized_";
    
    outputFileNamePNG += variable + "_" + unfAlg ;
    if (doVarWidth)  outputFileNamePNG += "_VarWidth";
    outputFileNamePNG += ".pdf";
    can->Print(outputFileNamePNG.c_str());
}


//cout << "Stop after line " << __LINE__ << endl;
////////////////////////////////////////////////////////////////////////////////////



