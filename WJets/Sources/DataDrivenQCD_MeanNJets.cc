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
#include "DataDrivenQCD_MeanNJets.h"
using namespace std;

//---  Setting global variables --------------------------------------------------------------
#include "fileNames.h"
#include "variablesOfInterestVarWidth.h"

const int Ny = 15; // number of jet (multiplicity) bins 
const int NQCD = 4;
TFile *fData[NQCD]; //*fSignal[NQCD];   // 0 = central, 1 = JES Up, 2 = JES Down
TFile *fMC[NQCD][NFILESWJETS];
        //bool isMuon(0);
        //string unfAlg = "Bayes";
string energy = getEnergy();
int JetPtMin(30);
int JetPtMax(0);
TFile *fOut;
//---  End Setting global variables ----------------------------------------------------------


void DataDrivenQCD_MeanNJets(string leptonFlavor, int METcut, int doBJets){
    
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    
    FuncOpenAllFiles(leptonFlavor, METcut, false, true, doBJets);
    
    vector<string> histoNameRun = getVectorOfHistoNames_MeanNJets();
    
    for (unsigned int i(0); i < histoNameRun.size(); i++){
        FuncQCD_MeanNJets(histoNameRun[i]);
    }
    
    // Closing files
    for ( int i=0 ; i < NQCD ; i++ ){
        fData[i]->Close();
        for ( int j = 0 ; j < NFILESWJETS ; j++){
            fMC[i][j]->Close();
        }
    }
    fOut ->Close();
}


////// --- Definition of functions used ---////////////////////////////////////////////////////////////////////////
void FuncOpenAllFiles(string leptonFlavor,int METcut, bool doFlat , bool doVarWidth, int doBJets){
    //get data file
    for ( int i=0 ; i < NQCD ; i++ ){
        // FILESDIRECTORY = "HistoFiles/" and DATAFILENAME = 0 (see fileNames.h)
        fData[i] = getFile(FILESDIRECTORY, leptonFlavor, energy, ProcessInfo[DATAFILENAME].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, i, 0, 0, METcut, doBJets, "", "0");
        //create output root file
        if ( i == 0 ) {
            string nameQCDout = fData[i]->GetName();
            nameQCDout.insert(nameQCDout.find("Data") + 4,"QCD_ForMeanNJets");
            fOut = new TFile(nameQCDout.c_str(),"recreate");
        }
    }
    //get MC files
    for ( int i=0 ; i < NQCD ; i++){
        cout << endl;
        // NFILESWJETS => 14 (see fileNames.h)
        // ProcessInfo[sel] => sel = 24 and sel from 2 to 13
        double countTemp(0);
        double countTempSig(0);
        double countTempData(0);
        
        for ( int j = 0 ; j < NFILESWJETS ; j++){
            int sel = j ;
            if ( j == 1 ) sel = 24 ;
            cout << endl;
            fMC[i][j] = getFile(FILESDIRECTORY, leptonFlavor, energy, ProcessInfo[sel].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, i , 0, 0, METcut, doBJets, "", "0");
            
            
            TH1D *hTemp = getHisto(fMC[i][j], "ZNGoodJets_Zexc");
            cout << " going to fetch " << ProcessInfo[sel].filename << "   total # of events = " << hTemp ->Integral()<<endl;
            
            if (j == 0){countTempData = hTemp ->Integral();}
            else if (j == 1){countTempSig = hTemp ->Integral();}
            else if (j > 1){countTemp += hTemp ->Integral();}
        }
        
        //My Test
        //TH1D *hTempData = getHisto(fData[i], "ZNGoodJets_Zexc");
        //cout << " FILE QCD:" << i << " Number of Events in Data = " << hTempData->Integral()  << " " << countTempData << endl;
        //cout << " FILE QCD:" << i << " Number of Events in MC   = " << countTempSig + countTemp << endl;
        //cout << " FILE QCD:" << i << " Number of Events in Sig   = " << countTempSig << endl;
        //cout << " FILE QCD:" << i << " Number of Events in BG   = " << countTemp << endl;
    }
    cout << endl;
    cout << " opened all data and MC files "<< endl;
}

vector<string> getVectorOfHistoNames_MeanNJets(){
    //variables to plot
    vector<string> histoName;
    histoName.push_back("MeanNJetsHT_Zinc1jet");
    histoName.push_back("MeanNJetsHT_Zinc2jet");
    histoName.push_back("MeanNJetsdRapidity_Zinc2jet");
    histoName.push_back("MeanNJetsdRapidityFB_Zinc2jet");
    cout << " we will produce "<< histoName.size() << " QCD histograms" << endl;
    return histoName;
}

TH2D* get2DHisto(TFile *File, string variable){
    TH2D *histo2D = (TH2D*) File->Get(variable.c_str());
    histo2D->SetDirectory(0);
    return histo2D;
}

void FuncQCD_MeanNJets(string variable){
    
    TH2D **dataHist2D = new TH2D*[NQCD];
    TH2D **BGHist2D = new TH2D*[NQCD];
    TH2D **SigHist2D = new TH2D*[NQCD];
    
    for ( int i=0 ; i < NQCD ; i++){
        cout << endl;
        
        //get 2D Histogram for Data
        dataHist2D[i] = get2DHisto(fData[i], variable);
        cout << " got data histo " << variable << " for QCD = " << i << "  #events = "<< dataHist2D[i]->Integral() << endl;
        
        //get 2D Histogram for MC
        TH2D* hTemp2D;
        TH2D* hTemp2D_2;
        for ( int j = 0 ; j < NFILESWJETS ; j++){
            
            hTemp2D_2 = get2DHisto(fMC[i][j], variable);
            cout << "histo loop " << i << " " << j << " " << " #events = " << hTemp2D_2->Integral() << endl;
            
            if (j == 0) continue;
            else if (j == 1){
                SigHist2D[i] = get2DHisto(fMC[i][j], variable);
                cout << " got Signal histo   " << variable << " for QCD: " << i << " file #label " << j << "  #events = " << SigHist2D[i]->Integral() << endl;
            }
            else if (j == 2){
                BGHist2D[i] = get2DHisto(fMC[i][j], variable);
                cout << " got BG base histo  " << variable << " for QCD: " << i << " file #label " << j << "  #events = " << BGHist2D[i]->Integral() << endl;
            }
            else if (j > 2){
                hTemp2D = get2DHisto(fMC[i][j], variable);
                cout << "      got BG histo    " << variable << " for QCD: " << i << " file #label " << j << "  #events = " << hTemp2D->Integral() << endl;
                
                BGHist2D[i]->Add(hTemp2D);
                cout << "      Added BG events " << variable << " for QCD: " << i << " file #label " << j << "  #events = " << BGHist2D[i]->Integral() << endl;
            }
        }
        cout << " got BG histo " << variable << " for QCD = " << i <<  "  #events = " << BGHist2D[i]->Integral() <<endl;
        cout << " # events in MC " << SigHist2D[i]->Integral() + BGHist2D[i]->Integral() << endl;
        
        cout << " RECHECK #events QCD: " << i << " " << variable << " " << " Data = " << dataHist2D[i]->Integral() << endl;
        cout << " RECHECK #events QCD: " << i << " " << variable << " " << " MC   = " << SigHist2D[i]->Integral() + BGHist2D[i]->Integral() << endl;
        cout << " RECHECK #events QCD: " << i << " " << variable << " " << " Sig  = " << SigHist2D[i]->Integral() << endl;
        cout << " RECHECK #events QCD: " << i << " " << variable << " " << " BG   = " << BGHist2D[i]->Integral() << endl;
    }
    
    // MyTest
    /*
    
    for (int i = 0; i < 1; i++ ){
        TH1D *hTemp1D_Test0 = getHisto(fMC[3][13], "ZNGoodJets_Zexc");
        cout << "histo loop " << 3 << " " << 13 << " " << " #events = " << hTemp1D_Test0->Integral() << endl;
    }

    TH1D* ZNGoodJetsData;
    TH1D* ZNGoodJetsSig;
    TH1D* ZNGoodJetsBG;
    
    for ( int i=0 ; i < 4 ; i++){
    
        ZNGoodJetsData = getHisto(fData[i], "ZNGoodJets_Zexc");
        
        TH1D* hTemp1D;
        TH1D* hTemp1D_2;
        for ( int j = 0 ; j < 14 ; j++){
            
            hTemp1D_2 = getHisto(fMC[i][j], "ZNGoodJets_Zexc");
            cout << "histo loop " << i << " " << j << " " << " #events = " << hTemp1D_2->Integral() << endl;
            
            if (j == 0) continue;
            else if (j == 1) {
                ZNGoodJetsSig = getHisto(fMC[i][j], "ZNGoodJets_Zexc");
                cout << " Sig events " << "QCD: " << i << " " << ZNGoodJetsSig->Integral() << endl;
            }
            else if ( j == 2 ) {
                    ZNGoodJetsBG = getHisto(fMC[i][j], "ZNGoodJets_Zexc");
                    cout << " BG Base events  " << "QCD: " << i << " " << ZNGoodJetsBG->Integral() << endl;
                }
            else if ( j > 2) {
                    hTemp1D = getHisto(fMC[i][j], "ZNGoodJets_Zexc");
                    cout << "histo loop " << i << " " << j << " " << " #events = " << hTemp1D->Integral() << endl;
                
                    ZNGoodJetsBG->Add(hTemp1D);
                    cout << " Added BG events " << "QCD: " << i << " " << ZNGoodJetsBG->Integral() << endl; 
            }
        }
        
        cout << endl;
        cout << " RECHECK #events QCD: " << i << " ZNGoodJets_Zexc Data   = " << ZNGoodJetsData->Integral() << endl;
        cout << " RECHECK #events QCD: " << i << " ZNGoodJets_Zexc Signal = " << ZNGoodJetsSig->Integral() << endl;
        cout << " RECHECK #events QCD: " << i << " ZNGoodJets_Zexc BG     = " << ZNGoodJetsBG->Integral() << endl;
    }

    for (int i = 0; i < 5; i++ ){
        TH1D *hTemp1D_Test1 = getHisto(fMC[3][13], "ZNGoodJets_Zexc");
        cout << "histo loop " << 3 << " " << 13 << " " << " #events = " << hTemp1D_Test1->Integral() << endl;
    }
    
    
    TCanvas* canvas1 = new TCanvas("dataHist2D", "canvas1");
    TCanvas* canvas2 = new TCanvas("SigHist2", "canvas2");
    TCanvas* canvas3 = new TCanvas("BGHist2D", "canvas3");
    
    TCanvas* canvas4 = new TCanvas("ZNGoodJetsData", "canvas4");
    TCanvas* canvas5 = new TCanvas("ZNGoodJetsSig", "canvas5");
    TCanvas* canvas6 = new TCanvas("ZNGoodJetsBG", "canvas6");
    
    dataHist2D[3]->SetName("dataHist2D") ;
    SigHist2D[3]->SetName("SigHist2") ;
    BGHist2D[3]->SetName("BGHist2D") ;
    ZNGoodJetsData->SetName("ZNGoodJetsData") ;
    ZNGoodJetsSig->SetName("ZNGoodJetsSig") ;
    ZNGoodJetsBG->SetName("ZNGoodJetsBG") ;
    
    fOut ->cd();
    
    canvas1->cd();
    dataHist2D[3]->Draw("LEGO1 0");
    canvas1->Write();
    
    canvas2->cd();
    SigHist2D[3]->Draw("LEGO1 0");
    canvas2->Write();
    
    canvas3->cd();
    BGHist2D[3]->Draw("LEGO1 0");
    canvas3->Write();
    
    canvas4->cd();
    ZNGoodJetsData->Draw();
    canvas4->Write();
    
    canvas5->cd();
    ZNGoodJetsSig->Draw();
    canvas5->Write();
    
    canvas6->cd();
    ZNGoodJetsBG->Draw();
    canvas6->Write();
    
    dataHist2D[3]->Write();
    SigHist2D[3]->Write();
    BGHist2D[3]->Write();
    
    ZNGoodJetsData->Write();
    ZNGoodJetsSig->Write();
    ZNGoodJetsBG->Write();
    
    */
    
    
    int nBinsX(dataHist2D[0]->GetNbinsX());
    int nBinsY(dataHist2D[0]->GetNbinsY());
    double xmin = dataHist2D[0]->GetXaxis()->GetXmin();
    double xmax = dataHist2D[0]->GetXaxis()->GetXmax();
    double ymin = dataHist2D[0]->GetYaxis()->GetXmin();
    double ymax = dataHist2D[0]->GetYaxis()->GetXmax();
    const double *xBins = new double[nBinsX+1];
    xBins = dataHist2D[0]->GetXaxis()->GetXbins()->GetArray();
    
    //get slices of 2D Histograms
    
    TH1D* hData[NQCD][Ny];
    TH1D* hSignal[NQCD][Ny];
    TH1D* hBack[NQCD][Ny];
    string dataProj = variable + "_Data";
    string SigProj  = variable + "_Sig";
    string BGProj   = variable + "_BG";
    
    for ( int i=0 ; i < NQCD ; i++){
    
                        //cout << "*************************************************** new QCD file ********************************" << endl;
                        //cout << endl;
        for (int k = 0; k < nBinsY; k++){
            
            TH1D* hDataTemp;
            TH1D* hSignalTemp;
            TH1D* hBackTemp;
            
            hDataTemp   = dataHist2D[i]->ProjectionX( dataProj.c_str(), k+1, k+1, "e");
            hSignalTemp = SigHist2D[i] ->ProjectionX( SigProj.c_str(),  k+1, k+1, "e");
            hBackTemp   = BGHist2D[i]  ->ProjectionX( BGProj.c_str(),   k+1, k+1, "e");
            // need to specify different names for each projected hist, otherwise the projected hist go to the same output hist (address)
            // this is also why I need temp variable, so that I do not have to change the names for evrery loop
            
            hData[i][k] = (TH1D *) hDataTemp->Clone();
            hSignal[i][k] = (TH1D *) hSignalTemp->Clone();
            hBack[i][k] = (TH1D *) hBackTemp->Clone();
            
            //My Test
            /*
            cout << " check bin content " << endl;
            for (int a = 0; a < 5; a ++){
                cout << "Data :" << hDataTemp->GetBinContent(a+1) <<  " " << dataHist2D[i]->GetBinContent(a+1,k+1) << " " << hData[i][k]->GetBinContent(a+1)  <<  " // ";
            }
            cout << endl;
            
            for (int a = 0; a < 5; a ++){
                cout << "Sig : " << hSignalTemp->GetBinContent(a+1) <<  " " << SigHist2D[i]->GetBinContent(a+1,k+1) << " " << hSignal[i][k]->GetBinContent(a+1)  <<  " // ";
            }
            cout << endl;
            
            for (int a = 0; a < 5; a ++){
                cout << "BG :" << hBackTemp->GetBinContent(a+1) <<  " " << BGHist2D[i]->GetBinContent(a+1,k+1) << " " << hBack[i][k]->GetBinContent(a+1)  <<  " // ";
            }
            cout << endl;
            */
        }
        cout << endl;
    }
    
    //--- QCD estiamtion procedure : followed as descirbed in AN2012_331 pages 10,11 --- 
    cout << " ***** QCD estimation ***** " << endl;
    TH1D* hQCD[NQCD][Ny];

    //--- loop over multiplicity bin -------
    
    for (int k = 0; k < nBinsY; k++){
        
        // step 0 : initial normalization of Wjets and data
        double NormFactor(0);
        if( hSignal[0][k]->Integral() > 0 ){
            NormFactor = ( hData[0][k]->Integral() - hBack[0][k]->Integral() ) / hSignal[0][k]->Integral() ;
        }
        else NormFactor = 0;
        
        for ( int j = 0  ; j < 15 ; j++){
            
            if ( j > 0 ){
                if( hSignal[0][k]->Integral() > 0 ){
                    NormFactor = (hData[0][k]->Integral() - hBack[0][k]->Integral() - hQCD[0][k] ->Integral() ) / hSignal[0][k]->Integral() ;
                }
                else NormFactor = 0;
            }
            
            // step 1:
            for ( int i = 0  ; i < NQCD ; i++){
                
                        //cout << "   file QCD " << i << " Integral Data = " << hData[i][k]->Integral() << " Sig + BG = " << hSignal[i][k]->Integral() + hBack[i][k]->Integral() << " Sig = " << hSignal[i][k]->Integral() << " BG = " << hBack[i][k]->Integral() << endl;
                
                TH1D* scaledMC[NQCD][nBinsY];
                scaledMC[i][k] = (TH1D*) hSignal[i][k]->Clone();
                scaledMC[i][k]->Scale(NormFactor) ;
                hQCD[i][k] = (TH1D*) hData[i][k]->Clone();
                hQCD[i][k]->Add(scaledMC[i][k],-1);
                hQCD[i][k]->Add(hBack[i][k],-1);
                
                        //cout << "             //--- applying fw  = " << NormFactor << " and calculate QCD hist ---- " << endl;
                        //cout << "             file QCD " << i << " Integral Data = " << hData[i][k]->Integral() << " Scaled_Sig + BG = " << scaledMC[i][k]->Integral() + hBack[i][k]->Integral() << " Scaled_Sig = " << scaledMC[i][k]->Integral() << " BG = " << hBack[i][k]->Integral() << endl;
                        //cout << "             file QCD " << i << " Integral QCD = " << hQCD[i][k]->Integral() << endl;
                
                // set the negative bins to 0
                for ( int m = 1 ; m < hData[i][k]->GetNbinsX()+1 ; m++ ){
                    if (  hQCD[i][k]->GetBinContent(m) < 0 ) hQCD[i][k]->SetBinContent(m, 0.);
                }
                        //cout << "             //--- set the negative bins of QCD hist to 0  ---- " << endl;
                        //cout << "             file QCD " << i << " Integral QCD = " << hQCD[i][k]->Integral() << endl;
            }
                        //cout << "   Reprint calculated QCD" << endl;
                        //cout << "   integral QCD 1: " << hQCD[1][k]->Integral() << " integral QCD 2: " << hQCD[2][k]->Integral() << " integral QCD 3: " << hQCD[3][k]->Integral() << endl;
            
            // step 2: isoaltion fake rate
            double NormFactorISO =  hQCD[1][k]->Integral(); 
            if ( hQCD[3][k]->Integral() > 0 ) NormFactorISO /= hQCD[3][k]->Integral();
            else NormFactorISO = 0;
            
            
            // step 3 : isolation fake-rate from step 2 is aplied to QCD[2]
            hQCD[0][k] = (TH1D*) hQCD[2][k]->Clone();
            hQCD[0][k] ->Scale(NormFactorISO);
            
            cout << endl;
            //cout << " *** RESULTS ******* " << endl;
            cout << " itter_th: " << j << " multiplicity: " << k+1 << "   ratio of regions B to D = " << NormFactorISO << endl;
            cout << " itter_th: " << j << " multiplicity: " << k+1 << "                        fw = " << NormFactor << endl;
            cout << " result integral QCD 0 " << hQCD[0][k]->Integral() << endl;
        }
    }
    //--- End loop over multiplicity bin ---
    
    //get 2D Histogram for QCD
    TH2D* QCDHist2D;
    if (xBins == 0){QCDHist2D = new TH2D(variable.c_str(), variable.c_str(), nBinsX, xmin, xmax, nBinsY, ymin, ymax);}
    else {QCDHist2D = new TH2D(variable.c_str(), variable.c_str(), nBinsX, xBins, nBinsY, ymin, ymax);}
    
    for (int k = 0; k < nBinsY; k++){
        for (int m = 0; m < nBinsX; m++){
            double tempBinContent = hQCD[0][k]->GetBinContent(m+1);
            double tempBnError = hQCD[0][k]->GetBinError(m+1);
            QCDHist2D->SetBinContent(m+1, k+1, tempBinContent);
            QCDHist2D->SetBinError(m+1, k+1, tempBnError);
        }
    }
    
    TCanvas* canvas1 = new TCanvas((variable + "_QCD").c_str(), (variable + "_QCD").c_str());
    canvas1->cd();
    QCDHist2D->SetStats(0);
    QCDHist2D->SetFillColor(kYellow);
    QCDHist2D->Draw("LEGO1 0");
    
    // ---- save to file
    cout << " now save results of " << variable << " to file" << endl;
    fOut ->cd();
    QCDHist2D ->Write();
    canvas1 ->Write();
    
}


