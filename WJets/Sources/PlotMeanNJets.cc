#include <iostream>
#include <sstream>
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
#include "getFilesAndHistograms.h"

//--  Setting global variables --------------------------------------------------------------
#include "fileNames.h"
//-------------------------------------------------------------------------------------------

using namespace std;

TH2D* get2DHisto(TFile*, string);

void PlotMeanNJets(string leptonFlavor = "Muons", int JetPtMin = 30,
                   int doQCD = 0, bool doSSign = 0, bool doInvMassCut = 0, int MET = 0 , int doBJets = 0 ,
                   int JetPtMax = 0, int ZEtaMin = -999999, int ZEtaMax = 999999,
                   bool doRoch = 0, bool doFlat = 0, bool doVarWidth = 1)
{
    cout << endl << "Running the Plotter with the following files as input: " << "doQCD = " << doQCD << "   " << "METCut = " << MET << "   " << "doBJets = " << doBJets << endl;
    
    
    //set histogram error calculation
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    // sets a global switch disabling the reference (the same as SetDirectory(0) for each histo
    //TH1::AddDirectory(kFALSE);
    //TH2::AddDirectory(kFALSE);
    
    gStyle->SetOptStat(0);
    
    string energy = getEnergy();
    string year = "2012";
    if (energy == "7TeV") year = "2011";
    
    
    //--- initial setting legend names and Colors for Reco plot ---
    int Colors[] = {kBlack, kSpring+5, kOrange, kOrange-3, kRed+1, kPink-6, kViolet+5, kPink, kAzure+4, kBlue, kCyan+1, kCyan+1, kCyan+1};
    string legendNames[] = {" #mu#mu ", " ZZJets2L2Nu", " ZZJets4L", " ZZJets2L2Q",
        " WZJets3LNu", " WZJets2L2Q", " WWJets2L2Nu", " Single Top",
        " DYtautau", " TTJets", " DYJets MD", " DYJets Po", " DYJets Sh"};
    if (leptonFlavor == "Electrons") legendNames[0] = " ee ";
    else if  (leptonFlavor == "Muons") legendNames[0] = " #mu#mu ";
    else if  (leptonFlavor == "SMuE") legendNames[0] = " #mue ";
    else if  (leptonFlavor == "SMu" || leptonFlavor == "Muon" ) legendNames[0] = " #mu#nu ";
    else if  (leptonFlavor == "SE" || leptonFlavor == "Electron") legendNames[0] = " e#nu ";
    
    
    //--- get the files ------------------------------------
    int nFiles = NFILESDYJETS;
    bool isDoubleLep(1);
    if ( leptonFlavor == "SMuE" || leptonFlavor == "SMu" || leptonFlavor == "Muon" || leptonFlavor == "Electron") {
        isDoubleLep = 0;
        nFiles = NFILESTTBAR;
    }
    TFile *file[nFiles];
    int countFiles = 0 ;
    for (unsigned short i = 0; i < nFiles; i++){
        int fileSelect = FilesDYJets[i] ;
        if (!isDoubleLep) fileSelect = FilesTTbarWJets[i] ;
        if ( leptonFlavor == "SMuE"  ) fileSelect = FilesTTbar[i] ;
        
        string fileNameTemp =  ProcessInfo[fileSelect].filename ;
        
        //change the name of QCD file got from ProcessInfo[fileSelect]
        if (fileNameTemp.find("QCD") != string::npos) fileNameTemp = "DataQCD_ForMeanNJets_dR_5311";
        
        cout << "Is double lepton:" << isDoubleLep << "   " << leptonFlavor <<"   " << fileNameTemp << endl;
        
        //prevent crashing when doQCD != 0 by skipping QCD file in FilesTTbarWJets[]
        if ((doQCD > 0 || doInvMassCut || doSSign ) && fileNameTemp.find("QCD") != string::npos) continue;
        
        //getFile
        file[countFiles] = getFile(FILESDIRECTORY, leptonFlavor, energy, fileNameTemp, JetPtMin, JetPtMax, doFlat, doVarWidth, doQCD, doSSign, doInvMassCut, MET, doBJets);
        
        //--- re-setting legend names and Colors for Reco plot ---
        if ( i == 0 ){
            if (leptonFlavor == "Electrons") legendNames[0] = " ee ";
            else if  (leptonFlavor == "Muons") legendNames[0] = " #mu#mu ";
            else if  (leptonFlavor == "SMuE") legendNames[0] = " #mue ";
            else if  (leptonFlavor == "SMu" || leptonFlavor == "Muon" ) legendNames[0] = " #mu#nu ";
            else if  (leptonFlavor == "SE" || leptonFlavor == "Electron") legendNames[0] = " e#nu ";
            legendNames[0] += "Data - Bg";
        }
        else legendNames[countFiles] = ProcessInfo[fileSelect].legend;
        Colors[countFiles] = ProcessInfo[fileSelect].color;
        countFiles++;
    }
    nFiles = countFiles ;
    
    
    //--- create output File ---
    ostringstream JetPtMinStr;  JetPtMinStr << JetPtMin;
    ostringstream JetPtMaxStr;  JetPtMaxStr << JetPtMax;
    ostringstream ZEtaMinStr;   ZEtaMinStr << abs(ZEtaMin);
    ostringstream ZEtaMaxStr;   ZEtaMaxStr << abs(ZEtaMax);
    ostringstream doQCDStr;     doQCDStr << doQCD ;
    ostringstream METStr;       METStr << MET ;
    
    string outputFileName = "PNGFiles/Comparison_" + leptonFlavor + "_" + energy + "_MeanNJets_";
    outputFileName += "JetPtMin_" + JetPtMinStr.str();
    if (JetPtMax > JetPtMin) outputFileName += "_JetPtMax_" + JetPtMaxStr.str();
    if (ZEtaMin > -999999 && ZEtaMin <  0 ) outputFileName += "_ZEtaMin_m" + ZEtaMinStr.str();
    if (ZEtaMin > -999999 && ZEtaMin >= 0 ) outputFileName += "_ZEtaMin_" + ZEtaMinStr.str();
    if (ZEtaMax <  999999 && ZEtaMax >= 0 ) outputFileName += "_ZEtaMax_" + ZEtaMaxStr.str();
    if (ZEtaMax <  999999 && ZEtaMax <  0 ) outputFileName += "_ZEtaMax_m" + ZEtaMaxStr.str();
    if (doRoch) outputFileName += "_rochester";
    if (doFlat) outputFileName += "_Flat";
    if (doVarWidth) outputFileName += "_VarWidth";
    if (doInvMassCut) outputFileName +=  "_InvMass";
    if (doSSign )   outputFileName += "_SS";
    if (doBJets > 0 ) outputFileName += "_BJets";
    if (doBJets < 0 ) outputFileName += "_BVeto";
    if (doQCD>0) outputFileName += "_QCD" + doQCDStr.str();
    if ( MET > 0 ) outputFileName += "_MET"+METStr.str();
    outputFileName += "_SFInvers";
    if (doInvMassCut) outputFileName += "_InvMass";
    //create the directory if it doesn't exist
    string command = "mkdir -p " + outputFileName;
    system(command.c_str());
    string outputFileRoot = outputFileName + ".root";
    
    cout << endl;
    cout << "Output directory is: " << outputFileName << endl;
    cout << "Output root file is: " << outputFileRoot << endl;
    cout << endl;
    
    TFile *outputFile = new TFile(outputFileRoot.c_str(), "RECREATE");
    outputFile->cd();
    //----------------------------
    
    
    //variables to plot
    vector<string> variableMean;
    variableMean.push_back("MeanNJetsHT_Zinc1jet");
    variableMean.push_back("MeanNJetsHT_Zinc2jet");
    variableMean.push_back("MeanNJetsdRapidity_Zinc2jet");
    variableMean.push_back("MeanNJetsdRapidityFB_Zinc2jet");
    
    vector<string> variableMean1DBase;
    variableMean1DBase.push_back("JetsHT_1_Zinc1jet");
    variableMean1DBase.push_back("JetsHT_1_Zinc2jet");
    variableMean1DBase.push_back("dRapidityJets_Zinc2jet");
    variableMean1DBase.push_back("dRapidityJetsFB_Zinc2jet");
    
    //--- Begin Loop ---
    for (int k = 0; k < variableMean.size(); k++){
        
        //--- get Xaxis Title ---
        TH1D *dataHist1DBase;
        dataHist1DBase = getHisto(file[0], variableMean1DBase[k]);
        const char *xTitle = dataHist1DBase->GetXaxis()->GetTitle();
    
        //-- get data histogram ---
        TH2D *dataHist2D = (TH2D*) file[0]->Get(variableMean[k].c_str());
        dataHist2D->SetDirectory(0);
        
        //--- get WJets MC madgraph histogram ---
        TH2D *WJetHist2D = (TH2D*) file[nFiles-1]->Get(variableMean[k].c_str());
        WJetHist2D->SetDirectory(0);
        
        // --- get BG histogram ---
        int nBG(nFiles-2);
        TH2D **BGHist2D = new TH2D*[nBG];
        int countFilesBG = 0;
        for (int i = 0; i < nBG; i++){
            int fileSelect = FilesDYJets[i+1] ;
            if (!isDoubleLep) fileSelect = FilesTTbarWJets[i+1] ;
            if ( leptonFlavor == "SMuE"  ) fileSelect = FilesTTbar[i+1] ;
            
            string fileNameTemp =  ProcessInfo[fileSelect].filename ;
            
            //change the name of QCD file got from ProcessInfo[fileSelect]
            if (fileNameTemp.find("QCD") != string::npos) fileNameTemp = "DataQCD_ForMeanNJets_dR_5311";
            
            cout << "file " << i+1 << " " << fileNameTemp << endl;
            //BGHist2D[i] = (TH2D*) file[i+1]->Get(variableMean[0].c_str());
            BGHist2D[countFilesBG] = get2DHisto(file[i+1], variableMean[k]);
            //cout << "Stop after line " << __LINE__ << endl;
            //BGHist2D[i]->SetDirectory(0);
            countFilesBG++;
        }
        nBG = countFilesBG;
        cout << "nBG = " << nBG << endl;
        cout << endl;
        
        //--- get (data - bg) ---
        TH2D *dataSubBGHist2D = (TH2D*) dataHist2D->Clone();
        for (int i = 0; i < nBG; i++){
            dataSubBGHist2D->Add(BGHist2D[i], -1);
            //cout << "Stop after line " << __LINE__ << endl;
        }
        
        //--- produce projected 1D hist for (Data-Bg) ----
        int nBinsX(dataHist2D->GetNbinsX());
        int nBinsY(dataHist2D->GetNbinsY());
        double xmin = dataHist2D->GetXaxis()->GetXmin();
        double xmax = dataHist2D->GetXaxis()->GetXmax();
        
        const double *xBins = new double[nBinsX+1];
        xBins = dataHist2D->GetXaxis()->GetXbins()->GetArray();
        cout << "k = " << k << " " << "nBinsX = " << nBinsX << endl;
        cout << "k = " << k << " " << "nBinsY = " << nBinsY << endl;
        cout << "k = " << k << " " << "xBins = "  << xBins  << endl;
        
        TH1D* dataSubBGHist1D;
        TH1D* WJetHist1D;
        if (xBins == 0){
            dataSubBGHist1D = new TH1D(variableMean[k].c_str(), variableMean[k].c_str(), nBinsX, xmin, xmax);
            WJetHist1D = new TH1D(variableMean[k].c_str(), variableMean[k].c_str(), nBinsX, xmin, xmax);
        }
        else{
            dataSubBGHist1D = new TH1D(variableMean[k].c_str(), variableMean[k].c_str(), nBinsX, xBins);
            WJetHist1D = new TH1D(variableMean[k].c_str(), variableMean[k].c_str(), nBinsX, xBins);
        }
        
        for (int i=0; i<nBinsX; i++){
            // calculate mean # of jets
            double meanNJets(0);
            double NEvents(0);
            double NJetsTot(0);
            for(int j=0; j<nBinsY; j++){
                double NEventsTemp(0);
                double NJetsTemp(0);
                NEventsTemp = dataSubBGHist2D->GetBinContent(i+1,j+1);
                NEvents = NEvents + NEventsTemp;
                
                NJetsTemp = (dataSubBGHist2D->GetBinContent(i+1,j+1)) * (j+1);
                NJetsTot = NJetsTot + NJetsTemp;
            }
            if (NEvents > 0) {meanNJets = NJetsTot/NEvents;}
            dataSubBGHist1D->SetBinContent(i+1, meanNJets);
            // calculate error
            double errOfMean(0);
            if (NEvents > 0) {
                for(int j=0; j<nBinsY; j++){
                    double errTemp(0);
                    errTemp = (((NEvents * (j+1)) - NJetsTot) / pow(NEvents, 2)) * (dataSubBGHist2D->GetBinError(i+1,j+1));
                    errTemp = pow(errTemp, 2);
                    errOfMean = errOfMean + errTemp;
                }
                errOfMean = sqrt(errOfMean);
            }
            dataSubBGHist1D->SetBinError(i+1, errOfMean);
            
            /*
            double variance(0);
            double ErrorOfMean(0);
            for(int j=0; j<nBinsY; j++){
                double devSquareTemp(0);
                devSquareTemp = (dataSubBGHist2D->GetBinContent(i+1,j+1)) * pow((j+1) - meanNJets, 2);
                variance = variance + devSquareTemp;
            }
            variance = sqrt( variance/(NEvents-1) );
            ErrorOfMean = variance/(sqrt(NEvents));
            */
            /*
            double errNJetsTot(0), errNJetsTotRel(0);
            double errNEvents(0), errNEventsRel(0);
            double errOfMean(0);
            for(int j=0; j<nBinsY; j++){
                double errNJetsTemp(0);
                double errNEventsTemp(0);
                errNJetsTemp = pow( (dataSubBGHist2D->GetBinError(i+1,j+1)) * (j+1), 2 );
                errNJetsTot = errNJetsTot + errNJetsTemp;
                
                errNEventsTemp = pow( dataSubBGHist2D->GetBinError(i+1,j+1), 2 );
                errNEvents = errNEvents + errNEventsTemp;
            }
            errNJetsTot = sqrt(errNJetsTot);
            errNEvents = sqrt(errNEvents);
            
            if (NEvents > 0) errNEventsRel = errNEvents/NEvents;
            else errNEventsRel = 0;
            if (NJetsTot > 0) errNJetsTotRel = errNJetsTot/NJetsTot;
            else errNJetsTotRel = 0;
            errOfMean = meanNJets * sqrt(pow(errNJetsTotRel, 2) + pow(errNEventsRel, 2));
            */
        }
        
        //--- produce projected 1D hist for WJets MC ----
        for (int i=0; i<nBinsX; i++){
            // calculate mean # of jets
            double meanNJets_W(0);
            double NEvents_W(0);
            double NJetsTot_W(0);
            for(int j=0; j<nBinsY; j++){
                double NJetsTemp_W(0);
                double NEventsTemp_W(0);
                NEventsTemp_W = WJetHist2D->GetBinContent(i+1,j+1);
                NEvents_W = NEvents_W + NEventsTemp_W;
                
                NJetsTemp_W = (WJetHist2D->GetBinContent(i+1,j+1)) * (j+1);
                NJetsTot_W = NJetsTot_W + NJetsTemp_W;
            }
            if (NEvents_W > 0) meanNJets_W = NJetsTot_W/NEvents_W;
            WJetHist1D->SetBinContent(i+1, meanNJets_W);
            // calculate error
            double errOfMean_W(0);
            if (NEvents_W > 0) {
                for(int j=0; j<nBinsY; j++){
                    double errTemp_W(0);
                    errTemp_W = (((NEvents_W * (j+1)) - NJetsTot_W) / pow(NEvents_W, 2)) * (WJetHist2D->GetBinError(i+1,j+1));
                    errTemp_W = pow(errTemp_W, 2);
                    errOfMean_W = errOfMean_W + errTemp_W;
                }
                errOfMean_W = sqrt(errOfMean_W);
            }
            WJetHist1D->SetBinError(i+1, errOfMean_W);
            
            /*
            double variance_w(0);
            double ErrorOfMean_w(0);
            for(int j=0; j<nBinsY; j++){
                double devSquareTemp_w(0);
                devSquareTemp_w = (WJetHist2D->GetBinContent(i+1,j+1)) * pow((j+1) - meanNJets_w, 2);
                variance_w = variance_w + devSquareTemp_w;
            }
            variance_w = sqrt( variance_w/(NEvents_w-1) );
            ErrorOfMean_w = variance_w/(sqrt(NEvents_w));
            */
            /*
            double errNJetsTot_W(0), errNJetsTotRel_W(0);
            double errNEvents_W(0), errNEventsRel_W(0);
            double errOfMean_W(0);
            for(int j=0; j<nBinsY; j++){
                double errNJetsTemp_W(0);
                double errNEventsTemp_W(0);
                errNJetsTemp_W = pow( (WJetHist2D->GetBinError(i+1,j+1)) * (j+1), 2 );
                errNJetsTot_W = errNJetsTot_W + errNJetsTemp_W;
                
                errNEventsTemp_W = pow( WJetHist2D->GetBinError(i+1,j+1), 2 );
                errNEvents_W = errNEvents_W + errNEventsTemp_W;
            }
            errNJetsTot_W = sqrt(errNJetsTot_W);
            errNEvents_W = sqrt(errNEvents_W);
            
            if (NEvents_W > 0) errNEventsRel_W = errNEvents_W/NEvents_W;
            else errNEventsRel_W = 0;
            if (NJetsTot_W > 0) errNJetsTotRel_W = errNJetsTot_W/NJetsTot_W;
            else errNJetsTotRel_W = 0;
            errOfMean_W = meanNJets_W * sqrt(pow(errNJetsTotRel_W, 2) + pow(errNEventsRel_W, 2));
            */
        }
        
        
        //--- plotting ---
        TCanvas *canvas;
        TPad *pad1;
        TPad *pad2;
        TLegend *legend;
        TLatex *cmsColl;
        TLatex *jetAlgo;
        TLatex *jetCuts;
        TLatex *intLumi;
        
        double xLowLeg(0.74), xHighLeg(0.78);
        legend = new TLegend(xLowLeg, 0.44, xHighLeg, 0.66);
        legend->SetFillStyle(0);
        legend->SetBorderSize(0);
        legend->SetTextSize(0.032);
        legend->SetTextFont(42);
        
        cmsColl= new TLatex();
        cmsColl->SetTextSize(0.05);
        cmsColl->SetTextFont(62);
        cmsColl->SetLineWidth(2);
        cmsColl->SetTextColor(kBlack);
        cmsColl->SetNDC();
        cmsColl->SetTextAlign(11);
        
        intLumi = new TLatex();
        intLumi->SetTextSize(0.035);
        intLumi->SetTextFont(42);
        intLumi->SetLineWidth(2);
        intLumi->SetTextColor(kBlack);
        intLumi->SetNDC();
        intLumi->SetTextAlign(11);
        
        jetAlgo = new TLatex();
        jetAlgo->SetTextSize(0.035);
        jetAlgo->SetTextFont(42);
        jetAlgo->SetLineWidth(2);
        jetAlgo->SetTextColor(kBlack);
        jetAlgo->SetNDC();
        jetAlgo->SetTextAlign(11);
        
        jetCuts = new TLatex();
        jetCuts->SetTextSize(0.035);
        jetCuts->SetTextFont(42);
        jetCuts->SetLineWidth(2);
        jetCuts->SetTextColor(kBlack);
        jetCuts->SetNDC();
        jetCuts->SetTextAlign(11);
        
        
        dataSubBGHist1D->SetTitle("");
        WJetHist1D->SetTitle("");
        
        dataSubBGHist1D->SetMarkerStyle(20);
        dataSubBGHist1D->SetMarkerColor(Colors[0]);
        dataSubBGHist1D->SetLineColor(Colors[0]);
        
        WJetHist1D->SetMarkerStyle(32);
        WJetHist1D->SetMarkerColor(Colors[nFiles-1]);
        WJetHist1D->SetLineColor(Colors[nFiles-1]);
        
        legend->AddEntry(WJetHist1D, legendNames[nFiles-1].c_str(), "pl");
        legend->AddEntry(dataSubBGHist1D, legendNames[0].c_str(), "pl");
        
        unsigned short nBins(dataSubBGHist1D->GetNbinsX());
        
        canvas = new TCanvas(variableMean[k].c_str(), variableMean[k].c_str(), 700, 900);
        pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1);
        pad1->SetTopMargin(0.055);
        pad1->SetBottomMargin(0.);
        pad1->SetRightMargin(0.04);
        pad1->SetLeftMargin(0.12);
        pad1->SetTicks();
        pad1->SetLogy(0);
        pad1->Draw();
        pad1->cd();
        
        WJetHist1D->Draw("e");
        WJetHist1D->GetYaxis()->SetLabelSize(0.05);
        WJetHist1D->GetYaxis()->SetLabelOffset(0.002);
        WJetHist1D->GetYaxis()->SetLabelFont(42);
        WJetHist1D->GetYaxis()->SetTitle("< N_{jets} >");
        WJetHist1D->GetYaxis()->SetTitleFont(42);
        WJetHist1D->GetYaxis()->SetTitleSize(0.05);
        WJetHist1D->GetYaxis()->SetTitleOffset(1.2);
        //WJetHist1D->SetMinimum(8);
        if (variableMean[k] == "MeanNJetsdRapidity_Zinc2jet") {
            WJetHist1D->SetMaximum(2.45);
            WJetHist1D->SetMinimum(2.05);
        }
        else WJetHist1D->SetMaximum(1.2*WJetHist1D->GetMaximum());
        
        
        /// first pad plots
        dataSubBGHist1D->DrawCopy("e same");
        
        legend->Draw();
        cmsColl->DrawLatex(0.2,0.87, "CMS Preliminary");
        if (energy == "7TeV") intLumi->DrawLatex(0.16,0.82, "#int L dt = 5.05 fb^{-1},  #sqrt{s} = 7 TeV");
        if (energy == "8TeV") intLumi->DrawLatex(0.16,0.82, "#int L dt = 19.6 fb^{-1},  #sqrt{s} = 8 TeV");
        
        ostringstream ptLegend;
        ptLegend << "p_{T}^{jet} > " << JetPtMin << "GeV,  |#eta^{jet}| < 2.4";
        jetCuts->DrawLatex(0.16,0.72, ptLegend.str().c_str());
        jetAlgo->DrawLatex(0.16,0.77, "anti-k_{t} jets,  R = 0.5");
        pad1->Draw();
        
        canvas->cd();
        
        pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.3);
        pad2->SetTopMargin(0.);
        pad2->SetBottomMargin(0.26);
        pad2->SetRightMargin(0.04);
        pad2->SetLeftMargin(0.12);
        pad2->SetGridy();
        pad2->SetTicks();
        pad2->Draw();
        pad2->cd();
        
        dataSubBGHist1D->SetStats(0);
        dataSubBGHist1D->SetTitle("");
        
        dataSubBGHist1D->GetXaxis()->SetTickLength(0.03);
        dataSubBGHist1D->SetXTitle(xTitle);
        dataSubBGHist1D->GetXaxis()->SetTitleSize(0.11);
        dataSubBGHist1D->GetXaxis()->SetTitleOffset(1.05);
        dataSubBGHist1D->GetXaxis()->SetTitleFont(42);
        dataSubBGHist1D->GetXaxis()->SetLabelSize(0.09);
        dataSubBGHist1D->GetXaxis()->SetLabelOffset(0.018);
        dataSubBGHist1D->GetXaxis()->SetLabelFont(42);
        
        //dataSubBGHist1D->GetYaxis()->SetRangeUser(0.51,1.49);
        dataSubBGHist1D->GetYaxis()->SetRangeUser(0.79, 1.21);
        dataSubBGHist1D->GetYaxis()->SetNdivisions(5,5,0);
        dataSubBGHist1D->GetYaxis()->SetTitle("MC / Data");
        dataSubBGHist1D->GetYaxis()->SetTitleFont(42);
        dataSubBGHist1D->GetYaxis()->SetTitleSize(0.12);
        dataSubBGHist1D->GetYaxis()->SetTitleOffset(0.5);
        dataSubBGHist1D->GetYaxis()->CenterTitle();
        dataSubBGHist1D->GetYaxis()->SetLabelSize(0.1);
        dataSubBGHist1D->GetYaxis()->SetLabelFont(42);
        
        dataSubBGHist1D->Divide(WJetHist1D);
        for (unsigned short j(1); j <= nBins; j++){
            double content(dataSubBGHist1D->GetBinContent(j));
            double contentErr(dataSubBGHist1D->GetBinError(j));
            if (content > 0){
                dataSubBGHist1D->SetBinContent(j, 1./content);
                dataSubBGHist1D->SetBinError(j, contentErr/(content * content));
            }
        }
        
        dataSubBGHist1D->DrawCopy("EP");
        canvas->cd();
        
        string outputFilePNG = outputFileName + "/" + variableMean[k] + ".pdf";
        canvas->Print(outputFilePNG.c_str());
        
        string Histo2DNameData = variableMean[k] + "_Data";
        string Histo2DNameWJets = variableMean[k] + "_WJets";
        string Histo2DNameDataSubBG = variableMean[k] + "_DataSubBG";
        
        TCanvas* canvas1 = new TCanvas(Histo2DNameData.c_str(), Histo2DNameData.c_str());
        TCanvas* canvas2 = new TCanvas(Histo2DNameWJets.c_str(), Histo2DNameWJets.c_str());
        TCanvas* canvas3 = new TCanvas(Histo2DNameDataSubBG.c_str(), Histo2DNameDataSubBG.c_str());
        
        canvas1->cd();
        dataHist2D->SetStats(0);//remove stat box
        dataHist2D->Draw("COLZ");
        canvas2->cd();
        WJetHist2D->SetStats(0);
        WJetHist2D->Draw("COLZ");
        canvas3->cd();
        dataSubBGHist2D->SetStats(0);
        dataSubBGHist2D->SetFillColor(kYellow);
        dataSubBGHist2D->Draw("LEGO1 0");
        
        outputFile->cd();
        canvas->Write();
        canvas1->Write();
        canvas2->Write();
        canvas3->Write();
        //dataHist2D->Write();
        //WJetHist2D->Write();
        dataSubBGHist2D->Write();
    }
    //--- end loop --
    
    
    
    outputFile->cd();
    outputFile->Close();
    
    cout << "I m closing the files" << endl;
    //-- Close all the files ------------------------------
    for (unsigned short i(0); i < nFiles; i++) closeFile(file[i]);
    //-----------------------------------------------------
    cout << "Everything went fine" << endl;
    
}


TH2D* get2DHisto(TFile *File, string variable)
{
    TH2D *histo2D = (TH2D*) File->Get(variable.c_str());
    histo2D->SetDirectory(0);
    return histo2D;
}
