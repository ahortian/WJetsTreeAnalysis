#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <vector>
#include <string.h>

#include <TROOT.h>
#include <TFile.h>
#include <TDirectory.h>

#include <TH1.h>
#include <TH2.h>
#include <TKey.h>
#include <TList.h>
#include <TMatrixD.h>
#include <TLorentzVector.h>

#include <RooUnfoldResponse.h>
#include <RooUnfold.h>
#include <RooUnfoldBayes.h>
#include <RooUnfoldSvd.h>
#include <RooUnfoldBinByBin.h>
#include <RooUnfoldInvert.h>
#include <TSVDUnfold.h>

#include <TCanvas.h>
#include <TPad.h>
#include <TLegend.h>
#include <TGraph.h>
#include <TPaveStats.h>
#include <TText.h>
#include <THStack.h>
#include <TLatex.h>
#include <TStyle.h>
#include <TDatime.h>
#include <TRandom3.h>
#include <TMath.h>

#include "getFilesAndHistograms.h"
#include "myFinalUnfoldEdit.h"

using namespace std;

//--  Setting global variables --------------------------------------------------------------
#include "fileNames.h"
#include "variablesOfInterestVarWidth.h"
TRandom3* RandGen = new TRandom3();

const int NDatSyst(3);
const int NDYJSyst(14);
const int NBkgSyst(12);
const int NCaseSyst(18);
//-------------------------------------------------------------------------------------------

TH1D* getSystematicMC(int myNToys = 1000, TH1D* hMeas = NULL, RooUnfoldResponse* response = NULL, int kterm = 4);
TH1D* getErrorMeasToy(int myNToys = 1000, TH1D* hMeas = NULL, RooUnfoldResponse* response = NULL, int kterm = 4);

TH1D* Unfold(string unfAlg, RooUnfoldResponse* response, TH1D* hData, TH1D* hSumBG, int Kterm, string hOutName, bool useOverFlow = true);

void getFilesNew(string histoFilesDirectory, TFile *Files[], string leptonFlavor, string energy, string Name, int JetPtMin, int JetPtMax, bool doFlat, bool doVarWidth, int doQCD, bool doSSign, bool doInvMassCut, int MET, int doBJets, bool useUnfoldingFiles);
void closeFilesNew(TFile *Files[]);
void getHistosNew(TH1D *histograms[], TFile *Files[], string variable, bool isDoubleLep);

void getRespsNew(RooUnfoldResponse *responses[], TFile *Files[], string variable, string genName);
RooUnfoldResponse* getResp(TFile *File, string variable, TH1D *recoMad, TH1D *genMad);

TH1D* SFAnalyzeTTbar(string leptonFlavor, string variable, int JetPtMin, int JetPtMax, bool doFlat, bool doVarWidth, int doQCD, bool doSSign, bool doInvMassCut, int MET, int doBJets);

void setNegBinZero(TH1D *histograms1D);

TH1D* getHistoRespSystematics(string leptonFlavor, string variable, int JetPtMin, int JetPtMax, bool doFlat, bool doVarWidth, int doQCD, bool doSSign, bool doInvMassCut, int MET, int doBJets, int Bayeskterm);

TH2D* get2DHisto(TFile *File, string variable);




void myFinalUnfoldEdit(int start, int end, string leptonFlavor, string unfAlg)
{
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    
    //--- check whether muon or electron is used ---
    bool isMuon (0);
    if (leptonFlavor.find("Mu") != string::npos) isMuon = 1;
    cout << " Are you doing Muon? : " << isMuon << endl;
    //---------------------------------------------
    
    if (end == -1) end = start + 1;
    for (int i(start); i < end /*NVAROFINTERESTZJETS*/; i++) {
        
        if (leptonFlavor == "SMu" || leptonFlavor == "SE"){
                if (isMuon) FuncUnfold(leptonFlavor, isMuon, unfAlg, VAROFINTERESTWJETS[i].name, VAROFINTERESTWJETS[i].MuBayeskterm, VAROFINTERESTWJETS[i].MuSVDkterm);
                else        FuncUnfold(leptonFlavor, isMuon, unfAlg, VAROFINTERESTWJETS[i].name, VAROFINTERESTWJETS[i].EBayeskterm,  VAROFINTERESTWJETS[i].ESVDkterm);
        }
        /*
            else if (leptonFlavor == "DMu" || leptonFlavor == "DE"){
                if (isMuon) FuncUnfold(leptonFlavor, isMuon, unfAlg, VAROFINTERESTZJETS[i].name, VAROFINTERESTZJETS[i].MuBayeskterm, VAROFINTERESTZJETS[i].MuSVDkterm);
                else FuncUnfold(leptonFlavor, isMuon, unfAlg, VAROFINTERESTZJETS[i].name, VAROFINTERESTZJETS[i].EBayeskterm, VAROFINTERESTZJETS[i].ESVDkterm);
            }
        */
    }
}


void FuncUnfold(string leptonFlavor, bool isMuon, string unfAlg, string variable, int UsedKtermBayes, int UsedKtermSVD, bool doFlat, bool doVarWidth)
{
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    
    int JetPtMin(30);
    int JetPtMax(0);
    
    string energy = getEnergy();
    string outputDirectory = "PNGFiles/FinalUnfold/";
    string command = "mkdir -p " + outputDirectory;
    system(command.c_str());
    
    //--- set number of toys used in getErrorRespToy()
    int NumberOfToys(2000), oppNumberOfToys(4);
    int UsedKterm = UsedKtermBayes, oppUsedKterm = UsedKtermSVD;
    string oppUnfAlg = "SVD";
    if (unfAlg == "SVD") {
        oppUnfAlg = "Bayes";
        NumberOfToys = 4;
        oppNumberOfToys = 2000;
        UsedKterm = UsedKtermSVD;
        oppUsedKterm = UsedKtermBayes;
    }
    
    /*
    //====== set efficiency errors ===============
    double muonIDIsoHLTError = 0.025;// 2012 : id,iso = 0.005, 0.002 --> 1.4% for dimuon + HLT: 2% ??????
    double electronIDIsoHLTError = 0.005; // SF factors for 2012: 0.002
    double EffError = muonIDIsoHLTError;
    if (!isMuon) EffError = electronIDIsoHLTError;
    //--------------------------------------------
    */
    cout << " We now unfold:  " << variable << " with jet pt cut of:" << JetPtMin << " - " << JetPtMax << endl;
    
    //-- fetch the data files and histograms --------------
    cout << " Fetch Data files " << endl;
    TFile *fData[NDatSyst] = {NULL};
    TH1D *hData[NDatSyst] = {NULL};
    getFilesNew(FILESDIRECTORY, fData, leptonFlavor, energy, ProcessInfo[DATAFILENAME].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, -1, false);
    getHistosNew(hData, fData, variable, false);
    
    
    //-- fetch the WJets files and histograms ----------------
    cout << " Fetch WJets files " << endl;
    TFile *fDYMadGraph[NDYJSyst] = {NULL};
    TH1D *hDY[NDYJSyst] = {NULL}; // Reco Hist
    TH1D *hDYGenMadGraph = NULL; // GenMadGraph Hist
    
    getFilesNew(FILESDIRECTORY, fDYMadGraph, leptonFlavor, energy, ProcessInfo[24].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, -1, false);
    getHistosNew(hDY, fDYMadGraph, variable, false); // Reco Wjets Hist => actually these are not used
    string genName = "gen" + variable;
    hDYGenMadGraph = getHisto(fDYMadGraph[0], genName);
    cout << "line th " << __LINE__ << endl;
    //-----------------------------------------------------
    
    
    //--- Get MadGraph Resp Matrix ------------------------
    RooUnfoldResponse *resDY[NDYJSyst] = {NULL};
    getRespsNew(resDY, fDYMadGraph, variable, genName);
    cout << "line th " << __LINE__ << endl;
    //-----------------------------------------------------
    
    
    //-- fetch the BG files and histograms ----------------
    cout << " Fetch background files " << endl;
    int nFilesBkg(0);
    
    TFile *fBG[15][NBkgSyst] = {{NULL}};
    TH1D *hBG[15][NBkgSyst] = {{NULL}}, *hSumBG[NBkgSyst] = {NULL};
    
    int nFiles = NFILESDYJETS; // from "Includes/fileNames.h"
    bool isDoubleLep(1);
    if (leptonFlavor == "SMuE" || leptonFlavor == "SMu" || leptonFlavor == "Muon") {
        isDoubleLep = 0;
        //nFiles = NFILESTTBAR; //NFILESTTBAR(9);
        nFiles = NFILESWJETS_M;
    }
    
    int countFiles = 0;
    
    for (int i(0); i < nFiles; i++){
        int fileSelect = FilesDYJets[i];
        //if (!isDoubleLep) fileSelect = FilesTTbarWJets[i]; //FilesTTbarWJets[NFILESTTBAR] = {0, 4, 3, 2, 23, 20, 22, 11, 24};
        if (!isDoubleLep) fileSelect = FilesWJets_M[i];
        string fileNameTemp = ProcessInfo[fileSelect].filename;
        
        // Tau is not considered as background. It is taken into account by the unfolding
        if (fileNameTemp.find("Tau") != string::npos) continue;
        
        if ((fileNameTemp.find("Data") != string::npos) && (fileNameTemp.find("QCD") == string::npos)) continue;
        //if (fileNameTemp.find("Data") != string::npos) continue;
        
        // DYJets is not a background, it is the signal (for double lepton)
        if (isDoubleLep && fileNameTemp.find("DYJets") != string::npos) continue;
        
        // WJets is not a background, it is the signal (for single lepton)
        if (!isDoubleLep && fileNameTemp.find("WJets") != string::npos && fileNameTemp.find("UNF") != string::npos) continue;
        
        nFilesBkg++;
        
        //--- fetch the BG file and histos ---
        getFilesNew(FILESDIRECTORY, fBG[countFiles], leptonFlavor, energy, fileNameTemp, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, -1, false);
        getHistosNew(hBG[countFiles], fBG[countFiles], variable, false);
        //------------------------------------
        
        cout << " Adding 1D Background of " << fileNameTemp << endl;
        for (int j(0); j < NBkgSyst; j++){
            if (countFiles == 0) {
                hSumBG[j] = (TH1D*) hBG[countFiles][j]->Clone();
            }
            else {
                hSumBG[j]->Add(hBG[countFiles][j]);
            }
            cout << " : Integral : " << hSumBG[j]->Integral() << endl;
        }
        countFiles++;
    }
    
    //------ End fetch the BG files and histograms ------------------
    
    cout << " --- loaded all files: 1 Data : 1 Wjets : " <<  nFilesBkg << " Background files" << endl;
    
    
    
    //------ set the output
    string outputRootFileName = outputDirectory + leptonFlavor + "_" + energy + "_unfolded_" + variable + "_histograms_" + unfAlg;
    if (doVarWidth) outputRootFileName += "_VarWidth";
    outputRootFileName += ".root";
    TFile* outputRootFile = new TFile(outputRootFileName.c_str(), "RECREATE");
    
    // FIXME : buitd a function to write the reco information into output file.
    //-- save data reco
    outputRootFile->cd(); hData[0]->Write("Data"); hData[1]->Write("DataJESup"); hData[2]->Write("DataJESdown");
    //-- save SumBG reco
    outputRootFile->cd(); hSumBG[0]->Write("RecoBg"); hSumBG[1]->Write("RecoBgPUup"); hSumBG[2]->Write("RecoBgPUdown");
                          hSumBG[3]->Write("RecoBgXSECup"); hSumBG[4]->Write("RecoBgXSECdown");
    //-- save WJETs reco
    outputRootFile->cd(); hDY[0]->Write("RecoWJETs"); hDY[1]->Write("RecoWJETsPUup"); hDY[2]->Write("RecoWJETsPUdown");
                          hDY[3]->Write("RecoWJETsJERup");
    //-- save madgraph gen also in the file
    outputRootFile->cd(); hDYGenMadGraph->Write("genMad");
    cout << " saved data reco and madgraph files" << endl;
    
    // unfolding 15 different cases
    cout << "------ Start unfolding with kterm : " << UsedKterm << endl;
    string hNames[NCaseSyst] = {"Central", "JESup", "JESdown", "PUup", "PUdown", "XSECup", "XSECdown", "JERup", "JERdown", "LepSFup", "LepSFdown", "BtagEFFup", "BtagEFFdown", "MESup", "MESdown", "MER", "WB", "RESP"};
    int SelData[NCaseSyst] = {0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0};
    int SelDY[NCaseSyst]   = {0, 0, 0, 1, 2, 0, 0, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    int SelBG[NCaseSyst]   = {0, 0, 0, 1, 2, 3, 4, 0, 0, 5, 6, 7, 8, 9, 10, 11,  0,  0};
    for ( int i(0); i < NCaseSyst; i++){
        if ( hNames[i] == "RESP" && variable.find("MeanNJets") == string::npos ) {
            cout << "--- ignore syst files for Resp --- " << endl;
            continue; // we will calculate this from central
        }
        cout << "--- Start unfolding histograms for " << hNames[i] <<endl;
        bool useOverFlow = true;
        TH1D* hUnfolded = Unfold(unfAlg, resDY[SelDY[i]], hData[SelData[i]], hSumBG[SelBG[i]],  UsedKterm, hNames[i], useOverFlow);
        outputRootFile->cd();  hUnfolded->Write();
    }
    
    
    //----- get SystematicMC (MC)
    TH1D *hMeas = (TH1D*) hData[0]->Clone();
    hMeas->Add(hSumBG[0], -1);
    TH1D* hSystematicMC = getSystematicMC(NumberOfToys, hMeas, resDY[0], UsedKterm);
    outputRootFile->cd();
    hSystematicMC->Write("MC");
    //----- End get (MC) ----------
    
    
    //----- get uncertainty contribution from (ttbar) modeling
    // Get Scale Factor
    TH1D* hisSF = SFAnalyzeTTbar("SMu", variable, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, false, false, 0, 2);
    cout << "Start unfolding histograms on : " << "Syst TTBAR" << endl;
    TH1D *hMeasForTT = (TH1D*) hData[0]->Clone();
    TH1D *hSumBGForTT = NULL;
    for (int i(0); i < nFilesBkg; i++){
        
        if (i == 0) hSumBGForTT = (TH1D*) hBG[0][0]->Clone();
        else if (i == 10){ // index 10 in hBG is ttbar (index 11 in  FilesWJets_M[i])
            TH1D *TempScaledBG = (TH1D*) hBG[i][0]->Clone();
            TempScaledBG->Multiply(hisSF);
            hSumBGForTT->Add(TempScaledBG);
        }
        else hSumBGForTT->Add(hBG[i][0]);
    }
    hMeasForTT->Add(hSumBGForTT, -1);
    setNegBinZero(hMeasForTT); // set negative bin to zero
    TH1D *hUnfoldedCtt = NULL;
    RooUnfoldBayes unfoldCtt(resDY[0], hMeasForTT, UsedKterm);
    unfoldCtt.SetVerbose(0);
    hUnfoldedCtt = (TH1D*) unfoldCtt.Hreco();
    outputRootFile->cd(); hUnfoldedCtt->Write("TTBAR");
    //----- End get uncertainty (ttbar) --------------------
    
    
    //-- Close all the input files ------------------------
    closeFilesNew(fData);
    closeFilesNew(fDYMadGraph);
    for (int i(0); i < nFilesBkg; i++) closeFilesNew(fBG[i]);
    //-----------------------------------------------------
    
    
    //----- get uncertainty contribution for Resp
    if (variable.find("ZNGoodJets") == string::npos){
        TH1D* histRespSystematics = getHistoRespSystematics("SMu", variable, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, false, false, 0, -1, UsedKterm);
        outputRootFile->cd();
        histRespSystematics->Write("RESP");
    }
    //----- End get uncertainty (Resp) ----------
    
    
    ///------
    closeFile(outputRootFile);
}





void getRespsNew(RooUnfoldResponse *responses[], TFile *Files[], string variable, string genName)
{
    string fileName = Files[0]->GetName();
    int nFiles;
    if ((fileName.find("Data") != string::npos) && (fileName.find("QCD") == string::npos)) nFiles = NDatSyst;
    else if (fileName.find("UNFOLDING") != string::npos) nFiles = NDYJSyst;
    else nFiles = NBkgSyst; // for BG
    
    for (int i(0); i < nFiles; i++){
        TH1D *recoMad = (TH1D*) Files[i]->Get(variable.c_str());
        TH1D *genMad  = (TH1D*) Files[i]->Get(genName.c_str());
        responses[i] = (RooUnfoldResponse*) getResp(Files[i], variable, recoMad, genMad);
        
        if (variable.find("MeanNJets") == string::npos) responses[i]->UseOverflow();
        
        cout << "Getting Response with Dimension (Mea x Truth): " << responses[i]->GetDimensionMeasured() << " x " << responses[i]->GetDimensionTruth() << endl;
    }
}

RooUnfoldResponse* getResp(TFile *File, string variable, TH1D *recoMad, TH1D *genMad)
{
    string hrespName = "hresponse" + variable;
    TH2D *hresponse = (TH2D*) File->Get(hrespName.c_str());
    RooUnfoldResponse *response = new RooUnfoldResponse(recoMad, genMad, hresponse);
    return response;
}

void getFilesNew(string histoFilesDirectory, TFile *Files[], string leptonFlavor, string energy, string Name, int JetPtMin, int JetPtMax, bool doFlat, bool doVarWidth, int doQCD, bool doSSign, bool doInvMassCut, int MET, int doBJets, bool useUnfoldingFiles)
{
    
    //--- make sure leptonFlavor is short version ---
    if (leptonFlavor == "Muons" || leptonFlavor == "DMu_") leptonFlavor = "DMu";
    else if (leptonFlavor == "Electrons" || leptonFlavor == "DE_") leptonFlavor = "DE";
    else if (leptonFlavor == "Electron" || leptonFlavor == "SE_") leptonFlavor = "SE";
    else if (leptonFlavor == "Muon" || leptonFlavor == "SMu_") leptonFlavor = "SMu";
    else if (leptonFlavor == "MuonElectron" || leptonFlavor == "SMuE_") leptonFlavor = "SMuE";
    //-----------------------------------------------
    
    //--- set the double lepton flag ---
    bool isDoubleLep(0);
    if (leptonFlavor == "DMu" || leptonFlavor == "DE") isDoubleLep = 1;
    //----------------------------------
    
    vector<string> Syst;
    if ((Name.find("Data") != string::npos) && (Name.find("QCD") == string::npos)) { // for data we have:
        Syst.push_back("0");                 //   0: central
        Syst.push_back("2_Up");              //   2 up: JES up
        Syst.push_back("2_Down");            //   2 down: JES down
    }
    else if (Name.find("UNFOLDING") != string::npos && ((isDoubleLep && Name.find("DYJets") != string::npos) || (!isDoubleLep && Name.find("WJets") != string::npos))) {
        // for DYJets in case of Z+Jets or for WJets in case of W+Jets analysis we have:
        Syst.push_back("0");         // 0:      central
        Syst.push_back("1_Up");      // 1 up:   PU up
        Syst.push_back("1_Down");    // 1 down: PU down
        Syst.push_back("4_Up");      // 4 up:   JER up
        Syst.push_back("4_Down");    // 4 down: JER down
        Syst.push_back("5_Up");      // 5 up:   LepSF up
        Syst.push_back("5_Down");    // 5 down: LepSF down
        Syst.push_back("6_Up");      // 6 up:   BtagEff up
        Syst.push_back("6_Down");    // 6 down: BtagEff down
        Syst.push_back("7_Up");      // 7 up:   MES up
        Syst.push_back("7_Down");    // 7 down: MES down
        Syst.push_back("8_Up");      // 8 up:   MER
        Syst.push_back("9_Up");      // 9 up:   Wb
        Syst.push_back("10_Up");     // 10 up:  Resp
    }
    else { // for background we have
        Syst.push_back("0");         // 0:      central
        Syst.push_back("1_Up");      // 1 up:   PU up
        Syst.push_back("1_Down");    // 1 down: PU down
        Syst.push_back("3_Up");      // 3 up:   XSec up
        Syst.push_back("3_Down");    // 3 down: Xsec down
        Syst.push_back("5_Up");      // 5 up:   LepSF up
        Syst.push_back("5_Down");    // 5 down: LepSF down
        Syst.push_back("6_Up");      // 6 up:   BtagEff up
        Syst.push_back("6_Down");    // 6 down: BtagEff down
        Syst.push_back("7_Up");      // 7 up:   MES up
        Syst.push_back("7_Down");    // 7 down: MES down
        Syst.push_back("8_Up");      // 8 up:   MER
    };
    
    //--- determnie how many files we have and open them all ---
    int nSyst(Syst.size());
    for (int i(0); i < nSyst; i++) {
        Files[i] = getFile(histoFilesDirectory, leptonFlavor, energy, Name, JetPtMin, JetPtMax, doFlat, doVarWidth, doQCD, doSSign, doInvMassCut, MET, doBJets, "", Syst[i], false, useUnfoldingFiles);
    }
    //----------------------------------------------------------
}

void closeFilesNew(TFile *Files[])
{
    if (Files[0]) {
        string fileName = Files[0]->GetName();
        int nFiles;
        if ((fileName.find("Data") != string::npos) && (fileName.find("QCD") == string::npos)) nFiles = NDatSyst;
        else if (fileName.find("UNFOLDING") != string::npos) nFiles = NDYJSyst;
        else nFiles = NBkgSyst; // for BG
        
        for (int i(0); i < nFiles; i++){
            closeFile(Files[i]);
        }
    }
}

void getHistosNew(TH1D *histograms[], TFile *Files[], string variable, bool isDoubleLep)
{
    string fileName = Files[0]->GetName();
    int nFiles;
    if ((fileName.find("Data") != string::npos) && (fileName.find("QCD") == string::npos)) nFiles = NDatSyst;
    else if (((isDoubleLep && fileName.find("DYJets") != string::npos) || (!isDoubleLep && fileName.find("WJets") != string::npos)) && fileName.find("UNFOLDING") != string::npos) nFiles = NDYJSyst;
    else nFiles = NBkgSyst; // for BG
    
    cout << "  " << nFiles << " Histos will be obtained " << endl;
    for (int i(0); i < nFiles; i++){
        histograms[i] = (TH1D*) Files[i]->Get(variable.c_str());
        cout << " get histo " << i << " " << histograms[i]->Integral() << endl;
    }
}


TH1D* getSystematicMC(int myNToys, TH1D* hMeas, RooUnfoldResponse* response, int kterm)
{
    cout << "--- Begin Toy experiment on Response matrix with NToys: " << myNToys << endl;
    double uncerFromToyResp[50]; // output
    TH1D * hUncerFromToyResp = (TH1D*) hMeas->Clone("Error from Resp Toy");
    
    //TRandom3* RandGen = new TRandom3();
    //RandGen->SetSeed(0); // set random seed
    double xiSum[50];
    double xiSquareSum[50];
    for(int i = 0; i<50; i++){
        xiSum[i] = 0;
        xiSquareSum[i] = 0;
    }
    int numBinXFinal = response->GetNbinsMeasured();
    
    //--- test distribution of unfolded hist
    RooUnfoldBayes unfoldCTest(response, hMeas, kterm);
    unfoldCTest.SetVerbose(0);
    TH1D *hUnfoldedCTest = (TH1D*) unfoldCTest.Hreco();
    double meanC = hUnfoldedCTest->GetBinContent(2);
    double sigmaC = hUnfoldedCTest->GetBinError(2);
    TH1D *distriUnfolded = new TH1D ("ErrTest for Smeared Resp", "ErrTest for Smeared Resp", 20, -0.2, 0.2);
    //-----------
    
    int myCount = 0;
    for (int nthToy = 0; nthToy < myNToys; nthToy ++){
        
        // creat new RooUnfoldResponse object
        RooUnfoldResponse *newRespToy =  (RooUnfoldResponse*) response->RunToy();
        // creat new RooUnfold object and Unfolded histo
        RooUnfoldBayes unfoldToyResp(newRespToy, hMeas, kterm);
        unfoldToyResp.SetVerbose(0);
        TH1D *hUnfoldedToyResp = NULL;
        hUnfoldedToyResp = (TH1D*) unfoldToyResp.Hreco();
        
        // next calculate the uncertainty
        for (int i = 1; i <= numBinXFinal; i++){
            double xi = hUnfoldedToyResp->GetBinContent(i);
            //cout << " k: " << nthToy << " binth : " << i << " xi: " << xi << endl;
            if (xi < 0){
                //cout << " unfolded value less than Zero " << nthToy << " " << i << endl;
                xi = 0;
            }
            xiSum[i] += xi;
            xiSquareSum[i] += xi * xi;
            
            //--- test distribution of unfolded hist ( second bin only [exclusive 1 jet] )
            if (i == 2) {
                double distriVal = (xi - meanC)/sigmaC;
                distriUnfolded->Fill(distriVal);
            }
            //---------------
        }
    }
    
    //cout << " Counts for bins of New Resp < zero : " << myCount << endl;
    //--- test distribution of unfolded hist
    //TCanvas* canvasErrTest = new TCanvas("canvasErrTest for Smeared Resp", "Uncertainty Test for Smeared Resp (xi - meanC)/sigmaC");
    //canvasErrTest->cd();
    //distriUnfolded->Draw();
    //canvasErrTest->Print("PNGFiles/FinalUnfold/TestToyErrorResp.pdf");
    //-------------
    
    // set up output
    for (int i = 1; i <= numBinXFinal; i++){
        uncerFromToyResp[i] = sqrt( (xiSquareSum[i]/myNToys) - (pow( xiSum[i]/myNToys, 2)) );
        hUncerFromToyResp->SetBinContent(i, uncerFromToyResp[i]);
        hUncerFromToyResp->SetBinError(i, 0.0);
        cout << " sumSquare: " << xiSquareSum[i]/myNToys
             << " squareOfSum: " << pow( xiSum[i]/myNToys, 2)
             <<  " diff: " << (xiSquareSum[i]/myNToys) - (pow( xiSum[i]/myNToys, 2))
             << endl;
    }
    /*
    cout << " resulting values " << endl;
    for (int i = 1; i <= numBinXFinal; i++) {
        cout << uncerFromToyResp[i] << endl;
    }
    cout << " central values " << endl;
    for (int i = 1; i <= numBinXFinal; i++){
        cout << hUnfoldedCTest->GetBinContent(i) << endl;
    }
    */
    cout << " stat uncer of central (2nd binth): " << sigmaC << endl;
    cout << " syts uncer from MC (2nd binth)   : " << uncerFromToyResp[2] << endl;
    
    TH1D *hSystematicMC = (TH1D*) hUncerFromToyResp->Clone();
    hSystematicMC->Add(hUnfoldedCTest);
    cout << "--- End Toy experiment on Response matrix ---" << endl;
    
    return hSystematicMC;
}

TH1D* getErrorMeasToy(int myNToys, TH1D* hMeas, RooUnfoldResponse* response, int kterm)
{
    //--- Toy experiment on hMeas ------
    double uncerFromToyMeas[50]; // output
    TH1D * hUncerFromToyMeas = (TH1D*) hMeas->Clone("Error from Meas Toy");
    
    //TRandom3* RandGen = new TRandom3();
    RandGen->SetSeed(0); // set random seed
    double xiSum[50];
    double xiSquareSum[50];
    for(int i = 0; i<50; i++){
        xiSum[i] = 0;
        xiSquareSum[i] = 0;
    }
    int numBinXFinal = response->GetNbinsMeasured();
    
    //--- test distribution of unfolded hist
    RooUnfoldBayes unfoldCTest(response, hMeas, kterm);
    unfoldCTest.SetVerbose(0);
    TH1D *hUnfoldedCTest = (TH1D*) unfoldCTest.Hreco();
    double meanC = hUnfoldedCTest->GetBinContent(2);
    double sigmaC = hUnfoldedCTest->GetBinError(2);
    TH1D *distriUnfoldedMeas = new TH1D ("ErrTest for Smeared Meas", "ErrTest for Smeared Meas", 20, -3.0, 3.0);
    //-----------
    
    
    for (int nthToy = 0; nthToy < myNToys; nthToy ++){
        
        // creat new Meas
        TH1D *hMeasToy = (TH1D*) hMeas->Clone();
        TH1D *hNewMeasToy = (TH1D*) hMeas->Clone();
        
        //int numBinUsed(0);
        //int numBinX = response->GetNbinsMeasured();
        int binthStart = 1;
        int binthEnd = numBinXFinal;
        //numBinUsed = numBinXFinal;
        if (response->UseOverflowStatus()){
            binthStart = 0;
            binthEnd = numBinXFinal +1;
            //numBinUsed = numBinXFinal +2 ;
        }
        
        for (int i = binthStart; i <= binthEnd; i++) {
            
            double errMeasTemp (0);
            double contentMeasTemp (0);
            errMeasTemp = hMeasToy->GetBinError(i);
            contentMeasTemp = hMeasToy->GetBinContent(i);
            if (errMeasTemp > 0.0){
                contentMeasTemp += RandGen->Gaus(0,errMeasTemp);
                if (contentMeasTemp < 0){
                    contentMeasTemp = 0;
                }
            }
            hNewMeasToy->SetBinContent(i, contentMeasTemp);
            
        }
        
        
        // creat new RooUnfold object and Unfolded histo
        RooUnfoldBayes unfoldToyMeas(response, hNewMeasToy, kterm);
        unfoldToyMeas.SetVerbose(0);
        TH1D *hUnfoldedToyMeas = NULL;
        hUnfoldedToyMeas = (TH1D*) unfoldToyMeas.Hreco();
        
        // next calculate the uncertainty
        //cout << endl;
        for (int i = 1; i <= numBinXFinal; i++){
            double xi = hUnfoldedToyMeas->GetBinContent(i);
            //cout << " k: " << nthToy << " binth : " << i << " xi: " << xi << endl;
            if (xi < 0){
                cout << " unfolded value less than Zero " << nthToy << " " << i << endl;
                xi = 0;
            }
            xiSum[i] += xi;
            xiSquareSum[i] += xi * xi;
            
            //--- test distribution of unfolded hist ( second bin only [exclusive 1 jet] )
            if (i == 2) {
                double distriVal = (xi - meanC)/sigmaC;
                distriUnfoldedMeas->Fill(distriVal);
            }
            //---------------
            
        }
        
        //cout << " numBinUsedInReap: " << numBinUsed << endl;
    }
    
    
    //--- test distribution of unfolded hist
    //TCanvas* canvasErrTestMeas = new TCanvas("canvasErrTest for Smeared Meas", "canvasErrTest for Smeared Meas");
    //canvasErrTestMeas->cd();
    //distriUnfoldedMeas->Draw();
    //canvasErrTestMeas->Print("PNGFiles/TestToyErrorMeas.pdf");
    //-------------
    
    // set up output
    for (int i = 1; i <= numBinXFinal; i++){
        uncerFromToyMeas[i] = sqrt( (xiSquareSum[i]/myNToys) - (pow( xiSum[i]/myNToys, 2)) );
        hUncerFromToyMeas->SetBinContent(i, uncerFromToyMeas[i]);
        hUncerFromToyMeas->SetBinError(i, 0.0);
        
        cout << " sumSquare: " << xiSquareSum[i]/myNToys << " squareOfSum: " << pow( xiSum[i]/myNToys, 2) <<  " diff: " << (xiSquareSum[i]/myNToys) - (pow( xiSum[i]/myNToys, 2)) << endl;
    }
    
    cout << " sigmaC:   " << sigmaC << endl;
    cout << " sigmaToy: " << uncerFromToyMeas[2] << endl;
    
    return hUncerFromToyMeas;
    
    //--- End Toy experiment on hMeas ------
    
}


TH1D* Unfold(string unfAlg, RooUnfoldResponse* response, TH1D* hData, TH1D* hSumBG, int Kterm, string hOutName, bool useOverFlow)
{
    if (useOverFlow) response->UseOverflow();
    RooUnfold* RObject = NULL;
    TH1D * hDataClone = (TH1D*) hData->Clone();
    hDataClone->Add(hSumBG, -1);
    if (unfAlg == "SVD")   RObject = (RooUnfold*) RooUnfold::New( RooUnfold::kSVD,   response, hDataClone, Kterm);
    if (unfAlg == "Bayes") RObject = (RooUnfold*) RooUnfold::New( RooUnfold::kBayes, response, hDataClone, Kterm);
    RObject->SetVerbose(0);
    TH1D* hCorrected = (TH1D*) RObject->Hreco();
    hCorrected->SetName(hOutName.c_str());
    
    return hCorrected;
}


// ttbar calculation
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
        nsel = NFILESWJETS_M; // NFILESWJETS_M(15)
        sel = new int[nsel];
        sel = FilesWJets_M; // {0, 2, 3, 4, 23, 5, 6, 7, 8, 9, 10, 11, 12, 13, 24};
    }
    
    //--- fetch the files and histograms of the selected variable --------------
    TH1D **histoArr = new TH1D*[nsel];
    for (int i(0); i < nsel; i++) {
        TFile *fCurrent;
        string fileNameTemp = ProcessInfo[sel[i]].filename;
        
        // change the name of QCD files got from ProcessInfo[sel[i]].filename
        if ((fileNameTemp.find("QCD") != string::npos) && (variable.find("MeanNJets") != string::npos) ) fileNameTemp = "DataQCD_ForMeanNJets_dR_5311";
        
        fCurrent = getFile(HistoFilesDirTT, leptonFlavor, energy, fileNameTemp, JetPtMin, JetPtMax, doFlat, doVarWidth, doQCD , doSSign,  doInvMassCut, MET, doBJets,  "", "0");
        
        std::cout << "opened :  " << i << "   " << sel[i] << "   " << FilesWJets_M[i] << "  " << ProcessInfo[sel[i]].filename << "   " << leptonFlavor.find("SMuE") << std::endl;
        
        histoArr[i] = getHisto(fCurrent, variable);
        // close all input root files
        fCurrent->Close();
    }
    std::cout << "Closed all files" << std::endl;
    
    //--- calculate hMeasTTbar ---------
    TH1D *hMeasTTbar = (TH1D*) histoArr[0]->Clone();
    int nBins(hMeasTTbar->GetNbinsX());
    
    int nselTTbar = 11; // index 11 in sel[] is ttbar
    for (int i = 1; i < nsel; i++) {
        if (i == nselTTbar) continue; // we want data minus WjetsMC and all bgs but ttbar
        hMeasTTbar->Add(histoArr[i], -1);
        cout << " subtracting : " << ProcessInfo[sel[i]].filename << endl;
        //cout << hMeasTTbar->GetBinContent(9) << " " << hMeasTTbar->GetBinError(9) << endl;
    }
    setNegBinZero(hMeasTTbar);
    
    TH1D *histScaleF = (TH1D*) hMeasTTbar->Clone();
    histScaleF->Divide(histoArr[nselTTbar]);
    
    // For ZNGoodJets_Zexc the 0-jet and 1-jet bins are irrelevant.
    if (variable == "ZNGoodJets_Zexc") {
        histScaleF->SetBinContent(1, 1.0);
        histScaleF->SetBinError(1, 0.0);
        histScaleF->SetBinContent(2, 1.0);
        histScaleF->SetBinError(2, 0.0);
    }
    
    // Print out SF for inspection
    for (int j = 1; j <= nBins; j++) {
        cout << hMeasTTbar->GetBinContent(j)  << "\t" << hMeasTTbar->GetBinError(j)  << "\t";
        cout << histoArr[7]->GetBinContent(j) << "\t" << histoArr[7]->GetBinError(j) << "\t";
        cout << histScaleF->GetBinContent(j)  << "\t" << histScaleF->GetBinError(j)  << "\t";
        //cout << hMeasTTbar->GetBinContent(j)/histoArr[7]->GetBinContent(j) ;
        cout << endl;
    }
    
    cout << "...>>> Scale Factors are obtained ..." << endl;
    cout << "...>>>" << endl;
    
    TCanvas *canSF = new TCanvas("can1", "can1", 900, 600);
    canSF->cd();
    histScaleF->GetYaxis()->SetRangeUser(0.51, 1.49);
    histScaleF->GetYaxis()->SetTitle("Scale Factor");
    histScaleF->SetTitle(("TTbarSyst for " + variable).c_str());
    histScaleF->Draw("E");
    
    string TTbaroutputDir = "PNGFiles/TTbarSystematics/";
    string command1 = "mkdir -p " + TTbaroutputDir;
    system(command1.c_str());
    
    string SFplotName = "PNGFiles/TTbarSystematics/SFplots_" + variable + ".pdf";
    canSF->Print(SFplotName.c_str());
    
    return histScaleF;
    
}

void setNegBinZero(TH1D *histograms1D)
{
    int nBinsX(histograms1D->GetNbinsX());
    for (int i = 1; i <= nBinsX; i++){
        if ( histograms1D->GetBinContent(i) < 0 ){
            histograms1D->SetBinContent(i, 0.);
            histograms1D->SetBinError(i, 0.);
        }
    }
    
}

// resp syst calculation
TH1D* getHistoRespSystematics(string leptonFlavor, string variable, int JetPtMin, int JetPtMax, bool doFlat, bool doVarWidth, int doQCD, bool doSSign, bool doInvMassCut, int MET, int doBJets, int Bayeskterm)
{
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    gStyle->SetOptStat(0);
    gStyle->SetPaintTextFormat("1.2f");
    
    cout << "...>>>" << endl;
    cout << "...>>>" << endl;
    cout << "...>>> Processing Resp Systematic;  Variable is: " << variable << endl;
    
    string variableStd = variable;
    string variableFine = variable;
    variableFine.replace(variable.find("_"), 1,"_2_");
    cout << variableFine << endl;
    
    string energy = getEnergy();
    string HistoFilesDir = "HistoFiles/";
    
    int nsel = 0;
    const int *sel = NULL;
    if (leptonFlavor == "DMu" || leptonFlavor == "DE") {
        nsel = NFILESDYJETS;
        sel = new int[nsel];
        sel = FilesDYJets;
    }
    else if (leptonFlavor == "SMu" || leptonFlavor == "SE") {
        nsel = NFILESWJETS_M; // NFILESWJETS_M(15)
        sel = new int[nsel];
        sel = FilesWJets_M; // {0, 2, 3, 4, 23, 5, 6, 7, 8, 9, 10, 11, 12, 13, 24};
    }
    
    //--- fetch the files and histograms of the selected variable --------------
    TH1D **histoArrStd = new TH1D*[nsel];
    TH1D **histoArrFine = new TH1D*[nsel];
    
    
    
    
    for (int i(0); i < nsel; i++) {
        TFile *fCurrent;
        string fileNameTemp = ProcessInfo[sel[i]].filename;
        
        // change the name of QCD files got from ProcessInfo[sel[i]].filename
        if ((fileNameTemp.find("QCD") != string::npos) && (variableStd.find("MeanNJets") != string::npos) ) fileNameTemp = "DataQCD_ForMeanNJets_dR_5311";
        
        fCurrent = getFile(HistoFilesDir, leptonFlavor, energy, fileNameTemp, JetPtMin, JetPtMax, doFlat, doVarWidth, doQCD , doSSign,  doInvMassCut, MET, doBJets,  "", "0");
        
        std::cout << "opened :  " << i << "   " << sel[i] << "   " << FilesWJets_M[i] << "  " << ProcessInfo[sel[i]].filename << "   " << leptonFlavor.find("SMuE") << std::endl;
        
        histoArrStd[i] = getHisto(fCurrent, variableStd);
        histoArrFine[i] = getHisto(fCurrent, variableFine);
        
        // close all input root files
        fCurrent->Close();
    }
    std::cout << "----------- Closed all files -----------" << std::endl;
    
    
    
    TH1D *measStd = (TH1D*) histoArrStd[0]->Clone();
    TH1D *measSubBGStd = (TH1D*) measStd->Clone();
    for (int i(1); i < 14; i++) {
        measSubBGStd->Add(histoArrStd[i], -1);
    }
    
    TH1D *meas = (TH1D*) histoArrFine[0]->Clone();
    TH1D *measSubBG = (TH1D*) meas->Clone();
    for (int i(1); i < 14; i++) {
        measSubBG->Add(histoArrFine[i], -1);
    }
    
    //-- build the repsonse ------------------------
    TFile *DY = NULL;
    DY = getFile(FILESDIRECTORY, leptonFlavor, energy, ProcessInfo[24].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, -1, "", "0", false, 0);
    
    string hrespName = "hresponse" + variableFine;
    string genName = "gen" + variableFine;
    
    TH2D *histoResponse = get2DHisto(DY, hrespName);
    TH1D *genMad  = getHisto(DY, genName);
    TH1D *recoMad = getHisto(DY, variableFine);
    
    string hrespNameStd = "hresponse" + variableStd;
    string genNameStd = "gen" + variableStd;
    TH2D *histoResponseStd = get2DHisto(DY, hrespNameStd);
    TH1D *genMadStd  = getHisto(DY, genNameStd);
    TH1D *recoMadStd = getHisto(DY, variableStd);
    closeFile(DY);
    //TH1D *recoMad = (TH1D*) histoArrFine[14]->Clone();
    //cout << "Stop after line " << __LINE__ << endl;
    //cout << histoArrFine[14]->Integral() << endl;
    //cout << histoResponse->Integral() << endl;
    //cout << genMad->Integral() << endl;
    //cout << recoMad->Integral() << endl;
    //cout << "Stop after line " << __LINE__ << endl;
    //cout << histoResponseStd->Integral() << endl;
    //cout << genMadStd->Integral() << endl;
    //cout << recoMadStd->Integral() << endl;
    
    RooUnfoldResponse *response = new RooUnfoldResponse(recoMad, genMad, histoResponse);
    response->UseOverflow();
    RooUnfoldResponse *responseStd = new RooUnfoldResponse(recoMadStd, genMadStd, histoResponseStd);
    responseStd->UseOverflow();
    //--
    
    //--- Bins definition : nominal binning ---
    const int nBinsStd(measSubBGStd->GetNbinsX());
    double BinsArray[nBinsStd+1];
    for (int i = 0; i <= nBinsStd; i++){
        if (i == 0) BinsArray[i] = measSubBGStd->GetXaxis()->GetBinLowEdge(i+1);
        else BinsArray[i] = measSubBGStd->GetXaxis()->GetBinUpEdge(i);
    }
    
    cout << " Number of bin used = " << measSubBG->GetNbinsX() << endl;
    
    TH2D *hResponse      = (TH2D*) response->Hresponse();
    TH1D *histoMCtruth   = (TH1D*) response->Htruth();
    TH1D *histoMCreco    = (TH1D*) response->Hmeasured();
    
    cout << " integral " << measSubBG->Integral() << " " << histoMCtruth->Integral() << " " << histoMCreco->Integral() << " " << recoMad->Integral() << endl;
    
    //--- calculate scaling factor
    const int nBinsFine(recoMad->GetNbinsX());
    double fwFirst[nBinsFine];
    cout << "----------- calculate scaling factor fw -------------" << endl;
    for (int i = 1; i <= nBinsFine; i++){
        fwFirst[i] = 1;
        if ((recoMad->GetBinContent(i) > 0) & (measSubBG->Integral() > 0)){
            fwFirst[i] = (measSubBG->GetBinContent(i) / recoMad->GetBinContent(i)) * (recoMad->Integral() / measSubBG->Integral());
        }
        //cout << fwFirst[i] << endl;
    }
    
    //-- this is fwFirst[i] but set as histogram for plotting
    TH1D *histoFw = (TH1D*) measSubBG->Clone();
    histoFw->Divide(recoMad);
    histoFw->Scale(recoMad->Integral() / measSubBG->Integral());
    //cout << " integral " << histoFw->Integral() << endl;
    
    /*
     double RecoToProjectX[nBinsFine];
     for (int j = 1; j <= hResponse->GetXaxis()->GetNbins(); j++){
     RecoToProjectX[j] = 1;
     if(hResponse->ProjectionX()->GetBinContent(j) > 0){
     RecoToProjectX[j]  = (histoMCreco->GetBinContent(j) / hResponse->ProjectionX()->GetBinContent(j));
     }
     //cout << " RecoToProjectX   " << RecoToProjectX[j] << endl;
     }
     */
    double TruthToProjectY[nBinsFine];
    for (int j = 1; j <= hResponse->GetYaxis()->GetNbins(); j++){
        TruthToProjectY[j] = 1;
        if(hResponse->ProjectionY()->GetBinContent(j) > 0){
            TruthToProjectY[j]  = (histoMCtruth->GetBinContent(j) / hResponse->ProjectionY()->GetBinContent(j));
        }
        //cout << " TruthToProjectY   " << TruthToProjectY[j] << endl;
    }
    
    TH2D *hResponseTemp = (TH2D*) hResponse->Clone();
    TH1D *MCtruthTemp = (TH1D*) histoMCtruth->Clone();
    TH1D *MCrecoTemp = (TH1D*) histoMCreco->Clone();
    //--- hResponseTemp
    for (int j = 1; j <= nBinsFine; j++){
        for (int i = 1; i <= nBinsFine; i++){
            hResponseTemp->SetBinContent(i, j, fwFirst[i] * hResponse->GetBinContent(i, j));
        }
    }
    //--- MCtruthTemp, MCrecoTemp
    for (int j = 1; j <= nBinsFine; j++){
        MCtruthTemp->SetBinContent(j, TruthToProjectY[j] * hResponseTemp->ProjectionY()->GetBinContent(j));
        MCrecoTemp->SetBinContent(j, fwFirst[j] * histoMCreco->GetBinContent(j));
    }
    
    
    //--- Produce the response and unfolded with nominal binning.
    //--- Note that in real analysis measSubBG should be taken from filling the nominal binning.
    //--- Only response-relevant should be produced here.
    TH1D *MCtruthTempNominal    = (TH1D*) MCtruthTemp ->Rebin(nBinsStd,"MCtruthNorminal",   BinsArray);
    TH1D *MCrecoTempNominal     = (TH1D*) MCrecoTemp  ->Rebin(nBinsStd,"MCrecoNorminal",    BinsArray);
    //--- Rebin 2D response matrix
    TH2D *hResponseTempNominal = new TH2D("responseNorminal", hResponse->GetTitle(), nBinsStd, BinsArray, nBinsStd, BinsArray);
    for (int j = 1; j <= hResponseTemp->GetYaxis()->GetNbins(); j++){
        for (int i = 1; i <= hResponseTemp->GetXaxis()->GetNbins(); i++){
            hResponseTempNominal->Fill(hResponseTemp->GetXaxis()->GetBinCenter(i), hResponseTemp->GetYaxis()->GetBinCenter(j), hResponseTemp->GetBinContent(i,j));
        }
    }
    
    RooUnfoldResponse *responseReweightedNominal = new RooUnfoldResponse (MCrecoTempNominal, MCtruthTempNominal, hResponseTempNominal);
    responseReweightedNominal->UseOverflow();
    //cout << MCrecoTempNominal->Integral() << endl;
    //cout << MCtruthTempNominal->Integral() << endl;
    //cout << hResponseTempNominal->Integral() << endl;
    
    //--- RooUnfold Object constructed with measSubBGStd
    RooUnfoldBayes unfoldObjTempNominal(responseReweightedNominal, measSubBGStd, Bayeskterm);
    unfoldObjTempNominal.SetVerbose(0);
    TH1D *hUnfoldedTempNominal = NULL;
    hUnfoldedTempNominal = (TH1D*) unfoldObjTempNominal.Hreco();
    //cout << hUnfoldedTempNominal->Integral() << endl;
    
    //--- Get central histo for comparison -----------
    RooUnfoldBayes unfoldObjCompare(responseStd, measSubBGStd, Bayeskterm);
    unfoldObjCompare.SetVerbose(0);
    TH1D *hUnfoldedCompare = (TH1D*) unfoldObjCompare.Hreco();
    //cout << hUnfoldedCompare->Integral() << endl;
    //----------------------------------------------------------------

    
    //---
    TCanvas* canvas3 = new TCanvas("c3", "canvas3", 900, 700);
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
    
    string outputDirectory = "PNGFiles/RespSystematics/";
    string command1 = "mkdir -p " + outputDirectory;
    system(command1.c_str());
    string FWplotName = "PNGFiles/RespSystematics/FWplots_" + variableStd + ".pdf";
    canvas3->Print(FWplotName.c_str());
    cout << " >>> Plotting fw: output in : " << FWplotName << endl;
    
    //---
    TCanvas* canvas2 = new TCanvas("c2", "canvas2", 700, 900);
    canvas2->cd();
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1);
    pad1->SetTopMargin(0.11);
    pad1->SetBottomMargin(0);
    pad1->SetRightMargin(0.04);
    pad1->Draw();
    pad1->cd();
    pad1->SetLogy();
    pad1->SetTicks();
    
    genMadStd->SetTitle(variableStd.c_str());
    genMadStd->GetYaxis()->SetTitleOffset(1.4);
    genMadStd->SetLineColor(kBlack);
    genMadStd->SetLineWidth(2);
    genMadStd->SetFillStyle(3003);
    genMadStd->SetFillColor(kGray+2);
    genMadStd->DrawCopy("HIST");
    
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
    legend1->AddEntry(genMadStd, "Gen", "pl");
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
    
    
    cout << "---------- Systematic Error in % --------" << endl;
    for(int i = 1; i <= Clone1->GetNbinsX(); i++){
        cout << fabs(Clone1->GetBinContent(i) - 1) * 100  << endl;
    }
    
    
    /*
     TH1D *Clone1 = (TH1D*)hUnfoldedTempNominal->Clone();
     cout << "---------- Systematic Error in % --------" << endl;
     for(int i = 1; i <= Clone1->GetNbinsX(); i++){
     cout << fabs(Clone1->GetBinContent(i) - hUnfoldedCompare->GetBinContent(i) ) * 100/(hUnfoldedCompare->GetBinContent(i) ) << endl;
     }
     */
    
    
    return hUnfoldedTempNominal;

}


TH2D* get2DHisto(TFile *File, string variable)
{
    TH2D *histo2D = (TH2D*) File->Get(variable.c_str());
    histo2D->SetDirectory(0);
    return histo2D;
}



