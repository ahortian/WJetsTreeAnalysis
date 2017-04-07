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
#include <TLatex.h>
#include "getFilesAndHistograms.h"

//-- Setting global variables -------------
#include "fileNames.h"
#include "variablesOfInterestVarWidth.h"

using namespace std;
//--------------------------------


//--- Declaring functions -------------
void PlotUnfoSyst(int start = 0, int end = -1);
void FuncPlot(string  variable = "ZNGoodJets_Zexc", bool log = 1, bool decrease = 1);
//void plotSysVar(TH1D* hCentral, TH1D* hVarUp, TH1D* hVarDown);

//--------------------------------


//--- More global variables -------------
//double Luminosity(19290.); // muLumi(19.549) => DoubleMu number with pixelCalc 19.244
double Luminosity(19.244 * 1000); // for SMu
bool isMuon = 1;
string leptonFlavor = "SMu";
string unfAlg = "Bayes";
bool doVarWidth = true ;
int JetPtMin = 30 ;
//--------------------------------


void PlotUnfoSyst(int start, int end)
{
    //NVAROFINTEREST = 41;
    //for (int i(0); i < 1 /*NVAROFINTEREST*/; i++){
    if (end == -1) end = start + 1;
    for (int i(start); i < end /*NVAROFINTERESTZJETS*/; i++) {
        FuncPlot(VAROFINTERESTWJETS[i].name, VAROFINTERESTWJETS[i].log, VAROFINTERESTWJETS[i].decrease);
    }
    
}

void FuncPlot(string variable, bool logZ, bool decrease)
{
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
    
    TH1D *dataCentral       = (TH1D*) f->Get("Central"); // unfolded central (data -BG)
    //TH1D *data              = (TH1D*) f->Get("Data"); // reco data
    //TH1D *genMad            = (TH1D*) f->Get("genMad");
    //TH1D *dataCentralBackUp = (TH1D*) dataCentral->Clone();
    
    // get unfolded histogram for systematics
    int nSyst(18), nGroup(11);
    TH1D *hSyst[19];
    hSyst[0]          = (TH1D*) f->Get("JESup");
    hSyst[1]          = (TH1D*) f->Get("JESdown");
    hSyst[2]          = (TH1D*) f->Get("PUup");
    hSyst[3]          = (TH1D*) f->Get("PUdown");
    hSyst[4]          = (TH1D*) f->Get("XSECup");
    hSyst[5]          = (TH1D*) f->Get("XSECdown");
    hSyst[6]          = (TH1D*) f->Get("JERup");
    hSyst[7]          = (TH1D*) f->Get("JERdown");
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
        nGroup = 12;
    }
    
    string hNames[19] = {"JESup", "JESdown", "PUup", "PUdown", "XSECup", "XSECdown", "JERup", "JERdown", "LepSFup", "LepSFdown", "BtagSFup", "BtagSFdown", "MESup", "MESdown", "MER", "Wb", "ttbar", "MC", "Resp"};
    string cutNames[12] = {"JES", "PU", "XSEC", "JER", "LepSF", "BtagSF", "MES", "MER", "Wb", "ttbar", "MC", "Resp"};
    
    
    // print out integral value for testing
    const int nBins(dataCentral->GetNbinsX());
    cout << " central integral:  " << variable << "   " << dataCentral->Integral(1, nBins) << " with under/over:  " << dataCentral->Integral(0, nBins+1) << endl;
    for (int syst(0); syst < nSyst; syst++){
        double tempScale = hSyst[syst]->Integral(1, nBins) ;
        cout << " original integral for " << syst << " : " << tempScale << endl;
    }
    
    string command = "mkdir -p PNGFiles/plotUnfoldSyst" ;
    system(command.c_str());
    
    for (int i = 0 ; i < 7 ; i ++){
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
        
        hSyst[2*i]->SetTitle("");
        hSyst[2*i]->SetMarkerStyle(24);
        hSyst[2*i]->SetMarkerColor(kBlue+1);
        hSyst[2*i]->SetLineColor(kBlue+1);
        hSyst[2*i]->SetLineWidth(2);
        hSyst[2*i]->DrawCopy("same");
        
        hSyst[2*i+1]->SetTitle("");
        hSyst[2*i+1]->SetMarkerStyle(32);
        hSyst[2*i+1]->SetMarkerColor(kRed+1);
        hSyst[2*i+1]->SetLineColor(kRed+1);
        hSyst[2*i+1]->SetLineWidth(2);
        hSyst[2*i+1]->DrawCopy("same");
        
        double xLowLeg(0.76), xHighLeg(0.95);
        if (!decrease){
            xLowLeg = 0.14;
            xHighLeg = 0.47;
        }
        TLegend *leg = new TLegend(xLowLeg, 0.63, xHighLeg, 0.89);
        leg->SetFillStyle(0);
        leg->SetBorderSize(0);
        leg->AddEntry(dataCentral, " Central ", "fl");
        leg->AddEntry(hSyst[2*i], hNames[2*i].c_str(), "pl");
        leg->AddEntry(hSyst[2*i+1], hNames[2*i+1].c_str(), "pl");
        leg->Draw();
        
        cmsColl->DrawLatex(0.11,0.905, (" Systematic: " + cutNames[i]).c_str());
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
        
        hSyst[2*i]->Divide(dataCentral);
        hSyst[2*i+1]->Divide(dataCentral);
        
        hSyst[2*i]->SetTitle("");
        hSyst[2*i]->GetXaxis()->SetTickLength(0.03);
        hSyst[2*i]->GetXaxis()->SetTitleSize(0.1);
        hSyst[2*i]->GetXaxis()->SetLabelSize(0.09);
        hSyst[2*i]->GetXaxis()->SetLabelOffset(0.018);
        hSyst[2*i]->GetXaxis()->SetTitleOffset(1.2);
        
        if (i != 0){
            hSyst[2*i]->GetYaxis()->SetRangeUser(0.81, 1.19);
            hSyst[2*i]->GetYaxis()->SetNdivisions(5,5,0);
        }
        else{
            hSyst[2*i]->GetYaxis()->SetRangeUser(0.31, 1.69);
            hSyst[2*i]->GetYaxis()->SetNdivisions(10,5,0);
        }
        
        
        hSyst[2*i]->GetYaxis()->SetTitle(" Syst / Central ");
        hSyst[2*i]->GetYaxis()->SetTitleSize(0.1);
        hSyst[2*i]->GetYaxis()->SetTitleOffset(0.5);
        hSyst[2*i]->GetYaxis()->CenterTitle();
        hSyst[2*i]->GetYaxis()->SetLabelSize(0.08);
        
        hSyst[2*i]->DrawCopy("E1");
        hSyst[2*i+1]->DrawCopy("E1same");
        pad2->Draw();
        
        
        string outputDirectory = "PNGFiles/plotUnfoldSyst/";
        string outputFileName = outputDirectory + variable + "Unfolded" + cutNames[i] + ".pdf";
        canUnfoldedJES->Print(outputFileName.c_str());
        cout << " save pdf file of " << outputFileName << " " << hSyst[2*i]->GetName() << " " << hSyst[2*i+1]->GetName() << endl;
        
    }
    
    
    for (int k = 14; k < nSyst; k++){
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
        
        hSyst[k]->SetTitle("");
        hSyst[k]->SetMarkerStyle(24);
        hSyst[k]->SetMarkerColor(kBlue+1);
        hSyst[k]->SetLineColor(kBlue+1);
        hSyst[k]->SetLineWidth(2);
        hSyst[k]->DrawCopy("same");
        
        
        double xLowLeg(0.76), xHighLeg(0.95);
        if (!decrease){
            xLowLeg = 0.14;
            xHighLeg = 0.47;
        }
        TLegend *leg = new TLegend(xLowLeg, 0.63, xHighLeg, 0.89);
        leg->SetFillStyle(0);
        leg->SetBorderSize(0);
        leg->AddEntry(dataCentral, " Central ", "fl");
        leg->AddEntry(hSyst[k], hNames[k].c_str(), "pl");
        leg->Draw();
        
        cmsColl->DrawLatex(0.11,0.905, (" Systematic: " + cutNames[k-7]).c_str());
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
        
        hSyst[k]->Divide(dataCentral);
        
        
        hSyst[k]->SetTitle("");
        hSyst[k]->GetXaxis()->SetTickLength(0.03);
        hSyst[k]->GetXaxis()->SetTitleSize(0.1);
        hSyst[k]->GetXaxis()->SetLabelSize(0.09);
        hSyst[k]->GetXaxis()->SetLabelOffset(0.018);
        hSyst[k]->GetXaxis()->SetTitleOffset(1.2);
        
        if (k == 16){
            hSyst[k]->GetYaxis()->SetRangeUser(0.51, 1.49);
            hSyst[k]->GetYaxis()->SetNdivisions(5,5,0);
        }
        else{
            hSyst[k]->GetYaxis()->SetRangeUser(0.81, 1.19);
            hSyst[k]->GetYaxis()->SetNdivisions(5,5,0);
        }
        hSyst[k]->GetYaxis()->SetTitle(" Syst / Central ");
        hSyst[k]->GetYaxis()->SetTitleSize(0.1);
        hSyst[k]->GetYaxis()->SetTitleOffset(0.5);
        hSyst[k]->GetYaxis()->CenterTitle();
        hSyst[k]->GetYaxis()->SetLabelSize(0.08);
        
        hSyst[k]->DrawCopy("E1");
        
        pad2->Draw();
        
        
        string outputDirectory = "PNGFiles/plotUnfoldSyst/";
        string outputFileName = outputDirectory + variable + "Unfolded" + cutNames[k-7] + ".pdf";
        canUnfoldedJES->Print(outputFileName.c_str());
        cout << " save pdf file of " << outputFileName << " " << hSyst[k]->GetName() << " " << cutNames[k-7] << endl;
    }
    

    f->Close();
    
}

////////////////////////////////////////////////////////////////////////////////////



