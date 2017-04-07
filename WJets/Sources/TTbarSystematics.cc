
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
#include "TTbarSystematics.h"
using namespace std;



void computeTTbarSyst(string leptonFlavor, string var, bool closureTest)
{
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    gStyle->SetOptStat(0);
    gStyle->SetPaintTextFormat("1.2f");
    
    string command = "mkdir -p PNGFiles/TTbarSystematics" ;
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
            if (!isMuon) UnfoldSystTTbar(sel, nsel, leptonFlavor, varStruct[i].name, varStruct[i].log, varStruct[i].decrease, varStruct[i].ESVDkterm, varStruct[i].EBayeskterm);
            if (isMuon)  UnfoldSystTTbar(sel, nsel, leptonFlavor, varStruct[i].name, varStruct[i].log, varStruct[i].decrease, varStruct[i].MuSVDkterm, varStruct[i].MuBayeskterm);
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
            if (!isMuon) UnfoldSystTTbar(sel, nsel, leptonFlavor, varStruct[j].name, varStruct[j].log, varStruct[j].decrease, varStruct[j].ESVDkterm, varStruct[j].EBayeskterm, closureTest);
            if (isMuon)  UnfoldSystTTbar(sel, nsel, leptonFlavor, varStruct[j].name, varStruct[j].log, varStruct[j].decrease, varStruct[j].MuSVDkterm, varStruct[j].MuBayeskterm, closureTest);
        }
        else{
            if (!isMuon) UnfoldSystTTbar(sel, nsel, leptonFlavor, var.c_str(),1,1,5,5);
            if (isMuon)  UnfoldSystTTbar(sel, nsel, leptonFlavor, var.c_str(),1,1,5,5);
        }
    }
    
}


void UnfoldSystTTbar(const int *sel, int nsel, string leptonFlavor, string variable, bool logZ, bool decrease, int SVDkterm, int Bayeskterm, bool closureTest, int JetPtMin, int JetPtMax, bool doFlat, bool doVarWidth)
{
    //--- Get Scale Factor
    TH1D* hisSF = SFAnalyzeTTbar("SMu", variable, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, false, false, 0, 2);
    
    
    
    //--- make sure we have trailing "_" at the end of leptonFlavor
    if (leptonFlavor == "Muons" || leptonFlavor == "DMu") leptonFlavor = "DMu_";
    else if (leptonFlavor == "Electrons" || leptonFlavor == "DE") leptonFlavor = "DE_";
    else if (leptonFlavor == "Muon" || leptonFlavor == "SE") leptonFlavor = "SE_";
    else if (leptonFlavor == "Electron" || leptonFlavor == "SMu") leptonFlavor = "SMu_";
    
    //--- retreive the year from current location and set the energy accordingly
    string energy = getEnergy();
    
    //if (variable.find("JetPt")!= string::npos && variable.find("Highest") == string::npos && JetPtMin == 30)  JetPtMin = 20 ;
    
    ostringstream nameStr;  nameStr << "PNGFiles/TTbarSystematics/TTbarSystematics_";
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
    TH1D *hMeasScaled = (TH1D*) meas->Clone();
    int nBins(hMeas->GetNbinsX());
    for (int i(0); i < nBG; i++) {
        hMeas->Add(hBG[i], -1);
        
        TH1D *ScaledBG = (TH1D*) hBG[i]->Clone();
        if(i == 6){
            cout << "...>>> this is: " << ProcessInfo[sel[i+1]].filename << endl;
            ScaledBG->Multiply(hisSF);
        }
        hMeasScaled->Add(ScaledBG, -1);
    }
    
    
    //-- build the repsonse matrix ------------------------
    string respName = "response" + variable;
    RooUnfoldResponse *response = (RooUnfoldResponse*) DY->Get(respName.c_str());
    response->UseOverflow();
    
    string method = "Bayes";
    cout << "...>>> Start processing Unfolding [Central] using method: " << method << " with kterm: " << Bayeskterm << endl;
    RooUnfoldBayes unfoldC(response, hMeas, Bayeskterm);
    unfoldC.SetVerbose(0);
    TH1D *hUnfoldedC = NULL;
    hUnfoldedC = (TH1D*) unfoldC.Hreco();
    
    cout << "...>>> Start processing Unfolding [TTSyst] using method: " << method << " with kterm: " << Bayeskterm << endl;
    RooUnfoldBayes unfoldT(response, hMeasScaled, Bayeskterm);
    unfoldT.SetVerbose(0);
    TH1D *hUnfoldedT = NULL;
    hUnfoldedT = (TH1D*) unfoldT.Hreco();
    
    
    cout << " save root file of " << nameStr.str().c_str() << endl;
    outputRootFile->cd();
    hUnfoldedC->Write("central");
    hUnfoldedT->Write("TTsyst");
    hisSF->Write("SF");
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
    leg->AddEntry(hSyst, "TTbarSyst", "pl");
    leg->Draw();
    
    cmsColl->DrawLatex(0.11, 0.905, " Systematic: TTbarSyst");
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
    
    string outputDirectory = "PNGFiles/TTbarSystematics/";
    string outputFileName = outputDirectory + variable + "UnfoldedTTbarSyst.pdf";
    canUnfoldedJES->Print(outputFileName.c_str());
    cout << " save pdf file of " << outputFileName << " " << hSyst->GetName() << endl;
    
    
    
    //--- close every files ----------------------------------------------------------
    outputRootFile->Close();
    closeFile(DataFile);
    closeFile(DY);
    for (int i(0); i < nBG; i++) closeFile(BG[i]);
    //-------------------------

}




TH1D* SFAnalyzeTTbar(string leptonFlavor, string variable, int JetPtMin, int JetPtMax, bool doFlat, bool doVarWidth, int doQCD, bool doSSign, bool doInvMassCut, int MET, int doBJets)
{
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    gStyle->SetOptStat(0);
    gStyle->SetPaintTextFormat("1.2f");
    
    cout << "...>>>" << endl;
    cout << "...>>> Processing Data and MC from TTbar Systematic Control Region ; Variable is: " << variable << endl;
   
    
    string energy = getEnergy();
    string HistoFilesDirTT = "HistoFilesTT/";
    
    int nsel = 0;
    const int *sel = NULL;
    if (leptonFlavor == "DMu" || leptonFlavor == "DE") {
        nsel = NFILESDYJETS;
        sel = new int[nsel];
        sel = FilesDYJets;
    }
    else if (leptonFlavor == "SMu" || leptonFlavor == "SE") {
        nsel = NFILESTTBAR;
        sel = new int[nsel];
        sel = FilesTTbarWJets;
    }
    
    
    //--- fetch the files and histograms of the selected variable --------------
    TH1D **histoArr = new TH1D*[nsel];
    for (int i(0); i < nsel; i++) {
        
        TFile *fCurrent;
        fCurrent = getFile(HistoFilesDirTT, leptonFlavor, energy, ProcessInfo[sel[i]].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, doQCD , doSSign,  doInvMassCut, MET, doBJets,  "", "0");
        
        std::cout << "opened :  " << i << "   " << sel[i] << "   " << FilesTTbarWJets[i] << "  " << ProcessInfo[sel[i]].filename << "   " << leptonFlavor.find("SMuE") << std::endl;
        
        histoArr[i] = getHisto(fCurrent, variable);
        // close all input root files
        fCurrent->Close();
    }
    std::cout << "Closed all files" << std::endl;
    
    //--- calculate hMeasTTbar ---------
    TH1D *hMeasTTbar = (TH1D*) histoArr[0]->Clone();
    int nBins(hMeasTTbar->GetNbinsX());
    
    for (int i = 1; i < nsel; i++) {
        if (i == 7) continue;
        hMeasTTbar->Add(histoArr[i], -1);
        cout << " subtracting : " << ProcessInfo[sel[i]].filename << endl;
        cout << hMeasTTbar->GetBinContent(9) << " " << hMeasTTbar->GetBinError(9) << endl;
    }
    
    TH1D *histScaleF = (TH1D*) hMeasTTbar->Clone();
    histScaleF->Divide(histoArr[7]);
    
    // For ZNGoodJets_Zexc the 0-jet and 1-jet bins are irrelevant.
    if (variable == "ZNGoodJets_Zexc") {
        histScaleF->SetBinContent(1, 1.0);
        histScaleF->SetBinError(1, 0.0);
        histScaleF->SetBinContent(2, 1.0);
        histScaleF->SetBinError(2, 0.0);
    }
    
    for (int j = 1; j <= nBins; j++) {
    cout << hMeasTTbar->GetBinContent(j)  << "\t" << hMeasTTbar->GetBinError(j)  << "\t";
    cout << histoArr[7]->GetBinContent(j) << "\t" << histoArr[7]->GetBinError(j) << "\t";
    cout << histScaleF->GetBinContent(j)  << "\t" << histScaleF->GetBinError(j)  << "\t";
    //cout << hMeasTTbar->GetBinContent(j)/histoArr[7]->GetBinContent(j) ;
    cout << endl;
    }
    
    cout << "... >> Scale Factors are obtained ..." << endl;
    cout << "..." << endl;
    
    TCanvas *canSF = new TCanvas("can1", "can1", 900, 600);
    canSF->cd();
    histScaleF->GetYaxis()->SetRangeUser(0.51, 1.49);
    histScaleF->GetYaxis()->SetTitle("Scale Factor");
    histScaleF->SetTitle(("TTbarSyst for " + variable).c_str());
    histScaleF->Draw("E");
    
    string SFplotName = "PNGFiles/TTbarSystematics/SFplots_";
    string fileExtension = ".pdf";
    SFplotName = SFplotName + variable + fileExtension;
    canSF->Print(SFplotName.c_str());
    
    return histScaleF;
    
}




void getStatisticsVar(string leptonFlavor, string variable, int JetPtMin, int JetPtMax, bool doFlat, bool doVarWidth, int doQCD, bool doSSign, bool doInvMassCut, int MET, int doBJets)
{
    string energy = getEnergy();
    string HistoFilesDirTT = "HistoFilesTT/";
    
    std::cout <<" let us get: " <<  variable << " :statistics " << std::endl;
    
    int NBins = 0;
    double DataEv[20][50] = {{0}};
    
    //-- fetch the data files and histograms --------------
    int usedFiles = NFILESTTBAR ;
    bool doDY(0) ;
    if (leptonFlavor.find("Muons") != string::npos ||  leptonFlavor.find("Electrons") != string::npos ) {
        usedFiles = NFILESDYJETS;
        doDY = true;
    }
    
    for (int i(0); i < usedFiles; i++) {
        TFile *fData;
        int sel = i;
        if (doDY) sel = FilesDYJets[i];
        else if (leptonFlavor.find("SMuE") != string::npos) sel = FilesTTbar[i];
        else sel = FilesTTbarWJets[i];
        
        if ((doQCD > 0 || doInvMassCut || doSSign ) && ProcessInfo[sel].filename.find("QCD") != string::npos) continue;
        
        fData = getFile(HistoFilesDirTT,  leptonFlavor, energy, ProcessInfo[sel].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, doQCD , doSSign,  doInvMassCut, MET, doBJets,  "", "0");
        std::cout << "opened :  " << i << "   " << sel << "   " << FilesTTbarWJets[i] << "  " << ProcessInfo[sel].filename << "   " << leptonFlavor.find("SMuE") << std::endl;
        TH1D *hTemp = getHisto(fData, variable);
        
        if (i == 0) NBins = int(hTemp->GetNbinsX());
        
        for (int j = 1 ; j < NBins + 1 ; j++ ){
            Double_t binContent = hTemp->GetBinContent(j);
            DataEv[i][j] = binContent;
            if ( i > 0 ) DataEv[usedFiles][j]+=int(binContent);
        }
        
        // close all input root files
        fData->Close();
    }
    
    std::cout << "Closed all files" << std::endl;
    
    ostringstream nameStr;  nameStr << "PNGFiles/outputTable_" << leptonFlavor << "_JetPtMin_" << JetPtMin;
    if (doInvMassCut) nameStr << "_InvMass";
    if (doSSign)   nameStr << "_SS";
    if (doQCD > 0) nameStr << "_QCD" <<doQCD;
    if (MET > 0) nameStr << "_MET" << MET;
    if (doBJets < 0) nameStr <<"_BVeto";
    else if (doBJets > 0) nameStr << "_BJets_";
    nameStr << variable;
    nameStr << ".tex";
    
    FILE *outFile = fopen(nameStr.str().c_str(), "w");
    string lineOne = "\\footnotesize{\n";
    
    string numColumn = "";
    for (int i = 0; i < NBins; i++){
        numColumn = numColumn + "c";
    }
    string lineTwo = "\\begin{tabular}{l|" + numColumn;
    lineTwo = lineTwo + "} \n";
    
    string lineThree = "";
    string preBin = " &  binth =";
    for (int i = 0; i < NBins; i++){
        ostringstream binth;  binth << i;
        lineThree = lineThree + preBin + binth.str();
    }
    lineThree = lineThree + " \\\\ \\hline \n ";
    
    fprintf( outFile, lineOne.c_str() );
    fprintf( outFile, lineTwo.c_str() );
    fprintf( outFile, lineThree.c_str() );
    
    //// print statistics of all the MC samples
    for (int i=1; i < usedFiles + 1 ; i++){
        int sel = i ;
        if ( doDY ) sel = FilesDYJets[i];
        else sel = FilesTTbarWJets[i];
        
        if (i < usedFiles) fprintf(outFile, " %s        & ", ProcessInfo[sel].legend.c_str());
        else {
            fprintf( outFile, "\\hline \n");
            fprintf( outFile, " TOTAL & ");
        }
        for (int j = 1 ; j < NBins + 1  ; j++ ) {
            if (j < NBins ) fprintf( outFile, "%d & ", int(DataEv[i][j]));
            else fprintf( outFile, "%d \\\\ \n ", int(DataEv[i][j]));
        }
        std::cout << std::endl;
    }
    
    // print data statistics
    std:: cout << "MC" <<  "\t\t" << "Data" << std::endl;
    fprintf( outFile, "\\hline \n");
    fprintf( outFile, " Data          & ");
    for (int j = 1; j< NBins + 1 ; j++){
        if (j < NBins ) fprintf( outFile, "%d & ",  int(DataEv[0][j]));
        else fprintf( outFile, "%d \\\\ \n ",  int(DataEv[0][j]));
    }
    // print ratio of MC/data
    
    fprintf( outFile, " Ratio          & ");
    for (int j=1; j< NBins + 1; j++){
        double temp = DataEv[usedFiles][j]/DataEv[0][j];
        std:: cout << DataEv[usedFiles][j] << "\t\t" << DataEv[0][j] << std::endl;
        if (j < NBins) fprintf( outFile, "%f & ", float(temp));
        else fprintf( outFile, "%f \\\\ \n ",temp);
        
    }
    fprintf( outFile, "\\end{tabular}}");
    fclose(outFile);
}



