void myReplace(string& str, const string& oldStr, const string& newStr)
{
   size_t pos = 0;
   while((pos = str.find(oldStr, pos)) != std::string::npos)
   {
      str.replace(pos, oldStr.length(), newStr);
      pos += newStr.length();
   }
}

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

void writeCrossSectionTable(string leptonFlavor, string variable, string title, string xTitle, double binWidth, TGraphAsymmErrors *grCentralStat, TGraphAsymmErrors *grCentralSyst)
{
   TGraphAsymmErrors *grCentStat = (TGraphAsymmErrors*) grCentralStat->Clone();
   TGraphAsymmErrors *grCentSyst = (TGraphAsymmErrors*) grCentralSyst->Clone();
   int nBins(grCentStat->GetN());
   int nBinsInit(0);
   if (variable.find("ZNGoodJets") != string::npos) {
      cout << "Jet Counter" << endl;
      nBinsInit = 1;
   }
   cout << grCentStat->GetXaxis()->GetTitle() << endl;



   string outputFile = "CrossSectionsTables/";  
   outputFile = outputFile + leptonFlavor + "_CrossSectionTable_";
   outputFile += variable + ".tex";


   std::ofstream out(outputFile.c_str());
   std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
   std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!



   if (variable.find("ZNGoodJets") != string::npos) cout << "{\n \\renewcommand{\\arraystretch}{1.5} \n \\begin{tabular}{|c|cll|} \n \\multicolumn{4}{c}{";
   else cout << "{\n \\renewcommand{\\arraystretch}{1.5} \n \\begin{tabular}{|rcl|lll|} \n \\multicolumn{6}{c}{";
   cout << title;
   if (leptonFlavor == "Muons")          cout << " $Z\\rightarrow \\mu^{+}\\mu^{-}$";
   else if (leptonFlavor == "Electrons") cout << " $Z\\rightarrow e^{+}e^{-}$";
   cout << "} \\\\ \\hline " << endl;
   //cout << "$N_{\\text{jets}}$ &";
   if (variable.find("ZNGoodJets") != string::npos) cout << " " << xTitle << " &";
   else cout << " \\multicolumn{3}{|c|}{" << xTitle << "} &";
   cout << " \\hspace*{0.4cm} $d\\sigma$ \\hspace*{0.4cm}  &  $\\varepsilon_{\\text{stat.}}$ &  $\\varepsilon_{\\text{syst.}}$ \\\\ \\hline" << endl; 
   for (int i(nBinsInit); i < nBins; i++){
      double x(0.), y(0.);
      grCentStat->GetPoint(i, x, y);
      double xLow = x - 0.5*binWidth;
      double xHigh = x + 0.5*binWidth;
      if ( fabs(xLow) < 0.00001) xLow = 0.;
      if ( fabs(xHigh) < 0.00001) xHigh = 0.;
      double statError = grCentStat->GetErrorYlow(i);
      double systErrorLow = pow(grCentSyst->GetErrorYlow(i), 2) - pow(statError, 2);
      systErrorLow = sqrt(systErrorLow);
      double systErrorHigh = pow(grCentSyst->GetErrorYhigh(i), 2) - pow(statError, 2);
      systErrorHigh = sqrt(systErrorHigh);

      if (variable.find("ZNGoodJets_Zexc") != string::npos) cout << "$ = " << i; 
      else if (variable.find("ZNGoodJets_Zinc") != string::npos) cout << "$ \\geq " << i; 
      else cout << "$ " << setprecision(4) << setw(6) << xLow << " $ & - &  $ " << setw(6) << xHigh ;
      cout << "  $ & $  " << setprecision(3) << setw(6) << y << " $ & $\\pm~ " << setprecision(1) << statError;
      cout << "  $ & $  _{-" << systErrorLow  << "}^{+" << systErrorHigh << "} $ \\\\" << endl; 

   }
   cout << "\\hline \n \\end{tabular} \n }" << endl;

   std::cout.rdbuf(coutbuf); //reset to standard output again



}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void writeCorrTable(string leptonFlavor, string variable, string title, string fileNameTable, TH2D* hCov )
{
	ofstream myFile(fileNameTable.c_str());	


	myFile << "\\begin{table}[htb!]\\begin{center}" << endl;
	myFile << "\\caption{Correlation matrix of the " << title << " due to unfolding procedure and statistical fluctuations in signal and response matrix in the ";
	if (leptonFlavor == "DMu") myFile << " muon decay channel.";
	if (leptonFlavor == "DE") myFile << " electron decay channel.";
	myFile << " }"<<endl;
	myFile << "\\scriptsize{" << endl;
//	hCov->Draw();
        int nBins = hCov->GetXaxis()->GetNbins() + 1;
	int iStart = 1;
	
	if (title.find("Counter") != string::npos ) { iStart += 1 ; nBins -= 1;} 
	myFile.precision(3);
	myFile << "\\begin{tabular}{c|";
	 for (int i(iStart); i <= nBins; i++)   myFile << "c";

        myFile << "}\n \\multicolumn{";
	myFile << nBins ;
         myFile <<"}{c}{";
	if (leptonFlavor == "Muons" || leptonFlavor == "DMu")         myFile << " $Z\\rightarrow \\MM$: ";
	else if (leptonFlavor == "Electrons" || leptonFlavor == "DE") myFile << " $Z\\rightarrow \\EE$: ";
	myFile << title << "} \\\\" << endl;
        myFile << " Bins  " ;
	 for (int i(1); i <= nBins +1 - iStart ; i++)   myFile << "   &   " <<  i  ;
	 myFile << " \\\\ \\hline" << endl;
         int count = 1;
	for (int i(iStart); i <= nBins  ; i++){
	          myFile << count ; 
		 for (int j(iStart); j <= nBins  ; j++){   
			 myFile << "  &  " << hCov->GetBinContent(i,j) ; 
		 }
         //myFile << " \\\\ \\hline" << endl;
	count++;
	}

	//	myFile << xtitle << " & ";
	//	myFile << sigmaTitleTab << " & stat & JES & PU & XSEC & JER & Unf. Meth. & tot \\\\ \\hline" << endl;

	//	const int nBins(hCov->GetNbinsX());
	// TH2D* hCorr = (TH2D *) h ->Clone();
	//   int xbin(h->GetNbinsX()), ybin(h->GetNbinsY());
	 myFile << "\\end{tabular}}" << endl;
  myFile << "\\label{tab:Correlation" << leptonFlavor ;
  myFile << variable << "}"<< endl;
  myFile << "\\end{center}"  << endl;
  myFile << "\\end{table}" << endl;


	myFile.close();

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void writeCrossSectionTable(string leptonFlavor, TCanvas *can)
{
	TGraphAsymmErrors *grCentStat = (TGraphAsymmErrors*) can->FindObject("grCentralStat");
	TGraphAsymmErrors *grCentSyst = (TGraphAsymmErrors*) can->FindObject("grCentralSyst");
	string variable = can->GetName();
	string genVariable = "gen" + variable;
	TH1D *hSheMC = (TH1D*) can->FindObject(genVariable.c_str());

	double binWidth( hSheMC->GetBinWidth(1));

	string title = hSheMC->GetTitle();
	if (title.find("gen ") != string::npos){
		string tmp = title.substr(title.find("gen ") + 4, title.size());
		title = tmp;
	}
	//cout << "Title: " << title << endl;
	changeToLatexFormat(title);
	//cout << "Title: " << title << endl;
	string xTitle = hSheMC->GetXaxis()->GetTitle();
	//cout << xTitle << endl;
	changeToLatexFormat(xTitle);
	//cout << xTitle << endl;
	//TLatex *latexLabel = (TLatex*) can->FindObject("TITLE"); 

	writeCrossSectionTable(leptonFlavor, variable, title, xTitle, binWidth, grCentStat, grCentSyst);

}

