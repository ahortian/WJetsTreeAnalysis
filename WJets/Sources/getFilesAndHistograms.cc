#include <iostream>
#include <sstream>
#include <RooUnfoldResponse.h>
#include <TFile.h>
#include "fileNames.h"
#include "getFilesAndHistograms.h"
#include <string.h>
#include <algorithm>
using namespace std;

//------------------------------------------------------------
// getEnergy() returns a string, either "7TeV" or "13TeV"
// according to the name of the directory from which the 
// code is being executed.
//------------------------------------------------------------
string getEnergy()
{
    string energy = "";
    ostringstream fileBeingProcessed; fileBeingProcessed << __FILE__;
    if (fileBeingProcessed.str().find("Analysis2012") != string::npos) {
        energy = "13TeV";
    }
    else if (fileBeingProcessed.str().find("Analysis2011") != string::npos) {
        energy = "7TeV";
    }
    else 
    {
        std::cout << "WARNING ! Impossible to retrieve te energy from the current location !" << std::endl;
        energy = "Unknown";
    }
    fileBeingProcessed.str("");

    return energy;
}
//------------------------------------------------------------

TFile* getFile(string histoFilesDirectory, string leptonFlavor, string energy, string Name, int JetPtMin, int JetPtMax, bool doFlat, bool doVarWidth, int doQCD, bool doSSign, bool doInvMassCut, int MET, int doBJets, string closureTest, string syst, bool dodR, bool useUnfoldingFiles)
{
    
    string fileName = histoFilesDirectory; // string to contain the name of the file

    //--- make sure leptonFlavor is short version ---
    if (leptonFlavor == "Muons" || leptonFlavor == "DMu_") leptonFlavor = "DMu";
    else if (leptonFlavor == "Electrons" || leptonFlavor == "DE_") leptonFlavor = "DE";
    else if (leptonFlavor == "Electron" || leptonFlavor == "SE_") leptonFlavor = "SE";
    else if (leptonFlavor == "Muon" || leptonFlavor == "SMu_") leptonFlavor = "SMu";
    else if (leptonFlavor == "MuonElectron" || leptonFlavor == "SMuE_") leptonFlavor = "SMuE";

    fileName += leptonFlavor + "_" + energy + "_" + Name; // update fileName with lepton information
    //-----------------------------------------------


    //--- make string from numbers ---
    ostringstream JetPtMinStr; JetPtMinStr << JetPtMin;
    ostringstream JetPtMaxStr; JetPtMaxStr << JetPtMax;
    ostringstream doQCDStr; doQCDStr << doQCD;
    ostringstream METStr; METStr << MET;
    //--------------------------------

    //--- deal with efficiency correction applied or not ---
    string effiCorr = "1", trigCorr = "0";
    if (Name.find("Data") == 0 || energy == "13TeV") trigCorr = "1"; // trigger correction is applied to data and MC at 13TeV but only to data at 7TeV 
    if (useUnfoldingFiles) { // for cross-section measurement: correct data for efficiencies difference wrt MC
        if (Name.find("Data") == 0) effiCorr = "1";
        else effiCorr = "0";
    }
    else { // for control plots: correct MC forefficiencies difference wrt Data
        if (Name.find("Data") == 0) effiCorr = "0";
        else effiCorr = "1";
    }

    //--- special case for the generator comparison ---
    if (Name.find("Powheg") != string::npos || Name.find("Sherpa") != string::npos) {
        trigCorr = "0"; 
        effiCorr = "0";
    }
    //-------------------------------------------------

    fileName += "_EffiCorr_" + effiCorr + "_TrigCorr_" + trigCorr; // update fileName with efficiency correction information
    //------------------------------------------------------

    //--- update fileName for a bunch of other things ---
    fileName += "_Syst_" + syst + "_JetPtMin_" + JetPtMinStr.str();
    if (JetPtMax != 0 && JetPtMax > JetPtMin) fileName += "_JetPtMax_" + JetPtMaxStr.str();
    if (doFlat && Name.find("Data") == string::npos) fileName += "_Flat";
    if (closureTest != "") fileName += closureTest;
    if (doVarWidth) fileName += "_VarWidth";
    if (dodR) fileName += "_dR5";
    if (doInvMassCut) fileName += "_InvMass";
    if (doSSign) fileName += "_SS";
    if (doBJets > 0) fileName += "_BJets";
    if (doBJets < 0) fileName += "_BVeto";
    if (doQCD > 0) fileName += "_QCD" + doQCDStr.str();
    if (MET > 0) fileName += "_MET" + METStr.str();
    //---------------------------------------------------

    //--- fileName is complete: just add the extension and open it ---
    fileName += ".root";
    TFile *File = new TFile(fileName.c_str(), "READ");
    std::cout << "Opening: " << fileName << "   --->   Opened ? " << File->IsOpen() << std::endl;
    return File;
    //----------------------------------------------------------------
}

void getFiles(string histoFilesDirectory, TFile *Files[], string leptonFlavor, string energy, string Name, int JetPtMin, int JetPtMax, bool doFlat, bool doVarWidth, int doQCD, bool doSSign, bool doInvMassCut, int MET, int doBJets, bool useUnfoldingFiles)
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
        if (leptonFlavor == "DE") {          // additionaly for electron:
            Syst.push_back("5_Up");          //   5 up: scale factor up
            Syst.push_back("5_Down");        //   5 down: scale factor down
        }
    }
    else if (Name.find("UNFOLDING") != string::npos && ((isDoubleLep && Name.find("DYJets") != string::npos) || (!isDoubleLep && Name.find("WJets") != string::npos))) {
        // for DYJets in case of Z+Jets or for WJets in case of W+Jets analysis we have:
        Syst.push_back("0");         // 0: central
        Syst.push_back("1_Up");      // 1 up: PU up
        Syst.push_back("1_Down");    // 1 down: PU down
        Syst.push_back("4_Up");      // 4 up: JER up
    }
    else { // for background we have
        Syst.push_back("0");         // 0: central
        Syst.push_back("1_Up");      // 1 up: PU up
        Syst.push_back("1_Down");    // 1 down: PU down
        Syst.push_back("3_Up");      // 3 up: XSec up
        Syst.push_back("3_Down");    // 3 down: Xsec down
    };

    //--- determnie how many files we have and open them all ---
    int nSyst(Syst.size());
    for (int i(0); i < nSyst; i++) {
        Files[i] = getFile(histoFilesDirectory, leptonFlavor, energy, Name, JetPtMin, JetPtMax, doFlat, doVarWidth, doQCD, doSSign, doInvMassCut, MET, doBJets, "", Syst[i], false, useUnfoldingFiles);
    }
    //----------------------------------------------------------
}

//------------------------------------------------------------
// Close the file if open and delete the pointer
//------------------------------------------------------------
void closeFile(TFile *File)
{
    if (File) {
        if (File->IsOpen()) File->Close();
        std::cout << "Closing: " << File->GetName() << "   --->   Closed ? " << (!(File->IsOpen())) << std::endl;
        //delete File;
        //File = NULL;
    }
}

void closeFiles(TFile *Files[])
{
    if (Files[0]) {
        string fileName = Files[0]->GetName();
        int nFiles;
        //if (fileName.find("Data") != string::npos) {
        if ((fileName.find("Data") != string::npos) && (fileName.find("QCD") == string::npos)){
            nFiles = 3; 
            if (fileName.find("DE") != string::npos) nFiles = 5;
        }
        else if (fileName.find("UNFOLDING") != string::npos) nFiles = 4; 
        else nFiles = 5;

        for (int i(0); i < nFiles; i++){
            closeFile(Files[i]);
        }
    }
}

void closeFiles(TFile *Files[], int nFiles)
{
    string fileName = Files[0]->GetName();
    for (int i(0); i < nFiles; i++){
        closeFile(Files[i]);
        std::cout << "Closing file: " << Files[i]->GetName() << "   --->   Closed ? " << (!(Files[i]->IsOpen())) << std::endl;
    }
}


TH1D* getHisto(TFile *File, const string variable)
{
    TH1D *histo = (TH1D*) File->Get(variable.c_str());
    histo->SetDirectory(0);
    return histo;
}

void getHistos(TH1D *histograms[], TFile *Files[], string variable, bool isDoubleLep)
{
    string fileName = Files[0]->GetName();
    int nFiles;
    //if (fileName.find("Data") != string::npos){
    if ((fileName.find("Data") != string::npos) && (fileName.find("QCD") == string::npos)){
        nFiles = 3; 
        if (fileName.find("DE") != string::npos) nFiles = 5;
    }
    else if (((isDoubleLep && fileName.find("DYJets") != string::npos) || (!isDoubleLep && fileName.find("WJets") != string::npos)) && fileName.find("UNFOLDING") != string::npos) nFiles = 4; 
    else nFiles = 5;

    for (int i(0); i < nFiles; i++){
        histograms[i] = (TH1D*) Files[i]->Get(variable.c_str());
    } 
}

void getResp(RooUnfoldResponse *response, TFile *File, string variable)
{
    response = (RooUnfoldResponse*) File->Get(variable.c_str());
    if (!response) std::cout << "Couldn't load response" << std::endl;
}

RooUnfoldResponse* getResp(TFile *File, string variable)
{
    std::cout << "fetching: "<< variable <<" from " << File->GetName() << "   --->   Opened ? " << File->IsOpen() << std::endl;
    RooUnfoldResponse *response = (RooUnfoldResponse*) File->Get(variable.c_str());
    return response;
}

void getResps(RooUnfoldResponse *responses[], TFile *Files[], string variable)
{
    string fileName = Files[0]->GetName();
    int nFiles;
    //if (fileName.find("Data") != string::npos) nFiles = 3;
    if ((fileName.find("Data") != string::npos) && (fileName.find("QCD") == string::npos)) nFiles = 3;
    else if (fileName.find("UNFOLDING") != string::npos) nFiles = 4; 
    else nFiles = 5;

    for (int i(0); i < nFiles; i++){
        responses[i] = (RooUnfoldResponse*) Files[i]->Get(variable.c_str());
    } 
}


void getStatistics(string leptonFlavor, int JetPtMin, int JetPtMax, bool doFlat, bool doVarWidth, int doQCD, bool doSSign, bool doInvMassCut, int MET, int doBJets)
{
    std::string  variable = "ZNGoodJets_Zexc";
    string energy = getEnergy();

    std::cout <<" let us get jet multiplicity statistics " << std::endl;
    // jet counter
    //int NBins = 11
    int NBins = 8 ;
    double DataEv[20][20] = {{0}};

    //-- fetch the data files and histograms --------------
    int usedFiles = NFILESTTBAR ; 
    bool doDY(0) ;
    if (leptonFlavor.find("Muons") != string::npos ||  leptonFlavor.find("Electrons") != string::npos ) {
        usedFiles = NFILESDYJETS;
        doDY = true;
        NBins = 8 ; /// FIXED !!!!
    }
    for (int i(0); i < usedFiles; i++) {
        TFile *fData;
        int sel = i; 
        if (doDY) sel = FilesDYJets[i];
        else if (leptonFlavor.find("SMuE") != string::npos) sel = FilesTTbar[i];
        else sel = FilesTTbarWJets[i];

        if ((doQCD > 0 || doInvMassCut || doSSign ) && ProcessInfo[sel].filename.find("QCD") != string::npos) continue;
        fData = getFile(FILESDIRECTORY,  leptonFlavor, energy, ProcessInfo[sel].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, doQCD , doSSign,  doInvMassCut, MET, doBJets,  "","0");
        std::cout << "opened :  " << i << "   " << sel <<"   " << FilesTTbarWJets[i] <<"  " << ProcessInfo[sel].filename <<"   " << leptonFlavor.find("SMuE") << std::endl;
        TH1D *hTemp = getHisto(fData, variable);

        for (int j = 1 ; j < NBins + 1 ; j++ ){
            Double_t binContent = hTemp->GetBinContent(j);
            DataEv[i][j] = binContent;
            if ( i > 0 ) DataEv[usedFiles][j]+=int(binContent);
        }
        // close all input root files
        fData->Close();
    }

    std::cout << "Closed all files" << std::endl;

    ostringstream nameStr;  nameStr << "outputTable_" << leptonFlavor <<"_JetPtMin_" <<JetPtMin;
    if (doInvMassCut) nameStr << "_InvMass";
    if (doSSign )   nameStr << "_SS";
    //if (doBJets) nameStr << "_BJets";
    if (doQCD>0) nameStr << "_QCD"<<doQCD;
    if ( MET > 0 ) nameStr << "_MET"<< MET;
    if ( doBJets < 0 ) nameStr <<"_BVeto";
    nameStr<<".tex";

    FILE *outFile = fopen(nameStr.str().c_str(),"w");
    fprintf( outFile, "\\footnotesize{\n\\begin{tabular}{l|cccccccc} \n ");
    fprintf( outFile, " &  $N_{\\text{jets}} = 0 $ & $N_{\\text{jets}} = 1 $ & $N_{\\text{jets}} = 2 $ & $N_{\\text{jets}} = 3 $ & $N_{\\text{jets}} = 4 $ & $N_{\\text{jets}} = 5 $ & $N_{\\text{jets}} = 6 $ & $N_{\\text{jets}} = 7$ \\\\ \\hline \n ");

    //// print statistics of all the MC samples
    for (int i=1; i< usedFiles + 1 ; i++){
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
    fprintf( outFile, "\\hline \n");
    fprintf( outFile, " Data          & ");
    for (int j = 1; j< NBins + 1 ; j++){
        if (j < NBins ) fprintf( outFile, "%d & ",  int(DataEv[0][j]));
        else fprintf( outFile, "%d \\\\ \n ",  int(DataEv[0][j]));
    }
    // print ratio of MC/data
    fprintf( outFile, " Ratio          & ");
    for (int j=1; j<NBins + 1; j++){
        double temp= DataEv[usedFiles][j]/DataEv[0][j];
        std:: cout << DataEv[usedFiles][j] << "   " << DataEv[0][j] << std::endl;
        if (j<NBins) fprintf( outFile, "%f & ", float(temp));
        else fprintf( outFile, "%f \\\\ \n ",temp);

    }
    fprintf( outFile, "\\end{tabular}}");
    fclose(outFile);
}



