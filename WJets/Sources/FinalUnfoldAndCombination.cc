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
#include <TList.h>
#include "UnfoldingSyst.h"
#include "getFilesAndHistograms.h"
#include "unfoldingFunctions.h"
#include "FinalUnfoldAndCombination.h"
#include "FastPlots.h"

using namespace std;

//--  Setting global variables --------------------------------------------------------------
#include "fileNames.h"
#include "variablesOfInterestVarWidth.h"
string outputDirectory="PNGFiles/CombinedUnfold/";
//
bool isMuon(0);
string unfAlg = "Bayes";
string energy = getEnergy();
int JetPtMin(30);
int JetPtMax(0);
////-------------------------------------------------------------------------------------------
//
// not the smartest to open files for each variable ....
int nFilesBkg(0);
const int NBkgSyst(5);
TFile *fData[2][5];             // 0 = central, 1 = JES Up, 2 = JES Down , 3 - SF Up , 4 - SF down
TFile *fDYMadGraph[2][4];
TFile *fDYSherpaUnf[2] ;               // 0 = central, 1 = PU Up,  2 = PU Down,  3 = JER Up
TFile *fDYSherpa = NULL;               // 0 = central, 1 = PU Up,  2 = PU Down,  3 = JER Up
TFile *fDYPowheg = NULL, *fDYPowhegUp = NULL, *fDYPowhegDown = NULL;
TFile *fBG[2][15][NBkgSyst];            // 0 = central, 1 = PU Up,  2 = PU Down,  3 = XS Up,  4 = XS Down 

int nFiles = NFILESDYJETS;
bool isDoubleLep(1);

void FinalUnfoldAndCombination(int start, int finish)
{
//    getAllFiles();
    for (int i = start; i < finish /*NVAROFINTEREST*/; i++){
        FuncUnfold(VAROFINTERESTZJETS[i].name, VAROFINTERESTZJETS[i].BayesktermCombine);
        closeAllFiles();
    }
}


void FuncUnfold(string variable,  int UsedKtermBayes, bool doFlat , bool doVarWidth )
{
    TH1::SetDefaultSumw2();
    JetPtMin = 30 ;
    if (variable.find("JetPt")!= string::npos && variable.find("Highest") == string::npos && JetPtMin == 20)  JetPtMin = 15 ;
    if (variable.find("JetPt")!= string::npos && JetPtMin == 30)  JetPtMin = 20 ;

    string leptonFlavor = "Combine";

    // set efficiency errors
    double muonIDIsoHLTError = 0.025;// 2012 : id,iso = 0.005, 0.002 --> 1.4% for dimuon + HLT: 2% ??????
    double electronIDIsoHLTError = 0.005; // SF factors for 2012: 0.002
    double EffError = muonIDIsoHLTError ;
    if ( !isMuon ) EffError = electronIDIsoHLTError ;


    int NumberOfToys(400),  oppNumberOfToys(4);
    int UsedKterm = UsedKtermBayes, oppUsedKterm = UsedKtermBayes;
    UsedKterm = 150 ;
    TH1D *hDY[4];
    RooUnfoldResponse *resDY[4];
    TH2D * respHistos[4];
    TH1D *hDYGenMadGraph = NULL, *hDYGenSherpa = NULL,  *hDYGenPowheg = NULL, *hDYGenPowhegUp = NULL, *hDYGenPowhegDown = NULL;
    /* 
       TFile *fDYMadGraph[2][4];
       TFile *fDYSherpa = NULL;               // 0 = central, 1 = PU Up,  2 = PU Down,  3 = JER Up
       TFile *fDYPowheg = NULL, *fDYPowhegUp = NULL, *fDYPowhegDown = NULL;
       */
    int NBins = 0 ; 
    int countBins = 0 ;
    int MinNumberInBin = 1;
    int NFilesMC(4);
    int NFilesData(5); // number of data histograms -- realted to the field bellow
    // loaded objects
    TH1D *hDataOrig[2][5];
    TH1D *hGen[2][3];
    TH1D *hDYOriginal[3][4];
    TH2D* hResponse[2][5];
    RooUnfoldResponse *resDYOriginal[2][4];
    RooUnfoldResponse *resDYSherpaOriginal[2];
    RooUnfoldResponse *resDYSherpa;


    /// merged histograms
    TH1D *hData[5];
    TH1D *hDYT[5];
    TH1D* hGenMerged[5];
    TH1D* hGenCombine[5];
    TH2D* hResCombine[5];
    TH1D* hDYmerge[5];
    // needed sherpa combined histograms
/*    TH1D* hGenSherpaMerged;
    TH1D* hDYSherpamerge;
    TH2D* hResSherpaCombine;
*/

    for ( int i = 0; i < 2 ; i++){
        leptonFlavor = "DMu";
        if ( i == 1 ) leptonFlavor = "DE";

        //-- fetch the data files and histograms --------------
        getFiles(FILESDIRECTORY, fData[i], leptonFlavor, energy, ProcessInfo[DATAFILENAME].filename, JetPtMin, JetPtMax, doFlat,  doVarWidth, 0, 0, 0, 0, 0, 1);
        getHistos(hDataOrig[i], fData[i], variable);
        //-----------------------------------------------------
        // for now we do not produce scale error corrected histograms for muons
        // once we have SF for muons we should reproduce histos as in the electron case
        if ( i == 0 ){
            hDataOrig[i][4] = (TH1D*) hDataOrig[i][0]->Clone();
            hDataOrig[i][4]->Scale(1 + muonIDIsoHLTError );
            hDataOrig[i][3] = (TH1D*) hDataOrig[i][0]->Clone();
            hDataOrig[i][3]->Scale(1 - muonIDIsoHLTError );
        }

        cout << " got data , now let's try to get UNF DYJets or WJets" << endl;
        //-- fetch the DY files and histograms ----------------

        getFiles(FILESDIRECTORY, fDYMadGraph[i], leptonFlavor, energy, ProcessInfo[DYMADGRAPHFILENAME].filename, JetPtMin, JetPtMin, doFlat,            doVarWidth , 0, 0, 0, 0, 0, 1);
        fDYSherpaUnf[i] = getFile(FILESDIRECTORY, leptonFlavor, energy, DYSHERPAZjFILENAME, JetPtMin, JetPtMin, doFlat, doVarWidth, 0, 0, 0, 0, 0, "","0",0, 1);
        fDYSherpa = getFile(FILESDIRECTORY, leptonFlavor, energy, DYSHERPAFILENAME, JetPtMin, JetPtMin, doFlat, doVarWidth);
        fDYPowheg = getFile(FILESDIRECTORY, leptonFlavor, energy, DYPOWHEGFILENAME, JetPtMin, JetPtMin, doFlat, doVarWidth);

        // files are open now let's fetch histos of interest
        RooUnfoldResponse *resDYT[4];

        TH1D *hDYGenMadGraphT = NULL, *hDYGenSherpaT = NULL,  *hDYGenPowhegT = NULL, *hDYGenPowhegUpT = NULL, *hDYGenPowhegDownT = NULL;
        getHistos(hDYT, fDYMadGraph[i], variable);
        getHistos(hDYOriginal[i], fDYMadGraph[i], variable);
        if ( i == 1 ) hDYOriginal[i][0]->Draw("sranje");
        hDYGenMadGraphT = getHisto(fDYMadGraph[i][0], "gen" + variable);
        hDYGenSherpaT = getHisto(fDYSherpa, "gen" + variable);
        hDYGenPowhegT = getHisto(fDYPowheg, "gen" + variable);
        hGen[i][0] = getHisto(fDYMadGraph[i][0], "gen" + variable);
        hGen[i][1] = getHisto(fDYSherpa, "gen" + variable);
        hGen[i][2] = getHisto(fDYPowheg, "gen" + variable);

        //hDYGenPowhegUp = getHisto(fDYPowhegUp, "gen" + variable);
        //hDYGenPowhegDown = getHisto(fDYPowhegDown, "gen" + variable);
        cout <<__LINE__ << endl;
        getResps(resDYT, fDYMadGraph[i], "response" + variable);
        getResps(resDYOriginal[i], fDYMadGraph[i], "response" + variable);


        cout << " got DY files" << DYMADGRAPHFILENAME << endl;
        if ( i == 0 )  NBins = hDYT[0]->GetNbinsX() + 1 ; // include overflow as bin ---
        cout <<__LINE__ << endl;
        TArrayD*  GetXbins = (TArrayD *)  hDYT[0]->GetXaxis()->GetXbins();
        //TArrayD*  GetXbins = (TArrayD *)  hDYT[0]->GetXaxis()->GetXbins()->fArray;
        //TArrayD  GetXbins = (TArrayD *)  hDYT[0]->GetXaxis()->GetXbins()->fArray;
        cout << hDYT[0]->GetXaxis()->GetXbins()->fN << endl;
        cout <<__LINE__ << endl;
        //Double_t*  test = GetXbins->GetArray() ;
        cout << " aaaa" << NBins << endl;

        // get SHerpa secondary unfolding files
        cout << " get sherpa response " << endl;
        resDYSherpaOriginal[i] = (RooUnfoldResponse*) getResp(fDYSherpaUnf[i], "response" + variable);



        // check if overflow bin has some minimum number of events in data and MC
        cout <<__LINE__ << endl;
        for ( int l = 0 ; l <  NFilesMC ; l++){
            if (    hDYT[l]->GetBinContent(NBins ) <  MinNumberInBin ) NBins = hDYT[l]->GetNbinsX() ;
            if (    hGen[i][0]->GetBinContent(NBins ) <  MinNumberInBin ) NBins = hDYT[l]->GetNbinsX() ;
        }






    }


    //// NOW CREATE MERGED DATA HISTOGRAMS 
    cout << " lets plot the data histograms" << endl;
    for ( int i = 0 ; i < NFilesData ; i++){
        stringstream ss;        ss <<"mergeData"<< i;
        string str = ss.str();
        cout << hDataOrig[1][i]->GetNbinsX() << endl;
        hData[i] = (TH1D * ) combinedHisto(hDataOrig[0][i], hDataOrig[1][i], NBins ) ;
        createCanvas(hData[i], str); 
        cout << " we merged data "<< i  << endl;
    }

    cout << " lets plot the DY gen histograms" << endl;
    for ( int i = 0 ; i < NFilesMC ; i++){
        // NOW CREATE COMBINED GEN HISTOGRAMS
        cout << " number of bins before combine:" << NBins << endl;
        stringstream ss;        ss <<"mergedGen"<< i;
        string str = ss.str();
        //hGenMerged[i] = (TH1D * ) combinedHisto(hGen[0][i], hGen[1][i], NBins ) ;
        hGenMerged[i] = (TH1D * ) combinedHisto((TH1D*) resDYOriginal[0][i]->Htruth()->Clone(), (TH1D*) resDYOriginal[1][i]->Htruth()->Clone(), NBins ) ;
        createCanvas(hGenMerged[i], str);
        ss <<"combineGen"<< i;
        str = ss.str();
        cout << " befogre mergeGenHisto "<<__LINE__<< endl;
        //hGenCombine[i] = mergeGenHisto(hGen[0][i], hGen[1][i], NBins);
        hGenCombine[i] = mergeGenHisto((TH1D*) ( (TH1D*) resDYOriginal[0][i]->Htruth())->Clone(), (TH1D*) resDYOriginal[1][i]->Htruth()->Clone(), NBins);
        createCanvas(hGenCombine[i], str);
        // lets create RECO from DY
        cout << " lets create common  2 x Nbins RECO DY histo " << __LINE__ << endl;
        hDYmerge[i] = (TH1D * ) combinedHisto(hDYOriginal[0][i], hDYOriginal[1][i], NBins ) ;
        hDYmerge[i] = combinedHisto((TH1D*) ( (TH1D*)resDYOriginal[0][i]->Hmeasured())->Clone(), (TH1D*) resDYOriginal[1][i]->Hmeasured()->Clone(), NBins);
        cout << " created 2 x Nbins RECO DY histo with N bins = " << hDYmerge[i]->GetNbinsX() << endl;

        hResCombine[i] = ( TH2D*) combineTH2DRes( (TH2D*) ((TH2D*) resDYOriginal[0][i]->Hresponse())->Clone() , (TH2D*) ( (TH2D*)resDYOriginal[1][i]->Hresponse())->Clone() , NBins);
        cout << " just before ccreating combined roounfold object " << endl;
        cout << hDYmerge[i]->GetNbinsX() <<"   " << hGenCombine[i]->GetNbinsX()<<"  " << hResCombine[i]->GetNbinsX() <<"  " << hResCombine[i]->GetNbinsY()<< endl;
        resDY[i] = new RooUnfoldResponse((TH1D*) hDYmerge[i]->Clone(),(TH1D* ) hGenCombine[0]->Clone(),(TH2D*) hResCombine[i]->Clone());
        cout << " check gen combine"<< endl;

    }
    for ( int i =1 ; i < hGenCombine[0]->GetNbinsX()+1; i++){
        cout << " test gen:  " << i << "   "<< hGenCombine[0]->GetBinContent(i)<< "   data" << hData[0]->GetBinContent(i)<< endl;


    }
    // sherpa merged histograms and repsonse objects
    cout<<__LINE__<<"    " << variable << endl;
    resDYSherpaOriginal[0]->Write("sherpaEleResponse");
    TH1D* hGenSherpaMerged   = (TH1D * ) combinedHisto((TH1D*) resDYSherpaOriginal[0]->Htruth()->Clone(), (TH1D*) resDYSherpaOriginal[1]->Htruth()->Clone(), NBins ) ;
    TH1D* hGenSherpaCombine= mergeGenHisto((TH1D*) ( (TH1D*) resDYSherpaOriginal[0]->Htruth())->Clone(), (TH1D*) resDYSherpaOriginal[1]->Htruth()->Clone(), NBins);
    TH1D* hDYSherpamerge     = combinedHisto((TH1D*) ( (TH1D*)resDYSherpaOriginal[0]->Hmeasured())->Clone(), (TH1D*) resDYSherpaOriginal[1]->Hmeasured()->Clone(), NBins);
    TH2D* hResSherpaCombine  = ( TH2D*) combineTH2DRes( (TH2D*) ((TH2D*) resDYSherpaOriginal[0]->Hresponse())->Clone() , (TH2D*) ( (TH2D*)resDYSherpaOriginal[1]->Hresponse())->Clone() , NBins);
        
        resDYSherpa = new RooUnfoldResponse((TH1D*) hDYSherpamerge->Clone(),(TH1D* ) hGenSherpaCombine->Clone(),(TH2D*) hResSherpaCombine->Clone());
    ///
    //-- fetch the BG files and histograms ----------------
    cout << " SELECTION = " << variable << endl; 
    cout << " Fetch background files " << endl;
    const int NBkgGroups(6);
    nFilesBkg = 0 ;

    TH1D *hBG[15][10], *hSumBG[NBkgSyst],*hSumBGgroup[15][10];
    TH1D *hBGTmp[2][15][10], *hSumBGTmp[2][NBkgSyst],*hSumBGgroupTmp[2][15][10];
    int group[2] = {-1, -1} ;
    double sumEve = 0.;
    //    int nFiles = NFILESDYJETS;
    //    bool isDoubleLep(1);
    if ( leptonFlavor == "SMuE" || leptonFlavor == "SMu" || leptonFlavor == "Muon") {
        isDoubleLep = 0;
        nFiles = NFILESTTBAR;
    }
    int countFiles = 0 ;
    //int countBGp[2][5]={-1,-1, -1, -1, -1, -1, -1, -1, -1, -1};
    int countBGp[2][5]={{-1,-1, -1, -1, -1},{ -1, -1, -1, -1, -1}};
    cout << "AAAABBBB"<<endl;
    for (int i(0); i < 2; i++){
        for (int j(0); j < 5; j++){
            cout << countBGp[i][j]<<"   ";
        }
        cout<< endl;
    }
    for (int i(0); i < nFiles; i++){
        int fileSelect = FilesDYJets[i] ;
        if (!isDoubleLep) fileSelect = FilesTTbarWJets[i] ;
        string fileNameTemp =  ProcessInfo[fileSelect].filename ;
        if ( fileNameTemp.find("Tau") != string::npos || fileNameTemp.find("Data") != string::npos) continue;
        if ( isDoubleLep && fileNameTemp.find("DYJets") != string::npos) continue;
        if ( !isDoubleLep && fileNameTemp.find("WJets") != string::npos && fileNameTemp.find("UNF") != string::npos) continue;
        cout << " Fetch background files " << fileNameTemp << endl;
        nFilesBkg++;
        for ( int k = 0; k < 2 ; k++){
            leptonFlavor = "DMu";
            if ( k == 1 ) leptonFlavor = "DE";
            //getFiles(FILESDIRECTORY, fBG[countFiles], leptonFlavor, energy, BGFILENAMESGrouped[i + 1], JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0,  1 );
            cout <<" back file:   "<<  i <<"    "<< variable << "   " << countBGp[0][0] << "   " << countBGp[1][0] << endl;
            int selectBkg(0);
            if ( isDoubleLep ){
                if ( fileNameTemp.find("ZZ") != string::npos){
                    if (countBGp[k][0] == -1 ) group[k]++ ;

                }
                else if  ( fileNameTemp.find("WZ") != string::npos ){
                    if ( countBGp[k][1] == -1 ) group[k]++ ;
                    selectBkg =  1;
                }

                else  {group[k]++ ;
                    selectBkg = group[k] ;
                }
            }

            else  { 
                group[k]++ ;
                selectBkg = group[k] ;
            }

            cout << "group" << "   " << group[k] << fileNameTemp <<"  sel = " << selectBkg << endl;
            /*
               TFile *fBG[15][NBkgSyst];            // 0 = central, 1 = PU Up,  2 = PU Down,  3 = XS Up,  4 = XS Down 
               getFiles(FILESDIRECTORY, fBG[countFiles], leptonFlavor, energy, fileNameTemp, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, 0, 1 );
               getHistos(hBGTmp[k][countFiles], fBG[countFiles], variable);
               closeFiles(fBG[countFiles], NBkgSyst );
               */
            getFiles(FILESDIRECTORY, fBG[k][countFiles], leptonFlavor, energy, fileNameTemp, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, 0, 1 );
            getHistos(hBGTmp[k][countFiles], fBG[k][countFiles], variable);

            for (int j(0); j < NBkgSyst ; j++){
                cout << "usbgroup " << j << "   " << countFiles << endl;
                if (countFiles == 0) hSumBGTmp[k][j] = (TH1D*) hBGTmp[k][countFiles][j]->Clone();
                else {
                    cout <<" AAAA" << endl;   
                    hSumBGTmp[k][j]->Add(hBGTmp[k][countFiles][j]);
                    cout <<" BBBB" << endl;   

                }
                cout << "USBG " << fileNameTemp <<"  " << group[k] <<"    " << k <<"   " <<  j << "   " << countFiles << " count bck  "<< selectBkg <<"   " << countBGp[k][selectBkg] <<endl;

                if ( fileNameTemp.find("ZZ") != string::npos ){
                    cout <<"ZZ  "<< countBGp[k][0] << endl;
                    //if ( countBGp[k][0] == -1 ) {
                    if ( countBGp[k][selectBkg] == -1 ) {
                        if ( j == 4 ) countBGp[k][0] = group[k] ;
                        hSumBGgroupTmp[k][j][group[k]] = (TH1D*) hBGTmp[k][countFiles][j]->Clone();
                    }
                    else {
                        hSumBGgroupTmp[k][j][ countBGp[k][0]]->Add(hBGTmp[k][countFiles][j]);
                    }
                }
                else if  ( fileNameTemp.find("WZ") != string::npos ){
                    if ( countBGp[k][1] == -1 ) {
                        if ( j == 4 ) countBGp[k][1] = group[k] ;
                        hSumBGgroupTmp[k][j][group[k]] = (TH1D*) hBGTmp[k][countFiles][j]->Clone();
                    }
                    else {
                        hSumBGgroupTmp[k][j][ countBGp[k][1]]->Add(hBGTmp[k][countFiles][j]);
                    }
                }

                else {
                    cout << " tu :" << j<< "  " << group[k] << endl;
                    hSumBGgroupTmp[k][j][group[k]] = (TH1D*) hBGTmp[k][countFiles][j]->Clone();
                    //else  hSumBGgroup[j][group]->Add(hBG[countFiles][j]);
                }

                if ( j == 0 ) {
                    sumEve += hBGTmp[k][countFiles][j]->GetBinContent(1);
                }

                cout << " end for systematics labaled : "<<i<<"   " << j << "   " << group[k] << endl;
                } // end loop over number of systematics for background
                cout<<" JUST BEFORE CLOSING"<< endl;
                //           closeFiles(fBG[countFiles], NBkgSyst );
            }   // end loop over lepton flavors
            countFiles++;
        }
        cout << " create a common background histograms " <<endl;
        for (int i(0); i < NBkgSyst ; i++){
            double sum = 0. ;
            hSumBG[i] = (TH1D * ) combinedHisto(hSumBGTmp[0][i], hSumBGTmp[1][i], NBins ) ;
            for (int j(0); j < group[0] + 1  ; j++){
                cout << hSumBGTmp[0][i]->GetBinContent(1)<< "   " << hSumBGTmp[1][i]->GetBinContent(1)<<endl;
                cout << hSumBGgroupTmp[0][i][j]->GetBinContent(1)<< "   " << hSumBGgroupTmp[1][i][j]->GetBinContent(1)<<endl;
                hSumBGgroup[i][j] = (TH1D * ) combinedHisto(hSumBGgroupTmp[0][i][j], hSumBGgroupTmp[1][i][j], NBins ) ;
                cout << " combined backgournd :" << i <<"   " << j << "    " << hSumBGgroupTmp[0][i][j]->GetBinContent(1)<< "   " << hSumBGgroupTmp[1][i][j]->GetBinContent(1) << endl;
                sum +=  hSumBGgroupTmp[0][i][j]->GetBinContent(1) ;
            }
            cout << " checking kg: " << hSumBG[i]->GetBinContent(1) <<"   " << sum << endl;
        }
        cout << " created commmon background histograms " << group[0] <<"   " << group[1] << endl;

        TH1D* hDataNoBkg = (TH1D* )  hData[0]->Clone() ;
        cout << " check bakground events" << endl;
        for ( int i = 0 ; i < group[0]+1  ; i++){
            hDataNoBkg->Add(hSumBGgroup[0][i], -1);
            for ( int j = 0 ; j < hData[0]->GetNbinsX(); j++){
                cout << hSumBGgroup[0][i]->GetBinContent(j+1)<<"   " ;

            }
            cout << endl;
        }

        resDY[0]->Draw();
        hData[0]->Draw();





        cout << " let's unfold " << hData[0]->GetXaxis()->GetNbins()<<"  resp:" <<resDY[0]->Hresponse()->GetXaxis()->GetNbins()<<"    " << resDY[0]->Hresponse()->GetYaxis()->GetNbins()<< endl;
        RooUnfold* RObject;
        //RObject = (RooUnfold*) RooUnfold::New( RooUnfold::kBayes, resDY[0], (TH1D*) hData[0]->Clone(),     3);
        RObject = (RooUnfold*) RooUnfold::New( RooUnfold::kBayes, resDY[0], (TH1D*) hDataNoBkg->Clone(),     3);
        cout << " let's unfold " << endl;
        RObject->SetVerbose(0);
        cout << " let's unfold " << endl;
        TH1D* hCorrected = (TH1D*) RObject->Hreco();
        cout << " let's unfold " << endl;



        //-----------------------------------------------------
        //
        //  // unfold central and shifted data and MC 
        //    cout << " loaded all unfolded files"<<endl;
        // set the output
        string outputRootFileName = outputDirectory + "Combined_" + energy +  "_unfolded_"+variable+"_histograms_" + unfAlg ;
        if (doVarWidth) outputRootFileName += "_VarWidth";
        outputRootFileName += ".root";
        TFile* outputRootFile = new TFile(outputRootFileName.c_str(), "RECREATE");
        //outputRootFile = new TFile(outputRootFileName.c_str(), "RECREATE");
        //
        //TFile *outputRootFile = new TFile("sranje.root", "RECREATE");
        outputRootFile->cd();

        resDY[0]->Write("response object");
        hData[0]->Write();
        hCorrected->Write("corrected");
        //hDYGenMadGraph->Write();
        //
        hDataOrig[0][0]->Write("dataMuon");
        hDataOrig[1][0]->Write("dataEle");
        hGen[0][0]->Write("genMuon");
        hGen[1][0]->Write("genEle");
        ((TH1D*) resDYOriginal[0][0]->Htruth())->Write("truthMuon");
        ((TH1D*) resDYOriginal[1][0]->Htruth())->Write("truthEle");
        hGenCombine[0]->Write("genCombine");

        /*
           for ( int i = 0 ; i < NBins; i++){
           cout <<i<< "   " << hDY[0]->GetBinContent(i)<<"   " << "   " << hDY[1]->GetBinContent(i)<<"   " << hDYGenMadGraph->GetBinContent(i) <<"   " << hCorrected->GetBinContent(i)  << "   " << hDYT[0]->GetBinContent(i)<<  "   " << hDYOriginal[0][0]->GetBinContent(i) <<"   " << hDYOriginal[1][0]->GetBinContent(i) <<endl;


           }
           */
        //    hCorrected->Divide(hDY[0]);
        //hCorrected->Draw("TEXT");
        createCanvas(hCorrected, "dataCorrected");


        RooUnfoldResponse *response = new RooUnfoldResponse((TH1D*) hDYmerge[0]->Clone(), (TH1D*) ((TH1D*) resDYOriginal[0][0]->Htruth())->Clone()) ;
        cout << response->Hresponse()->GetXaxis()->GetNbins()<<"    " << response->Hresponse()->GetYaxis()->GetNbins()<< "  truth: " << ((TH1D*) resDYOriginal[0][0]->Htruth())->GetXaxis()->GetNbins() << "  reco : " << hDYmerge[0]->GetXaxis()->GetNbins()<<endl;


        //////////////////////// PROPER UNFOL
        //////////////////////// PROPER UNFOL
        //////////////////////// PROPER UNFOL
        //////////////////////// PROPER UNFOL
        //
        //
        cout << "  PROPER UNFOL  " << endl;

        //-- save data reco
        outputRootFile->cd();  hData[0]->Write("Data"); hData[1]->Write("DataJECup");hData[2]->Write("DataJECdown");
        cout << " saved data reco and madgraph files" << endl;
        //-- save madgraph gen also in the file
        outputRootFile->cd();  hGenCombine[0]->Write("genMadCombine");hGenMerged[0]->Write("genMadMerge");
        cout << " saved data reco and madgraph files" << endl;
        //-- save sherpa and powheg gen also in the file
        outputRootFile->cd();
        //hDYGenSherpa->Write("genShe");
        //-- save powheg gen also in the file
        outputRootFile->cd();
        //hDYGenPowheg->Write("genPow");  
        //hDYGenPowhegUp->Write("genPowUp");  
        //hDYGenPowhegDown->Write("genPowDown");
        cout << "Start unfolding offseted histograms on RECO "<<endl;

        string hNames[8] = {"Central", "JECup", "JECdown", "PUup", "PUdown", "XSECup", "XSECdown", "JERup"};
        int SelData[8] = {0, 1, 2, 0, 0, 0, 0, 0};
        int SelDY[8]   = {0, 0, 0, 1, 2, 0, 0, 3};
        int SelBG[8]   = {0, 0, 0, 1, 2, 3, 4, 0};
        cout << "Start unfolding offseted histograms on RECO "<<endl;
        bool useOverFlow = false ;

        for ( int i(0); i < 8; i++){
            cout << "Start unfolding offseted histograms on RECO "<< SelDY[i] <<"   " <<   SelData[i] <<"   " <<  SelBG[i] <<"   " <<  UsedKterm << endl;
            cout <<resDY[SelDY[i]]->Hresponse()->GetNbinsX() <<"   " << hData[SelData[i]]->GetNbinsX()<<"  " << hSumBG[SelBG[i]]->GetNbinsX() <<"  " << endl;
            TH1D* hUnfolded = Unfold(unfAlg, resDY[SelDY[i]], hData[SelData[i]], hSumBG[SelBG[i]],  UsedKterm, hNames[i], useOverFlow );
            outputRootFile->cd();  hUnfolded->Write();
        }


        //int xbin(hData[0]->GetNbinsX());
        int xbin( hGen[0][0]->GetNbinsX());
        //now for central values compute Roounfold Cov matrix
        cout << " compute covarican mattrix from roounfold " << endl;
        TH2D* covCentralRoo    = CovFromRoo(unfAlg, resDY[0], hData[0], hSumBG[0], UsedKterm, "CentralCov",      1 , useOverFlow);
        TH2D* covCentralRooToy = CovFromRoo(unfAlg, resDY[0], hData[0], hSumBG[0], UsedKterm, "CentralCovToy", NumberOfToys , useOverFlow );
        outputRootFile->cd();  covCentralRoo->Write();  covCentralRooToy->Write();

        TH2D*  corCentralRoo= CovToCorr2((TH2D*) covCentralRoo->Clone("RooCor"), "RooCor");
        TH2D*  corCentralRooToy= CovToCorr2((TH2D*) covCentralRooToy->Clone("RooCorToy"), "RooCorToy");
        outputRootFile->cd();  corCentralRoo->Write();  corCentralRooToy->Write();

        resDY[0]->Hresponse()->Write("Pu cen");
        resDY[1]->Hresponse()->Write("Pu down");
        resDY[2]->Hresponse()->Write("Pu up");
        resDY[3]->Hresponse()->Write("JER");

        cout << "Start setting error histograms on RECO "<<endl;


        // finished with central stuff, now do covariance 
        // and my toy error propagation --> MY PRIVATE MAGIC 
        // first set errors
        //TH1D *hDataJES = SetSystErrorsMax(hData[0], hData[1], hData[2], "JESerrors");     // JES errors
        TH1D *hDataJES = SetSystErrorsMean(hData[0], hData[1], hData[2], "JESerrors");     // JES errors
        TH1D *hDataEFF = SetSystErrorsMean(hData[0], EffError, "EFFerrors");     // lepton EFF errors
        cout << "Set errors on response object "<<endl;
        //hData[3]->Draw();
        cout << "Set errors on response object "<<endl;
        //  if ( leptonFlavor == "DE" ) hDataEFF = SetSystErrorsMean(hData[0],hData[0], hData[3], hData[4],EffError,                  "EFFerrors");     // lepton EFF errors
        TH1D *hDataJER = SetSystErrorsMean(hData[0], hDYmerge[0], hDYmerge[3], hDYmerge[3], "JERerrors", 0 );     // JER errors
        cout << "Set errors on response object "<<endl;
        TH1D *hDataPU = SetSystErrorsMean(hData[0], hDYmerge[0], hDYmerge[1], hDYmerge[2], "JERerrors", 0 );      // PU errors
        cout << "Set errors on response object "<<endl;
        TH2D *hResPUErrors = SetResponseErrors( (TH2D*) resDY[0]->Hresponse(), (TH2D*) resDY[1]->Hresponse(), (TH2D*) resDY[2]->Hresponse());    // PU effect on response 
        cout << "Set errors on response object "<<endl;
        TH2D *hResJERErrors = SetResponseErrors( (TH2D*) resDY[0]->Hresponse(), (TH2D*) resDY[3]->Hresponse(), (TH2D*) resDY[3]->Hresponse());    // JER effect on response 
        cout << "Set errors on response object "<<endl;

        // my stat variation of data, BG and response object 
        RooUnfoldResponse *responseNEW[3];
        responseNEW[0] = new RooUnfoldResponse((TH1D*) resDY[0]->Hmeasured(), (TH1D*) resDY[0]->Htruth(), resDY[0]->Hresponse());
        responseNEW[1] = new RooUnfoldResponse((TH1D*) resDY[0]->Hmeasured(), (TH1D*) resDY[0]->Htruth(), hResPUErrors);
        responseNEW[2] = new RooUnfoldResponse((TH1D*) resDY[0]->Hmeasured(), (TH1D*) resDY[0]->Htruth(), hResJERErrors);

        // PU and XSEC errors on background
        TH1D* hSumBGErrors[3];
        hSumBGErrors[0] = (TH1D*) hSumBG[0]->Clone();
        hSumBGErrors[1] = SetSystErrorsMax(hSumBG[0], hSumBG[1], hSumBG[2],"BGPUerrors");    // PU errors
        hSumBGErrors[2] = SetSystErrorsMax(hSumBG[0], hSumBG[3], hSumBG[4],"BGXSECerrors");  //  XSEC errors


        TH1D* hSumBGgroupErrors[5][NBkgGroups];
        for (int i(0); i < NBkgGroups ; i++){
            hSumBGgroupErrors[0][i]  = (TH1D*) hSumBGgroup[0][i]->Clone();
            hSumBGgroupErrors[1][i]  = SetSystErrorsMax(hSumBGgroup[0][i],hSumBGgroup[1][i], hSumBGgroup[2][i], "gBGPUerrors");
            hSumBGgroupErrors[2][i]  = SetSystErrorsMax(hSumBGgroup[0][i],hSumBGgroup[3][i], hSumBGgroup[4][i], "gBGXSECerrors");
            //    outputRootFile->cd(); hSumBGgroupErrors[2][i]->Write(); 
        }

        plotChi2OfChange(responseNEW[0], UsedKtermBayes , (TH1D*) hData[0]->Clone(),  hSumBG, 1, "Combine_", variable, energy, outputDirectory, outputRootFile, 0 );

        outputRootFile->cd();
        resDY[0]->Hmeasured()->Write("recoNulti");
        resDY[0]->Htruth()->Write("genNulti");
        responseNEW[0]->Write("nulti");

        /////////////////////////////////////////////////////////////////////////////////////////////////
        cout << " do secondary unfolding : Sherpa  " << endl;
        TH1D* hUnfoldedSherpa = Unfold(unfAlg, resDYSherpa, hData[0], hSumBG[0],  UsedKterm, "CentralSherpa", useOverFlow );
        outputRootFile->cd();  hUnfoldedSherpa->Write();


        /////////////////////////////////////////////////////////////////////////////////////////////////
        cout << " finished central unfolding " << endl;
        const int nUnfold =9 ;
        string Cov[nUnfold] = {"MyToy","MyToyMC", "MyToyJES", "MyToyPU", "MyToyXSEC", "MyToyJER", "MyToyEFF","MyToyJER2","MyToyPU2"};
        int selRes[nUnfold]      = { 0, 0, 0, 1, 0, 2 , 0, 0, 0 };
        int selBGerrors[nUnfold] = { 0, 0, 0, 1, 2, 0 , 0, 0, 0};
        TH2D *hCovariance[nUnfold];
        TH2D *hCorrelation[nUnfold];
        TH1D *hOyMySyst[nUnfold];
        int SetTypeOfVariation[nUnfold] = {1, 101, 10, 2, 11, 4 , 20, 10, 10 }; // select what and how to varry
        /// option 3 --- data bins are varries independently; option 10 -- global variation pf all bins
        //  //int SetTypeOfVariation[6] = {1, 3, 2, 11, 4 , 3}; // select what and how to varry : ORIGINAL SELECTION
        //    //if (variable.find("ZNG")!= string::npos ) SetTypeOfVariation[1] = 103 ;
        //      // option (1)10: systeamtics is completly correlated on reco --> translated to cov matrix
        //        // option 3: assume errors in reco bins are uncorrelated
        //          //int myToyN[5] = {400, 400, 400, 400, 400};
        for (int i(0); i < nUnfold; i++){
            //for (int i(0); i < 1; i++){ 

            cout << endl;
            cout << endl;
            cout << "Doing my TOY " << Cov[i] << " with type  " << SetTypeOfVariation[i] << endl;
            cout << endl;
            string CovName = Cov[i] + "Cov";
            string CorName = Cov[i] + "Cor";
            string hNameTemp = "h" + Cov[i];
            TH1D *hDataTem = (TH1D*) hData[0]->Clone();
            if (i == 2) hDataTem = (TH1D*) hDataJES->Clone();
            if (i == 6) hDataTem = (TH1D*) hDataEFF->Clone();
            if (i == 7) hDataTem = (TH1D*) hDataJER->Clone();
            if (i == 8) hDataTem = (TH1D*) hDataPU->Clone();
            hCovariance[i] = new TH2D(CovName.c_str(), CovName.c_str(), xbin, 0.5, xbin+0.5, xbin, 0.5, xbin+0.5);
            hCorrelation[i] = new TH2D(CorName.c_str(), CorName.c_str(), xbin, 0.5, xbin+0.5, xbin, 0.5, xbin+0.5);
            RooUnfoldResponse *responseTemp = (RooUnfoldResponse*) responseNEW[selRes[i]]->Clone();
            TH1D* vhBack[1] ;
            vhBack[0] = (TH1D*) hSumBG[selBGerrors[i]]->Clone();
            // this function has to be cleaned
            TH1D *hToy2 ;
            if ( i == 4 ) hToy2 = (TH1D*) ToyMCErrorsStat(unfAlg, hDataTem, hSumBGgroupErrors[selBGerrors[i]], responseTemp, UsedKterm, hCovariance[i],  hCorrelation[i], NumberOfToys , SetTypeOfVariation[i], useOverFlow );
            else hToy2 = (TH1D*) ToyMCErrorsStat(unfAlg, hDataTem, vhBack, responseTemp, UsedKterm, hCovariance[i], hCorrelation[i], NumberOfToys,       SetTypeOfVariation[i], useOverFlow );
            //TH1D *hToy2 = (TH1D*) ToyMCErrorsStat(unfAlg, hDataTem, hSumBGgroupErrors[selBGerrors[i]], responseTemp, UsedKterm, hCovariance[i],        hCorrelation[i], myToyN[i], SetTypeOfVariation[i]);
            //TH1D *hToy2 = (TH1D*) ToyMCErrorsStat(unfAlg, hDataTem, (TH1D*) hSumBG[selBGerrors[i]]->Clone(), responseTemp, UsedKterm, hCovariance[i],  hCorrelation[i], myToyN[i], SetTypeOfVariation[i]);
            hOyMySyst[i] = (TH1D*) hToy2->Clone(hNameTemp.c_str());
            cout <<  " number of bins in background  histo" << vhBack[0]->GetNbinsX() << "    " <<  hNameTemp << endl;
            outputRootFile->cd();
            hOyMySyst[i]->Write();
            hCovariance[i]->Write();
            hCorrelation[i]->Write();
            //      TCanvas* aC = new TCanvas();
            //      aC->cd();
            //      responseTemp->DrawCopy();
            //    hDataTem->DrawCopy();
            //    hToy2->DrawCopy("same");     
            //    hSumBG[selBGerrors[i]]->DrawCopy("same");     
        }
        outputRootFile->Close();

        }
        /////////////////////////////////////////////////
        void getAllFiles( string leptonFlavor, bool doFlat  , bool doVarWidth){
            /* JetPtMin = 30 ;
               if (variable.find("JetPt")!= string::npos && variable.find("Highest") == string::npos && JetPtMin == 20)  JetPtMin = 15 ;
               if (variable.find("JetPt")!= string::npos && JetPtMin == 30)  JetPtMin = 20 ;
               */
            leptonFlavor = "Combine";

            if ( leptonFlavor == "SMuE" || leptonFlavor == "SMu" || leptonFlavor == "Muon") {
                isDoubleLep = 0;
                nFiles = NFILESTTBAR;
            }
            int countFiles = 0 ;

            leptonFlavor = "DMu";
            for ( int i = 0; i < 2 ; i++){
                if ( i == 1 ) leptonFlavor = "DE";

                //-- fetch the data files and histograms --------------
                getFiles(FILESDIRECTORY, fData[i], leptonFlavor, energy, ProcessInfo[DATAFILENAME].filename, JetPtMin, JetPtMax, doFlat,  doVarWidth, 0, 0, 0, 0, 0, 1);
                getFiles(FILESDIRECTORY, fDYMadGraph[i], leptonFlavor, energy, ProcessInfo[DYMADGRAPHFILENAME].filename, JetPtMin, JetPtMin, doFlat,                 doVarWidth , 0, 0, 0, 0, 0, 1);
                fDYSherpa = getFile(FILESDIRECTORY, leptonFlavor, energy, DYSHERPAFILENAME, JetPtMin, JetPtMin, doFlat, doVarWidth);
                fDYPowheg = getFile(FILESDIRECTORY, leptonFlavor, energy, DYPOWHEGFILENAME, JetPtMin, JetPtMin, doFlat, doVarWidth);
            }
            /// get bacground files
            for (int i(0); i < nFiles; i++){
                int fileSelect = FilesDYJets[i] ;
                if (!isDoubleLep) fileSelect = FilesTTbarWJets[i] ;
                string fileNameTemp =  ProcessInfo[fileSelect].filename ;
                if ( fileNameTemp.find("Tau") != string::npos || fileNameTemp.find("Data") != string::npos) continue;
                if ( isDoubleLep && fileNameTemp.find("DYJets") != string::npos) continue;
                if ( !isDoubleLep && fileNameTemp.find("WJets") != string::npos && fileNameTemp.find("UNF") != string::npos) continue;
                cout << " Fetch background files " << fileNameTemp << endl;
                nFilesBkg++;
                for ( int k = 0; k < 2 ; k++){
                    leptonFlavor = "DMu";
                    if ( k == 1 ) leptonFlavor = "DE";
                    getFiles(FILESDIRECTORY, fBG[k][countFiles], leptonFlavor, energy, fileNameTemp, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, 0, 1 );

                }
                countFiles++;
            }

        }
        /////////////////////////////////////////////////
        void closeAllFiles(){    
            //-- Close all the files ------------------------------
            cout << " CLOSE ALL FILES"<< endl;
            closeFile(fDYSherpa);
            closeFile(fDYPowheg);
            //    //closeFile(fDYPowhegUp);
            //      //closeFile(fDYPowhegDown);
            for (int j(0); j < 2 ; j++) {
                closeFiles(fData[j], 3);
                closeFiles(fDYMadGraph[j]);
                for (int i(0); i < nFilesBkg ; i++)     closeFiles(fBG[j][i]);
                //
            }
        }

