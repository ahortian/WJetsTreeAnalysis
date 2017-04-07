{
    string srcdir = "Sources/";
    
    vector<string> sources;
    sources.push_back("getFilesAndHistograms");
    sources.push_back("functions");
    sources.push_back("DataDrivenQCD_MeanNJets");
    
    //--- Load shared libraries -------
    unsigned int nSources = sources.size();
    for (unsigned int i(0); i < nSources; i++){        
        cout <<"Compiling " << srcdir + sources[i] << ".cc" << endl;
        gROOT->ProcessLine(string(".L " + srcdir + sources[i] + ".cc++").c_str());
    }
    //--- End Load shared libraries ---
        
        
    DataDrivenQCD_MeanNJets("SMu", 0, -1);
        
        
    //--- clean the *_cc.d and *_cc.so files -------
    string cmd = "if ls *_cc.d &> .ls_tmp.list; then rm *_cc.d; fi";
    system(cmd.c_str());
    
    cmd = "if ls *_cc.so &> .ls_tmp.list; then rm *_cc.so; fi";
    system(cmd.c_str());
    
    cmd = "if ls " + srcdir + "*_cc.d &> .ls_tmp.list; then rm " + srcdir + "*_cc.d; fi";
    system(cmd.c_str());
    
    cmd = "if ls " + srcdir + "*_cc.so &> .ls_tmp.list; then rm " + srcdir + "*_cc.so; fi";
    system(cmd.c_str());
    
    system("rm .ls_tmp.list");
    //--- End clean the *_cc.d and *_cc.so files ---
        
}
