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
#include "UnfoldingSyst.h"
#include "getFilesAndHistograms.h"
#include "FinalUnfold.h"


using namespace std;

//--  Setting global variables --------------------------------------------------------------
#include "fileNames.h"
#include "variablesOfInterestVarWidth.h"
string outputDirectory="PNGFiles/FinalUnfold/";

bool isMuon(0);
string unfAlg = "Bayes";
string energy = getEnergy();
int JetPtMin(30);
int JetPtMax(0);
//-------------------------------------------------------------------------------------------

void FinalUnfold()
{
//  variableStruct VAROFINTEREST[] = VAROFINTERESTZJETS ; 
//  for (int i(0); i < NVAROFINTERESTZJETS/*NVAROFINTEREST*/; i++){
  for (int i(0); i < 1/*NVAROFINTEREST*/; i++){
    for (int j(0); j < 2; j++){
      isMuon = j ;
      if (isMuon) FuncUnfold(VAROFINTERESTZJETS[i].name, VAROFINTERESTZJETS[i].MuBayeskterm, VAROFINTERESTZJETS[i].MuSVDkterm);
      else FuncUnfold(VAROFINTERESTZJETS[i].name, VAROFINTERESTZJETS[i].EBayeskterm, VAROFINTERESTZJETS[i].ESVDkterm);
    }
  }

}


void FuncUnfold(string variable,  int UsedKtermBayes, int UsedKtermSVD, bool doFlat , bool doVarWidth )
{
  TH1::SetDefaultSumw2();
  JetPtMin = 30 ;
  if (JetPtMin == 30) outputDirectory="PNGFiles/FinalUnfold_30_test/";
  //if (JetPtMin == 30) outputDirectory="PNGFiles/FinalUnfold_30/";
  if (variable.find("JetPt")!= string::npos && variable.find("Highest") == string::npos && JetPtMin == 20)  JetPtMin = 15 ;
  if (variable.find("JetPt")!= string::npos && variable.find("Highest") == string::npos && JetPtMin == 30)  JetPtMin = 20 ;

  string leptonFlavor = "DMu";
  if (!isMuon) leptonFlavor = "DE";

  int NumberOfToys( 10),  oppNumberOfToys(4);
  int UsedKterm = UsedKtermBayes, oppUsedKterm = UsedKtermSVD;
  string oppUnfAlg = "SVD";
  if (unfAlg == "SVD" ) {
    UsedKterm = UsedKtermSVD; 
    NumberOfToys = 3;  oppNumberOfToys = 400;
    oppUnfAlg = "Bayes";
    oppUsedKterm = UsedKtermBayes;
  }
  // set efficiency errors
  double muonIDIsoHLTError = 0.025;// 2012 : id,iso = 0.005, 0.002 --> 1.4% for dimuon + HLT: 2% ??????
  double electronIDIsoHLTError = 0.005; // SF factors for 2012: 0.002
  double EffError = muonIDIsoHLTError ;
  if ( !isMuon ) EffError = electronIDIsoHLTError ;


  cout << " WE NOW UNFOLD :  " << variable << " jet pt:" << JetPtMin <<"  " << JetPtMax <<endl;

  //-- fetch the data files and histograms --------------
  TFile *fData[5];             // 0 = central, 1 = JES Up, 2 = JES Down , 3 - SF Up , 4 - SF down
  getFiles(FILESDIRECTORY, fData, leptonFlavor, energy, ProcessInfo[DATAFILENAME].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, 0, 1);
  cout << " got data " << endl;
  TH1D *hData[5];  
  getHistos(hData, fData, variable);
  cout << " got data " << endl;
  //-----------------------------------------------------


  //-- fetch the DY files and histograms ----------------
  TFile *fDYMadGraph[4]; 
  TFile *fDYSherpa = NULL;               // 0 = central, 1 = PU Up,  2 = PU Down,  3 = JER Up
  TFile *fDYPowheg = NULL, *fDYPowhegUp = NULL, *fDYPowhegDown = NULL;
  getFiles(FILESDIRECTORY, fDYMadGraph, leptonFlavor, energy, ProcessInfo[DYMADGRAPHFILENAME].filename, JetPtMin, JetPtMin, doFlat, doVarWidth , 0, 0, 0, 0,  0, 1);
  
  fDYSherpa = getFile(FILESDIRECTORY, leptonFlavor, energy, DYSHERPAFILENAME, JetPtMin, JetPtMin, doFlat, doVarWidth);
  fDYPowheg = getFile(FILESDIRECTORY, leptonFlavor, energy, DYPOWHEGFILENAME, JetPtMin, JetPtMin, doFlat, doVarWidth);
  //fDYPowhegUp = getFile(FILESDIRECTORY, leptonFlavor, energy, DYPOWHEGUPFILENAME, JetPtMin, JetPtMin, doFlat, doVarWidth);
  //fDYPowhegDown = getFile(FILESDIRECTORY, leptonFlavor, energy, DYPOWHEGDOWNFILENAME, JetPtMin, JetPtMin, doFlat, doVarWidth);
  TH1D *hDY[4]; 
  TH1D *hDYGenMadGraph = NULL, *hDYGenSherpa = NULL,  *hDYGenPowheg = NULL, *hDYGenPowhegUp = NULL, *hDYGenPowhegDown = NULL;
  getHistos(hDY, fDYMadGraph, variable);
  hDYGenMadGraph = getHisto(fDYMadGraph[0], "gen" + variable);
  hDYGenSherpa = getHisto(fDYSherpa, "gen" + variable);
  hDYGenPowheg = getHisto(fDYPowheg, "gen" + variable);
  //hDYGenPowhegUp = getHisto(fDYPowhegUp, "gen" + variable);
  //hDYGenPowhegDown = getHisto(fDYPowhegDown, "gen" + variable);
  RooUnfoldResponse *resDY[4]; 
  getResps(resDY, fDYMadGraph, "response" + variable);
  cout << " got DY files" << DYMADGRAPHFILENAME << endl;
  //-----------------------------------------------------


  //-- fetch the BG files and histograms ----------------
  cout << " Fetch background files " << endl;
  const int NBkgGroups(6);
  const int NBkgSyst(5);
  int nFilesBkg(0);

  TFile *fBG[15][NBkgSyst];            // 0 = central, 1 = PU Up,  2 = PU Down,  3 = XS Up,  4 = XS Down 
  TH1D *hBG[15][10], *hSumBG[NBkgSyst],*hSumBGgroup[15][10];
  int group = -1 ;
  double sumEve = 0.;
  int nFiles = NFILESDYJETS;
  bool isDoubleLep(1);
  if ( leptonFlavor == "SMuE" || leptonFlavor == "SMu" || leptonFlavor == "Muon") {
       isDoubleLep = 0;
       nFiles = NFILESTTBAR;
   }
  int countFiles = 0 ;
  int countBGp[5]={-1,-1};
  for (int i(0); i < nFiles; i++){
      int fileSelect = FilesDYJets[i] ;
      if (!isDoubleLep) fileSelect = FilesTTbarWJets[i] ;
      string fileNameTemp =  ProcessInfo[fileSelect].filename ;
      if ( fileNameTemp.find("Tau") != string::npos || fileNameTemp.find("Data") != string::npos) continue;
      if ( isDoubleLep && fileNameTemp.find("DYJets") != string::npos) continue;
      if ( !isDoubleLep && fileNameTemp.find("WJets") != string::npos && fileNameTemp.find("UNF") != string::npos) continue;
      cout << " Fetch background files " << fileNameTemp << endl;
     nFilesBkg++;
      //getFiles(FILESDIRECTORY, fBG[countFiles], leptonFlavor, energy, BGFILENAMESGrouped[i + 1], JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0,  1 );
      getFiles(FILESDIRECTORY, fBG[countFiles], leptonFlavor, energy, fileNameTemp, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0,  0, 0, 1 );
      getHistos(hBG[countFiles], fBG[countFiles], variable);
      cout << i << variable << endl;
      if ( isDoubleLep ){
          if ( fileNameTemp.find("ZZ") != string::npos){
              if (countBGp[0] == -1 ) group++ ;
          }
          else if  ( fileNameTemp.find("WZ") != string::npos ){
              if ( countBGp[1] == -1 ) group++ ;          
          }
          else  group++ ;


      }
      else  group++ ;
      cout << "group" << "   " << group << endl;
      for (int j(0); j < NBkgSyst ; j++){
          cout << "usbgroup " << j << "   " << countFiles << endl;
          if (countFiles == 0) hSumBG[j] = (TH1D*) hBG[countFiles][j]->Clone();
          else hSumBG[j]->Add(hBG[countFiles][j]);
          cout << "usbgroup " << fileNameTemp <<"  " << group <<"   " <<  j << "   " << countFiles << "   " << countBGp[1] <<endl;

          if ( fileNameTemp.find("ZZ") != string::npos ){
              if ( countBGp[0] == -1 ) { 
                  if ( j == 4 ) countBGp[0] = group ;  
                  hSumBGgroup[j][group] = (TH1D*) hBG[countFiles][j]->Clone();
              }
              else {
                  hSumBGgroup[j][ countBGp[0]]->Add(hBG[countFiles][j]);
              }
          }
          else if  ( fileNameTemp.find("WZ") != string::npos ){
              if ( countBGp[1] == -1 ) { 
                  if ( j == 4 ) countBGp[1] = group ;   
                  hSumBGgroup[j][group] = (TH1D*) hBG[countFiles][j]->Clone();
              }
              else {
                  hSumBGgroup[j][ countBGp[1]]->Add(hBG[countFiles][j]);
              }   
          }
          else {
              cout << " tu :" << j<< "  " << group << endl;
              hSumBGgroup[j][group] = (TH1D*) hBG[countFiles][j]->Clone();
          //else  hSumBGgroup[j][group]->Add(hBG[countFiles][j]);
            }

          if ( j == 0 ) {
              sumEve += hBG[countFiles][j]->GetBinContent(1);
          }

      cout << " end"<<i<<"   " << j << "   " << group << endl;
      }
      countFiles++;
  }
  //-----------------------------------------------------

  // unfold central and shifted data and MC 
  cout << " loaded all unfolded files"<<endl;
  // set the output
  string outputRootFileName = outputDirectory + leptonFlavor + "_" + energy +  "_unfolded_"+variable+"_histograms_" + unfAlg ;
  if (doVarWidth) outputRootFileName += "_VarWidth";
  outputRootFileName += ".root";
  TFile* outputRootFile = new TFile(outputRootFileName.c_str(), "RECREATE");

  //-- save data reco
  outputRootFile->cd();  hData[0]->Write("Data"); hData[1]->Write("DataJECup");hData[2]->Write("DataJECdown");
  //-- save madgraph gen also in the file
  outputRootFile->cd();  hDYGenMadGraph->Write("genMad");	
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


  // add other interesting MC samples for MPI for example

  if ( energy == "7TeV"){
      const	int NAddGEN = 5 ;
      string names[NAddGEN] = {"MadZ2MPIoff","MadZ2Star","MadZ2StarMPIoff","P84C","PowZjjMiNLO"};
      for ( int i(0) ; i < NAddGEN; i++){
          cout << GenMCFILENAMES[i] << endl;
          TFile* fDYGenTemp = NULL ;
          fDYGenTemp = getFile(FILESDIRECTORY, leptonFlavor, energy, GenMCFILENAMES[i], JetPtMin, JetPtMin, doFlat, doVarWidth);
          TH1D *hDYGenTempAdd = NULL;
          hDYGenTempAdd = getHisto(fDYGenTemp, "gen" + variable);
          string histoName = "gen" + names[i] ;
          outputRootFile->cd();  hDYGenTempAdd->Write(histoName.c_str());	
      }
  }


  string hNames[8] = {"Central", "JECup", "JECdown", "PUup", "PUdown", "XSECup", "XSECdown", "JERup"};
  int SelData[8] = {0, 1, 2, 0, 0, 0, 0, 0}; 
  int SelDY[8]   = {0, 0, 0, 1, 2, 0, 0, 3}; 
  int SelBG[8]   = {0, 0, 0, 1, 2, 3, 4, 0}; 
  for ( int i(0); i < 8; i++){
      TH1D* hUnfolded = Unfold(unfAlg, resDY[SelDY[i]], hData[SelData[i]], hSumBG[SelBG[i]],  UsedKterm, hNames[i]); 
      outputRootFile->cd();  hUnfolded->Write();
      cout << "Start unfolding offseted histograms on RECO "<<endl;
      if ( i == 3 ) { 
          hData[SelData[i]]->Write("testData");
          resDY[SelDY[i]]->Write("testDY");
          hSumBG[SelBG[i]]->Write("testBG");
      }
  }

  // unfold with the opposite methode to compute systematic error
  // from new prescription this is not to be used in the error estimation!!! but we can stil unfold
  TH1D* opphUnfolded = Unfold(oppUnfAlg, resDY[SelDY[0]], hData[SelData[0]], hSumBG[SelBG[0]], oppUsedKterm, "oppCentral"); 
  outputRootFile->cd();  opphUnfolded->Write();


  int xbin(hData[0]->GetNbinsX());	
  // now for central values compute Roounfold Cov matrix
  cout << " compute covarican mattrix from roounfold " << endl;
  TH2D* covCentralRoo    = CovFromRoo(unfAlg, resDY[0], hData[0], hSumBG[0], UsedKterm, "CentralCov",      1);
  TH2D* covCentralRooToy = CovFromRoo(unfAlg, resDY[0], hData[0], hSumBG[0], UsedKterm, "CentralCovToy", NumberOfToys );
  outputRootFile->cd();  covCentralRoo->Write();  covCentralRooToy->Write();

  TH2D*  corCentralRoo= CovToCorr2((TH2D*) covCentralRoo->Clone("RooCor"), "RooCor");
  TH2D*  corCentralRooToy= CovToCorr2((TH2D*) covCentralRooToy->Clone("RooCorToy"), "RooCorToy");
  outputRootFile->cd();  corCentralRoo->Write();  corCentralRooToy->Write();

  //// now the covariance and correlation of the Opposite Algo
  TH2D* covCentralRooOpp    = CovFromRoo(oppUnfAlg, resDY[0], hData[0], hSumBG[0], oppUsedKterm, "CentralCovOpp",      1);
  TH2D* covCentralRooToyOpp = CovFromRoo(oppUnfAlg, resDY[0], hData[0], hSumBG[0], oppUsedKterm, "CentralCovToyOpp", NumberOfToys );
  outputRootFile->cd();  covCentralRooOpp->Write("CentralCovOpp");  covCentralRooToyOpp->Write("CentralCovToyOpp");

  TH2D*  corCentralRooOpp= CovToCorr2((TH2D*) covCentralRooOpp->Clone("RooCorOpp"), "RooCorOpp");
  TH2D*  corCentralRooToyOpp= CovToCorr2((TH2D*) covCentralRooToyOpp->Clone("RooCorToyOpp"), "RooCorToyOpp");
  outputRootFile->cd();  corCentralRooOpp->Write();  corCentralRooToyOpp->Write();




  cout << "Start setting error histograms on RECO "<<endl;


  // finished with central stuff, now do covariance 
  // and my toy error propagation --> MY PRIVATE MAGIC 
  // first set errors
  //TH1D *hDataJES = SetSystErrorsMax(hData[0], hData[1], hData[2], "JESerrors");     // JES errors
  TH1D *hDataJES = SetSystErrorsMean(hData[0], hData[1], hData[2], "JESerrors");     // JES errors
  TH1D *hDataEFF = SetSystErrorsMean(hData[0], EffError, "EFFerrors");     // lepton EFF errors
  cout << "Set errors on response object "<<endl;
  hData[3]->Draw();
  cout << "Set errors on response object "<<endl;
//  if ( leptonFlavor == "DE" ) hDataEFF = SetSystErrorsMean(hData[0],hData[0], hData[3], hData[4],EffError, "EFFerrors");     // lepton EFF errors
  cout << "Set errors on response object "<<endl;
  TH1D *hDataJER = SetSystErrorsMean(hData[0], hDY[0], hDY[3], hDY[3], "JERerrors");     // JER errors
  TH1D *hDataPU = SetSystErrorsMean(hData[0], hDY[0], hDY[1], hDY[2], "JERerrors");      // PU errors
  TH2D *hResPUErrors = SetResponseErrors( (TH2D*) resDY[0]->Hresponse(), (TH2D*) resDY[1]->Hresponse(), (TH2D*) resDY[2]->Hresponse());	// PU effect on response 
  TH2D *hResJERErrors = SetResponseErrors( (TH2D*) resDY[0]->Hresponse(), (TH2D*) resDY[3]->Hresponse(), (TH2D*) resDY[3]->Hresponse());	// JER effect on response 

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


  const int nUnfold =9 ; 
  string Cov[nUnfold] = {"MyToy","MyToyMC", "MyToyJES", "MyToyPU", "MyToyXSEC", "MyToyJER", "MyToyEFF","MyToyJER2","MyToyPU2"};
  int selRes[nUnfold]      = { 0, 0, 0, 1, 0, 2 , 0, 0, 0 };
  int selBGerrors[nUnfold] = { 0, 0, 0, 1, 2, 0 , 0, 0, 0};
  TH2D *hCovariance[nUnfold];
  TH2D *hCorrelation[nUnfold];
  TH1D *hOyMySyst[nUnfold];
  int SetTypeOfVariation[nUnfold] = {1, 101, 10, 2, 11, 4 , 10, 10, 10 }; // select what and how to varry
  /// option 3 --- data bins are varries independently; option 10 -- global variation pf all bins
  //int SetTypeOfVariation[6] = {1, 3, 2, 11, 4 , 3}; // select what and how to varry : ORIGINAL SELECTION
  //if (variable.find("ZNG")!= string::npos ) SetTypeOfVariation[1] = 103 ;
  // option (1)10: systeamtics is completly correlated on reco --> translated to cov matrix
  // option 3: assume errors in reco bins are uncorrelated
  //int myToyN[5] = {400, 400, 400, 400, 400};
  for (int i(0); i < nUnfold; i++){	
//  for (int i(0); i < 3; i++){	
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
      if ( i == 4 ) hToy2 = (TH1D*) ToyMCErrorsStat(unfAlg, hDataTem, hSumBGgroupErrors[selBGerrors[i]], responseTemp, UsedKterm, hCovariance[i], hCorrelation[i], NumberOfToys , SetTypeOfVariation[i]);
      else hToy2 = (TH1D*) ToyMCErrorsStat(unfAlg, hDataTem, vhBack, responseTemp, UsedKterm, hCovariance[i], hCorrelation[i], NumberOfToys, SetTypeOfVariation[i]);
      //TH1D *hToy2 = (TH1D*) ToyMCErrorsStat(unfAlg, hDataTem, hSumBGgroupErrors[selBGerrors[i]], responseTemp, UsedKterm, hCovariance[i], hCorrelation[i], myToyN[i], SetTypeOfVariation[i]);
      //TH1D *hToy2 = (TH1D*) ToyMCErrorsStat(unfAlg, hDataTem, (TH1D*) hSumBG[selBGerrors[i]]->Clone(), responseTemp, UsedKterm, hCovariance[i], hCorrelation[i], myToyN[i], SetTypeOfVariation[i]);
      hOyMySyst[i] = (TH1D*) hToy2->Clone(hNameTemp.c_str());

      outputRootFile->cd();
      hOyMySyst[i]->Write();
      hCovariance[i]->Write();
      hCorrelation[i]->Write();
      //      TCanvas* aC = new TCanvas();
      //      aC->cd();
      //      responseTemp->DrawCopy();
      //	hDataTem->DrawCopy();
      //	hToy2->DrawCopy("same");     
      //	hSumBG[selBGerrors[i]]->DrawCopy("same");     
  }

  //-- Close all the files ------------------------------
  outputRootFile->Close();
  //closeFile(outputRootFile);
  closeFiles(fData);
  closeFiles(fDYMadGraph);
  //closeFile(fDYSherpa);
  //closeFile(fDYPowheg);
  //closeFile(fDYPowhegUp);
  //closeFile(fDYPowhegDown);
  for (int i(0); i < nFilesBkg; i++) closeFiles(fBG[i]);
  //-----------------------------------------------------

}



