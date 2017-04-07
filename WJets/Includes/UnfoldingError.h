#ifndef __UnfoldingSyst__ 
#define __UnfoldingSyst__
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void createZNGoodJets_Zinc2( TH1D* hCombinedStat, TH2D *hError2D[] , string unfAlg , bool doVarWidth , bool doXSec , bool doNormalize ){
  string energy = getEnergy();
  string variable1 = "ZNGoodJets_Zinc";
  int JetPtMin = 30 ;
  int JetPtMax = 0 ;
  string leptonFlavor = "DMu";
  //if (!isMuon) leptonFlavor = "DE";
  bool doFlat = 0 ;

  //-- fetch the data files and histograms --------------
  TFile *fData[3];             // 0 = central, 1 = JES Up, 2 = JES Down 
  getFiles(FILESDIRECTORY, fData, leptonFlavor, energy, ProcessInfo[DATAFILENAME].filename, JetPtMin, JetPtMax, doFlat, doVarWidth);
  cout << " got data " << endl;
  TH1D *hData[3];
  getHistos(hData, fData, variable1);
  cout << " got data " << endl;
  //-----------------------------------------------------


  //-- fetch the DY files and histograms ----------------
  TFile *fDYMadGraph[4];
  TFile *fDYSherpa = NULL;               // 0 = central, 1 = PU Up,  2 = PU Down,  3 = JER Up
  TFile *fDYPowheg = NULL ;
  getFiles(FILESDIRECTORY, fDYMadGraph, leptonFlavor, energy, ProcessInfo[DYMADGRAPHFILENAME].filename, JetPtMin, JetPtMin, doFlat, doVarWidth);
  fDYSherpa = getFile(FILESDIRECTORY, leptonFlavor, energy, DYSHERPAFILENAME, JetPtMin, JetPtMin, doFlat, doVarWidth);
  fDYPowheg = getFile(FILESDIRECTORY, leptonFlavor, energy, DYPOWHEGFILENAME, JetPtMin, JetPtMin, doFlat, doVarWidth);
  TH1D *hDY[4];
  TH1D *hDYGenMadGraph = NULL, *hDYGenSherpa = NULL,  *hDYGenPowheg = NULL;
  getHistos(hDY, fDYMadGraph, variable1);
  hDYGenMadGraph = getHisto(fDYMadGraph[0], "gen" + variable1);
  hDYGenSherpa = getHisto(fDYSherpa, "gen" + variable1);
  hDYGenPowheg = getHisto(fDYPowheg, "gen" + variable1);
  //-----------------------------------------------------

  TH1D* hOut = (TH1D*) hData[0]->Clone();

  // now sum merged histogram
  // then cum errors histograms
  const int nBins(hCombinedStat->GetNbinsX());
  int nSyst = 8 ;
  for (int bin(1); bin <= nBins; bin++){
    double sum = hCombinedStat->GetBinContent(bin);
    double sumErr[nSyst]  ;

    for (int syst(0); syst < nSyst; syst++) sumErr[syst] = hError2D[syst]->GetBinContent(bin , bin) ;


    for (int j(bin + 1 ); j <= nBins; j++){
      sum += hCombinedStat->GetBinContent(j);
      for (int syst(0); syst < nSyst; syst++) sumErr[syst] += hError2D[syst]->GetBinContent(j,j) ;
    }
    hOut->SetBinContent(bin, sum );
    for (int syst(0); syst < nSyst; syst++) hError2D[syst]->SetBinContent(bin , bin , sumErr[syst]) ;
    //		for (int syst(0); syst < nSyst; syst++) sumErr[syst] = hError2D[syst]->SetBinContent(bin , bin , 1. ) ;
    cout << variable1 << "   " << hCombinedStat->GetBinContent(bin) << "    " << sum << endl;
  }



  // and finally produce tables and plots
  /*
  // lets create som tables
  ostringstream optionCorrStr; optionCorrStr << optionCorr ;
  string fileNameTable = "PNGFiles/NiceUnfold/TableSystematics_" + energy +   "_" + variable + "_" + unfAlg  ;
  fileNameTable += "_CorrelationOption_" + optionCorrStr.str() ;
  if (doVarWidth) fileNameTable += "_VarWidth";
  fileNameTable +=  ".tex";
  cout << fileNameTable << endl;
  createTexTable( variable, fileNameTable ,  (TH1D*) h_combine->Clone() , allErrorsTH2  , doXSec , doNormalize , Luminosity);
  createTexTableCombOpt(variable,  (TH1D*) h_combine->Clone() , doXSec);
  // end table creation


  // create combination histogram with statistical errors
  TH1D* h_combine_stat =  (TH1D *) SetHistWithErrors(  (TH1D*)  h_combine->Clone() , combined_error_stat, "Stat");
  //testMat(  (TH1D*) h_combine->Clone()  , combined_error_stat);
  cout << " used correlation parameters :  " << correlationSameBin << "     "  << correlationDiffBin << endl;

  /// PLOT COMPARISON OF ELECTRONS AND MUONS TO COMBINED
  plotLepRatioComb(variable, (TH1D*) h_combine->Clone() , (TH1D*) dataCentral[0]->Clone(),(TH1D*) dataCentral[1]->Clone() );

  /// PLOT FINAL PLOTS: COMBINATION VS MC
  plotCombination(variable, (TH1D*) h_combine_stat->Clone() , (TH1D*) h_combine->Clone() , genMad , genShe , genPow );

   */
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void changeToLatexFormat(string& title)
{
  cout << "Start formating:  " << title << endl;
  myReplace(title, "#", "\\");
  //cout << "Remove #:         " << title << endl;
  vector<string> sentence;
  //cout << "Pieces : " << endl;
  while (title.find(" ") != string::npos){
    string tmp = title.substr(0, title.find(" "));
    //cout << tmp << endl;
    sentence.push_back(tmp);
    string space = " ";
    string tmp2 = title.substr(title.find(tmp) + tmp.size() + space.size(), title.size());
    title = tmp2;
  }
  //cout << title << endl;
  sentence.push_back(title);

  title = "";
  //cout << endl;
  for (int i(0); i < int(sentence.size()); i++){
    //cout << sentence[i] << endl;
    if (sentence[i].find("_") != string::npos || sentence[i].find("\\") != string::npos){
      sentence[i] = "$"+sentence[i]+"$";
    }
    //cout << sentence[i] << endl;
    title += sentence[i] + " ";
  }
  myReplace(title, "_{jets}", "_{\\text{jets}}");
  cout << " New Title:      " << title << endl;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void createTexTable(string variable, string fileNameTable, TH1D* data, TH2D *hError2D[] , bool doXSec , bool doNormalize , double Luminosity  ){
  cout << " my merged table is written : " << fileNameTable << endl;
  string temp, tempTab;
  if (doXSec ) {
    string xtitle = data->GetXaxis()->GetTitle();
    string shortVar = xtitle.substr(0, xtitle.find(" "));
    string unit = "";
    if (xtitle.find("[") != string::npos){
      size_t begin = xtitle.find("[") + 1;
      unit = xtitle.substr(begin);
      unit = unit.substr(0, unit.find("]"));
    }
    cout << shortVar << "   unit: " << unit << endl;
    temp = "d#sigma/d" + shortVar;
    tempTab = "$\\frac{d\\sigma}{d" + shortVar + "}$";
    if (doNormalize) temp = "1/#sigma " + temp;
    else if (doXSec){
      temp += "  [pb";
      tempTab += " \\tiny{$\\left[";
      if (unit != "" ){
        temp += "/" + unit;
        tempTab += "\\frac{ \\text{pb}}{" + unit + "}";
      }
      else tempTab += " \\text{pb}";
      temp += "]";
      tempTab += "\\right]$}";
    }
  }

  string title = data->GetTitle(); changeToLatexFormat(title);
  string xtitle = data->GetXaxis()->GetTitle(); changeToLatexFormat(xtitle);
  string sigmaTitle = temp; changeToLatexFormat(sigmaTitle);
  string sigmaTitleTab = tempTab;

  myReplace(title, "1st", "$1^{\\text{st}}$");
  myReplace(title, "2nd", "$2^{\\text{nd}}$");
  myReplace(title, "3rd", "$3^{\\text{rd}}$");
  myReplace(title, "4th", "$4^{\\text{th}}$");
  myReplace(title, "5th", "$5^{\\text{th}}$");
  myReplace(title, "6th", "$6^{\\text{th}}$");
  myReplace(title, "p_T", "\\pt");
  myReplace(title, "p_{T}", "\\pt");
  myReplace(title, "H_T", "\\HT");
  myReplace(title, "H_{T}", "\\HT");
  myReplace(title, "GeV", "\\GeV");
  myReplace(xtitle, "p_T", "\\pt");
  myReplace(xtitle, "p_{T}", "\\pt");
  myReplace(xtitle, "H_T", "\\HT");
  myReplace(xtitle, "H_{T}", "\\HT");
  myReplace(xtitle, "GeV", "\\GeV");
  myReplace(xtitle, "[", "\\tiny{[");
  myReplace(xtitle, "]", "]}");
  myReplace(sigmaTitleTab, "GeV", "\\GeV");
  myReplace(sigmaTitleTab, "p_T", "\\pt");
  myReplace(sigmaTitleTab, "p_{T}", "\\pt");
  myReplace(sigmaTitleTab, "H_T", "\\HT");
  myReplace(sigmaTitleTab, "H_{T}", "\\HT");



  ofstream myFile(fileNameTable.c_str());
  myFile << "\\begin{table}[htb!]\\begin{center}\n";
  myFile << "\\caption{ Differential cross section in " << title << " and break down of the systematic uncertainites for the combinaton of both decay channels." ; 
  myFile << "}"<<endl;
  myFile << "\\scriptsize{" << endl;
  myFile << "\\begin{tabular}{c|cc|cccccccc}\n \\multicolumn{11}{c}{";
  myFile << title << "} \\\\" << endl;
  myFile << xtitle << " & ";
  myFile << sigmaTitleTab << " & Err.Tot.  & stat & JES & PU & XSEC & JER & Lumi & Unf & Eff  \\\\ \\hline" << endl;

  int nSyst =  9 ;
  const int nBins(data->GetNbinsX());

  for (int bin(1); bin <= nBins; bin++){
    bool doPrint(1);
    if (variable.find("ZNGoodJet") != string::npos && bin == 1) doPrint = 0;

    double binW = data->GetBinWidth(bin);
    double Norm = 1.;
    if (doXSec)  Norm = binW * Luminosity ;


    double centralValue(data->GetBinContent(bin)/Norm );
    double totalStatistics( sqrt (hError2D[0]->GetBinContent(bin,bin)) / Norm );
    //cout << centralValue << "   "  << totalStatistics << endl;
    //cout << centralValue << "   "  << totalStatistics << "   " << binW << " "  << Luminosity << endl;

    if (variable.find("ZNGoodJets_Zexc") != string::npos && bin > 1) myFile <<  "$= " << bin - 1 << " $& ";
    else if (variable.find("ZNGoodJets_Zinc") != string::npos && bin > 1) myFile <<  "$\\geq " << bin - 1 << " $& ";
    else if (variable.find("ZNGoodJets") == string::npos) {
      if ( fabs(data->GetBinLowEdge(bin) + binW) < 0.01) myFile << data->GetBinLowEdge(bin) << "\\ -\\ " <<  0  << " & ";
      else myFile << data->GetBinLowEdge(bin) << "\\ -\\ " <<  data->GetBinLowEdge(bin) + binW  << " & ";


    }
    myFile.precision(3);
    if (variable.find("ZNGoodJets") != string::npos && bin > 1){
      myFile << centralValue ;
      myFile.precision(2);
    }
    else if (variable.find("ZNGoodJets") == string::npos ){
      myFile << centralValue  ;
      myFile.precision(3);
    }


    cout << "PRINTING IN TEX" << endl;
    for (int syst(0); syst < nSyst; syst++){
      cout << sqrt (hError2D[syst]->GetBinContent(bin,bin)) << "   " ;
      if (variable.find("ZNGoodJets") != string::npos && bin > 1) myFile <<  " & " << sqrt (hError2D[syst]->GetBinContent(bin,bin)) / Norm  << "  ";
      else if (variable.find("ZNGoodJets") == string::npos) myFile <<  " & " << sqrt (hError2D[syst]->GetBinContent(bin,bin)) / Norm << "  ";

    }
    cout << endl;

    if (doPrint)  myFile << " \\\\" << endl;

  }




  myFile << "\\end{tabular}}" << endl;
  myFile << "\\label{tab:" << variable << "}"<< endl;
  myFile << "\\end{center}\\end{table}\n";
  myFile.close();


}

/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ErrorMatrixToVec(int row, double vec[][10], TMatrixD &MatInp)
{
const int nrows  =  MatInp.GetNrows();
for(int i = 0; i < nrows  ; i++){
vec[row][i] = MatInp(i,i);
cout << vec[row][i] <<  endl;
}


}
double*  ErrorMatrixNew(int row,  TMatrixD &MatInp)
{
const int nrows  =  MatInp.GetNrows();
double aa[nrows];
for(int i = 0; i < nrows  ; i++){
aa[i] = MatInp(i,i);
cout << aa[i] <<  endl;
}

return aa;
}
 */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TH1D* SetHistWithErrors( TH1D* hInput, TMatrixD &MatInp, string name ){
  TH1D* hOut = (TH1D *)  hInput ->Clone(name.c_str());
  for (int i(1); i <= hInput->GetNbinsX(); i++){
    hOut ->SetBinContent( i, hInput->GetBinContent(i));
    hOut ->SetBinError( i, sqrt(MatInp(i-1,i-1)));

    //	cout << i << "   "  << hInput->GetBinContent(i)  << "     "   <<  sqrt(MatInp(i - 1 , i-1 )) << "     "  <<  hInput->GetBinError(i)   << endl;
  }

  return hOut;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TH2D* CovToCorr(TH2D *h)
{
  TH2D* hCorr = (TH2D *) h ->Clone();
  int xbin(h->GetNbinsX()), ybin(h->GetNbinsY());
  for (int i(1); i <= ybin; i++){
    for (int j(1); j <= xbin; j++){
      double temp = 0.;
      temp = h->GetBinContent(j,i) / sqrt( h->GetBinContent(i,i ) * h->GetBinContent(j,j) );
      hCorr->SetBinContent(j, i, temp );
    }
  }
  return hCorr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TH2D* setCovariance(TH2D *h , TH1D* hCent , double error)
{
  TH2D* hCorr = (TH2D *) h ->Clone();
  int xbin(h->GetNbinsX()), ybin(h->GetNbinsY());
  for (int i(1); i <= ybin; i++){
    for (int j(1); j <= xbin; j++){
      double temp = 0.;
      temp = h->GetBinContent(j,i) / sqrt( h->GetBinContent(i,i ) * h->GetBinContent(j,j) );
      hCorr->SetBinContent(j, i, temp * hCent->GetBinContent(i) * error * hCent->GetBinContent(j) * error );
      //cout << hCorr->GetBinContent(j,i) << "    " ;
    }
    //cout << endl;
  }
  return hCorr;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double getErrors(string variable, bool isMuonn, string unfAlgg, bool doVarWidthh)
{
  string energy = getEnergy();
  string leptonFlavor = "DMu";
  if (!isMuonn) leptonFlavor = "DE";
  gStyle->SetOptStat(0);
  string filesIN[4] = {"ZNGoodJets_Zexc","JetsHT","JetPt","JetEta"};
  string addNames[5] = {"First","Second","Third","Fourth","Fifth"};
/*
  //-- get number of jets --
  unsigned pos = variable.find("jet");         // position of "jet" in str
  string str3;
  if (variable.find("ZNGoodJets") != string::npos) str3 = "1";
  else str3 = variable.substr(pos-1,1);
  int num; istringstream(str3) >> num;
  stringstream ss; ss << num;
  cout << " which multiplicity : " << num << endl;
  //-----------------------

  double values[4] ;
  double min = 100000000.;
  double max = 0.;
  double mean = 0.;
  for (int i(0); i <= 3; i++){
    string variableT;
    if (i == 0) variableT = filesIN[i];
    else if (i == 1) variableT = filesIN[i] + "_Zinc" + ss.str() + "jet";
    else variableT = addNames[num - 1] + filesIN[i] + "_Zinc" + ss.str() + "jet";
    string fileName = "PNGFiles/FinalUnfold_30_test/" + leptonFlavor + "_" + energy + "_unfolded_" + variableT + "_histograms_" + unfAlgg ;
    if (doVarWidthh) fileName += "_VarWidth";
    fileName += ".root";
    cout << "opening : " << fileName <<endl;
    TFile *f  = new TFile(fileName.c_str());
    TH1D *dataCentral = (TH1D*) f->Get("Central");
    if ( i == 0 ) values[i] = dataCentral ->Integral(num+1,dataCentral->GetNbinsX()+1)  ;
    else values[i] = dataCentral ->Integral(1,dataCentral->GetNbinsX()+1)   ;
    cout << " grabbing central unfolds for errors  : "  <<  values[i] << endl;
    if (values[i] < min )  min = values[i] ;
    if (values[i] > max )  max = values[i] ;
    mean +=  values[i] ;
    f->Close();
  }

  cout << " Envelope error " << (max-min)/2 << " min : " << min << " max " << max << endl;
  return ((max-min)/2)/(mean/4);
*/

  return 0.;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TMatrixD setCovMatrixOfCombination(double luminosityErr, TH1D* hEle, TH1D* hMu,  int optionCorrTemp , double norm = 1.){
  double correlationSameBin  =  0.;
  double correlationDiffBin  =  0.;
  if ( optionCorrTemp  ==  1 ) correlationSameBin  =  0.;
  if ( optionCorrTemp  ==  2 ) correlationSameBin  =  1.;
  if ( optionCorrTemp  ==  3 ) correlationSameBin  =  1.;
  if ( optionCorrTemp  ==  4 ) {
    correlationSameBin  =  1.;
    correlationDiffBin  =  1.;
  }

  // declare the big matrix
  int nbins = hEle->GetNbinsX();
  const int NELE = 2*nbins;
  TMatrixD errorMTemp(NELE,NELE);

  for(int ibin = 0; ibin<nbins; ibin++){
    // electron channel
    double value_e  =  hEle->GetBinContent(ibin+1) ;
    // muon channel
    double value_m  =  hMu->GetBinContent(ibin+1) ;
    // first put electron error matrix component
    errorMTemp(ibin,ibin) = pow((luminosityErr * value_e),2);
    errorMTemp(nbins+ibin,nbins+ibin) = pow((luminosityErr * value_m),2);
    if ( optionCorrTemp > 0 ) {
      errorMTemp(ibin,nbins+ibin) = (luminosityErr * value_e * luminosityErr * value_m );
      errorMTemp(nbins + ibin,ibin) = (luminosityErr * value_e * luminosityErr * value_m );
    }
    for(int jbin = 0; jbin<nbins; jbin++){
      if(optionCorrTemp > 1 ){
        if(jbin !=  ibin ){
          // electron
          errorMTemp(ibin,jbin) = (luminosityErr * value_e * luminosityErr * hEle->GetBinContent(jbin+1)) ;	
          // muon
          errorMTemp(nbins+ibin,nbins+jbin) = (luminosityErr * value_m * luminosityErr * hMu->GetBinContent(jbin+1) );
        }
        if ( optionCorrTemp > 1 ) {
          // electron-muon channel
          errorMTemp(ibin,nbins+jbin) = (luminosityErr * value_e * luminosityErr * hMu->GetBinContent(jbin+1) );
          // muon-electron channel
          errorMTemp(nbins + ibin , jbin)   = (luminosityErr * hEle->GetBinContent(jbin+1) * luminosityErr * value_m );
        }
      }
    }

  }

  return errorMTemp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TMatrixD setCovMatrixOfCombinationLep(double muonIDIsoHLTError, double electronIDIsoHLTError,  TH1D* hEle, TH1D* hMu,  int optionCorrTemp , double norm = 1.){
  double correlationSameBin  =  0.;
  double correlationDiffBin  =  0.;
  /*
     if ( optionCorrTemp  ==  1 ) correlationSameBin  =  0.;
     if ( optionCorrTemp  ==  2 ) correlationSameBin  =  1.;
     if ( optionCorrTemp  ==  3 ) correlationSameBin  =  1.;
     if ( optionCorrTemp  ==  4 ) {
     correlationSameBin  =  1.;
     correlationDiffBin  =  1.;
     }
     if ( optionCorrTemp  > 0 ) {
     correlationSameBin  =  1.;
     correlationDiffBin  =  1.;
     }
   */
  // declare the big matrix
  int nbins = hEle->GetNbinsX();
  const int NELE = 2*nbins;
  TMatrixD errorMTemp(NELE,NELE);

  for(int ibin = 0; ibin<nbins; ibin++){
    // electron channel
    double value_e  =  hEle->GetBinContent(ibin+1) ;
    // muon channel
    double value_m  =  hMu->GetBinContent(ibin+1) ;
    // first put electron error matrix component
    errorMTemp(ibin,ibin) = pow((electronIDIsoHLTError * value_e),2);
    errorMTemp(nbins+ibin,nbins+ibin) = pow((muonIDIsoHLTError * value_m),2);
    if ( optionCorrTemp > 1 ) {
      errorMTemp(ibin,nbins+ibin) = correlationSameBin * (electronIDIsoHLTError * value_e * muonIDIsoHLTError * value_m );
      errorMTemp(nbins + ibin,ibin) = correlationSameBin *  (electronIDIsoHLTError * value_e * muonIDIsoHLTError * value_m );
    }
    for(int jbin = 0; jbin<nbins; jbin++){

      if(jbin !=  ibin  && optionCorrTemp > 0 ){
        // electron
        errorMTemp(ibin,jbin) =  (electronIDIsoHLTError * value_e * electronIDIsoHLTError * hEle->GetBinContent(jbin+1)) ;
        // muon
        errorMTemp(nbins+ibin,nbins+jbin) = (muonIDIsoHLTError * value_m * muonIDIsoHLTError * hMu->GetBinContent(jbin+1) );
        if ( optionCorrTemp > 1 ) {
          // electron-muon channel
          errorMTemp(ibin,nbins+jbin) = correlationDiffBin * (electronIDIsoHLTError * value_e * muonIDIsoHLTError * hMu->GetBinContent(jbin+1) );
          // muon-electron channel
          errorMTemp(nbins + ibin , jbin)   = correlationDiffBin * (electronIDIsoHLTError * hEle->GetBinContent(jbin+1) * muonIDIsoHLTError * value_m );
        }
      }
    }

  }

  return errorMTemp;
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TMatrixD getCovMatrixOfCombination(TH2D* CovEle,  TH2D* CovMuon, int optionCorrTemp , double norm = 1.){
  // get correlation matrix this is needed for option 3 of combination
  TH2D* hCorrEle = (TH2D*) CovToCorr(CovEle);
  TH2D* hCorrMu = (TH2D*)  CovToCorr(CovMuon);

  double correlationSameBin  =  0.;
  double correlationDiffBin  =  0.;
  if ( optionCorrTemp  ==  1 ) correlationSameBin  =  0.;
  if ( optionCorrTemp  ==  2 ) correlationSameBin  =  1.;
  if ( optionCorrTemp  ==  3 ) correlationSameBin  =  1.;
  if ( optionCorrTemp  >=  4 ) {
    correlationSameBin  =  1.;
    correlationDiffBin  =  1.;
  }


  // declare the big matrix
  //int nbins =  CovEle->GetNbinsX();
  int nbins =  8 ;
  const int NELE = 2*nbins;
  TMatrixD errorMTemp(NELE,NELE);

  for(int ibin = 0; ibin<nbins; ibin++){

    // first put electron error matrix component
    //errorMTemp(ibin,ibin)  =  total_e_2; 
    errorMTemp(ibin,ibin)  =  CovEle->GetBinContent(ibin+1,ibin+1);
    // then mu
    errorMTemp(nbins+ibin,nbins+ibin)  =  CovMuon->GetBinContent(ibin+1,ibin+1);
    // correlation between same bins in muon and electron??????????
    //correlationSameBin  =  1.;
    if ( optionCorrTemp > 1 ) {
      errorMTemp(ibin,nbins+ibin)  =  correlationSameBin * sqrt(CovEle->GetBinContent(ibin+1,ibin+1) * CovMuon->GetBinContent(ibin+1,ibin+1));

      errorMTemp(ibin+nbins,ibin)  =  errorMTemp(ibin,nbins+ibin) ;
    }
    for(int jbin = 0; jbin<nbins; jbin++){
      if(optionCorrTemp > 0 ){

        // electron channel
        errorMTemp(ibin,jbin) = CovEle-> GetBinContent(ibin+1,jbin+1);
        if ( optionCorrTemp == 5 )  errorMTemp(ibin,jbin) = sqrt(  CovEle-> GetBinContent(ibin+1,ibin + 1 ) *  CovEle-> GetBinContent(jbin+1 ,   jbin+1));
        // muon channel
        errorMTemp(nbins+ibin,nbins+jbin) = CovMuon->GetBinContent(ibin+1,jbin+1);
        if ( optionCorrTemp == 5 )  errorMTemp(nbins+ibin,nbins+jbin)  = sqrt (  CovMuon-> GetBinContent(ibin+1,ibin + 1 ) *  CovMuon-> GetBinContent(jbin+1 ,   jbin+1)) ;

        if ( ibin != jbin){
          //correlationDiffBin  =  1.;
          // electron-muon channel
          if ( optionCorrTemp  ==  3 ) {
            double valueEle = hCorrEle->GetBinContent(ibin, jbin);
            double valueMu = hCorrMu->GetBinContent(ibin, jbin);
            // correlationDiffBin = sqrt( fabs(hCorrEle->GetBinContent(ibin, jbin) * hCorrMu->GetBinContent(ibin, jbin)));
            correlationDiffBin = sqrt( fabs(valueEle * valueMu ));
            if ( valueEle < 0 && valueMu < 0 ) correlationDiffBin *= -1;
            if ( valueEle * valueMu < 0 ) {
              if (fabs(valueEle) > fabs(valueMu) ) correlationDiffBin *= ( valueEle / fabs(valueEle) ) ;
              else  correlationDiffBin *= ( valueMu / fabs(valueMu) ) ;
            } 	
            //	correlationDiffBin = 0;
          }
          errorMTemp(ibin,nbins+jbin) =  correlationDiffBin*sqrt(CovEle->GetBinContent(ibin+1,ibin+1)*CovMuon->GetBinContent(jbin+1,jbin+1));


          // muon-electron channel
          errorMTemp(nbins+ibin,jbin) =  correlationDiffBin*sqrt(fabs(CovMuon->GetBinContent(ibin+1,ibin+1)*CovEle->GetBinContent(jbin+1,jbin+1)));
        }
        cout << " correlation hist : " << ibin << "   " << jbin << "   "  <<  hCorrEle->GetBinContent(ibin, jbin) << "   " << hCorrMu->GetBinContent(ibin, jbin) <<"    "  << correlationDiffBin <<  "   " << optionCorrTemp << endl;

      }      


    }      

  } // loop over the number of bins


  for(int ibin = 0; ibin < 2 * nbins; ibin++){
    for(int jbin = 0; jbin < 2 * nbins; jbin++){
      errorMTemp(ibin,jbin) = errorMTemp(ibin,jbin)  / (norm * norm ) ;

    }
  }
  cout << " Print error matrix  " << endl;
  errorMTemp.Print();
  return errorMTemp;

}
//////////////////////////////////////////////////////////////////////////////////////
TMatrixD getCovMatrixOfCombinationUNF( double unfErrorDistr[], TH1D* hEle, TH1D* hMu,  TH1D* hEleOpp, TH1D* hMuOpp, int optionCorrTemp , double norm = 1.){

  double correlationSameBin  =  0.;
  double correlationDiffBin  =  0.;
  if ( optionCorrTemp  ==  1 ) correlationSameBin  =  0.;
  if ( optionCorrTemp  ==  2 ) correlationSameBin  =  1.;
  if ( optionCorrTemp  ==  3 ) correlationSameBin  =  1.;
  if ( optionCorrTemp  >=  4 ) {
    correlationSameBin  =  1.;
    correlationDiffBin  =  1. ;
  }


  // declare the big matrix
  int nbins =  hEle->GetNbinsX();
  //nbins =  8 ;
  const int NELE = 2*nbins;
  TMatrixD errorMTemp(NELE,NELE);
  double value_e[nbins];
  double value_m[nbins];
  double err_e[nbins];
  double err_m[nbins];
  double err_e_algo[nbins];
  double err_m_algo[nbins];

  for(int ibin = 0; ibin<nbins; ibin++){
    //measurement(ibin)  =  value_e / norm ;
    //measurement(nbins+ibin)  =  value_m / norm ;
    value_e[ibin] = hEle->GetBinContent(ibin+1) / norm;
    value_m[ibin] = hMu->GetBinContent(ibin+1) / norm;
    err_e[ibin]   =  hEle->GetBinContent(ibin+1) * unfErrorDistr[0] / norm;
    err_m[ibin]   =  hMu->GetBinContent(ibin+1) * unfErrorDistr[1] / norm ;
    err_e_algo[ibin] = fabs(value_e[ibin] - hEleOpp->GetBinContent(ibin+1)) / norm;
    err_m_algo[ibin] = fabs(value_m[ibin] - hMuOpp->GetBinContent(ibin+1)) / norm;

  }	

  for(int ibin = 0; ibin<nbins; ibin++){
    // electron channel
    errorMTemp(ibin,ibin) = pow(err_e[ibin],2) + pow(err_e_algo[ibin], 2 );
    // muon channel
    errorMTemp(nbins+ibin,nbins+ibin) = pow(err_m[ibin],2) + pow(err_m_algo[ibin], 2 );

    // this part is to setup correlation between same bins fo e and mu for the unfolding error
    for(int jbin = 0; jbin<nbins; jbin++){
      if (jbin !=  ibin && optionCorrTemp > 0 ) {
        errorMTemp(ibin,jbin) = err_e[ibin] * err_e[jbin] + err_e_algo[ibin] * err_e_algo[jbin] ;
        errorMTemp(jbin,ibin) =  errorMTemp(ibin,jbin) ;
        errorMTemp(nbins+ibin,nbins + jbin) = err_m[ibin] * err_m[jbin] + err_m_algo[ibin] * err_m_algo[jbin] ;
        errorMTemp(nbins+jbin,nbins + ibin) = errorMTemp(nbins+ibin,nbins + jbin);

      }
      if( optionCorrTemp > 1 ) {
        errorMTemp(ibin,nbins+jbin) =  correlationSameBin * ( err_e[ibin] * err_m[jbin] + err_e_algo[ibin] * err_m_algo[jbin] );
        // errorMTemp(ibin,nbins+jbin) =  unfErrorDistr[0] * value_e * unfErrorDistr[1] * hMu->GetBinContent(jbin+1);
        errorMTemp(nbins+ibin,jbin) = correlationSameBin * ( err_e[jbin] * err_m[ibin] + err_e_algo[jbin] * err_m_algo[ibin] ); ;
      }

    }

  }

  return errorMTemp;
}


#endif
