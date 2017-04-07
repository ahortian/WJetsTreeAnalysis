{
    string srcdir = "Sources/";
    
    vector<string> sources;
    sources.push_back("getFilesAndHistograms");
    sources.push_back("functions");
    sources.push_back("writeFastPlotsTex");
    sources.push_back("unfoldingFunctions");
    sources.push_back("unfoldMeanNJetsFastPlot");
    
    //--- Load shaared libraries ---
    unsigned int nSources = sources.size();
    gROOT->ProcessLine(".L /afs/cern.ch/cms/slc5_amd64_gcc434/external/lhapdf/5.8.5/lib/libLHAPDF.so");
    for (unsigned int i(0); i < nSources; i++){
        cout <<"Compiling " << srcdir + sources[i] << ".cc" << endl;
        gROOT->ProcessLine(string(".L " + srcdir + sources[i] + ".cc++").c_str());
        }
        
        unfoldMeanNJetsFastPlot("SMu", "MeanNJetsHT_Zinc1jet", true);
        unfoldMeanNJetsFastPlot("SMu", "MeanNJetsHT_Zinc1jet", false);
        
        unfoldMeanNJetsFastPlot("SMu", "MeanNJetsHT_Zinc2jet", true);
        unfoldMeanNJetsFastPlot("SMu", "MeanNJetsHT_Zinc2jet", false);
        
        unfoldMeanNJetsFastPlot("SMu", "MeanNJetsdRapidity_Zinc2jet", true);
        unfoldMeanNJetsFastPlot("SMu", "MeanNJetsdRapidity_Zinc2jet", false);
        
        unfoldMeanNJetsFastPlot("SMu", "MeanNJetsdRapidityFB_Zinc2jet", true);
        unfoldMeanNJetsFastPlot("SMu", "MeanNJetsdRapidityFB_Zinc2jet", false);
        
        
         //--- clean the *_cc.d and *_cc.so files ---
        
         string cmd = "if ls *_cc.d &> .ls_tmp.list; then rm *_cc.d; fi";
         system(cmd.c_str());
         cmd = "if ls *_cc.so &> .ls_tmp.list; then rm *_cc.so; fi";
         system(cmd.c_str());
         cmd = "if ls " + srcdir + "*_cc.d &> .ls_tmp.list; then rm " + srcdir + "*_cc.d; fi";
         system(cmd.c_str());
         cmd = "if ls " + srcdir + "*_cc.so &> .ls_tmp.list; then rm " + srcdir + "*_cc.so; fi";
         system(cmd.c_str());
         system("rm .ls_tmp.list");
         
        
}
