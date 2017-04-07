#include <TFile.h>
#include <TStyle.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TPaveStats.h>
#include <TLegend.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "writeFastPlotsTex.h"
#include "getFilesAndHistograms.h"
#include "Systematics.h"


using namespace std;

//--  Setting global variables -----------------------------
#include "fileNames.h"
#include "variablesOfInterestVarWidth.h"
//----------------------------------------------------------

void Systematics()
{
    //NVAROFINTEREST = 1;
    for (int i(1); i < 2; i++){
        cout << VAROFINTEREST[i].name << endl;
        systematics(VAROFINTEREST[i].name, VAROFINTEREST[i].log, VAROFINTEREST[i].decrease);
    }

    //-- for testing on specific variable
    //makeLISTOFVAROFINTEREST();
    //TObjString *name = new TObjString("ZNGoodJets_Zexc");
    //int index(LISTOFVAROFINTEREST->IndexOf(name));
    //systematics(VAROFINTEREST[index].name, VAROFINTEREST[index].log, VAROFINTEREST[i].decrease);
}

void systematics(string variable, bool log, bool decrease, string leptonFlavor, int JetPtMin, int JetPtMax, bool doFlat, bool doVarWidth)
{
    string energy = getEnergy();
    gStyle->SetOptStat(0);
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();

    ostringstream JetPtMinStr;  JetPtMinStr << JetPtMin;

    string outputRootFileName = "PNGFiles/SystematicsPlots_" + JetPtMinStr.str() + "/" + leptonFlavor + "_" + energy + "_" + variable + "_Systematics.root"; 
    TFile *outputFile = new TFile(outputRootFileName.c_str(), "RECREATE");

    //-- files and histograms for each systematics --------
    TFile *fData[3];  TH1D *hData[3];   // 0 = central, 1 = JES Up, 2 = JES Down
    TFile *fDY[4];    TH1D *hDY[4];     // 0 = central, 1 = PU Up,  2 = PU Down,  3 = JER Up
    TFile *fBG[9][5]; TH1D *hBG[9][5];  // 0 = central, 1 = PU Up,  2 = PU Down,  3 = XS Up,  4 = XS Down 
    TH1D *hSumBG[5], *hSumMC[6];
    //-----------------------------------------------------

    //-- fetch the files and histograms --------------------
    getFiles(FILESDIRECTORY, fData, leptonFlavor, energy, ProcessInfo[DATAFILENAME].filename, JetPtMin, JetPtMax, doFlat, doVarWidth);
    getHistos(hData, fData, variable);
    getFiles(FILESDIRECTORY, fDY, leptonFlavor, energy, ProcessInfo[DYMADGRAPHFILENAME].filename, JetPtMin, JetPtMin, doFlat, doVarWidth);
    getHistos(hDY, fDY, variable);
    for (int i(0); i < 9; i++){
        getFiles(FILESDIRECTORY, fBG[i], leptonFlavor, energy, BGFILENAMES[i], JetPtMin, JetPtMax, doFlat, doVarWidth);
        getHistos(hBG[i], fBG[i], variable);
        for (int j(0); j < 5; j++){
            if (i == 0 ) hSumBG[j] = (TH1D*) hBG[i][j]->Clone();
            else if (i != 7) hSumBG[j]->Add(hBG[i][j]);
        }
    }
    //-----------------------------------------------------

    //-- Sum up DY and Backgrounds ------------------------
    hSumMC[0] = (TH1D*) hDY[0]->Clone(); hSumMC[0]->Add(hSumBG[0]);  // central DY + Background
    hSumMC[1] = (TH1D*) hDY[1]->Clone(); hSumMC[1]->Add(hSumBG[1]);  // PU Up DY + Background
    hSumMC[2] = (TH1D*) hDY[2]->Clone(); hSumMC[2]->Add(hSumBG[2]);  // PU Down DY + Background
    hSumMC[3] = (TH1D*) hDY[0]->Clone(); hSumMC[3]->Add(hSumBG[3]);  // central DY + XS Up Background
    hSumMC[4] = (TH1D*) hDY[0]->Clone(); hSumMC[4]->Add(hSumBG[4]);  // central DY + XS Up Background
    hSumMC[5] = (TH1D*) hDY[3]->Clone(); hSumMC[5]->Add(hSumBG[0]);  // DY JER + central Background
    //-----------------------------------------------------


    //-- Plot each systemacis effect ----------------------
    plotSystematics(JetPtMin, "JES", hData[0],  hData[1],  hData[2],  outputFile, leptonFlavor, energy, log, decrease);
    plotSystematics(JetPtMin, "PU",  hSumMC[0], hSumMC[1], hSumMC[2], outputFile, leptonFlavor, energy, log, decrease);
    plotSystematics(JetPtMin, "XS",  hSumMC[0], hSumMC[3], hSumMC[4], outputFile, leptonFlavor, energy, log, decrease);
    plotSystematics(JetPtMin, "JER", hSumMC[0], hSumMC[5], hSumMC[5], outputFile, leptonFlavor, energy, log, decrease);
    //-----------------------------------------------------

    //-- Close all the files ------------------------------
    closeFiles(fData);
    closeFiles(fDY);
    for (int i(0); i < 9; i++) closeFiles(fBG[i]);
    //-----------------------------------------------------

}

void plotSystematics(int JetPtMin, string Syst, TH1D *hcent, TH1D *hup, TH1D *hdown, TFile *outputFile, string leptonFlavor, string energy, bool log, bool decrease)
{
    ostringstream JetPtMinStr;  JetPtMinStr << JetPtMin;
    string legName;
    if      (Syst == "JES") legName = "Systematic : JES";
    else if (Syst == "PU")  legName = "Systematic : PU";
    else if (Syst == "XS")  legName = "Systematic : XS";
    else if (Syst == "JER") legName = "Systematic : JER";

    hcent->SetLineColor(kBlack);
    hcent->SetLineWidth(2);
    hcent->SetFillStyle(3002);
    hcent->SetFillColor(kGray+2);
    hup->SetMarkerStyle(26);
    hup->SetMarkerSize(1.5);
    hup->SetMarkerColor(kGreen+2);
    hup->SetLineColor(kGreen+2);
    hup->SetLineWidth(2);
    if (Syst != "JER"){
        hdown->SetMarkerStyle(32);
        hdown->SetMarkerSize(1.5);
        hdown->SetMarkerColor(kRed+1);
        hdown->SetLineColor(kRed+1);
        hdown->SetLineWidth(2);
    }

    double xLowLeg(0.73), xHighLeg(0.99);
    if (!decrease){ 
        xLowLeg = 0.15;
        xHighLeg = 0.41;
    }
    TLegend *leg = new TLegend(xLowLeg, 0.61, xHighLeg, 0.9);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetHeader(legName.c_str());
    leg->AddEntry(hup,   "Syst. Up",   "pl");
    leg->AddEntry(hcent, "Central ",   "fl");
    if (Syst != "JER") leg->AddEntry(hdown, "Syst. Down", "pl");

    string canName = hcent->GetName();
    canName += "_" + Syst;
    TCanvas *can = new TCanvas(canName.c_str(), canName.c_str(), 700, 900); 
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1);
    pad1->SetTopMargin(0.11);
    pad1->SetBottomMargin(0.);
    pad1->SetRightMargin(0.05);
    if (log) pad1->SetLogy();
    pad1->Draw();
    pad1->cd();

    hcent->GetYaxis()->SetTitleSize(0.05);
    hcent->DrawCopy("HIST");
    hup->DrawCopy("same");
    if (Syst != "JER") hdown->DrawCopy("same");
    leg->Draw();
    pad1->Draw();

    can->cd();
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.3);
    pad2->SetTopMargin(0.);
    pad2->SetBottomMargin(0.3);
    pad2->SetRightMargin(0.05);
    pad2->SetGridy();
    pad2->SetTicks();
    pad2->Draw();
    pad2->cd();

    hup->Divide(hcent); 
    if (Syst != "JER") hdown->Divide(hcent); 
    int nBin = hup->GetNbinsX();
    float diff(0), minDiff(99999), maxDiff(0);
    for (int i = 1; i < nBin+1; i++){
        if (Syst != "JER") diff = 0.5*fabs(hup->GetBinContent(i) - hdown->GetBinContent(i));
        else diff = fabs(hcent->GetBinContent(i) - hup->GetBinContent(i)); 
        minDiff = diff > 0 ? ( minDiff < diff ? minDiff : diff)   : minDiff;
        maxDiff = maxDiff > diff ? maxDiff : diff;
    }

    //hup->GetYaxis()->SetRangeUser(1-2*maxDiff, 1+2*maxDiff);
    hup->GetYaxis()->SetRangeUser(0.81, 1.19);
    hup->GetYaxis()->SetTitle("Ratio to Central");
    hup->GetYaxis()->CenterTitle();
    hup->GetYaxis()->SetTitleSize(0.09);
    hup->GetYaxis()->SetTitleOffset(0.55);
    hup->GetYaxis()->SetLabelSize(0.07);
    hup->GetYaxis()->SetLabelOffset(0.009);
    hup->SetTitle("");
    hup->GetXaxis()->SetTitleSize(0.1);
    hup->GetXaxis()->SetTitleOffset(1.15);
    hup->GetXaxis()->SetLabelSize(0.1);
    hup->GetXaxis()->SetLabelOffset(0.01);
    hup->DrawCopy("E1 P");
    if (Syst != "JER") hdown->DrawCopy("E1 P SAME");
    pad2->Draw();
    can->cd();


    string output = "./PNGFiles/SystematicsPlots_" + JetPtMinStr.str() + "/";
    string command = "mkdir -p " + output;
    system(command.c_str());
    if (leptonFlavor == "Muons") output += "DMu_";
    else output += "DE_";
    output += energy + "_" + canName;
    output += ".png";
    can->Print(output.c_str());  
    outputFile->cd();
    can->Write();

}



