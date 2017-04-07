{
    string srcdir = "Sources/";
    
    vector<string> sources;
    sources.push_back("getFilesAndHistograms");
    sources.push_back("functions");
    //sources.push_back("writeFastPlotsTex");
    sources.push_back("unfoldingFunctions");
    //sources.push_back("myUnfoldingSyst");
    sources.push_back("myFinalUnfoldMeanNJets");
    
    //--- Load shaared libraries ---
    unsigned int nSources = sources.size();
    gROOT->ProcessLine(".L /afs/cern.ch/cms/slc5_amd64_gcc434/external/lhapdf/5.8.5/lib/libLHAPDF.so");
    
    for (unsigned int i(0); i < nSources; i++){
        cout << "Compiling " << srcdir + sources[i] << ".cc" << endl;
        gROOT->ProcessLine(string(".L " + srcdir + sources[i] + ".cc+").c_str());
    }
        
    myFinalUnfoldMeanNJets("MeanNJetsHT_Zinc1jet", "SMu", "Bayes");
    myFinalUnfoldMeanNJets("MeanNJetsHT_Zinc2jet", "SMu", "Bayes");
    myFinalUnfoldMeanNJets("MeanNJetsdRapidity_Zinc2jet", "SMu", "Bayes");
    myFinalUnfoldMeanNJets("MeanNJetsdRapidityFB_Zinc2jet", "SMu", "Bayes");
        
}
