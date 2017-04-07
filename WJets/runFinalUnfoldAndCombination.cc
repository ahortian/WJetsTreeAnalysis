{
  //string roounfolddir = "/user/tseva/analysis/RooUnfold-1.1.1/";
/*  string roounfolddir = "/Users/tseva/analysis/unfold/ZJet/RooUnfold-1.1.1/";
  gSystem->AddIncludePath(string("-I" + roounfolddir + "src").c_str());
  gSystem->Load(string(roounfolddir + "libRooUnfold").c_str());
*/

  string srcdir = "Sources/";
  string incdir = "Includes/";

  vector<string> sources;
  sources.push_back("getFilesAndHistograms");
  sources.push_back("functions");
  sources.push_back("writeFastPlotsTex");
  sources.push_back("unfoldingFunctions");
  sources.push_back("FinalUnfoldAndCombination");
  
  //--- Load shaared libraries ---
  //gSystem->Load("checkForCompilationNeeds.so");
  //checkForCompilationNeeds(srcdir, sources);
  unsigned int nSources = sources.size();
  for (unsigned int i(0); i < nSources; i++){
    //gSystem->Load(string(sources[i] + "_cc.so").c_str());
    cout <<"Compiling " << srcdir + sources[i] << ".cc" << endl;
    gROOT->ProcessLine(string(".L " + srcdir + sources[i] + ".cc++").c_str());
  }
  
//  FinalUnfoldAndCombination(0, 1);
//  FinalUnfoldAndCombination(21, 40);
//  FinalUnfoldAndCombination(41, 62);
//  FinalUnfoldAndCombination(41, 62);

}
