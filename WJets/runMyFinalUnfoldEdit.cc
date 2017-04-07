{
    string srcdir = "Sources/";
    
    vector<string> sources;
    sources.push_back("getFilesAndHistograms");
    sources.push_back("functions");
    //sources.push_back("writeFastPlotsTex");
    //sources.push_back("unfoldingFunctions");
    //sources.push_back("myUnfoldingSyst");
    sources.push_back("myFinalUnfoldEdit");
    
    //--- Load shaared libraries ---
    unsigned int nSources = sources.size();
    gROOT->ProcessLine(".L /afs/cern.ch/cms/slc5_amd64_gcc434/external/lhapdf/5.8.5/lib/libLHAPDF.so");
    
    for (unsigned int i(0); i < nSources; i++){
        cout << "Compiling " << srcdir + sources[i] << ".cc" << endl;
        gROOT->ProcessLine(string(".L " + srcdir + sources[i] + ".cc+").c_str());
    }
        
    myFinalUnfoldEdit(0, 1);
    
    
    myFinalUnfoldEdit(1, 6);
    myFinalUnfoldEdit(11, 16);
    myFinalUnfoldEdit(21, 26);
    myFinalUnfoldEdit(26, 43);
    myFinalUnfoldEdit(43, 48);
        
        
}
