// History
//---- 2015_06_23
// Initialize the FILE variables and put them in local.
//---- 2015_05_29
// Further fix the error calculation for the non ZNGoodJets distribution. Make sure all calculation is ok now.
//
//---------------

#include <vector>
#include <TDirectory.h>
#include <TFile.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TKey.h>
#include <TH1.h>
#include <TH2.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TGraph.h>
#include <TPaveStats.h>
#include <TText.h>
#include <TLegend.h>
#include <iostream>
#include <sstream>
#include <TLorentzVector.h>
#include <RooUnfoldResponse.h>
#include <RooUnfold.h>
#include <RooUnfoldBayes.h>
#include <RooUnfoldSvd.h>
#include <RooUnfoldBinByBin.h>
#include <RooUnfoldInvert.h>
#include <TSVDUnfold.h>
#include <TLatex.h>
#include <TStyle.h>
#include <TDatime.h>
#include <TRandom3.h>
#include <TMatrixD.h>
#include "getFilesAndHistograms.h"
#include "DataDrivenQCD.h"
using namespace std;

//--  Setting global variables --------------------------------------------------------------
#include "fileNames.h"
#include "variablesOfInterestVarWidth.h"

const int NQCD = 4 ;
const int NMC = 10 ;

string energy = getEnergy();
int JetPtMin(30);
int JetPtMax(0);

//-------------------------------------------------------------------------------------------
void DataDrivenQCD( string leptonFlavor, int METcut , int doBJets ){
    
    TH1::SetDefaultSumw2();
    
    TFile *fData[NQCD] = {NULL};
    TFile *fMC[NQCD][NMC] = {{NULL}};
    
    FuncOpenAllFiles(fData, fMC, leptonFlavor, METcut, false, true, doBJets);
    vector<string> histoNameRun = getVectorOfHistoNames(fData);
    
    string nameQCDout = fData[0]->GetName();
    nameQCDout.insert(nameQCDout.find("Data") + 4,"QCD");
    TFile *fOut = new TFile(nameQCDout.c_str(), "recreate");
    
    
    for (int i(0); i < int(histoNameRun.size()) ; i++){
    //for (int i(2); i < 3 ; i++){
        cout << endl; cout << endl;
        cout << " --- processing histogram: " << i << " : " << histoNameRun[i] << endl;
        FuncDataDrivenQCD(histoNameRun[i], fData, fMC, fOut);
    }
    
    //-- Close all the files ------------------------------
    cout << "I m closing the files" << endl;
    for (int i(0); i < NQCD; i++) {
        closeFile(fData[i]);
        for (int j(0); j < NMC; j++){
            closeFile(fMC[i][j]);
        }
    }
    fOut ->Close();
    cout << "QCD calculation is done. All files are closed." << endl;
    //-----------------------------------------------------
    
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void FuncOpenAllFiles(TFile *fData[], TFile *fMC[][10], string leptonFlavor,int METcut, bool doFlat , bool doVarWidth, int doBJets){
    // Get data files
    for ( int i = 0 ; i < NQCD ; i++){
        fData[i] = getFile(FILESDIRECTORY,  leptonFlavor, energy, ProcessInfo[DATAFILENAME].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, i, 0, 0, METcut, doBJets, "", "0");
    }
    /// get MC files
    for ( int i=0 ; i < NQCD ; i++){
        cout << endl;
        for ( int j = 0 ; j < NMC ; j++){
            
            cout << endl;
            
            string FilenameTemp;
            if (j == 0) FilenameTemp = "WJetsALL_MIX_UNFOLDING_dR_5311_List";
            if (j == 1) FilenameTemp = "DYJets50toInf_dR_5311_List";
            if (j == 2) FilenameTemp = "TTJets_dR_5311_List";
            if (j == 3) FilenameTemp = "ST_s_channel_dR_5311_List";
            if (j == 4) FilenameTemp = "ST_t_channel_dR_5311_List";
            if (j == 5) FilenameTemp = "ST_tW_top_channel_dR_5311_List";
            if (j == 6) FilenameTemp = "ST_tW_antitop_channel_dR_5311_List";
            if (j == 7) FilenameTemp = "WW_dR_5311_List";
            if (j == 8) FilenameTemp = "WZ_dR_5311_List";
            if (j == 9) FilenameTemp = "ZZ_dR_5311_List";
            
            
            fMC[i][j] = getFile(FILESDIRECTORY,  leptonFlavor, energy, FilenameTemp, JetPtMin, JetPtMax, doFlat, doVarWidth, i , 0, 0, METcut, doBJets, "", "0");
            TH1D *hTemp2 = getHisto(fMC[i][j], "ZNGoodJets_Zexc");
            cout << " going to fetch " << FilenameTemp << "   " << hTemp2 ->Integral()<<endl;
        }
    }
    cout << endl;
    cout << "--- opened all data and MC files ---"<< endl;
}

vector<string> getVectorOfHistoNames(TFile *fData[]){
    
    unsigned short nHist = fData[0]->GetListOfKeys()->GetEntries();
    vector<string> histoName;
    
    int countHist = 0 ;
    for (unsigned short i(0); i < nHist; i++) {
        string histoNameTemp = fData[0]->GetListOfKeys()->At(i)->GetName();
        TH1D* histTemp = (TH1D*) fData[0]->Get(histoNameTemp.c_str());
        if (histTemp->GetEntries() < 1) continue;
        if ( histTemp->InheritsFrom(TH1D::Class())) {
            histoName.push_back(fData[0]->GetListOfKeys()->At(i)->GetName());
            countHist++;
        }
    }
    cout << endl;
    cout << "--- checking the histogram names to be studied ---" << endl;
    cout << " we will produce "<< countHist << " QCD histograms  " <<  histoName.size() << endl;
    cout << "--------------------------------------------------" << endl;
    return histoName;
    
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FuncDataDrivenQCD(string variable, TFile *fData[], TFile *fMC[][NMC], TFile *fOut){
    cout << " test" << endl;
    cout << " --- now opening histograms from Data files ---" << endl;
    TH1D  *hData[NQCD], *hSignal[NQCD], *hBack[NQCD];
    
    //--- fetch the data histograms -------------
    for ( int i=0 ; i < NQCD ; i++){
        
        TH1D *hTemp = getHisto(fData[i], variable);
        hData[i] = (TH1D *) hTemp->Clone();
        cout << " got data of histo name: " << variable << " for QCD: " << i << " " << ProcessInfo[DATAFILENAME].filename << endl;
        cout << "   integral: " << hData[i]->Integral() << endl;
        delete hTemp;
    }
    cout << " --- got Data histos ---" << endl;
    
    //--- fetch the MC histograms --------------
    cout << endl;
    cout << " --- now opening histograms from all the MC files ---" << endl;
    for ( int i=0 ; i < NQCD ; i++){
        //cout << "QCD " << i << endl;
        for ( int j = 0 ; j < NMC ; j++){
            
            string FilenameTemp;
            if (j == 0) FilenameTemp = "WJetsALL_MIX_UNFOLDING_dR_5311_List";
            if (j == 1) FilenameTemp = "DYJets50toInf_dR_5311_List";
            if (j == 2) FilenameTemp = "TTJets_dR_5311_List";
            if (j == 3) FilenameTemp = "ST_s_channel_dR_5311_List";
            if (j == 4) FilenameTemp = "ST_t_channel_dR_5311_List";
            if (j == 5) FilenameTemp = "ST_tW_top_channel_dR_5311_List";
            if (j == 6) FilenameTemp = "ST_tW_antitop_channel_dR_5311_List";
            if (j == 7) FilenameTemp = "WW_dR_5311_List";
            if (j == 8) FilenameTemp = "WZ_dR_5311_List";
            if (j == 9) FilenameTemp = "ZZ_dR_5311_List";
            
            //cout << endl;
            TH1D *hTemp1 = getHisto(fMC[i][j], variable);
            cout << " going to fetch " << FilenameTemp << "   integral: " << hTemp1 ->Integral()<<endl;
            if ( FilenameTemp.find("WJetsALL") != string::npos ) {
                cout << " This is signal: " << FilenameTemp << endl;
                hSignal[i] = (TH1D *) hTemp1->Clone();
            }
            else{
                if ( j == 1 ) {
                    cout << " This is background ..." << endl;
                    hBack[i] = (TH1D *) hTemp1->Clone();
                }
                else {
                    cout << " adding... " << endl;
                    hBack[i]->Add(hTemp1);
                }
            }
            cout << " got MC histo " << "  QCD: " << i << "   MC type: " << j << "   integral: " << hTemp1->Integral() << endl;
            delete hTemp1;
        }
    }
    cout << " --- got MC histos ---" << endl;
    
    //--- QCD estiamtion procedure -------------
    cout << endl;
    cout << " --- now do QCD estimation --- " << endl;
    
    // followed as descirbed in AN2012_331 pages 10,11
    TH1D* scaledMC[NQCD], *hQCD[NQCD];
    double NormFactor_o[15], NormFactorISO_o[15], NormFactIsoError_o[15];
    
    if (variable.find("ZNGoodJets") == string::npos){
        
        // step 0 : initial normalization of Wjets and data
        double NormFactor = (hData[0]->Integral() - hBack[0]->Integral() ) / hSignal[0]->Integral() ;
        cout << " normalization of signal " << NormFactor << "   " << hData[0]->Integral() << "   " << hSignal[0]->Integral() << "   " <<  hBack[0]->Integral() << endl;
        /*
        // for a quick look at QCD contribution in each region
        for ( int i = 0  ; i < NQCD ; i++){
            cout << "initial integral (Data - MC) QCD " << i << " : " << hData[i]->Integral() - ( hBack[i]->Integral() + hSignal[i]->Integral() ) << "integral signal" << hSignal[i]->Integral() << endl;
        }
        */
        
        // 12 iterations to make sure the stability is achieved
        for ( int j = 0  ; j < 12 ; j++){
            // recalculate NormFactor by including QCD[0]
            if ( j > 0 )  NormFactor = (hData[0]->Integral() - hBack[0]->Integral() - hQCD[0] ->Integral() ) / hSignal[0]->Integral() ;
            cout << " NormFactor f_w : " << NormFactor << endl;
            
            // step 1:
            for ( int i = 0  ; i < NQCD ; i++){
                scaledMC[i] = (TH1D*) hSignal[i]->Clone();
                scaledMC[i]->Scale(NormFactor) ;
                hQCD[i] = (TH1D*) hData[i]->Clone();
                hQCD[i]->Add(scaledMC[i],-1);
                hQCD[i]->Add(hBack[i],-1);
                // set the negative bins to 0
                for ( int k = 0 ; k < hData[i]->GetNbinsX()+1 ; k++ ){
                    if ( hQCD[i]->GetBinContent(k) <= 0 ) {
                        hQCD[i]->SetBinContent(k, 0. ) ;
                        hQCD[i]->SetBinError(k, 0. ) ;
                    }
                }
                
                //double NormTemp =  hData[i]->Integral() / (scaledMC[i] ->Integral() + hBack[i]->Integral()) ;
                //cout << " data vs MC : " << i << "  " << NormTemp << "   " << hData[i]->Integral() << "   " << scaledMC[i] ->Integral() + hBack[i]->Integral() <<  endl;
            }
            //cout << " QCD intgral A B C and D   " << hQCD[0]->Integral() << "   " << hQCD[1]->Integral() << "   " << hQCD[2]->Integral() << "  " << hQCD[3]->Integral() <<  endl;
            
            // step 2: isoaltion fake rate
            double NormFactorISO(0);
            double NormFactIsoError(0);
            double errhqcd1(0), errhqcd3(0), inthqcd1(0), inthqcd3(0);
            if ( hQCD[3]->Integral() > 0 && hQCD[1]->Integral() > 0){
                inthqcd1 = hQCD[1]->IntegralAndError(1, hData[0]->GetNbinsX(), errhqcd1);
                inthqcd3 = hQCD[3]->IntegralAndError(1, hData[0]->GetNbinsX(), errhqcd3);
                
                NormFactorISO = inthqcd1/inthqcd3;
                NormFactIsoError = NormFactorISO * sqrt( pow( (errhqcd1/inthqcd1), 2) + pow( (errhqcd3/inthqcd3), 2) );
            }
            cout << " ratio of regions B to D   " << NormFactorISO << "   " << hQCD[1]->Integral() << "   " << hQCD[3]->Integral() <<  endl;
            
            // step 3 : isolation fake-rate from step 2 is aplied to QCD[2] to get QCD[0]
            for (int m = 1; m <= hQCD[0]->GetNbinsX(); m++){
                hQCD[0]->SetBinContent(m, NormFactorISO * hQCD[2]->GetBinContent(m));
                if ( hQCD[0]->GetBinContent(m) > 0){
                    // this condition ensures that both NormFactorISO and hQCD[2] != 0
                    hQCD[0]->SetBinError(m, hQCD[0]->GetBinContent(m) * sqrt( pow((NormFactIsoError/NormFactorISO), 2) + pow((hQCD[2]->GetBinError(m)/hQCD[2]->GetBinContent(m)), 2)));
                }
                else {
                    hQCD[0]->SetBinError(m, 0.);
                }
            }
            // Note: the only quantity that keeps updating is NormFactor and we treat it as constant without uncertainty. So, the uncertainty in QCD[i] is determined by the operation in the last loop, although we do calculate it in every loop. For QCD[0], the uncertainty also include the error in NormFactorISO.
            
            NormFactorISO_o[0] = NormFactorISO;
            NormFactIsoError_o[0] = NormFactIsoError;
        }
        
        NormFactor_o[0] = NormFactor;
        cout << "   print out error for " << variable << endl;
        cout << " NormFactorISO_o = " << NormFactorISO_o[0] << " Error = " << NormFactIsoError_o[0] << endl;
        
        // the folowing lines are for scaling the MT dist and are outdated
        /*
        if (variable == "MT"){
            cout << "This is MT : >> scaling...." << endl;
            hQCD[0]->Scale(1.33313/1.15459);
        }
        */
    }
    else{
        // for ZNGoodJets dist.
        for ( int i = 0  ; i < NQCD ; i++){
            scaledMC[i] = (TH1D*) hSignal[i]->Clone();
            hQCD[i] = (TH1D*) hData[i]->Clone();
        }
        
        for (int m = 1; m <= hData[0]->GetNbinsX(); m++){
            cout << " -------- processing binth : " << m << endl;
            // step 0 : initial normalization of Wjets and data
            double NormFactor(1);
            if(hSignal[0]->GetBinContent(m) > 0){NormFactor = (hData[0]->GetBinContent(m) - hBack[0]->GetBinContent(m)) / hSignal[0]->GetBinContent(m) ;}
            else {NormFactor = 1.;}
            
            for ( int j = 0  ; j < 12 ; j++){
                if ( j > 0 )  {
                    if(hSignal[0]->GetBinContent(m) > 0 ){
                        NormFactor = (hData[0]->GetBinContent(m) - hBack[0]->GetBinContent(m) - hQCD[0]->GetBinContent(m) ) / hSignal[0]->GetBinContent(m) ;
                    }
                    else {
                        NormFactor = 1. ;
                    }
                }
                
                // step 1:
                for ( int i = 0  ; i < NQCD ; i++){
                    scaledMC[i]->SetBinContent(m, NormFactor * hSignal[i]->GetBinContent(m));
                    scaledMC[i]->SetBinError(m, NormFactor * hSignal[i]->GetBinError(m));
                    
                    hQCD[i]->SetBinContent(m, hData[i]->GetBinContent(m) - (scaledMC[i]->GetBinContent(m) + hBack[i]->GetBinContent(m)) );
                    hQCD[i]->SetBinError(m,  sqrt(pow(hData[i]->GetBinError(m), 2) + pow(scaledMC[i]->GetBinError(m), 2) + pow(hBack[i]->GetBinError(m), 2)) );
                    
                    // set the negative bins to 0
                    if ( hQCD[i]->GetBinContent(m) <= 0 ) {
                        hQCD[i]->SetBinContent(m, 0. ) ;
                        hQCD[i]->SetBinError(m, 0. ) ;
                    }
                }
                //cout << " QCD intgral A B C and D   " << hQCD[0]->GetBinContent(m) << "   " << hQCD[1]->GetBinContent(m) << "   " << hQCD[2]->GetBinContent(m) << "  " << hQCD[3]->GetBinContent(m) <<  endl;
                
                // step 2: isoaltion fake rate
                double NormFactorISO(0);
                double NormFactIsoError(0);
                if ( hQCD[3]->GetBinContent(m) > 0 && hQCD[1]->GetBinContent(m) > 0 ) {
                    NormFactorISO = hQCD[1]->GetBinContent(m) / hQCD[3]->GetBinContent(m);
                    NormFactIsoError = NormFactorISO * sqrt( pow( (hQCD[1]->GetBinError(m)/hQCD[1]->GetBinContent(m) ), 2) + pow( (hQCD[3]->GetBinError(m)/hQCD[3]->GetBinContent(m) ), 2) );
                }
                cout << " ratio of regions B to D   " << NormFactorISO << "   " << hQCD[1]->GetBinContent(m) << "   " << hQCD[3]->GetBinContent(m) <<  endl;
                cout << " NormFactor: " << NormFactor << " f_B/D = " << NormFactorISO << " Error (%) = " << NormFactIsoError*100/NormFactorISO << endl;
                
                // step 3 : isolation fake-rate from step 2 is aplied to QCD[2] to get QCD[0]
                hQCD[0]->SetBinContent(m, NormFactorISO * hQCD[2]->GetBinContent(m));
                if ( hQCD[0]->GetBinContent(m) > 0){
                    hQCD[0]->SetBinError(m, hQCD[0]->GetBinContent(m) * sqrt( pow((NormFactIsoError/NormFactorISO), 2) + pow((hQCD[2]->GetBinError(m)/hQCD[2]->GetBinContent(m)), 2)));
                }
                else {
                    hQCD[0]->SetBinError(m, 0.);
                }
                
                // Note: the only quantity that keeps updating is NormFactor and we treat it as constant without uncertainty. So, the uncertainty in QCD[i] is determined by the operation in the last loop, although we do calculate it in every loop. For QCD[0], the uncertainty also include the error in NormFactorISO.
                
                NormFactorISO_o[m] = NormFactorISO;
                NormFactIsoError_o[m] = NormFactIsoError;
            }
            NormFactor_o[m] = NormFactor;
        }
        
        cout << "   print out error for " << variable << endl;
        cout << " \t" << "# event QCD" << " \t" << "error " << " \t" << "%error" << " \t" << "fw" << " \t" << "f_B/D" << " \t" << "%error f_B/D" << endl;
        for (int i = 1; i <= hQCD[0]->GetNbinsX(); i++){
            
            cout << "\t" <<  hQCD[0]->GetBinContent(i) << "\t" << hQCD[0]->GetBinError(i) << "\t" <<  hQCD[0]->GetBinError(i) * 100/hQCD[0]->GetBinContent(i) << "\t" << NormFactor_o[i] << "\t" << NormFactorISO_o[i] << "\t" << NormFactIsoError_o[i] *100/NormFactorISO_o[i] << endl;
        }
    }
    
    // ---- save to file
    cout << " --- now save results to file of: " << variable << endl;
    fOut ->cd();
    hQCD[0] ->Write();
    
}


