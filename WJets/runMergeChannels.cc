{
//    string srcdir = "Sources/";
    string srcdir = "/Users/tseva/analysis/TreeAnalysis2012/DYJets/Sources/";
    string incdir = "/Users/tseva/analysis/TreeAnalysis2012/DYJets/Includes/";
     string roounfolddir = "/Users/tseva/analysis/ZJet/RooUnfold-1.1.1/";

    gSystem->AddIncludePath(string("-I" + roounfolddir + "src").c_str());
    gSystem->AddDynamicPath(string(srcdir).c_str());
    gSystem->AddIncludePath(string("-I" + incdir).c_str());
    gSystem->Load(string(roounfolddir + "libRooUnfold").c_str());
    gROOT->ForceStyle();  

    vector<string> sources;
    sources.push_back("getFilesAndHistograms");
    sources.push_back("functions");
    sources.push_back("plottingFunctions");
    sources.push_back("unfoldingFunctions");
    sources.push_back("writeFastPlotsTex");
    sources.push_back("mergeChannels");

    //--- Load shared libraries ---
    unsigned int nSources = sources.size();
    for (unsigned int i(0); i < nSources; i++){
        cout <<"Compiling " << srcdir + sources[i] << ".cc" << endl;
        gROOT->ProcessLine(string(".L " + srcdir + sources[i] + ".cc+").c_str());
    }

    // 
   mergeChannels();


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
