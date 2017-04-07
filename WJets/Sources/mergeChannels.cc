#include <TFile.h>
#include <TCanvas.h>
#include <TMatrixD.h>
#include <TVectorD.h>
#include <TDecompLU.h>
#include <TDecompSVD.h>
#include <TSystem.h>
#include <TLegend.h>
#include <TGraphAsymmErrors.h>
#include <TLatex.h>
#include <TPaveText.h>
#include <iostream>
#include <TH1.h>
#include <TH2.h>
#include <TPad.h>
#include <TGraph.h>
#include <TStyle.h>
#include <fstream>
#include <sstream>
#include <iomanip> 
#include <RooUnfoldResponse.h>
#include <RooUnfoldSVD.h>
#include <RooUnfoldBayes.h>
#include <RooUnfoldBinByBin.h>
#include <RooUnfoldInvert.h>
#include <TSVDUnfold.h>
#include "Includes/unfoldingFunctions.h"
#include "Includes/fileNames.h"
#include "Includes/getFilesAndHistograms.h"
#include "Includes/variablesOfInterestVarWidth.h"
#include "Includes/UnfoldingError.h"

#define DEBUG              1
using namespace std;

//-- prototypes ---------------------------------------------------------------------------------//
void mergeChannelsRun(string var = "ZNGoodJets_Zexc", bool logZ = 0, bool decrease = 0);
void returnCov(TH1D *dataCentral, TH1D* hUp, TH1D* hDown, TH2D *cov , TH1D* hErrors);
void plotLepRatioComb(string variable, TH1D* h_combine, TH1D* hMuon, TH1D* hEle);
void plotCombination(string variable, TH1D* hCombinedStat,  TH1D* hCombinedTot,  TH1D* genMadTemp[] , TH1D* genSheTemp[], TH1D* genPowTemp[]);
void createZNGoodJets_Zinc( TH1D* hCombinedStat, TH1D* hCombinedSyst , TH2D *hError2D[] , string unfAlg , bool doVarWidth , bool doXSec , bool doNormalize );
void dumpElements(TMatrixD& a);
void dumpElements(TVectorD& a);
//-----------------------------------------------------------------------------------------------//

//-- global variables ---------------------------------------------------------------------------//
string VARIABLE  =   "ZNGoodJets_Zexc" ;
string OUTPUTDIRECTORY = "PNGFiles/NiceUnfold/";

int optionCorr = 2;      // 0 - simple weighted average, 
                          // 1 - full cov matrix for each channel 
                          // 2 - full cov matrix for each channel and  correlation = 1 for same bins in the two channels 
                          // 3 - full cov matrix for each channel is geometric average for two channels (for each bin)  
                          // 4 - full cov matrix for each channel and  correlation = 1 for ALL bins between the two channels 

bool LOGZ = 0, DECREASE = 0 ;
int doXSec  =  1;
int doNormalize  =  0;
double Luminosity(19789.);
string energy  =  getEnergy();
string unfAlg  =  "Bayes";
bool doVarWidth  =  true ;
double mergedValuesAllOpt[30][10] = {0};
double mergedErrorsAllOpt[30][10] = {0};
int tempNBin = 0;
int kCorrMax = 1; 
int SelComb[] = {0,1,3,4,5} ; //selection Of Combination Opi
//-----------------------------------------------------------------------------------------------//


void mergeChannels()
{
//  NVAROFINTERESTZJETS = 1;
//  for (int i(0); i < NVAROFINTERESTZJETS; i++){
  for (int i(0); i < 1; i++){
    for (int k(0); k < kCorrMax; k++){
      //optionCorr = k ;
      optionCorr = SelComb[k];
      mergeChannelsRun(VAROFINTERESTZJETS[i].name, VAROFINTERESTZJETS[i].log, VAROFINTERESTZJETS[i].decrease);
    }
    //// this should be put in a function (i started ... )
    //cout << " PRINT MERGED RESULTS FOR ALL THE CORRELATION OPTIONS " << endl;
    //for (int l(0); l < tempNBin; l++){
    //  for (int k(0); k < kCorrMax; k++){
    //    cout << mergedValuesAllOpt[l][SelComb[k]]/Luminosity  <<"   " <<  "  & " ;
    //  }
    //  cout <<  " \\\\" << endl;
    //}
  }
}

void mergeChannelsRun(string var,  bool logZ, bool decrease)
{
  LOGZ = logZ; DECREASE = decrease;
  VARIABLE = var;
  gStyle->SetOptStat(0);
  TH1::SetDefaultSumw2();

  //-- fetch Muon and Electron files produced by FinalUnfold.cc ---------------------
  string fileNameEl  =  "PNGFiles/FinalUnfold_30_test/DE_"  + energy + "_unfolded_" + VARIABLE + "_histograms_Bayes_VarWidth.root";
  string fileNameMu  =  "PNGFiles/FinalUnfold_30_test/DMu_" + energy + "_unfolded_" + VARIABLE + "_histograms_Bayes_VarWidth.root";

  TFile *f[2];
  f[0] = new TFile(fileNameEl.c_str());
  f[1] = new TFile(fileNameMu.c_str());
  //---------------------------------------------------------------------------------

  //-- Retrieve histograms from the files -------------------------------------------
  //double lumi[2] = {1,1}; not used
  // get histograms
  TH1D *dataReco[2], *dataCentral[2], *dataCentralOppAlgo[2], *genMad[2], *genShe[2], *genPow[2], *hErrors[2][4];
  TH2D *myToyStatCov[2], *myToyJESCov[2], *myToyPUCov[2], *myToyXSECCov[2], *myToyJERCov[2], *myToyEFFCov[2], *myToyLUMICov[2];

 
  
  int option = 0;
  double luminosityErr = 0.026;
  double muonIDIsoHLTError = 0.02;
  double electronIDIsoHLTError = 0.01;
  double unfErrorDistr[2];

  //---- loop to run on muon (i=1) and electron (i=0) ----
  for (int i(0); i < 2; i++){
    dataReco[i]           = (TH1D*) f[i]->Get("Data");
    dataCentral[i]        = (TH1D*) f[i]->Get("Central");
    dataCentralOppAlgo[i] = (TH1D*) f[i]->Get("oppCentral");
    genMad[i]             = (TH1D*) f[i]->Get("genMad");
    genShe[i]             = (TH1D*) f[i]->Get("genShe");
    genPow[i]             = (TH1D*) f[i]->Get("genPow");

    myToyStatCov[i]       = (TH2D*) f[i]->Get("MyToyCov");
    myToyJESCov[i]        = (TH2D*) f[i]->Get("MyToyJESCov");
    myToyPUCov[i]         = (TH2D*) f[i]->Get("MyToyPU2Cov");
    myToyXSECCov[i]       = (TH2D*) f[i]->Get("MyToyXSECCov");
    myToyJERCov[i]        = (TH2D*) f[i]->Get("MyToyJER2Cov");
    myToyEFFCov[i]        = (TH2D*) f[i]->Get("MyToyEFFCov");

    //---- call errors from different unfolding function ----
    unfErrorDistr[i] = getErrors(VARIABLE, i, unfAlg, doVarWidth);
    // set covariance for luminosity uncertainty --> take correlation from JES and rescale
    //myToyLUMICov[i] = (TH2D*) setCovariance(myToyJESCov[i] , dataCentral[i]	, luminosityErr );

    hErrors[i][0] = (TH1D*) dataReco[i]->Clone();
    hErrors[i][1] = (TH1D*) dataReco[i]->Clone();
    hErrors[i][2] = (TH1D*) dataReco[i]->Clone();
    if (option == 1) {
      TH1D *hSystUp    = (TH1D*) f[i]->Get("JECup");
      TH1D *hSystDown  = (TH1D*) f[i]->Get("JECdown");	
      returnCov(dataCentral[i], hSystUp, hSystDown , myToyJESCov[i], hErrors[i][0]);
      /// PU
      hSystUp   = (TH1D*) f[i]->Get("PUup");
      hSystDown = (TH1D*) f[i]->Get("PUdown");	
      returnCov(dataCentral[i], hSystUp, hSystDown , myToyPUCov[i],hErrors[i][1]);
      // XSEC
      hSystUp    =  (TH1D*) f[i]->Get("XSECup");
      hSystDown           =  (TH1D*) f[i]->Get("XSECdown");	
      returnCov(dataCentral[i], hSystUp, hSystDown , myToyXSECCov[i],hErrors[i][2]);
    }

    //dataCentral[i]->Scale(1./lumi[i]);
    //myToyStatCov[i]->Scale(1./(lumi[i]*lumi[i]));	
    //myToyJESCov[i]->Scale(1./(lumi[i]*lumi[i]));	

  }
  //---------------------------------------------------------------------------------

  TH1D* h_combine  =  (TH1D*) dataReco[0]->Clone();

  // declare the big matrix
  int nbins =  dataCentral[0]->GetNbinsX();
  tempNBin = nbins;
  //nbins =  8 ;
  const int NELE = 2*nbins;
  TMatrixD errorM(NELE,NELE);

  TMatrixD U(NELE,nbins);
  for(int irow = 0; irow < NELE; irow++)
    for(int icol = 0; icol < nbins; icol++)
      U(irow,icol)  =  ( (irow  ==   icol) || (irow  ==   icol+nbins) )? 1:0;
  // debug
  if (DEBUG) dumpElements(U);

  TMatrixD transposeU(nbins,NELE);
  transposeU.Transpose(U);
  if (DEBUG) dumpElements(transposeU);


  TVectorD measurement(NELE);
  // jet energy scale for electron channel
  TVectorD jesSys_ele(nbins);
  TVectorD jesSys_muo(nbins);


  ////////////////////////////////////////////////////////////////////////////////
  // get correlation matrix this is need for option 3 of combination

  //        TH2D* hCorrEle = (TH2D*) CovToCorr(CovEle);
  //        TH2D* hCorrMu = (TH2D*)  CovToCorr(CovMuon);

  // set correlation between channels
  double correlationSameBin  =  0.;
  double correlationDiffBin  =  0.;
  if ( optionCorr  ==  1 ) correlationSameBin  =  0.;
  if ( optionCorr  ==  2 ) correlationSameBin  =  1.;
  if ( optionCorr  ==  3 ) correlationSameBin  =  1.;
  if ( optionCorr  ==  4 ) {
    correlationSameBin  =  1.;
    correlationDiffBin  =  1.;
  }

  /// calculate statistical contribution to error
  int optStat = 0 ; if (optionCorr > 0 ) optStat = 1 ;
  TMatrixD covMatrixStat = getCovMatrixOfCombination(myToyStatCov[0],myToyStatCov[1], optStat , 1 );
  TMatrixD covMatrixJES  = getCovMatrixOfCombination(myToyJESCov[0],myToyJESCov[1], optionCorr , 1 );
  TMatrixD covMatrixPU   = getCovMatrixOfCombination(myToyPUCov[0],myToyPUCov[1], optionCorr , 1 );
  TMatrixD covMatrixXSEC = getCovMatrixOfCombination(myToyXSECCov[0],myToyXSECCov[1], optionCorr , 1 );
  TMatrixD covMatrixJER  = getCovMatrixOfCombination(myToyJERCov[0],myToyJERCov[1], optionCorr , 1 );
  TMatrixD covMatrixUNF = getCovMatrixOfCombinationUNF(unfErrorDistr, dataCentral[0], dataCentral[1], dataCentralOppAlgo[0], dataCentralOppAlgo[1], optionCorr , 1 );

  TMatrixD covMatrixLUMI = setCovMatrixOfCombination(luminosityErr, dataCentral[0], dataCentral[1], optionCorr, 1 );
  //TMatrixD covMatrixLEP  = setCovMatrixOfCombinationLep(muonIDIsoHLTError,electronIDIsoHLTError, dataCentral[0], dataCentral[1], optionCorr , 1);
  TMatrixD covMatrixLEP  = getCovMatrixOfCombination(myToyEFFCov[0],myToyEFFCov[1], optStat , 1 ); // two  leptons are independednt

  // create total covariance matrix
  // sum all matrices
  errorM = covMatrixStat;
  errorM += covMatrixJES; 
  errorM += covMatrixPU;
  errorM += covMatrixXSEC;
  errorM += covMatrixJER;
  errorM += covMatrixLEP;

  // if ( doXSec )  errorM += covMatrixLUMI ; // WE SHOULD ADD THIS AFTER COMBINATION ?
  // there is something shity when you use full covariance (options 3, 4 ) of luminosity --> correlation of ALL bins =1 
  // test this with  unmarking line below
  //errorM = covMatrixLUMI ;


  //errorM = covMatrixJES;
  //errorM = covMatrixXSEC;
  // now loop over bins to set the matrix content
  double norm = 1. ; // this I use to have a reasonable value for determinant ( not needed in general) -> important for inversion so that matrices don't deal with small/big numbers
  double myDet = 1. ;
  for(int ibin = 0; ibin<nbins; ibin++){
    // electron channel
    double value_e  =  dataCentral[0]->GetBinContent(ibin+1) ;

    // muon channel
    double value_m  =  dataCentral[1]->GetBinContent(ibin+1) ;

    cout << " mjerene vrijednosti koje pucam tu:"<<value_e << " muons:"<<value_m<<endl;

    measurement(ibin)  =  value_e / norm ;
    measurement(nbins+ibin)  =  value_m / norm ;
  }

  // add unfolding covariance matrix
  errorM += covMatrixUNF;

  // rescale error matrix for numerics, so the numbers are not to big (or small). should not effect resuls.
  for(int ibin = 0; ibin < 2 * nbins; ibin++){
    for(int jbin = 0; jbin < 2 * nbins; jbin++){
      errorM(ibin,jbin) = errorM(ibin,jbin)  / (norm * norm ) ;
      //errorM(ibin,ibin) = 1.; 
    }
  }
  cout << " Print error matrix  " << endl;
  errorM.Print();
  cout << " Finished Print error matrix  " << optionCorr<< endl;


  TMatrixD errorInverse  =  errorM;
  cout<< " lets print inverse of error matrix"<<endl;
  if (DEBUG) dumpElements(errorInverse);

  double* det;
  errorInverse.Invert(det);
  if (DEBUG) dumpElements(errorInverse);

  //   TMatrixD Unit  =  errorInverse*errorM;
  // dumpElements(Unit);

  cout<< " print mR"<<endl;
  TMatrixD matrixRight(nbins,NELE);  
  matrixRight  =  transposeU*errorInverse;
  if (DEBUG)  dumpElements(matrixRight);

  cout<< " print U transpose"<<endl;
  if (DEBUG)  dumpElements(transposeU);
  cout<< " print error inverse"<<endl;

  TMatrixD matrixLeft(nbins,nbins);
  matrixLeft  =  transposeU*(errorInverse*U);
  TMatrixD matrixLeft1(2*nbins,nbins);
  matrixLeft1  =  (errorInverse*U);

  cout<< " print mL "<< endl;
  if (DEBUG)  dumpElements(matrixLeft);



  cout<< " print mL inverse"<<endl;
  TDecompSVD svd(matrixLeft);
  TMatrixD matrixLeftInverse = svd.Invert();

  double det2 ;
  // old method to find inverse
  /*TMatrixD matrixLeftInverse  =  matrixLeft;
    matrixLeftInverse.Invert(det2);*/
  if (DEBUG)  dumpElements(matrixLeftInverse);
  det2 = matrixLeftInverse.Determinant();
  cout << " determinant of mL:  "<<det2<<endl;

  cout<< " print lambda"<<endl;
  TMatrixD lambda(nbins,NELE);
  lambda =  matrixLeftInverse*matrixRight;
  if (DEBUG)  dumpElements(lambda);

  cout<< " print lambda transpose"<<endl;
  TMatrixD transposeLambda(NELE,nbins);
  transposeLambda.Transpose(lambda);
  if (DEBUG)  dumpElements(transposeLambda);


  TVectorD combined_value(nbins);
  combined_value  =  lambda*measurement;
  if (DEBUG)  dumpElements(combined_value);

  TMatrixD combined_error_LUMI(nbins,nbins);
  TMatrixD combined_error(nbins,nbins);
  combined_error  =  lambda*(errorM*transposeLambda);

  // after all matrix operation, now set the histogram
  // store combined result and all the individual contributions to error in vector
  double result[10][10];
  for(int i = 0; i<nbins;i++){

    h_combine->SetBinContent(i+1, combined_value(i) * norm );
    combined_error_LUMI(i,i) = pow(luminosityErr *   combined_value(i) * norm , 2 );
    double error  =  combined_error(i,i) * norm * norm + combined_error_LUMI(i,i);
    /// add luminosity error to the combination result instead of each of channels before the combination
    cout << " LUMI ERROR:   " << pow(luminosityErr *  h_combine->GetBinContent(i+1) , 2 ) << " before : " << error << endl;
    //		error +=  pow(luminosityErr *  h_combine->GetBinContent(i+1),2 ) ;
    if(error>1e-10)error  =  sqrt(error);
    else error = 1e-10;

    h_combine->SetBinError(i+1,error);
    mergedValuesAllOpt[i][optionCorr] = h_combine->GetBinContent(i+1) ;
    mergedErrorsAllOpt[i][optionCorr] = error;

    result[0][i] =  h_combine->GetBinContent(i) ;
    result[1][i] =  error ;
  }
  /// NOW ADD LUMI ERRORS TO COMBINATION
  combined_error += combined_error_LUMI;
cout << " number of bins in matrix:" << nbins << endl;
  for(int i = 1; i  <=   nbins; i++){

    if(h_combine->GetBinContent(i)>-1e-10)
      cout << "COMBINATION Bin " << i << ": " << h_combine->GetBinContent(i) 
        << " +- " << h_combine->GetBinError(i) << "\t"
        << "error  =  " << 
        (h_combine->GetBinError(i)/h_combine->GetBinContent(i)) 
        << " ELECTRON  " << dataCentral[0]->GetBinContent(i)<<"+-"<<sqrt(myToyStatCov[0]->GetBinContent(i,i))<<"   +-(JEC)"<<sqrt(myToyJESCov[0]->GetBinContent(i,i))
        <<"  MUON:" << dataCentral[1]->GetBinContent(i)<<"+-"<<sqrt(myToyStatCov[1]->GetBinContent(i,i))<<"   +-(JEC)"<<sqrt(myToyJESCov[1]->GetBinContent(i,i)) <<endl; 
    cout    <<" RECO errors ELE&MU :  " << dataReco[0]->GetBinError(i)<<"   "<<dataReco[1]->GetBinError(i) <<" JES:  "<<hErrors[0][0]->GetBinContent(i)
      << endl;
  }



  // if (DEBUG) dumpElements(measurement);
  //  dumpElements(U);
  // errorM.Print();

  ////////////////////////////////////////////////////////////////////



  /// set covariance matrices for individual errors
  //int optStat = 0 ; if (optionCorr > 0 ) optStat = 1 ;
  //TMatrixD covMatrixStat = getCovMatrixOfCombination(myToyStatCov[0],myToyStatCov[1], optStat , 1 );	
  TMatrixD combined_error_stat(nbins,nbins);
  TMatrixD combined_error_JES(nbins,nbins);
  TMatrixD combined_error_PU(nbins,nbins);
  TMatrixD combined_error_XSEC(nbins,nbins);
  TMatrixD combined_error_JER(nbins,nbins);
  TMatrixD combined_error_UNF(nbins,nbins);
  TMatrixD combined_error_LEP(nbins,nbins);

  cout << " TH2 for allErrors" << endl;
  cout << lambda.GetNrows() << "   " <<  lambda.GetNrows() <<"   " << covMatrixStat.GetNrows() << "   " <<  covMatrixStat.GetNcols() << transposeLambda.GetNrows() << "   " <<  transposeLambda.GetNcols() << endl;
  combined_error_stat  =  lambda * (covMatrixStat * transposeLambda);
  cout << " TH2 for allErrors" << endl;
  combined_error_JES   =  lambda * (covMatrixJES * transposeLambda);
  combined_error_PU    =  lambda * (covMatrixPU * transposeLambda);
  combined_error_XSEC  =  lambda * (covMatrixXSEC * transposeLambda);
  combined_error_JER   =  lambda * (covMatrixJER * transposeLambda);
  combined_error_UNF   =  lambda * (covMatrixUNF * transposeLambda);
  combined_error_LEP   =  lambda * (covMatrixLEP * transposeLambda);
  //	combined_error_LUMI  =  lambda * (covMatrixLUMI * transposeLambda); // 
  // 	combined_error_LUMI = setCovMatrixOfCombination(luminosityErr, (TH1D*) h_combine->Clone(), (TH1D*) h_combine->Clone() , 0, 1 );


  TH2D* allErrorsTH2[9];
  allErrorsTH2[0] = new TH2D( combined_error  ) ;
  allErrorsTH2[1] = new TH2D( combined_error_stat ) ;
  allErrorsTH2[2] = new TH2D( combined_error_JES  ) ;
  allErrorsTH2[3] = new TH2D( combined_error_PU   ) ;
  allErrorsTH2[4] = new TH2D( combined_error_XSEC ) ;
  allErrorsTH2[5] = new TH2D( combined_error_JER  ) ;
  allErrorsTH2[6] = new TH2D( combined_error_LUMI ) ;
  allErrorsTH2[7] = new TH2D( combined_error_UNF  ) ;
  allErrorsTH2[8] = new TH2D( combined_error_LEP  ) ;


  // for debugging and filling LUMI error histogram
  for(int i = 0; i  <   nbins; i++){


    /// add luminosity error to the combination result instead of each of channels before the combination
    //                allErrorsTH2[6]->SetBinContent(i+1,i+1, pow(luminosityErr *  h_combine->GetBinContent(i+1) , 2 ) );

    //	cout << " TEST Eror: " << sqrt(combined_error(i,i)) <<"   " << sqrt(combined_error_stat(i,i)) << "   " << sqrt(combined_error_JES(i,i)) << "  " << sqrt(combined_error_LUMI(i,i)) / h_combine->GetBinContent(i+1)  << "   " << sqrt(combined_error(i,i)) /h_combine->GetBinContent(i+1) << "   "   <<  endl;

    //	cout << sqrt(errorM(i,i)) << "   " << sqrt(covMatrixJES(i,i))  << "    " << sqrt(covMatrixLUMI(i,i)) << endl;
    //	cout << sqrt(covMatrixJES(i,i)) << "  " << sqrt(covMatrixJES(nbins+i,nbins+ i)) << "    " << sqrt(combined_error_JES(i,i)) << endl;
    cout << "CCCC    " << sqrt(allErrorsTH2[7]->GetBinContent(i+1,i+1)) << "   " << sqrt(allErrorsTH2[1]->GetBinContent(i+1,i+1)) << "     " << sqrt(allErrorsTH2[5]->GetBinContent(i+1,i+1)) << endl;
    cout << " DDDD   " << sqrt(combined_error(i,i)) << "   " << sqrt(combined_error_JES(i,i)) << "   " << sqrt(combined_error_LUMI(i,i)) <<endl;
  }

  // end for debugging


  // lets create som tables
  ostringstream optionCorrStr; optionCorrStr << optionCorr ;
  string fileNameTable = "PNGFiles/NiceUnfold/TableSystematics_" + energy +   "_" + VARIABLE + "_" + unfAlg  ;
  fileNameTable += "_CorrelationOption_" + optionCorrStr.str() ;
  if (doVarWidth) fileNameTable += "_VarWidth";
  fileNameTable +=  ".tex";
  cout << fileNameTable << endl;
  createTexTable( VARIABLE, fileNameTable ,  (TH1D*) h_combine->Clone() , allErrorsTH2  , doXSec , doNormalize , Luminosity);
  // end table creation


  // create combination histogram with statistical errors
  TH1D* h_combine_stat =  (TH1D *) SetHistWithErrors(  (TH1D*)  h_combine->Clone() , combined_error_stat, "Stat");	
  //testMat(  (TH1D*) h_combine->Clone()  , combined_error_stat);
  cout << " used correlation parameters :  " << correlationSameBin << "     "  << correlationDiffBin << endl;

  /// PLOT COMPARISON OF ELECTRONS AND MUONS TO COMBINED
  plotLepRatioComb(VARIABLE, (TH1D*) h_combine->Clone(), (TH1D*) dataCentral[0]->Clone(),(TH1D*) dataCentral[1]->Clone() );

  /// PLOT FINAL PLOTS: COMBINATION VS MC
  plotCombination(VARIABLE, (TH1D*) h_combine_stat->Clone(), (TH1D*) h_combine->Clone(), genMad, genShe, genPow);




  f[0]->Close();
  f[1]->Close();
  cout << " print PU err" << endl;
  combined_error_PU.Print();
  cout << " print XSEC erro" << endl;
  combined_error_XSEC.Print();	
  cout << " print PU Cov" << endl;
  covMatrixPU.Print();
  cout << " print lambda" << endl;
  lambda.Print();

  cout << " create inclusive plot" <<endl;
//  if (VARIABLE.find("ZNGood") !=  std::string::npos) createZNGoodJets_Zinc( (TH1D*) h_combine_stat->Clone(), (TH1D*) h_combine->Clone(),  allErrorsTH2, unfAlg, doVarWidth, doXSec, doNormalize) ;
  //       combined_error_LUMI.Print();
  //allErrorsTH2[6]->Draw("TEXT");
  //covMatrixLEP.Print();
  //covMatrixUNF.Print();
  //covMatrixJES.Print();
  covMatrixUNF.Print();
  covMatrixStat.Print();
  errorM.Print();
}


//////////////////////////////////
void plotCombination(string VARIABLE, TH1D* hCombinedStat, TH1D* hCombinedTot, TH1D* genMadTemp[], TH1D* genSheTemp[], TH1D* genPowTemp[]){

  TH1D* genMad = (TH1D*) genMadTemp[0]->Clone();
  TH1D* genShe = (TH1D*) genSheTemp[0]->Clone();
  TH1D* genPow = (TH1D*) genPowTemp[0]->Clone();
  genMad->Add((TH1D*) genMadTemp[1]->Clone());
  genMad->Scale(0.5);
  genShe->Add((TH1D*) genSheTemp[1]->Clone());
  genShe->Scale(0.5);
  genPow->Add((TH1D*) genPowTemp[1]->Clone());
  genPow->Scale(0.5);


  /// load PDF systematic files
  //	TFile *fPDFMad = new TFile("DMu_8TeV_PDFSystematics_MadGraph_JetPtMin_30_VarWidth.root");
  TFile *fPDFMad = new TFile("HistoFiles/PDFSystematics_MadGraph.root");
  TFile *fPDFPow = new TFile("HistoFiles/PDFSystematics_Powheg.root");
  TFile *fPDFShe = new TFile("HistoFiles/PDFSystematics_Sherpa.root");

  // get histograms from PDF file
  string genVariable = "gen" + VARIABLE;
  TH1D *PDFSystMad = NULL ;
  TH1D *PDFSystPow = NULL ;
  TH1D *PDFSystShe = NULL ;
  /*
  TH1D *PDFSystMad = (TH1D*) fPDFMad->Get(genVariable.c_str());
  TH1D *PDFSystPow = (TH1D*) fPDFPow->Get(genVariable.c_str());
  TH1D *PDFSystShe = (TH1D*) fPDFShe->Get(genVariable.c_str());
  PDFSystMad->SetTitle("");
  PDFSystPow->SetTitle("");
  PDFSystShe->SetTitle("");
*/

  const int nBins(hCombinedStat->GetNbinsX());
  // renormalize first
  if (doXSec) {
    for (int i(1); i <= nBins; i++){ 
      double binW = hCombinedStat->GetBinWidth(i);
      hCombinedStat->SetBinContent(i, hCombinedStat->GetBinContent(i)*1./(Luminosity*binW));
      hCombinedTot->SetBinContent(i, hCombinedTot->GetBinContent(i)*1./(Luminosity*binW));
      genMad->SetBinContent(i, genMad->GetBinContent(i)*1./(Luminosity*binW));
      genShe->SetBinContent(i, genShe->GetBinContent(i)*1./(Luminosity*binW));
      genPow->SetBinContent(i, genPow->GetBinContent(i)*1./(Luminosity*binW));

      hCombinedStat->SetBinError(i, hCombinedStat->GetBinError(i)*1./(Luminosity*binW));
      hCombinedTot->SetBinError(i, hCombinedTot->GetBinError(i)*1./(Luminosity*binW));
      genMad->SetBinError(i, genMad->GetBinError(i)*1./(Luminosity*binW));
      genShe->SetBinError(i, genShe->GetBinError(i)*1./(Luminosity*binW));
      genPow->SetBinError(i, genPow->GetBinError(i)*1./(Luminosity*binW));

      if (DEBUG) cout << i << "   "  << hCombinedTot->GetBinContent(i)  << "     "   <<  hCombinedStat->GetBinError(i) << "     "  <<  hCombinedTot->GetBinError(i)   << endl;
      if (DEBUG) cout << i << "   "  << hCombinedTot->GetBinContent(i)  << "     "   <<genPow->GetBinContent(i)  << "     "   <<  hCombinedStat->GetBinError(i) << "     "  <<  genPow->GetBinError(i)   << endl;
    }
  }
  double xCoor[nBins], yCoor[nBins], xErr[nBins], yStat[nBins], ySystDown[nBins], ySystUp[nBins];
  double xCoorMadRatio[nBins], yCoorMadRatio[nBins], yStatMadRatio[nBins], ySystDownMadRatio[nBins], ySystUpMadRatio[nBins];
  double xCoorSheRatio[nBins], yCoorSheRatio[nBins], yStatSheRatio[nBins], ySystDownSheRatio[nBins], ySystUpSheRatio[nBins];
  double xCoorPowRatio[nBins], yCoorPowRatio[nBins], yStatPowRatio[nBins], ySystDownPowRatio[nBins], ySystUpPowRatio[nBins];
  double yCoorMC[nBins];



  double normFactor = 1. ;
  for (int bin(1); bin <= nBins; bin++){
    double totalSystematicsUp(0.), totalSystematicsDown(0.);
    double centralValue(hCombinedStat->GetBinContent(bin));
    double totalStatistics(hCombinedStat->GetBinError(bin));
    yStat[bin -1 ] = hCombinedStat->GetBinError(bin);
    ySystDown[bin -1 ] = hCombinedTot->GetBinError(bin);
    ySystUp[bin -1 ] = hCombinedTot->GetBinError(bin);



    double mcValue(genMad->GetBinContent(bin));
    yCoorMC[bin-1] = mcValue;
    xCoor[bin-1]    = hCombinedStat->GetBinCenter(bin);
    xCoorMadRatio[bin-1]    = hCombinedStat->GetBinCenter(bin);
    xCoorSheRatio[bin-1]    = hCombinedStat->GetBinCenter(bin);
    xCoorPowRatio[bin-1]    = hCombinedStat->GetBinCenter(bin);
    yCoor[bin-1]    = centralValue;

    xErr[bin-1]     = 0.;
    yStatMadRatio[bin-1]    = totalStatistics/(genMad->GetBinContent(bin));
    yStatSheRatio[bin-1]    = totalStatistics/(genShe->GetBinContent(bin));
    yStatPowRatio[bin-1]    = totalStatistics/(genPow->GetBinContent(bin));

    if(genMad->GetBinContent(bin) > 0){
      yCoorMadRatio[bin-1]       = centralValue / (genMad->GetBinContent(bin));
      ySystDownMadRatio[bin-1]   = ySystDown[bin-1]/(genMad->GetBinContent(bin));
      ySystUpMadRatio[bin-1]     = ySystUp[bin-1]/(genMad->GetBinContent(bin));
    }
    else{
      xCoorMadRatio[bin-1]       = -999.;
      yCoorMadRatio[bin-1]       = 0.1;
      ySystDownMadRatio[bin-1]   = 0.1;
      ySystUpMadRatio[bin-1]     = 0.1;
    }
    if(genShe->GetBinContent(bin) > 0){
      yCoorSheRatio[bin-1]       = centralValue / (genShe->GetBinContent(bin));
      ySystDownSheRatio[bin-1]   = ySystDown[bin -1 ] /(genShe->GetBinContent(bin));
      ySystUpSheRatio[bin-1]     = ySystUp[bin -1 ]/(genShe->GetBinContent(bin));
    }
    else{
      xCoorSheRatio[bin-1]       = -999.;
      yCoorSheRatio[bin-1]       = 0.1;
      ySystDownSheRatio[bin-1]   = 0.1;
      ySystUpSheRatio[bin-1]     = 0.1;
    }
    if(genPow->GetBinContent(bin) > 0){
      yCoorPowRatio[bin-1]       = centralValue / (genPow->GetBinContent(bin));
      ySystDownPowRatio[bin-1]   = ySystDown[bin -1 ] /(genPow->GetBinContent(bin));
      ySystUpPowRatio[bin-1]     = ySystUp[bin -1 ]/(genPow->GetBinContent(bin));
    }
    else{
      xCoorPowRatio[bin-1]       = -999.;
      yCoorPowRatio[bin-1]       = 0.1;
      ySystDownPowRatio[bin-1]   = 0.1;
      ySystUpPowRatio[bin-1]     = 0.1;
    }

    if (DEBUG) cout << xCoorPowRatio[bin-1]  << "     "  << yCoorPowRatio[bin-1]  << "    "  << ySystUpPowRatio[bin-1] << "   "  << ySystDownPowRatio[bin-1] <<endl;

  }

  /////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////
  ////// go to canvas, this should be the same as in runPlotting.cc
  /////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////
  string temp;

  if (doXSec || doNormalize) {
    string xtitle = genMad->GetXaxis()->GetTitle();
    string shortVar = xtitle.substr(0, xtitle.find(" "));
    string unit = "";
    if (xtitle.find("[") != string::npos){
      size_t begin = xtitle.find("[") + 1;
      unit = xtitle.substr(begin);
      unit = unit.substr(0, unit.find("]"));
    }
    if (DEBUG) cout << shortVar << "   unit: " << unit << endl;
    temp = "d#sigma/d" + shortVar;
    if (doNormalize) temp = "1/#sigma " + temp;
    else if (doXSec){
      temp += "  [pb";
      if (unit != "" ) temp += "/" + unit;
      temp += "]";
    }
    //genMad->GetYaxis()->SetTitle(temp.c_str());
    //genShe->GetYaxis()->SetTitle(temp.c_str());
    genMad->GetYaxis()->SetTitle("");
    genShe->GetYaxis()->SetTitle("");
  }



  TGraphAsymmErrors *grCentralStat         = new TGraphAsymmErrors(nBins, xCoor, yCoor, xErr, xErr, yStat, yStat);
  TGraphAsymmErrors *grCentralStatMadRatio = new TGraphAsymmErrors(nBins, xCoorMadRatio, yCoorMadRatio, xErr, xErr, yStatMadRatio, yStatMadRatio);
  TGraphAsymmErrors *grCentralStatSheRatio = new TGraphAsymmErrors(nBins, xCoorSheRatio, yCoorSheRatio, xErr, xErr, yStatSheRatio, yStatSheRatio);
  TGraphAsymmErrors *grCentralStatPowRatio = new TGraphAsymmErrors(nBins, xCoorPowRatio, yCoorPowRatio, xErr, xErr, yStatPowRatio, yStatPowRatio);
  TGraphAsymmErrors *grCentralSystMadRatio = new TGraphAsymmErrors(nBins, xCoorMadRatio, yCoorMadRatio, xErr, xErr, ySystDownMadRatio, ySystUpMadRatio);
  TGraphAsymmErrors *grCentralSystSheRatio = new TGraphAsymmErrors(nBins, xCoorSheRatio, yCoorSheRatio, xErr, xErr, ySystDownSheRatio, ySystUpSheRatio);
  TGraphAsymmErrors *grCentralSystPowRatio = new TGraphAsymmErrors(nBins, xCoorPowRatio, yCoorPowRatio, xErr, xErr, ySystDownPowRatio, ySystUpPowRatio);
  grCentralStat->SetMarkerStyle(20);
  grCentralStat->SetMarkerSize(0.8);
  grCentralStat->SetMarkerColor(kBlack);
  grCentralStat->SetLineColor(kBlack);
  //grCentralStat->SetLineWidth(2);
  TGraphAsymmErrors *grCentralSyst = new TGraphAsymmErrors(nBins, xCoor, yCoor, xErr, xErr, ySystDown, ySystUp);
  grCentralSyst->SetLineColor(kRed+1);
  //grCentralSyst->SetLineWidth(2);
  grCentralSyst->SetMarkerStyle(20);
  grCentralSyst->SetMarkerSize(0.8);

  cout << " create canvas " << endl;


  TCanvas *can = new TCanvas(VARIABLE.c_str(), VARIABLE.c_str(), 600, 900);
  can->cd();
  TPad *pad1 = new TPad("pad1","pad1", 0, 0.5, 1, 1);
  pad1->SetTopMargin(0.11);
  pad1->SetBottomMargin(0);
  pad1->SetRightMargin(0.02);
  pad1->SetLeftMargin(0.11);
  pad1->SetTicks();
  pad1->Draw();
  pad1->cd();
  if (LOGZ) pad1->SetLogy();
  double MineYMax = genMad->GetMaximum();
  double lowerLimit = genMad->GetXaxis()->GetXmin();
  if (grCentralSyst->GetMaximum() > MineYMax) MineYMax = grCentralSyst->GetMaximum();
  if (genPow->GetMaximum() > MineYMax) MineYMax = genPow->GetMaximum();
  if (VARIABLE.find("ZNGood") != string::npos){
    //MineYMax = 85.;
    cout << "Remove first bin: " << genMad->GetXaxis()->GetXmin() + genMad->GetBinWidth(1) << endl;
    genMad->GetXaxis()->SetRangeUser(genMad->GetXaxis()->GetXmin() + genMad->GetBinWidth(1), genMad->GetXaxis()->GetXmax());
    cout << "Remove first bin: " << genMad->GetXaxis()->GetXmin() + genMad->GetBinWidth(1) << endl;
    lowerLimit = genMad->GetXaxis()->GetXmin() + genMad->GetBinWidth(1);
    //		if (genPow->GetMinimum() < lowerLimit) MineYMin = genPow->GetMinimum();
  }
  grCentralSyst->GetXaxis()->SetLimits(lowerLimit, genMad->GetXaxis()->GetXmax());
  genMad->SetFillStyle(3001);
  genMad->SetFillColor(kAzure-9);
  genMad->SetLineColor(kAzure-9);
  genMad->GetYaxis()->SetTitleSize(0.05);
  genMad->GetYaxis()->SetTitleOffset(1.45);
  genMad->GetYaxis()->SetLabelSize(0.045);
  if (LOGZ) genMad->SetMaximum(MineYMax*5);
  else genMad->SetMaximum(MineYMax*1.5);



  genMad->SetTitle("");
  genMad->DrawCopy("E2");
  genShe->SetFillStyle(3002);
  genShe->SetFillColor(kOrange-3);
  genShe->SetLineColor(kOrange-3);
  genShe->SetFillStyle(3002);
  genPow->SetFillColor(kViolet-6);
  genPow->SetLineColor(kViolet-6);

  genShe->DrawCopy("E2 same text");
  genPow->SetFillStyle(3002);
  genPow->DrawCopy("E2 same");
  grCentralSyst->Draw("P same");
  grCentralStat->Draw("P same");
  pad1->Draw();
  // set the legend
  double xLowLeg(0.57), xHighLeg(0.98);
  if (!DECREASE){
    xLowLeg = 0.14;
    xHighLeg = 0.55;
  }
  TLegend *legend = new TLegend(xLowLeg, 0.52, xHighLeg, 0.91);
  legend->SetHeader("  ");
  legend->SetFillStyle(0);
  legend->SetBorderSize(0);
  legend->AddEntry(genMad, "MG+Py6 at NNLO Stat. Err.", "pf");
  legend->AddEntry(genShe, "Sherpa2 (NLO) Stat. Err.", "pf");
  legend->AddEntry(genPow, "Powheg+Py6  Stat. Err.", "pf");
  legend->AddEntry(grCentralStat, "Data Stat. Err. ", "pl");
  legend->AddEntry(grCentralSyst, "Data Tot. Err.", "l");
  legend->Draw();

  TLatex *variableLabel = new TLatex();
  variableLabel->SetTextSize(0.04);
  variableLabel->SetTextFont(42);
  variableLabel->SetLineWidth(2);
  variableLabel->SetTextColor(kBlack);
  variableLabel->SetNDC();
  variableLabel->SetTextAlign(21);
  variableLabel->DrawLatex(0.54,0.965,hCombinedStat->GetTitle());

  TLatex *cmsColl = new TLatex();
  cmsColl->SetTextSize(0.04);
  cmsColl->SetTextFont(42);
  cmsColl->SetLineWidth(2);
  cmsColl->SetTextColor(kBlack);
  cmsColl->SetNDC();
  cmsColl->SetTextAlign(11);
  cmsColl->DrawLatex(0.11,0.905, "CMS Preliminary");

  TLatex *sqrtXTeV = new TLatex();
  sqrtXTeV->SetTextSize(0.04);
  sqrtXTeV->SetTextFont(42);
  sqrtXTeV->SetLineWidth(2);
  sqrtXTeV->SetTextColor(kBlack);
  sqrtXTeV->SetNDC();
  sqrtXTeV->SetTextAlign(21);
  if (energy == "7TeV") sqrtXTeV->DrawLatex(0.54,0.905, "#sqrt{s} = 7 TeV");
  if (energy == "8TeV") sqrtXTeV->DrawLatex(0.54,0.905, "#sqrt{s} = 8 TeV");

  TLatex *intLumi = new TLatex();
  intLumi->SetTextSize(0.03);
  intLumi->SetTextFont(42);
  intLumi->SetLineWidth(2);
  intLumi->SetTextColor(kBlack);
  intLumi->SetNDC();
  intLumi->SetTextAlign(31);
  if (energy == "7TeV") intLumi->DrawLatex(0.98,0.91, "#int L dt = 5.05 fb^{-1}");
  if (energy == "8TeV") intLumi->DrawLatex(0.98,0.91, "#int L dt = 19.8 fb^{-1}");

  TLatex *ytitle = new TLatex();
  ytitle->SetTextSize(0.05);
  ytitle->SetTextFont(42);
  ytitle->SetLineWidth(2);
  ytitle->SetTextColor(kBlack);
  ytitle->SetNDC();
  ytitle->SetTextAlign(33);
  ytitle->SetTextAngle(90);
  if (doXSec || doNormalize) ytitle->DrawLatex(0.008,0.91,temp.c_str());
  pad1->cd();


  ////  NEW TPAD
  can->cd();
  TPad *pad2 = new TPad("pad2","pad2", 0, 0.35, 1, 0.5);
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0);
  pad2->SetRightMargin(0.02);
  pad2->SetLeftMargin(0.11);
  pad2->SetGridy();
  pad2->SetTicks();
  pad2->Draw();
  pad2->cd();
  grCentralStatMadRatio->SetMarkerStyle(20);
  grCentralStatMadRatio->SetMarkerSize(0.8);
  grCentralStatMadRatio->SetMarkerColor(kBlack);
  grCentralStatMadRatio->SetLineColor(kBlack);
  grCentralSystMadRatio->SetLineColor(kRed+1);
  grCentralSystMadRatio->SetMarkerStyle(20);
  grCentralSystMadRatio->SetMarkerSize(0.8);
  grCentralSystMadRatio->GetXaxis()->SetLimits(lowerLimit, genMad->GetXaxis()->GetXmax());
  grCentralSystMadRatio->GetYaxis()->SetRangeUser(0.41,1.59);
  grCentralSystMadRatio->GetYaxis()->SetTitle("Data / MC");
  grCentralSystMadRatio->GetYaxis()->SetTitleSize(0.135);
  grCentralSystMadRatio->GetYaxis()->SetTitleOffset(0.44);
  grCentralSystMadRatio->GetYaxis()->CenterTitle();
  grCentralSystMadRatio->GetYaxis()->SetLabelSize(0.12);
  grCentralSystMadRatio->GetYaxis()->SetLabelOffset(0.01);
  grCentralSystMadRatio->SetTitle("");
  grCentralSystMadRatio->GetXaxis()->SetTickLength(0.06);
  grCentralSystMadRatio->Draw("AP");
  /*
  PDFSystMad->SetFillColor(kGreen-3);
  PDFSystMad->SetLineColor(kGreen-3);
  PDFSystMad->SetFillStyle(3001);
  PDFSystMad->DrawCopy("E2 same");
  */

  grCentralSystMadRatio->Draw("P same");
  grCentralStatMadRatio->Draw("P same");
  TLatex *latexLabel = new TLatex();
  latexLabel->SetTextSize(0.15);
  latexLabel->SetTextFont(42);
  latexLabel->SetLineWidth(2);
  latexLabel->SetTextColor(kAzure-5);
  latexLabel->SetNDC();
  latexLabel->DrawLatex(0.15,0.09,"Madgraph");
  TLegend *legend2 = new TLegend(0.75,0.85,0.9,0.95);
  legend2->SetFillStyle(0);
  legend2->SetBorderSize(0);
  legend2->AddEntry(PDFSystMad, " PDF Syst.", "f");
  legend2->Draw();

  pad2->Draw();
  pad2->cd();

  can->cd();
  cout << " sherpa pad in the combination plot" << endl;
  TPad *pad3 = new TPad("pad3","pad3", 0, 0.2, 1, 0.35);
  pad3->SetTopMargin(0);
  pad3->SetBottomMargin(0.0);
  pad3->SetRightMargin(0.02);
  pad3->SetLeftMargin(0.11);
  pad3->SetGridy();
  pad3->SetTicks();
  pad3->Draw();
  pad3->cd();
  grCentralStatSheRatio->SetMarkerStyle(20);
  grCentralStatSheRatio->SetMarkerSize(0.8);
  grCentralStatSheRatio->SetMarkerColor(kBlack);
  grCentralStatSheRatio->SetLineColor(kBlack);
  grCentralSystSheRatio->SetLineColor(kRed+1);
  grCentralSystSheRatio->SetMarkerStyle(20);
  grCentralSystSheRatio->SetMarkerSize(0.8);
  grCentralSystSheRatio->GetXaxis()->SetLimits(lowerLimit, genMad->GetXaxis()->GetXmax());
  grCentralSystSheRatio->GetXaxis()->SetTitle(genMad->GetXaxis()->GetTitle());
  grCentralSystSheRatio->GetXaxis()->SetTitleSize(0.1);
  grCentralSystSheRatio->GetXaxis()->SetTitleOffset(1.05);
  grCentralSystSheRatio->GetXaxis()->SetLabelSize(0.1);
  grCentralSystSheRatio->GetXaxis()->SetLabelOffset(0.014);
  grCentralSystSheRatio->GetYaxis()->SetRangeUser(0.41,1.59);
  grCentralSystSheRatio->GetYaxis()->SetTitle("Data / MC");
  grCentralSystSheRatio->GetYaxis()->SetTitleSize(0.135);
  grCentralSystSheRatio->GetYaxis()->SetTitleOffset(0.44);
  grCentralSystSheRatio->GetYaxis()->CenterTitle();
  grCentralSystSheRatio->GetYaxis()->SetLabelSize(0.12);
  grCentralSystSheRatio->GetYaxis()->SetLabelOffset(0.01);
  grCentralSystSheRatio->SetTitle("");
  grCentralSystSheRatio->GetXaxis()->SetTickLength(0.06);
  grCentralSystSheRatio->Draw("AP");
  grCentralStatSheRatio->Draw("P same");
/*  PDFSystShe->SetFillColor(kGreen-3);
  PDFSystShe->SetLineColor(kGreen-3);
  PDFSystShe->SetFillStyle(3001);
  PDFSystShe->DrawCopy("E2 same");
  */
  grCentralSystSheRatio->Draw("P same");
  grCentralStatSheRatio->Draw("P same");
  TLatex *latexLabel1 = new TLatex();
  latexLabel1->SetTextSize(0.15);
  latexLabel1->SetTextFont(42);
  latexLabel1->SetLineWidth(2);
  latexLabel1->SetTextColor(kOrange+2);
  latexLabel1->SetNDC();
  latexLabel1->DrawLatex(0.15,0.09,"Sherpa2");
  pad3->Draw();
  pad3->cd();
  can->cd();
  cout << " powheg pad in the combination plot" << endl;
  // yet another pad
  TPad *pad4 = new TPad("pad4","pad4", 0, 0., 1, 0.2);
  pad4->SetTopMargin(0);
  pad4->SetBottomMargin(0.25);
  pad4->SetRightMargin(0.02);
  pad4->SetLeftMargin(0.11);
  pad4->SetGridy();
  pad4->SetTicks();
  pad4->Draw();
  pad4->cd();
  grCentralStatPowRatio->SetMarkerStyle(20);
  grCentralStatPowRatio->SetMarkerSize(0.8);
  grCentralStatPowRatio->SetMarkerColor(kBlack);
  grCentralStatPowRatio->SetLineColor(kBlack);
  grCentralSystPowRatio->SetLineColor(kRed+1);
  grCentralSystPowRatio->SetMarkerStyle(20);
  grCentralSystPowRatio->SetMarkerSize(0.8);
  grCentralSystPowRatio->GetXaxis()->SetLimits(lowerLimit, genMad->GetXaxis()->GetXmax());
  grCentralSystPowRatio->GetXaxis()->SetTitle(genMad->GetXaxis()->GetTitle());
  grCentralSystPowRatio->GetXaxis()->SetTitleSize(0.1);
  grCentralSystPowRatio->GetXaxis()->SetTitleOffset(1.05);
  grCentralSystPowRatio->GetXaxis()->SetLabelSize(0.1);
  grCentralSystPowRatio->GetXaxis()->SetLabelOffset(0.014);
  grCentralSystPowRatio->GetYaxis()->SetRangeUser(0.41,1.59);
  grCentralSystPowRatio->GetYaxis()->SetTitle("Data / MC");
  grCentralSystPowRatio->GetYaxis()->SetTitleSize(0.1);
  grCentralSystPowRatio->GetYaxis()->SetTitleOffset(0.6);
  grCentralSystPowRatio->GetYaxis()->CenterTitle();
  grCentralSystPowRatio->GetYaxis()->SetLabelSize(0.09);
  grCentralSystPowRatio->GetYaxis()->SetLabelOffset(0.012);
  grCentralSystPowRatio->SetTitle("");
  grCentralSystPowRatio->GetXaxis()->SetTickLength(0.06);
  grCentralSystPowRatio->Draw("AP");
  grCentralStatPowRatio->Draw("P same");
  /*
  PDFSystPow->SetFillColor(kGreen-3);
  PDFSystPow->SetLineColor(kGreen-3);
  PDFSystPow->SetFillStyle(3001);
  PDFSystPow->DrawCopy("E2 same");
  */
  grCentralSystPowRatio->Draw("P same");
  grCentralStatPowRatio->Draw("P same");
  TLatex *latexLabel2 = new TLatex();
  latexLabel2->SetTextSize(0.11);
  latexLabel2->SetTextFont(42);
  latexLabel2->SetLineWidth(2);
  latexLabel2->SetTextColor(kViolet-6);
  latexLabel2->SetNDC();
  latexLabel2->DrawLatex(0.15,0.3,"Powheg");
  pad4->Draw();
  pad4->cd();
  can->cd();

  //end of yet another pad



  string outputFileNamePNG  =  OUTPUTDIRECTORY;
  if (doXSec  ==  1) {  outputFileNamePNG +=  "Combination_XSec_";}
  else if (doNormalize  ==  1)  outputFileNamePNG +=  "Combination_Normalized_";

  outputFileNamePNG +=  VARIABLE ;
  ostringstream optionCorrStr; optionCorrStr << optionCorr;
  outputFileNamePNG +="_CorrelationOption_" + optionCorrStr.str() ;
  if (doVarWidth)  outputFileNamePNG += "_VarWidth";
  outputFileNamePNG +=  ".png";

  can->Print(outputFileNamePNG.c_str());


  fPDFMad->Close();
  fPDFPow->Close();
  fPDFShe->Close();

}


//////////////////////// NICE plots
void plotLepRatioComb(string VARIABLE, TH1D* hCombined,  TH1D* hEle , TH1D* hMuon){

  TH1D* hCombErr = (TH1D *) hCombined ->Clone();
  hCombErr->GetYaxis()->SetTitle("");
  /*	if ( doXSec   ==   1 ){
      h_combine->Scale(1/(hCombined->GetBinWidth(1)*Luminosity));
      dataCentral[0]->Scale(1/(h_combine->GetBinWidth(1)*Luminosity));
      dataCentral[1->Scale(1/(h_combine->GetBinWidth(1)*19380.));
      }
   */
  const int nBins =  hCombined->GetNbinsX();	
  double xCoor[nBins], yCoor[nBins], xErr[nBins], yStat[nBins], ySystDown[nBins], ySystUp[nBins];
  double xCoorMuonRatio[nBins], yCoorMuonRatio[nBins], yStatMuonRatio[nBins], ySystDownMuonRatio[nBins], ySystUpMuonRatio[nBins];
  double xCoorEleRatio[nBins], yCoorEleRatio[nBins], yStatEleRatio[nBins], ySystDownEleRatio[nBins], ySystUpEleRatio[nBins];
  double yCoorMC[nBins];
  TH1D* data  =  (TH1D *) hMuon->Clone();

  for(int bin = 1; bin  <=   nBins; bin++){
    hCombErr->SetBinContent(bin , 1 );
    double ErrCom = hCombined->GetBinError(bin) / hCombined->GetBinContent(bin)  ;
    hCombErr->SetBinError(bin , ErrCom );
    if (DEBUG ) cout << " Error : " << ErrCom << "  "  <<  hCombined->GetBinError(bin) <<  endl;
    double binNormWidth  =  1.;
    if ( doXSec   ==   1) { binNormWidth  =  hCombined->GetBinWidth(bin) * Luminosity ;
      double aCom = hCombined->GetBinContent(bin) / binNormWidth ;
      double aMu = hMuon->GetBinContent(bin) / binNormWidth ;
      double aE  = hEle->GetBinContent(bin) / binNormWidth ;
      double ErrMu =  hMuon->GetBinError(bin) / binNormWidth;
      double ErrEle = hEle->GetBinError(bin) / binNormWidth ;
      ErrCom = hCombined->GetBinError(bin) /binNormWidth ;
      hCombined->SetBinContent(bin , aCom );
      hCombined->SetBinError(bin , ErrCom );
      hEle->SetBinContent(bin , aE );
      hEle->SetBinError(bin , ErrEle );
      hMuon->SetBinContent(bin , aMu );
      hMuon->SetBinError(bin , ErrMu );
    }

    double totalSystematicsUp(0.), totalSystematicsDown(0.);
    double totalStatistics(hCombined->GetBinError(bin));
    double centralValue(hCombined->GetBinContent(bin));
    double mcValue(hMuon->GetBinContent(bin));


    xCoor[bin-1]     =  hCombined->GetBinCenter(bin);
    yCoor[bin-1]     =  centralValue;
    xErr[bin-1]      =  0.;			
    yStat[bin-1]     =  totalStatistics;

    /// get ratio od muon to merged
    yCoorMuonRatio[bin-1]  =  (hMuon->GetBinContent(bin)) / centralValue ;
    yStatMuonRatio[bin-1]     =  totalStatistics / centralValue ;
    ySystDownMuonRatio[bin-1]    =  sqrt(totalStatistics * totalStatistics + totalSystematicsDown )/(hMuon->GetBinContent(bin));
    ySystUpMuonRatio[bin-1]      =  sqrt(totalStatistics * totalStatistics + totalSystematicsUp )/(hMuon->GetBinContent(bin));

    /// get ratio od Electron to merged
    yCoorEleRatio[bin-1]  =  (hEle->GetBinContent(bin)) / centralValue ;
    yStatEleRatio[bin-1]     =  totalStatistics / centralValue ;
    ySystDownEleRatio[bin-1]    =  sqrt(totalStatistics * totalStatistics + totalSystematicsDown )/(hMuon->GetBinContent(bin));
    ySystUpEleRatio[bin-1]      =  sqrt(totalStatistics * totalStatistics + totalSystematicsUp )/(hMuon->GetBinContent(bin));
    // set errors to zero
    yStatMuonRatio[bin-1]        = 0. ;
    ySystDownMuonRatio[bin-1]    = 0. ;
    ySystUpMuonRatio[bin-1]      = 0. ;

    yStatEleRatio[bin-1]        = 0. ;
    ySystDownEleRatio[bin-1]    = 0. ;
    ySystUpEleRatio[bin-1]      = 0. ; 
    // set errors to zero




    if ( DEBUG ) cout << " merging bin " << bin <<" of "<< nBins << "   " << centralValue  <<"   " << hEle->GetBinContent(bin) << "  "  << hMuon->GetBinContent(bin) << endl; 

  }
  cout<< " things are merged lets go to the plotting"<< endl;
  TGraph *mcGraph  =  new TGraph(nBins, xCoor, yCoor);
  TGraphAsymmErrors *grCentralStat       =  new TGraphAsymmErrors(nBins, xCoor, yCoor, xErr, xErr, yStat, yStat);
  TGraphAsymmErrors *grCentralStatMuonRatio  =  new TGraphAsymmErrors(nBins, xCoor, yCoorMuonRatio, xErr, xErr, yStatMuonRatio, yStatMuonRatio);
  TGraphAsymmErrors *grCentralStatEleRatio  =  new TGraphAsymmErrors(nBins, xCoor, yCoorEleRatio, xErr, xErr, yStatEleRatio, yStatEleRatio);
  TGraphAsymmErrors *grCentralSystMuonRatio  =  new TGraphAsymmErrors(nBins, xCoor, yCoorMuonRatio, xErr, xErr, ySystDownMuonRatio, ySystUpMuonRatio);
  TGraphAsymmErrors *grCentralSystEleRatio  =  new TGraphAsymmErrors(nBins, xCoor, yCoorEleRatio, xErr, xErr, ySystDownEleRatio, ySystUpEleRatio);


  cout<< " TGraphs are done"<< endl;
  //	grCentralStat->Draw();
  //	grCentralStatMuonRatio->Draw("same");


  grCentralStat->SetMarkerStyle(20);
  grCentralStat->SetMarkerSize(0.8);
  grCentralStat->SetMarkerColor(kBlack);
  grCentralStat->SetLineColor(kBlack);
  //grCentralStat->SetLineWidth(2);
  TGraphAsymmErrors *grCentralSyst  =  new TGraphAsymmErrors(nBins, xCoor, yCoor, xErr, xErr, yStat, yStat);
  grCentralSyst->SetLineColor(kRed+1);
  //grCentralSyst->SetLineWidth(2);
  grCentralSyst->SetMarkerStyle(20);
  grCentralSyst->SetMarkerSize(0.8);



  ////////////////////////////////////// now do the canvas
  string mVariable = "Merged_"+VARIABLE;
  TCanvas *can  =  new TCanvas(mVariable.c_str(), mVariable.c_str(), 600, 900);
  can->cd();
  TPad *pad1  =  new TPad("pad1","pad1", 0, 0.4, 1, 1);
  pad1->SetBottomMargin(0);
  pad1->SetRightMargin(0.05);
  pad1->SetLeftMargin(0.1);
  if (LOGZ) pad1->SetLogy(); pad1->SetLogy();
  pad1->SetTickx();
  pad1->SetTicky();
  pad1->Draw();
  pad1->cd();
  grCentralSyst->GetXaxis()->SetLimits(hMuon->GetXaxis()->GetXmin(),hMuon->GetXaxis()->GetXmax());
  //grCentralSyst->GetYaxis()->SetLimits(yCoor[0]-0.4,xCoor[nBins-1]+0.4);
  //grCentralSyst->Draw("AP");
  double MineYMax  =  hMuon->GetMaximum();
  if ( grCentralSyst->GetMaximum() > MineYMax ) MineYMax  =  grCentralSyst->GetMaximum();
  hMuon->GetXaxis()->SetRangeUser(hMuon->GetXaxis()->GetXmin(),hMuon->GetXaxis()->GetXmax());
  if (VARIABLE.find("ZNGood") !=  std::string::npos) { MineYMax  =  65.;
    hMuon->GetXaxis()->SetRangeUser(hMuon->GetXaxis()->GetXmin()+hMuon->GetBinWidth(1),hMuon->GetXaxis()->GetXmax()- 2*hMuon->GetBinWidth(1));
    grCentralSyst->GetXaxis()->SetLimits(hMuon->GetXaxis()->GetXmin()+hMuon->GetBinWidth(1),hMuon->GetXaxis()->GetXmax() -2* hMuon->GetBinWidth(1));
  }


  hMuon->SetFillStyle(3001);
  hMuon->SetFillColor(kRed);
  hMuon->SetLineColor(kRed);
  hMuon->GetYaxis()->SetTitleSize(0.05);
  hMuon->GetYaxis()->SetTitleOffset(0.95);
  if (LOGZ) hMuon->SetMaximum(MineYMax*5);
  else hMuon->SetMaximum(MineYMax*1.5);
  if (doXSec  ==  1 || doNormalize   ==   1 ) { hMuon->GetYaxis()->SetTitle("");
    string temp;
    temp = "d#sigma/dH_{T} [pb/GeV]";
    if (VARIABLE.find("Pt") !=  std::string::npos) temp = "d#sigma/dp_{T} [pb/GeV]";
    if (VARIABLE.find("Eta") !=  std::string::npos) temp = "d#sigma/d#eta [pb]";
    if (VARIABLE.find("ZNGood") !=  std::string::npos){
      temp = "d#sigma/dN [pb]";

    }
    if (VARIABLE.find("ZNGood") !=  std::string::npos){
      temp = "d#sigma/dN [pb]";
    }

    if (VARIABLE.find("SpT") !=  std::string::npos) {temp = "d#sigma/dSpT [pb]";
      if (doNormalize  ==  1)  temp = "1/#sigma d#sigma/dSpT";
    }
    if (VARIABLE.find("PHI") !=  std::string::npos || VARIABLE.find("Phi") !=  std::string::npos ) {temp = "d#sigma/d#Delta#Phi  [pb]";
      if (doNormalize  ==  1)  temp = "1/#sigma d#sigma/d#Delta#Phi";
    }
    const char *tempYAxisTitle = temp.c_str();
    hMuon->GetYaxis()->SetTitle(tempYAxisTitle);
    hEle->GetYaxis()->SetTitle(tempYAxisTitle);

  }
  hMuon->Draw("E2");
  mcGraph->SetLineColor(kRed);
  //mcGraph->Draw("L same");
  //   mcGraph->Draw("L");
  hEle->SetFillStyle(3002);
  hEle->SetFillColor(kBlue);
  hEle->SetLineColor(kBlue);
  hEle->Draw("E2 same");
  grCentralSyst->Draw("P same");
  grCentralStat->Draw("P same");
  pad1->Draw();
  TPaveText *pt  =  new TPaveText(0.07,0.88,0.99,0.98,"NDC");
  pt->AddText("CMS Preliminary                    #sqrt{s}  =  8 TeV                    #int L dt  =  19.8 fb^{-1}  ");
  pt->SetBorderSize(0);
  pt->SetFillStyle(0);
  pt->Draw();
  // set the legend
  double xLowLeg(0.65), xHighLeg(0.95);
  if (!DECREASE){
    xLowLeg = 0.14;
    xHighLeg = 0.47;
  }
  TLegend *legend = new TLegend(xLowLeg, 0.66, xHighLeg, 0.91);
  //TLegend *legend  =  new TLegend(0.7,0.65,0.96,0.9);
  legend->SetHeader("  ");
  hMuon->SetTitle("");
  legend->SetFillStyle(0);
  legend->SetBorderSize(0);
  legend->AddEntry(hMuon, "Muon", "f");
  legend->AddEntry(hEle, "Electron", "f");
  legend->AddEntry(grCentralStat, "Combination", "pl");
  legend->AddEntry(hCombErr, " Comb. Err.", "f");
  legend->Draw();
  /*	TLatex *variableLabel  =  new TLatex();
      variableLabel->SetTextSize(0.025);
      variableLabel->SetTextFont(42);
      variableLabel->SetLineWidth(2);
      variableLabel->SetTextColor(kBlack);
      variableLabel->SetNDC();
      variableLabel->DrawLatex(0.63,0.82,hCombinedStat->GetTitle());
      pad1->cd();
   */

  can->cd();
  TPad *pad2  =  new TPad("pad2","pad2", 0, 0.236, 1, 0.4);
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0);
  pad2->SetRightMargin(0.05);
  pad2->SetLeftMargin(0.1);
  pad2->SetGridy();
  pad2->SetTickx();
  pad2->SetTicky();
  pad2->Draw();
  pad2->cd();
  grCentralStatMuonRatio->SetMarkerStyle(20);
  grCentralStatMuonRatio->SetMarkerSize(0.8);
  grCentralStatMuonRatio->SetMarkerColor(kBlack);
  grCentralStatMuonRatio->SetLineColor(kBlack);
  grCentralSystMuonRatio->SetLineColor(kRed+1);
  grCentralSystMuonRatio->SetMarkerStyle(20);
  grCentralSystMuonRatio->SetMarkerSize(0.8);
  grCentralSystMuonRatio->GetXaxis()->SetLimits(hMuon->GetXaxis()->GetXmin(),hMuon->GetXaxis()->GetXmax());
  if (VARIABLE.find("ZNGood") !=  std::string::npos) grCentralSystMuonRatio->GetXaxis()->SetLimits(hMuon->GetXaxis()->GetXmin()+hMuon->GetBinWidth(1),hMuon->GetXaxis()->GetXmax() - hMuon->GetBinWidth(1));
  //grCentralSystMuonRatio->GetYaxis()->SetRangeUser(0.41,1.59);
  grCentralSystMuonRatio->GetYaxis()->SetRangeUser(0.7,1.3);
  grCentralSystMuonRatio->GetYaxis()->SetTitle("Muon / Comb.");
  grCentralSystMuonRatio->GetYaxis()->SetTitleSize(0.14);
  grCentralSystMuonRatio->GetYaxis()->SetTitleOffset(0.35);
  grCentralSystMuonRatio->GetYaxis()->CenterTitle();
  grCentralSystMuonRatio->GetYaxis()->SetLabelSize(0.12);
  grCentralSystMuonRatio->GetYaxis()->SetLabelOffset(0.01);
  grCentralSystMuonRatio->SetTitle("");
  grCentralSystMuonRatio->Draw("AP");
  hCombErr->SetFillColor(kGreen-3);
  hCombErr->SetLineColor(kGreen-3);
  hCombErr->SetFillStyle(3001);
  hCombErr->DrawCopy("E2 same");
  grCentralSystMuonRatio->Draw("P same");
  grCentralStatMuonRatio->Draw("P same");
  TLatex *latexLabel  =  new TLatex();
  latexLabel->SetTextSize(0.15);
  latexLabel->SetTextFont(42);
  latexLabel->SetLineWidth(2);
  latexLabel->SetTextColor(kAzure-5);
  latexLabel->SetNDC();
  latexLabel->DrawLatex(0.15,0.09,"Muon");
  /*	TLegend *legend2  =  new TLegend(0.1,0.6,0.6,0.99);
      legend2->SetFillStyle(0);
      legend2->SetBorderSize(0);
      legend2->AddEntry(hCombErr, " Combination Error.", "f");
      legend2->Draw();
   */
  pad2->Draw();
  pad2->cd();

  can->cd();
  TPad *pad3  =  new TPad("pad3","pad3", 0, 0., 1, 0.236);
  pad3->SetTopMargin(0);
  pad3->SetBottomMargin(0.3);
  pad3->SetRightMargin(0.05);
  pad3->SetLeftMargin(0.1);
  pad3->SetGridy();
  pad3->SetTickx();
  pad3->SetTicky();
  pad3->Draw();
  pad3->cd();
  grCentralStatEleRatio->SetMarkerStyle(20);
  grCentralStatEleRatio->SetMarkerSize(0.8);
  grCentralStatEleRatio->SetMarkerColor(kBlack);
  grCentralStatEleRatio->SetLineColor(kBlack);
  grCentralSystEleRatio->SetLineColor(kRed+1);
  grCentralSystEleRatio->SetMarkerStyle(20);
  grCentralSystEleRatio->SetMarkerSize(0.8);
  grCentralSystEleRatio->GetXaxis()->SetLimits(hMuon->GetXaxis()->GetXmin(),hMuon->GetXaxis()->GetXmax());
  if (VARIABLE.find("ZNGood") !=  std::string::npos) grCentralSystEleRatio->GetXaxis()->SetLimits(hMuon->GetXaxis()->GetXmin()+hMuon->GetBinWidth(1),hMuon->GetXaxis()->GetXmax() - hMuon->GetBinWidth(1));
  grCentralSystEleRatio->GetXaxis()->SetTitle(hMuon->GetXaxis()->GetTitle());
  grCentralSystEleRatio->GetXaxis()->SetTitleSize(0.1);
  grCentralSystEleRatio->GetXaxis()->SetTitleOffset(1.14);
  grCentralSystEleRatio->GetXaxis()->SetLabelSize(0.1);
  grCentralSystEleRatio->GetXaxis()->SetLabelOffset(0.014);
  //grCentralSystEleRatio->GetYaxis()->SetRangeUser(0.41,1.59);
  grCentralSystEleRatio->GetYaxis()->SetRangeUser(0.65,1.35);
  grCentralSystEleRatio->GetYaxis()->SetTitle("Elec. / Comb.");
  grCentralSystEleRatio->GetYaxis()->SetTitleSize(0.1);
  grCentralSystEleRatio->GetYaxis()->SetTitleOffset(0.48);
  grCentralSystEleRatio->GetYaxis()->CenterTitle();
  grCentralSystEleRatio->GetYaxis()->SetLabelSize(0.085);
  grCentralSystEleRatio->GetYaxis()->SetLabelOffset(0.012);
  grCentralSystEleRatio->SetTitle("");
  grCentralSystEleRatio->Draw("AP");
  hCombErr->SetFillColor(kGreen-3);
  hCombErr->SetLineColor(kGreen-3);
  hCombErr->SetFillStyle(3001);
  hCombErr->DrawCopy("E2 same");
  grCentralStatEleRatio->Draw("P same");
  TLatex *latexLabel1  =  new TLatex();
  latexLabel1->SetTextSize(0.11);
  latexLabel1->SetTextFont(42);
  latexLabel1->SetLineWidth(2);
  latexLabel1->SetTextColor(kOrange+2);
  latexLabel1->SetNDC();
  latexLabel1->DrawLatex(0.15,0.35,"Electron");
  pad3->Draw();
  pad3->cd();
  can->cd();

  string outputFileNamePNG = OUTPUTDIRECTORY;
  if (doXSec) outputFileNamePNG += "CompLeptonsAndComb_XSec_";
  else if (doNormalize) outputFileNamePNG += "CompLeptonsAndComb_Normalized_";

  ostringstream optionCorrStr; optionCorrStr << optionCorr;
  outputFileNamePNG += VARIABLE ;
  outputFileNamePNG += "_CorrelationOption_" + optionCorrStr.str()  ;
  if (doVarWidth) outputFileNamePNG += "_VarWidth";
  outputFileNamePNG += ".png";

  can->Print(outputFileNamePNG.c_str());

}

///////////////////////////////////////////////
void returnCov(TH1D *dataCentral, TH1D* hUp, TH1D* hDown, TH2D *cov , TH1D *hErrors){

  const int nBins(dataCentral->GetNbinsX());
  TMatrixD outCov(nBins,nBins);

  double sigma[nBins];
  for (int bin(1); bin  <=   nBins; bin++){
    double centralValue  =  dataCentral->GetBinContent(bin);
    double difference1  =  -dataCentral->GetBinContent(bin)+hUp->GetBinContent(bin);	
    double difference2(dataCentral->GetBinContent(bin)-hDown->GetBinContent(bin));	
    if (fabs(difference1) > fabs(difference2) ) sigma[bin-1] = fabs(difference1);
    else sigma[bin-1] = fabs(difference2);
    hErrors->SetBinContent(bin,sigma[bin-1]);
  }

  for (int bin(1); bin  <=   nBins; bin++){

    for (int bin1(1); bin1  <=   nBins; bin1++){
      double corr  =  1;
      if ( VARIABLE.find("ZNGoodJets_Zexc") !=  std::string::npos && ((bin  ==  1 && bin1 !=  bin) || (bin1  ==  1 && bin1 !=  bin) ) ) corr  =  -1. ;
      corr  =  0.;
      if (bin  ==  bin1 ) corr  =  1;
      cov->SetBinContent(bin,bin1,corr * sigma[bin-1]*sigma[bin1-1]);
      cout<<cov->GetBinContent(bin,bin1)<<"  ";
    }
    cout<<endl;
  }
  cout << " END fetching cov matrix "<<endl;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void createZNGoodJets_Zinc( TH1D* hCombinedStat, TH1D* hCombinedSyst, TH2D *hError2D[], string unfAlg, bool doVarWidth, bool doXSec, bool doNormalize){
  string energy = getEnergy();
  string variable = "ZNGoodJets_Zinc";
  int JetPtMin = 30 ;
  int JetPtMax = 0 ;
  string leptonFlavor = "DE";
  //if (!isMuon) leptonFlavor = "DE";
  bool doFlat = 0 ;

  //-- fetch the data files and histograms --------------
  TFile *fData[3];             // 0 = central, 1 = JES Up, 2 = JES Down 
  getFiles(FILESDIRECTORY, fData, leptonFlavor, energy, ProcessInfo[DATAFILENAME].filename, JetPtMin, JetPtMax, doFlat, doVarWidth, 0, 0, 0, 0, 0, 1);
  cout << " got data " << endl;
  TH1D *hData[3];
  getHistos(hData, fData, variable);
  cout << " got data " << endl;
  //-----------------------------------------------------

  //variable = "ZNGoodJets_Zexc";

  //-- fetch the DY files and histograms ----------------
  TFile *fDYMadGraphMu[4];
  TFile *fDYMadGraphEl[4];
  TFile *fDYSherpa[2];               // 0 = central, 1 = PU Up,  2 = PU Down,  3 = JER Up
  TFile *fDYPowheg[2];
  getFiles(FILESDIRECTORY, fDYMadGraphMu, "DMu", energy, ProcessInfo[DYMADGRAPHFILENAME].filename, JetPtMin, JetPtMin, doFlat, doVarWidth, 0, 0, 0, 0, 0,  1);
  getFiles(FILESDIRECTORY, fDYMadGraphEl, "DE", energy, ProcessInfo[DYMADGRAPHFILENAME].filename, JetPtMin, JetPtMin, doFlat, doVarWidth, 0, 0, 0, 0, 0,  1);
  fDYSherpa[1] = getFile(FILESDIRECTORY, "DMu", energy, DYSHERPAFILENAME, JetPtMin, JetPtMin, doFlat, doVarWidth);
  fDYPowheg[1] = getFile(FILESDIRECTORY, "DMu", energy, DYPOWHEGFILENAME, JetPtMin, JetPtMin, doFlat, doVarWidth);
  fDYSherpa[0] = getFile(FILESDIRECTORY, "DE", energy, DYSHERPAFILENAME, JetPtMin, JetPtMin, doFlat, doVarWidth);
  fDYPowheg[0] = getFile(FILESDIRECTORY, "DE", energy, DYPOWHEGFILENAME, JetPtMin, JetPtMin, doFlat, doVarWidth);
  //TH1D *hDY[4];
  //TH1D *hDYGenMadGraph = NULL, *hDYGenSherpa = NULL,  *hDYGenPowheg = NULL;
  TH1D *hDYGenMadGraph[2], *hDYGenSherpa[2],  *hDYGenPowheg[2];
  //getHistos(hDY, fDYMadGraphMu, variable);
  hDYGenMadGraph[1] = getHisto(fDYMadGraphMu[0], "gen" + variable);
  hDYGenMadGraph[0] = getHisto(fDYMadGraphEl[0], "gen" + variable);
  hDYGenSherpa[0] = getHisto(fDYSherpa[0], "gen" + variable);
  hDYGenPowheg[0] = getHisto(fDYPowheg[0], "gen" + variable);
  hDYGenSherpa[1] = getHisto(fDYSherpa[1], "gen" + variable);
  hDYGenPowheg[1] = getHisto(fDYPowheg[1], "gen" + variable);
  //-----------------------------------------------------

  TCanvas *c = new TCanvas();
  c->cd();
  hDYGenSherpa[0]->Scale(1./19800.);
  hDYGenSherpa[0]->DrawCopy("text");
  hDYGenSherpa[0]->Scale(19800.);

  //variable = "ZNGoodJets_Zinc";

  TH1D *hOutStat = (TH1D*) hData[0]->Clone();
  TH1D *hOutSyst = (TH1D*) hData[0]->Clone();


  // now sum merged histogram
  // then cum errors histograms
  const int nBins(hCombinedStat->GetNbinsX());
  const int nSyst = 8 ;
  for (int bin(1); bin <= nBins; bin++){
    double sum = hCombinedStat->GetBinContent(bin);
    double sumStatErr = hCombinedStat->GetBinError(bin) * hCombinedStat->GetBinError(bin); 
    double sumSyst = hCombinedSyst->GetBinContent(bin);
    double sumSystErr = hCombinedSyst->GetBinError(bin) * hCombinedSyst->GetBinError(bin); 
    double sumErr[nSyst]  = {0} ;

    for (int syst(0); syst < nSyst; syst++) sumErr[syst] = hError2D[syst]->GetBinContent(bin , bin) ;


    for (int j(bin + 1); j <= nBins; j++){
      sum += hCombinedStat->GetBinContent(j);
      sumStatErr += hCombinedStat->GetBinError(j) * hCombinedStat->GetBinError(j);
      sumSystErr += hCombinedSyst->GetBinError(j) * hCombinedSyst->GetBinError(j);
      cout << sumSystErr << "   " <<  hCombinedSyst->GetBinContent(j) << endl; 
      for (int syst(0); syst < nSyst; syst++) sumErr[syst] += hError2D[syst]->GetBinContent(j,j) ;
    }
    hOutStat->SetBinContent(bin, sum);
    hOutStat->SetBinError(bin, sqrt(sumStatErr));
    hOutSyst->SetBinContent(bin, sum);
    hOutSyst->SetBinError(bin, sqrt(sumSystErr));

    for (int syst(0); syst < nSyst; syst++) hError2D[syst]->SetBinContent(bin , bin , sumErr[syst]) ;
    //              for (int syst(0); syst < nSyst; syst++) sumErr[syst] = hError2D[syst]->SetBinContent(bin , bin , 1. ) ;
    //                cout << variable << "   " << hCombinedStat->GetBinContent(bin) << "    " << sum << endl;
    cout << variable << "   " << hCombinedSyst->GetBinContent(bin) << "    " << sqrt(sumStatErr) << "   " << sqrt(sumSystErr)  << "    "<<hCombinedSyst->GetBinError(bin) << endl;
  }



  // and finally produce tables and plots
  // lets create som tables
  ostringstream optionCorrStr; optionCorrStr << optionCorr ;
  string fileNameTable = "PNGFiles/NiceUnfold/TableSystematics_" + energy + "_" + variable + "_" + unfAlg  ;
  fileNameTable += "_CorrelationOption_" + optionCorrStr.str() ;
  if (doVarWidth) fileNameTable += "_VarWidth";
  fileNameTable +=  ".tex";
  cout << fileNameTable << endl;

  createTexTable(variable, fileNameTable, (TH1D*) hOutStat->Clone(), hError2D, doXSec, doNormalize, Luminosity);
  /// PLOT FINAL PLOTS: COMBINATION VS MC
  plotCombination(variable, (TH1D*) hOutStat->Clone(), (TH1D*) hOutSyst->Clone(), hDYGenMadGraph, hDYGenSherpa, hDYGenPowheg);

  for (int i(0); i < 3; i++) fData[i]->Close();
  for (int i(0); i < 4; i++) fDYMadGraphMu[i]->Close();
  for (int i(0); i < 4; i++) fDYMadGraphEl[i]->Close();

  fDYSherpa[0]->Close();               // 0 = central, 1 = PU Up,  2 = PU Down,  3 = JER Up
  fDYPowheg[0]->Close();
  fDYSherpa[1]->Close();               // 0 = central, 1 = PU Up,  2 = PU Down,  3 = JER Up
  fDYPowheg[1]->Close();


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void dumpElements(TMatrixD& a)
{
  cout << endl << endl;
  const int nrows  =  a.GetNrows();
  const int ncols  =  a.GetNcols();
  if(nrows  ==  ncols) cout << "determinent  =  " << a.Determinant() << endl;
  a.Print();
  cout << endl << endl;
  return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void dumpElements(TVectorD& a)
{
  cout << endl << endl;
  a.Print();
  cout << endl << endl;
  return;
}


