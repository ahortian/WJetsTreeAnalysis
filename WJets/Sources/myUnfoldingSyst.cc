//#include "getFilesAndHistograms.h"
#include "myUnfoldingSyst.h"
#include <TFile.h>
#include <sstream>
#include <iostream>
#include <RooUnfold.h>
#include <RooUnfoldResponse.h>
#include <TRandom3.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TH2D* CovToCorr(const TH2D *h)
{
    TH2D* hCorr = (TH2D*) h->Clone();
    hCorr->SetDirectory(0);
    int xbin(h->GetNbinsX()), ybin(h->GetNbinsY());
    for (int i(1); i <= ybin; i++) {
        for (int j(1); j <= xbin; j++) {
            double temp = 0.;
            temp = h->GetBinContent(j, i) / sqrt(h->GetBinContent(i, i) * h->GetBinContent(j, j));
            hCorr->SetBinContent(j, i, temp);
        }
    }
    return hCorr;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TH2D* CovToCorr2(const TH2D *h, string title)
{  

    int xbin(h->GetNbinsX()), ybin(h->GetNbinsY());
    //   string title = h->GetTitle();
    TH2D* hCorrelation = new TH2D(title.c_str(),title.c_str(), xbin - 2 , 0.5, xbin - 1.5, xbin - 2, 0.5, xbin-1.5);

    //TH2D* hCorr = (TH2D *) h ->Clone();
    for (int i(2); i <= ybin - 1; i++){
        for (int j(2); j <= xbin - 1; j++){
            double temp = 0.;
            temp = h->GetBinContent(j,i) / sqrt( h->GetBinContent(i,i ) * h->GetBinContent(j,j) );
            hCorrelation->SetBinContent(j - 1 , i - 1 , temp );
            //hCorr->SetBinContent(j, i, temp );
        }
    }
    return hCorrelation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TH2D* setCovariance(const TH2D *h, const TH1D *hCent, double error)
{
    TH2D* hCorr = (TH2D*) h->Clone();
    int xbin(h->GetNbinsX()), ybin(h->GetNbinsY());
    for (int i(1); i <= ybin; i++){
        for (int j(1); j <= xbin; j++){
            double temp = 0.;
            temp = h->GetBinContent(j,i) / sqrt(h->GetBinContent(i,i) * h->GetBinContent(j,j));
            hCorr->SetBinContent(j, i, temp * hCent->GetBinContent(i) * error * hCent->GetBinContent(j) * error);
        }
    }
    return hCorr;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TH1D* getHisto(string histoFilesDirectory,string leptonFlavor, string energy, string DataHistoFilesName, string syst, string direction, int JetPtMin, int JetPtMax, string variable)
{
    //-- get histogram ----------------------------------------------------------
    TFile *DataFile;
    ostringstream JetPtMinStr;  JetPtMinStr << JetPtMin;
    ostringstream JetPtMaxStr;  JetPtMaxStr << JetPtMax;

    string name = histoFilesDirectory + leptonFlavor + "_"  + energy + "_" + DataHistoFilesName + syst + "_"; 
    if (syst !="0") name += direction+"_";
    name += "JetPtMin_"+JetPtMinStr.str();
    if (JetPtMax != 0 && JetPtMax > JetPtMin) name += "_JetPtMax_" + JetPtMaxStr.str();
    name += ".root";
    std::cout << "opening :" << name << std::endl;
    DataFile = new TFile(name.c_str(), "READ");
    TH1D *meas = (TH1D*) DataFile->Get(variable.c_str());
    TH1D *measClone = (TH1D*) meas->Clone();

    return measClone;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RooUnfoldResponse* getResponse(string histoFilesDirectory, string leptonFlavor, string energy, string DataHistoFilesName, string syst, string direction, int JetPtMin, int JetPtMax, string variable)
{
    //-- get histogram ----------------------------------------------------------
    TFile *DataFile;
    ostringstream JetPtMinStr;  JetPtMinStr << JetPtMin;
    ostringstream JetPtMaxStr;  JetPtMaxStr << JetPtMax;

    string name = histoFilesDirectory + leptonFlavor + "_"  + energy + "_" + DataHistoFilesName + syst + "_";
    if (syst !="0") name += direction+"_";
    name += "JetPtMin_"+JetPtMinStr.str();
    if (JetPtMax != 0 && JetPtMax > JetPtMin) name += "_JetPtMax_" + JetPtMaxStr.str();
    name += ".root";
    std::cout << "opening :" << name << std::endl;
    DataFile = new TFile(name.c_str(), "READ");
    string respName = "response" + variable;
    RooUnfoldResponse *responseGet = (RooUnfoldResponse*) DataFile->Get(respName.c_str());
    RooUnfoldResponse *response = (RooUnfoldResponse*) responseGet->Clone();

    return response;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TH1D* getSumBG(string histoFilesDirectory, string leptonFlavor, string energy, string* DataHistoFilesNameArray, string syst, string direction, int JetPtMin, int JetPtMax, string variable)
{
    //-- get histogram ----------------------------------------------------------
    TH1::SetDefaultSumw2();
    ostringstream JetPtMinStr;  JetPtMinStr << JetPtMin;
    ostringstream JetPtMaxStr;  JetPtMaxStr << JetPtMax;
    TH1D* hOut;
    for ( int i = 0 ; i < 8 ; i++){
        TFile *DataFile;
        string DataHistoFilesName = DataHistoFilesNameArray[i];
        string name = histoFilesDirectory + leptonFlavor + "_"  + energy + "_" + DataHistoFilesName + syst + "_";
        if (syst !="0") name += direction+"_";
        name += "JetPtMin_"+JetPtMinStr.str();
        if (JetPtMax != 0 && JetPtMax > JetPtMin) name += "_JetPtMax_" + JetPtMaxStr.str();
        name += ".root";
        std::cout << "opening :" << i << "   " << name << std::endl;
        DataFile = new TFile(name.c_str(), "READ");
        TH1D *meas = (TH1D*) DataFile->Get(variable.c_str());
        TH1D *measClone = (TH1D*) meas->Clone();
        if ( i == 0 ) hOut = (TH1D*) meas->Clone();
        else hOut->Add(measClone, 1);
        std::cout << "fetched: " << variable << " from " << name << std::endl;

    }

    return hOut;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TH1D* Unfold(string unfAlg, RooUnfoldResponse* response, TH1D* hData, TH1D* hSumBG, int Kterm, string hOutName, bool useOverFlow)
{
    if (useOverFlow) response->UseOverflow();
    RooUnfold* RObject;
    TH1D * hDataClone = (TH1D*) hData->Clone();
    hDataClone->Add(hSumBG, -1);
    if (unfAlg == "SVD")   RObject = (RooUnfold*) RooUnfold::New( RooUnfold::kSVD,   response, hDataClone, Kterm);
    if (unfAlg == "Bayes") RObject = (RooUnfold*) RooUnfold::New( RooUnfold::kBayes, response, hDataClone, Kterm);
    RObject->SetVerbose(0);
    TH1D* hCorrected = (TH1D*) RObject->Hreco();
    hCorrected->SetName(hOutName.c_str());

    return hCorrected;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TH2D* Unfold2D(string unfAlg, RooUnfoldResponse* response, TH2D* hData, TH2D* hSumBG, int Kterm, string hOutName, bool useOverFlow)
{
    if (useOverFlow) response->UseOverflow();
    RooUnfold* RObject;
    TH2D * hDataClone = (TH2D*) hData->Clone();
    hDataClone->Add(hSumBG, -1);
    if (unfAlg == "SVD")   RObject = (RooUnfold*) RooUnfold::New( RooUnfold::kSVD,   response, hDataClone, Kterm);
    if (unfAlg == "Bayes") RObject = (RooUnfold*) RooUnfold::New( RooUnfold::kBayes, response, hDataClone, Kterm);
    RObject->SetVerbose(0);
    TH2D* hCorrected = (TH2D*) RObject->Hreco();
    hCorrected->SetName(hOutName.c_str());

    return hCorrected;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TH2D* CovFromRoo(string unfAlg, RooUnfoldResponse* response, TH1D* hData, TH1D* hSumBG, int Kterm, string hOutName, int NToys, bool useOverFlow)
{

    if ( useOverFlow ) response->UseOverflow();
    RooUnfold* RObject;
    TH1D * hDataClone = (TH1D *) hData->Clone();
    hDataClone->Add(hSumBG, -1);
    if (unfAlg == "SVD")   RObject = (RooUnfold*) RooUnfold::New( RooUnfold::kSVD,   response, hDataClone, Kterm);
    if (unfAlg == "Bayes") RObject = (RooUnfold*) RooUnfold::New( RooUnfold::kBayes, response, hDataClone, Kterm);
    RObject->SetVerbose(0);
    RObject->SetNToys(NToys);

    if (hOutName == "CentralCovToy" ) {
        const TMatrixD covTemp = RObject->Ereco(RooUnfold::kCovToy); 
        TH2D* covarianceMat = new TH2D(covTemp);
        covarianceMat->SetName(hOutName.c_str());
        return covarianceMat;
    }
    else {
        const TMatrixD covTemp = RObject->Ereco(RooUnfold::kCovariance);	
        TH2D* covarianceMat = new TH2D(covTemp);
        covarianceMat->SetName("CentralCov");
        return covarianceMat;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void hBinCon(TH1D* hData)
{
    for(int j=1; j!=hData->GetNbinsX()+1; j++)
        std::cout << " bin : " << j << hData->GetBinContent(j) << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TH1D* SetSystErrorsMax(TH1D* hCent, TH1D* hUp, TH1D* hDown, string name)
{
    TH1D *hCentClone = (TH1D*) hCent->Clone(name.c_str());
    for(int j(0); j <= hCent->GetNbinsX() + 1 ; j++){
        double ErrUp( fabs(hUp->GetBinContent(j) - hCent->GetBinContent(j)) );
        double ErrDown( fabs(hCent->GetBinContent(j) - hDown->GetBinContent(j)) );
        double ErrMax( ErrUp > ErrDown ? ErrUp : ErrDown );
        hCentClone->SetBinError(j, ErrMax);
    }

    return hCentClone;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TH1D* SetSystErrorsMean(TH1D* hData, double Error, string name)
{
    TH1D *hDataClone = (TH1D*) hData->Clone(name.c_str());
    hDataClone->SetDirectory(0);
    for(int j = 0 ; j != hData->GetNbinsX()+1; j++){
        double binCon = hData->GetBinContent(j);
        if ( binCon  > 0 ) hDataClone->SetBinError(j, binCon * Error);
    }

    return hDataClone;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TH1D* SetSystErrorsMean(TH1D* hData, TH1D* hUp, TH1D* hDown, string name)
{
    TH1D *hDataClone = (TH1D*) hData->Clone(name.c_str());
    hDataClone->SetDirectory(0);
    for(int j(1); j != hData->GetNbinsX() + 1; j++) {
        double ErrUp = hUp->GetBinContent(j)-hData->GetBinContent(j);
        double ErrDown = hData->GetBinContent(j) - hDown->GetBinContent(j);
        double ErrMax = ErrUp;
        if ( ErrDown * ErrUp > 0 ) {
            if (  ErrDown > ErrUp ) ErrMax = ErrDown ;
        }
        else ErrMax = ( fabs(ErrUp) + fabs(ErrDown) )/2.;
        hDataClone->SetBinError(j,ErrMax);
    }

    return hDataClone;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TH1D* SetSystErrorsMean(TH1D* hData, TH1D* hCentral, TH1D* hUp, TH1D* hDown, string name, int option)
{
    TH1D *hDataClone = (TH1D*) hData->Clone(name.c_str());
    hDataClone->SetDirectory(0);
    int startBin = 0 ;
    int NBins = hData->GetNbinsX()+1  ;
    if ( option == 1 ){
        startBin = 1 ;
        NBins = hData->GetNbinsX() ;
    }
    for(int j = startBin ; j <= NBins  ; j++){
        if ( hCentral->GetBinContent(j) <= 0 ) continue;
        double ErrUp = hUp->GetBinContent(j)-hCentral->GetBinContent(j);
        double ErrDown = hCentral->GetBinContent(j) - hDown->GetBinContent(j);
        double ErrMax = ErrUp;
        if ( ErrDown * ErrUp > 0 ) {
            if (  ErrDown > ErrUp ) ErrMax = ErrDown ;
        }
        else ErrMax = ( fabs(ErrUp) + fabs(ErrDown) )/2.;
        ErrMax *= ( hData->GetBinContent(j) / hCentral->GetBinContent(j) ) ;
        hDataClone->SetBinError(j,ErrMax);
    }

    return hDataClone;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TH1D* SetSystErrorsMean(TH1D* hData, TH1D* hCentral, TH1D* hUp, TH1D* hDown, double Error, string name)
{
    TH1D *hDataClone = (TH1D*) hData->Clone(name.c_str());
    hDataClone->SetDirectory(0);
    for(int j(0); j != hData->GetNbinsX() + 1; j++) {
        if ( hCentral->GetBinContent(j) <= 0 ) continue;
        double ErrUp = hUp->GetBinContent(j)-hCentral->GetBinContent(j);
        double ErrDown = hCentral->GetBinContent(j) - hDown->GetBinContent(j);
        double ErrMax = ErrUp;
        if ( ErrDown * ErrUp > 0 ) {
            if (  ErrDown > ErrUp ) ErrMax = ErrDown ;
        }
        else ErrMax = ( fabs(ErrUp) + fabs(ErrDown) )/2.;
        //double ErrTot = sqrt(pow(ErrMax,2)+pow(Error * hCentral->GetBinContent(j),2));
        ErrMax *= ( hData->GetBinContent(j) / hCentral->GetBinContent(j) ) ;
        hDataClone->SetBinError(j, ErrMax);
    }

    return hDataClone;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TH2D* SetResponseErrors(TH2D* hCen, TH2D* hUp, TH2D* hDown)
{
    TH2D * hCenClone = (TH2D *) hCen->Clone();
    hCenClone->SetDirectory(0);
    for(int i(1); i != hCen->GetNbinsX() + 1; i++) {
        for(int j(1); j != hCen->GetNbinsY() + 1; j++) {
            double ErrUp = fabs(hUp->GetBinContent(i,j)-hCen->GetBinContent(i,j));
            double ErrDown = fabs(hCen->GetBinContent(i,j) - hDown->GetBinContent(i,j));
            double ErrMax = ErrUp;
            if ( ErrDown > ErrUp ) ErrMax = ErrDown ;
            hCenClone->SetBinError(i,j,ErrMax);
        }
    }
    return hCenClone;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// this is stil messy, stil thinking what is the best thing to do. until then ...
TH1D *ToyMCErrorsStat(string unfAlg, TH1D *hdata, TH1D* hBack0[], RooUnfoldResponse *response, int kterm, TH2D *hCovariance, TH2D *hCorrelation, int nPseudos, int selection, bool useOverFlow, int NBkgGroups)
{


    if (useOverFlow) response->UseOverflow();
    double lumiSyst = 0.026;
    ostringstream fileBeingProcessed; fileBeingProcessed << __FILE__;
    if (fileBeingProcessed.str().find("Analysis2011") != string::npos) lumiSyst = 0.022;

    // this lines are to incorporate correlaction between 0-jet bin and n-jet bins for JES, JER
    // this is not needed for global efficiencies like lepton, trigger, 
    // if we remove 0 - jet bin from unfoldong no need fot this three lines
    string hTitle = hdata->GetTitle();
    bool isZN( hTitle.find("Counter") != std::string::npos);
    //bool isZN( selection < 100 && hTitle.find("Counter") != std::string::npos);
    //if (selection > 100) selection -= 100;


    TH1D* hBack = (TH1D*) hBack0[0];
    //const int NBkgGroups(6);
    //Random for pseudos
    TRandom3* random = new TRandom3();
    random->SetSeed(0);
    //random->SetSeed(t.GetTime());

    //Options for doing Pseudos
    bool fluctPData = false;
    bool fluctRMat = false;
    bool fluctBkg = false;
    bool usePoisson = false;
    // this is for statistics on data, background and MC response
    if (selection == 1 ) { fluctPData = true;  usePoisson = true; }
    if (selection == 101 ) { fluctRMat = true; fluctBkg = true; usePoisson = true; }
    // PU effect 
    // assume independent gauss
    if (selection == 2 ) { fluctRMat = true; fluctBkg = true; }
    // JER effect on response matrix
    if (selection == 4 ) fluctRMat = true;
    // dependendt gauss for different bins
    if (selection == 12 ) { fluctPData = true; fluctBkg = true; }
    // gauss errors on JES
    if (selection == 3 ) fluctPData = true;  // bins are independent
    if (selection == 10) fluctPData = true;  // bins are NOT independent  
    if (selection == 20) fluctPData = true;  // bins are NOT independent  , this is needed for combined unfolding where we produce two different gaussian for muon and electrons
    if (selection == 20 || selection == 21) isZN = false ; // dont anticorrelate 0 jet  bin in case of error of the efficiecny scale factors 
    // gauss errors on JES XSEC
    if (selection == 5)  fluctBkg = true; 
    if (selection == 11) fluctBkg = true;


    TH1D *h_bkgsub;
    TH2D *h_response_temp;

    TH2D *hresponse = (TH2D*) response->Hresponse();
    TH1D *hTrue     = (TH1D*) response->Htruth();
    TH1D *hMeas     = (TH1D*) response->Hmeasured();
    TH1D *h_bkg     = (TH1D*) hBack->Clone();
    TH1D *hTrue1    = (TH1D*) hTrue->Clone();
    TH1D *h_meas    = (TH1D*) hdata->Clone();
    TH1D *hMeas1    = (TH1D*) hMeas->Clone();

    // output histogram
    const int NBins(hTrue1->GetNbinsX() + 1 );
    double mean[NBins], allPseudo[NBins][nPseudos];
    TH1D* hOut = (TH1D*) hTrue->Clone("MCpseudo");

    int weRunOnNPS = 0;
    for(int i(0); i < nPseudos; i++){

        h_meas = (TH1D*) hdata->Clone("Data");
        h_response_temp = (TH2D*) hresponse->Clone("Data");
        weRunOnNPS = i ;

        if(nPseudos > 1) {
            double dataIntegral =  h_meas->Integral() ;
            double countNJets(0.);

            //Poisson fluctuate the prediction if we're doing pseudos
            if ( selection == 1 || selection == 3 ) {
                for (int j(0); j <= h_meas->GetNbinsX() + 1 ; j++) {
                    double fluct(0.);
                    double BinCount( hdata->GetBinContent(j) );
                    double BinError( sqrt(BinCount) );
                    if ( hdata->GetBinError(j) > 0 ) BinError = hdata->GetBinError(j);
                    double ratio = 1. ;
                    if ( BinCount > 0 ){
                        ratio = (BinError * BinError )/BinCount ;
                        if ( usePoisson ) fluct = random->Poisson(BinCount/ratio);
                        if (!usePoisson ) {
                            ratio = 1.;
                            fluct = BinCount + random->Gaus(0,BinError);
                            if ( isZN && j > 1 ) countNJets += fluct;
                        }
                    }
                    if (fluct < 0) fluct = 0.;
                    //h_meas->SetBinError(j,sqrt(fluct));  ///  ????
                    h_meas->SetBinError(j, BinError);
                    h_meas->SetBinContent(j, fluct * ratio );
                }
                if (!usePoisson && isZN ) {
                    // total number fo Z bosons is fixed and indepedednt of JES... therefore we fix it and the 0 jet bin is set as Z_all - Z_(at least 1 jet)
                    h_meas->SetBinContent(1, dataIntegral - countNJets);
                    h_meas->SetBinError(1, hdata->GetBinError(1) );
                }
            }
            // fluctuate data with fixed bin correlation: like JES --> all bins increase or decrease ( watch out Z+0 jets bin) ?????
            if (( selection == 10 || selection == 20 || selection == 21 )&& fluctPData == true) {
                // not sure if I want to have always the same gaussian
                double fluct1sig=random->Gaus(0,1);
                double fluct1dir=fluct1sig/fabs(fluct1sig);
                // change in JES for jets influences all the jets in the same direction (either positive or negative) but with different offset ???
                //double fluct1dir(1.);

                for (int j(0); j <= h_meas->GetNbinsX() + 1; j++) {

                    // line below is need to propagate lepton efficiency uncertainty in the combined unfolding
                    if (  selection == 20 && int(h_meas->GetNbinsX() / 2 ) == j ) { 
                        fluct1sig=random->Gaus(0,1); fluct1dir=fluct1sig/fabs(fluct1sig);
                    }


                    double fluct(0);
                    //double fluct1sig( random->Gaus(0, 1) );
                    if (j == 2 && isZN  ) fluct1dir *= -1; /// JES:  n-jet bins always follow the same trend while 0 jet is opposite double fluct(0);
                    double BinCount( hdata->GetBinContent(j) );
                    double BinError( hdata->GetBinError(j) );
                    //if (BinCount > 0 && !usePoisson )  fluct= BinCount  + hDirectionDataJES->GetBinContent(j)* fluct1dir * fabs(fluct1sig) * BinError ;
                    if (BinCount > 0 && !usePoisson )  fluct = BinCount + fluct1dir * fabs(fluct1sig) * BinError ;
                    if ( isZN && j > 1 ) countNJets += fluct;
                    // do I need to set this bin error ???
                    h_meas->SetBinError(j, sqrt(fluct));
                    h_meas->SetBinContent(j, fluct);
                }
                if (!usePoisson && isZN ) {
                    // total number fo Z bosons is fixed and indepedednt of JES... therefore we fix it and the 0 jet bin is set as Z_all - Z_(at least 1 jet)
                    h_meas->SetBinContent(1, dataIntegral - countNJets);
                    h_meas->SetBinError(1, hdata->GetBinError(1) );

                }

            }


            /// NOW FLUCTUATIONS OF RESPONSE MATRIX                             
            if ( ( selection == 1 || selection == 2 || selection == 4 || selection == 101 ) && fluctRMat == true){
                //Poisson fluctuate the R Matrix
                for (int j(1); j <= h_meas->GetNbinsX() + 1 ; j++){
                    for (int k(1); k <= hTrue1->GetNbinsX() + 1 ; k++){
                        double fluctR(0.);
                        double BinCount( hresponse->GetBinContent(j, k) );
                        double BinError( hresponse->GetBinError(j, k) );
                        //since I have weights 1 set when filling response matrix I should use Poisson for statistical fluctuations
                        if (BinCount > 0 && usePoisson ) fluctR = random->Poisson(BinCount);
                        if (BinCount > 0 && !usePoisson ) fluctR = BinCount + random->Gaus(0,BinError);
                        if (fluctR < 0) fluctR = 0;
                        //if (BinCount > 0 ) fluctR = random->Poisson(BinCount);
                        h_response_temp->SetBinContent(j, k, fluctR);
                        //h_response_temp->SetBinError(j, k, sqrt(fluctR));
                        h_response_temp->SetBinError(j, k, BinError);
                    }
                }
            }

            if (fluctRMat == true && selection == 12) {
                //gauss fluctuate the R Matrix when setting the PU effect
                double fluct1sig( random->Gaus(0, 1) );	
                for (int j(1); j <= h_meas->GetNbinsX() + 1 ; j++) {
                    for (int k(1); k <= hTrue1->GetNbinsX() + 1 ; k++) {
                        double fluctR(0.);
                        double BinCount( hresponse->GetBinContent(j, k) );
                        double BinError( hresponse->GetBinError(j, k) );
                        //if (BinCount > 0 && usePoisson ) fluctR = random->Poisson(BinCount);
                        //fluctR = BinCount + hDirectionResPU->GetBinContent(j,k) * BinError * fluct1sig ;
                        fluctR = BinCount + BinError * fluct1sig ;
                        if (fluctR < 0) fluctR = 0;
                        if (BinCount > 0) fluctR = random->Poisson(BinCount);
                        h_response_temp->SetBinContent(j, k, fluctR);
                        h_response_temp->SetBinError(j, k, sqrt(fluctR));
                    }
                }
            }

            //bin independent Gauss and Poisson fluctuate the backgrounds
            if ( ( selection == 1 || selection == 2 || selection == 5 || selection == 101 ) && fluctBkg == true) {
                for (int j(1); j <= h_bkg->GetNbinsX() + 1 ; j++) {
                    double fluctB(0), fluctE(0);
                    double BinCount( hBack->GetBinContent(j) );	
                    double BinError( hBack->GetBinError(j) );
                    double alpha( BinError * BinError / BinCount );
                    double BinCount0 = BinCount / alpha;
                    if ( BinCount < BinCount0 ) { alpha = 1. ; BinCount0 = BinCount ;} /// take smaller of the two numbers to also propagate the statistical error on the estimation of the events from MC ?
                    if (BinCount > 0 && usePoisson ) {
                        fluctB = random->Poisson(BinCount0); 
                        fluctB *= alpha;
                        fluctE = alpha * sqrt(fluctB);
                    }   
                    if (BinCount > 0 && !usePoisson ) {
                        fluctB = BinCount + random -> Gaus(0, BinError);
                        if ( fluctB > 0 ) fluctE = sqrt(fluctB);
                    }
                    if (fluctB < 0 ) fluctB = 0.;
                    h_bkg->SetBinError(j, fluctE); // this is ify
                    h_bkg->SetBinContent(j, fluctB); 
                }
            }

            //gauss fluctuate the backgrounds due to LUMINOSITY or XSec
            // change in XSEC for back influences all the events in the same direction (either positive or negative) but with different offset ???
            if ( ( selection == 12 || selection == 11)  && fluctBkg == true) {
                double fluctB[50] = {0};
                //double fluctB[50] = {[0 ... 49] = 0};
                //double fluctLUMIsig( 1 + lumiSyst * random->Gaus(0, 1) );   // fluctuate LUMINOSITY -- check uncertainty
                for (int k(0); k < NBkgGroups ; k++) { // we have 6 bkg groups: ZZ,WZ,top,TTbar,WJets(orDYJets) and WW
                    double fluct1sig( random->Gaus(0, 1) );   // fluctuate background cross section for each source
                    hBack = (TH1D*) hBack0[k]->Clone();
                    for (int j(1); j <= h_bkg->GetNbinsX() + 1 ; j++) {
                        double fluctBtemp(0);
                        double BinCount( hBack->GetBinContent(j) );
                        double BinError( hBack->GetBinError(j) );
                        // LAZY TO SET THIS RIGHT
                        //fluctB = BinCount + hDirectionBkgPU->GetBinContent(j) *fluct1sig * BinError;
                        fluctBtemp = BinCount + fluct1sig * BinError;
                        //if (fluctBtemp > 0) fluctB[j] += fluctLUMIsig * fluctBtemp;  // NOT SURE I WANT TO ADD LUMI ERROR TO THIS SINCE IT'S GOING TO BE ADDED TO THE FINAL RESULT. DOUBLE COUNTING ?
                        if (fluctBtemp > 0) fluctB[j] += fluctBtemp ;
                        //else fluctB[j] = 0. ;
                    }
                }
                for (int j(1); j <= h_bkg->GetNbinsX() + 1 ; j++) {
                    h_bkg->SetBinError(j, sqrt(fluctB[j])); // this is ify
                    h_bkg->SetBinContent(j, fluctB[j]);
                }
            }

        }//end if nPE >1

        //Correct for backgrounds 
        h_bkgsub = (TH1D*) h_meas->Clone();
        h_bkgsub->Add(h_bkg, -1);

        RooUnfoldResponse response_temp(hMeas1, hTrue1, h_response_temp);
        response_temp.UseOverflow();
        RooUnfold* RObject;
        if (unfAlg == "SVD")   RObject = (RooUnfold*) RooUnfold::New( RooUnfold::kSVD,   &response_temp, h_bkgsub, kterm);
        if (unfAlg == "Bayes") RObject = (RooUnfold*) RooUnfold::New( RooUnfold::kBayes, &response_temp, h_bkgsub, kterm);
        RObject->SetVerbose(0);
        TH1D* h_unfold = (TH1D*) RObject->Hreco();


        for (int j(0); j < NBins; j++){
            double a( h_unfold->GetBinContent(j + 1) );
            if (i == 0) mean[j] = a;
            else mean[j] += a;
            allPseudo[j][i] = a;	
        }
    }// end loop over pseudos

    // fill output histogram
    double VecRMS[NBins];
    for (int i(0); i < NBins; i++){
        double sumRMS(0);
        double meanBin( mean[i] / nPseudos);
        for (int j(0); j < nPseudos; j++) sumRMS += pow(meanBin - allPseudo[i][j], 2);
        double RMS( sqrt(sumRMS / (nPseudos - 1) ));
        hOut->SetBinContent(i+1, meanBin);
        hOut->SetBinError(i+1, RMS);
        VecRMS[i] = RMS;
    }

    // now calculate the correlation matrix
    for (int i(0); i < NBins; i++){
        double mi( mean[i] / nPseudos );
        for (int k(0); k < NBins; k++) {			
            double mk(mean[k] / nPseudos);
            double sum(0.);
            for (int j(0); j < nPseudos; j++){
                sum += (mi - allPseudo[i][j]) * (mk - allPseudo[k][j]) ;
            }   
            // set bin content
            if (VecRMS[i] > 0 && VecRMS[k] > 0) hCovariance->SetBinContent(i+1, k+1, sum / (nPseudos-1));
            // line below is for the correlation matrix
            if (VecRMS[i] > 0 && VecRMS[k] > 0) hCorrelation->SetBinContent(i+1, k+1, sum / ((nPseudos-1) * VecRMS[i] * VecRMS[k]) );
        }
    }
    //hOut->Scale(1/Luminosity);
    //hOut->Scale(1/Luminosity);
    return hOut;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

